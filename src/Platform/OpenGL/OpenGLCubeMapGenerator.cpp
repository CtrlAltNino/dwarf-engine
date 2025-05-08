#include "pch.hpp"

#include "OpenGLCubeMapGenerator.hpp"
#include "Platform/OpenGL/OpenGLMeshBuffer.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"
#include <glad/glad.h>

namespace Dwarf
{
  OpenGLCubeMapGenerator::OpenGLCubeMapGenerator(
    std::shared_ptr<IDwarfLogger>           logger,
    std::shared_ptr<ITextureFactory>        textureFactory,
    const std::shared_ptr<IShaderRegistry>& shaderRegistry,
    const std::shared_ptr<IShaderSourceCollectionFactory>&
                                               shaderSourceCollectionFactory,
    const std::shared_ptr<IMeshFactory>&       meshFactory,
    const std::shared_ptr<IMeshBufferFactory>& meshBufferFactory,
    const std::shared_ptr<IFramebufferFactory> framebufferFactory)
    : mLogger(std::move(logger))
    , mTextureFactory(std::move(textureFactory))
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
    // 1. Create cubemap
    GLuint cubemap;
    glGenTextures(1, &cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

    for (unsigned int i = 0; i < 6; ++i)
    {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                   0,
                   GL_RGB16F,
                   resolution,
                   resolution,
                   0,
                   GL_RGB,
                   GL_FLOAT,
                   nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 2. Set up framebuffer
    GLuint captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(
      GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, cubemapSize, cubemapSize);
    glFramebufferRenderbuffer(
      GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // 3. Define view matrices for cubemap faces
    glm::mat4 captureProjection =
      glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] = { glm::lookAt(glm::vec3(0.0f),
                                             glm::vec3(1.0f, 0.0f, 0.0f),
                                             glm::vec3(0.0f, -1.0f, 0.0f)),
                                 glm::lookAt(glm::vec3(0.0f),
                                             glm::vec3(-1.0f, 0.0f, 0.0f),
                                             glm::vec3(0.0f, -1.0f, 0.0f)),
                                 glm::lookAt(glm::vec3(0.0f),
                                             glm::vec3(0.0f, 1.0f, 0.0f),
                                             glm::vec3(0.0f, 0.0f, 1.0f)),
                                 glm::lookAt(glm::vec3(0.0f),
                                             glm::vec3(0.0f, -1.0f, 0.0f),
                                             glm::vec3(0.0f, 0.0f, -1.0f)),
                                 glm::lookAt(glm::vec3(0.0f),
                                             glm::vec3(0.0f, 0.0f, 1.0f),
                                             glm::vec3(0.0f, -1.0f, 0.0f)),
                                 glm::lookAt(glm::vec3(0.0f),
                                             glm::vec3(0.0f, 0.0f, -1.0f),
                                             glm::vec3(0.0f, -1.0f, 0.0f)) };

    // 4. Render to each face
    auto* oglShader = dynamic_cast<OpenGLShader*>(mConvertShader.get());
    glUseProgram(oglShader->GetID());
    oglShader->SetParameter("u_Equirect", texture);
    oglShader->SetParameter("u_Projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());

    glViewport(0, 0, resolution, resolution);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

    for (unsigned int i = 0; i < 6; ++i)
    {
      oglShader->SetParameter("u_View", captureViews[i]);
      oglShader->UploadParameters();
      glFramebufferTexture2D(GL_FRAMEBUFFER,
                             GL_COLOR_ATTACHMENT0,
                             GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             cubemap,
                             0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      // glBindVertexArray(cubeVAO);
      auto* oglMesh = dynamic_cast<OpenGLMeshBuffer*>(mCubeMeshBuffer.get());
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return cubemap;
  }
}