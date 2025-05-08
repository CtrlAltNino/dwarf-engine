#include "pch.hpp"

#include "OpenGLCubeMapGenerator.hpp"
#include "OpenGLUtilities.hpp"
#include "Platform/OpenGL/OpenGLMeshBuffer.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Utilities/ImageUtilities/TextureCommon.hpp"
#include <glad/glad.h>

namespace Dwarf
{
  OpenGLCubeMapGenerator::OpenGLCubeMapGenerator(
    std::shared_ptr<IDwarfLogger>           logger,
    std::shared_ptr<ITextureFactory>        textureFactory,
    const std::shared_ptr<IShaderRegistry>& shaderRegistry,
    const std::shared_ptr<IShaderSourceCollectionFactory>&
                                                shaderSourceCollectionFactory,
    const std::shared_ptr<IMeshFactory>&        meshFactory,
    const std::shared_ptr<IMeshBufferFactory>&  meshBufferFactory,
    const std::shared_ptr<IFramebufferFactory>  framebufferFactory,
    const std::shared_ptr<IRendererApiFactory>& rendererApiFactory)
    : mLogger(std::move(logger))
    , mTextureFactory(std::move(textureFactory))
    , mRendererApi(rendererApiFactory->Create())
  {
    std::shared_ptr<IMesh> cubeMesh = meshFactory->CreateSkyboxCube();
    mCubeMeshBuffer = meshBufferFactory->Create(cubeMesh);
    mConvertShader = shaderRegistry->GetOrCreate(
      shaderSourceCollectionFactory
        ->CreateCubeMapConversionShaderSourceCollection());
  }

  auto
  OpenGLCubeMapGenerator::FromEquirectangular(std::shared_ptr<ITexture> texture,
                                              uint32_t resolution)
    -> std::shared_ptr<ITexture>
  {
    TextureParameters cubeMapParameters;
    cubeMapParameters.AnisoLevel = 1;
    cubeMapParameters.FlipY = false;
    cubeMapParameters.IsSRGB = true;
    cubeMapParameters.MinFilter = TextureMinFilter::LINEAR;
    cubeMapParameters.MagFilter = TextureMagFilter::LINEAR;
    cubeMapParameters.MipMapped = false;
    cubeMapParameters.WrapR = TextureWrap::CLAMP_TO_EDGE;
    cubeMapParameters.WrapS = TextureWrap::CLAMP_TO_EDGE;
    cubeMapParameters.WrapT = TextureWrap::CLAMP_TO_EDGE;

    std::shared_ptr<ITexture> cubeMap =
      mTextureFactory->Empty(TextureType::TEXTURE_CUBE_MAP,
                             TextureFormat::RGB,
                             TextureDataType::FLOAT,
                             glm::ivec2(resolution, resolution),
                             cubeMapParameters,
                             1);

    // 2. Set up framebuffer
    static const glm::mat4 captureProjection =
      glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    static const glm::mat4 captureViews[] = {
      glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), glm::vec3(0, -1, 0)),
      glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0)),
      glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1)),
      glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), glm::vec3(0, 0, -1)),
      glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, -1, 0)),
      glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, -1, 0))
    };

    // Create and set up framebuffer
    GLuint fbo, rbo;
    glCreateFramebuffers(1, &fbo);
    OpenGLUtilities::CheckOpenGLError(
      "glCreateFramebuffers", "OpenGLCubeMapGenerator", mLogger);
    glCreateRenderbuffers(1, &rbo);
    OpenGLUtilities::CheckOpenGLError(
      "glCreateRenderbuffers", "OpenGLCubeMapGenerator", mLogger);
    glNamedRenderbufferStorage(
      rbo, GL_DEPTH_COMPONENT24, resolution, resolution);
    OpenGLUtilities::CheckOpenGLError(
      "glNamedRenderbufferStorage", "OpenGLCubeMapGenerator", mLogger);
    glNamedFramebufferRenderbuffer(
      fbo, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
    OpenGLUtilities::CheckOpenGLError(
      "glNamedFramebufferRenderbuffer", "OpenGLCubeMapGenerator", mLogger);

    auto* shader = dynamic_cast<OpenGLShader*>(mConvertShader.get());
    glUseProgram(shader->GetID());
    OpenGLUtilities::CheckOpenGLError(
      "glUseProgram", "OpenGLCubeMapGenerator", mLogger);
    glUniform1i(glGetUniformLocation(shader->GetID(), "equirectangularMap"), 0);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1i", "OpenGLCubeMapGenerator", mLogger);
    glUniformMatrix4fv(glGetUniformLocation(shader->GetID(), "projection"),
                       1,
                       GL_FALSE,
                       glm::value_ptr(captureProjection));
    OpenGLUtilities::CheckOpenGLError(
      "glUniformMatrix4fv", "OpenGLCubeMapGenerator", mLogger);

    glActiveTexture(GL_TEXTURE0);
    OpenGLUtilities::CheckOpenGLError(
      "glActiveTexture", "OpenGLCubeMapGenerator", mLogger);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    OpenGLUtilities::CheckOpenGLError(
      "glBindTexture", "OpenGLCubeMapGenerator", mLogger);

    glViewport(0, 0, resolution, resolution);
    OpenGLUtilities::CheckOpenGLError(
      "glViewport", "OpenGLCubeMapGenerator", mLogger);
    // glDisable(GL_CULL_FACE);
    // glDisable(GL_DEPTH_TEST); // <- Try this if depth is not needed

    for (unsigned int i = 0; i < 6; ++i)
    {
      glUniformMatrix4fv(glGetUniformLocation(shader->GetID(), "view"),
                         1,
                         GL_FALSE,
                         glm::value_ptr(captureViews[i]));
      OpenGLUtilities::CheckOpenGLError(
        "glUniformMatrix4fv", "OpenGLCubeMapGenerator", mLogger);

      // Attach face
      glNamedFramebufferTextureLayer(
        fbo, GL_COLOR_ATTACHMENT0, cubeMap->GetTextureID(), 0, i);
      OpenGLUtilities::CheckOpenGLError(
        "glNamedFramebufferTextureLayer", "OpenGLCubeMapGenerator", mLogger);
      glBindFramebuffer(GL_FRAMEBUFFER, fbo);
      OpenGLUtilities::CheckOpenGLError(
        "glBindFramebuffer", "OpenGLCubeMapGenerator", mLogger);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      OpenGLUtilities::CheckOpenGLError(
        "glClear", "OpenGLCubeMapGenerator", mLogger);

      // Draw cube (assumes cubeVAO is bound to a unit cube with in vec3 aPos)
      auto* oglMesh = dynamic_cast<OpenGLMeshBuffer*>(mCubeMeshBuffer.get());
      oglMesh->Bind();
      glDrawElements(
        GL_TRIANGLES, oglMesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
      OpenGLUtilities::CheckOpenGLError(
        "glDrawArrays", "OpenGLCubeMapGenerator", mLogger);
      oglMesh->Unbind();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer Unbind", "OpenGLCubeMapGenerator", mLogger);

    glDeleteFramebuffers(1, &fbo);
    glDeleteRenderbuffers(1, &rbo);

    return std::move(cubeMap);
  }
}