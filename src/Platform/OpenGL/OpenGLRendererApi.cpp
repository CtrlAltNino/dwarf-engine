#include "pch.h"

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.h"
#include "OpenGLFramebuffer.h"
#include "Platform/OpenGL/OpenGLComputeShader.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "Platform/OpenGL/OpenGLMesh.h"
#include "Platform/OpenGL/OpenGLRendererApi.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLUtilities.h"
#include <glad/glad.h>

namespace Dwarf
{
  OpenGLRendererApi::OpenGLRendererApi(
    std::shared_ptr<IAssetDatabase>      assetDatabase,
    std::shared_ptr<IShaderRegistry>     shaderRegistry,
    std::shared_ptr<IDwarfLogger>        logger,
    std::shared_ptr<IEditorStats>        editorStats,
    std::shared_ptr<IOpenGLStateTracker> stateTracker,
    std::shared_ptr<IShaderSourceCollectionFactory>
                                        shaderSourceCollectionFactory,
    std::shared_ptr<IMeshFactory>       meshFactory,
    std::shared_ptr<IMeshBufferFactory> meshBufferFactory)
    : mAssetDatabase(std::move(assetDatabase))
    , mShaderRegistry(std::move(shaderRegistry))
    , mLogger(std::move(logger))
    , mEditorStats(std::move(editorStats))
    , mStateTracker(std::move(stateTracker))
    , mShaderSourceCollectionFactory(std::move(shaderSourceCollectionFactory))
    , mMeshFactory(std::move(meshFactory))
    , mMeshBufferFactory(std::move(meshBufferFactory))
  {
    mLogger->LogDebug(Log("OpenGLRendererApi created.", "OpenGLRendererApi"));
    mErrorShader = mShaderRegistry->GetOrCreate(
      mShaderSourceCollectionFactory->CreateErrorShaderSourceCollection());
    mErrorShader->Compile();

    std::unique_ptr<IMesh> screenQuad = mMeshFactory->CreateFullscreenQuad();
    mScreenQuad = mMeshBufferFactory->Create(screenQuad);

    mStateTracker->SetDepthTest(true);
    mStateTracker->SetDepthFunction(GL_LESS);
    glEnable(GL_LINE_SMOOTH);
    OpenGLUtilities::CheckOpenGLError(
      "glEnable GL_LINE_SMOOTH", "OpenGLRendererApi", mLogger);
  }

  OpenGLRendererApi::~OpenGLRendererApi()
  {
    mLogger->LogDebug(Log("OpenGLRendererApi destroyed.", "OpenGLRendererApi"));
  }

  void
  OpenGLRendererApi::SetViewport(uint32_t x,
                                 uint32_t y,
                                 uint32_t width,
                                 uint32_t height)
  {
    mStateTracker->SetViewport(x, y, width, height);
  }

  void
  OpenGLRendererApi::SetClearColor(const glm::vec4& color)
  {
    mStateTracker->SetClearColor(color);
  }

  void
  OpenGLRendererApi::Clear()
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before clearing", "OpenGLRendererApi", mLogger);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    OpenGLUtilities::CheckOpenGLError("glClear", "OpenGLRendererApi", mLogger);
  }

  void
  OpenGLRendererApi::Clear(uint32_t value)
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before clearing", "OpenGLRendererApi", mLogger);
    glClearBufferuiv(
      GL_COLOR, 0, &value); // Use glClearBufferuiv for integer types
    OpenGLUtilities::CheckOpenGLError(
      "glClearBufferuiv", "OpenGLRendererApi", mLogger);
    glClear(GL_DEPTH_BUFFER_BIT);
    OpenGLUtilities::CheckOpenGLError("glClear", "OpenGLRendererApi", mLogger);
  }

  void
  OpenGLRendererApi::RenderIndexed(IMeshBuffer& mesh,
                                   IMaterial&   material,
                                   ICamera&     camera,
                                   glm::mat4    modelMatrix)
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before rendering", "OpenGLRendererApi", mLogger);
    auto&         oglMesh = dynamic_cast<OpenGLMesh&>(mesh);
    IShader&      baseShader = *material.GetShader();
    OpenGLShader& shader = baseShader.IsCompiled()
                             ? dynamic_cast<OpenGLShader&>(baseShader)
                             : dynamic_cast<OpenGLShader&>(*mErrorShader);
    int           textureInputCounter = 0;

    mStateTracker->SetShaderProgram(shader);

    mStateTracker->SetBlendMode(material.GetMaterialProperties().IsTransparent);

    mStateTracker->SetBlendFunction(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mStateTracker->SetCullMode(!material.GetMaterialProperties().IsDoubleSided);
    mStateTracker->SetCullFace(GL_BACK);

    // TODO: Move this to OpenGLShader.cpp
    uint32_t textureCount = 0;
    for (auto const& identifier :
         material.GetShaderParameters()->GetParameterIdentifiers())
    {
      if (material.GetShaderParameters()->HasParameter(identifier))
      {
        /*ParameterValue& shaderParameterValue =
          material.GetShaderParameters()->GetParameter(identifier);
        std::visit(SetShaderParameterVisitor{ .mShader = shader,
                                              .mParameterName = identifier,
                                              .mAssetDatabase = mAssetDatabase,
                                              .mLogger = mLogger,
                                              .mTextureCount = textureCount },
                   shaderParameterValue);*/
        shader.SetParameter(
          identifier, material.GetShaderParameters()->GetParameter(identifier));
      }
    }

    /*shader.SetUniform("modelMatrix", modelMatrix);
    shader.SetUniform("viewMatrix", camera.GetViewMatrix());
    shader.SetUniform("projectionMatrix", camera.GetProjectionMatrix());
    shader.SetUniform("_Time", (float)mEditorStats->GetTimeSinceStart());
    shader.SetUniform("viewPosition",
                      camera.GetProperties().Transform.GetPosition());*/
    shader.SetParameter("modelMatrix", modelMatrix);
    shader.SetParameter("viewMatrix", camera.GetViewMatrix());
    shader.SetParameter("projectionMatrix", camera.GetProjectionMatrix());
    shader.SetParameter("_Time", (float)mEditorStats->GetTimeSinceStart());
    shader.SetParameter("viewPosition",
                        camera.GetProperties().Transform.GetPosition());

    oglMesh.Bind();

    glDrawElements(GL_TRIANGLES, oglMesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glDrawElements", "OpenGLRendererApi", mLogger);
  }

  void
  OpenGLRendererApi::ApplyComputeShader(
    std::shared_ptr<IComputeShader> computeShader,
    std::shared_ptr<IFramebuffer>   framebuffer,
    uint32_t                        sourceAttachment,
    uint32_t                        destinationAttachment)
  {
    /*std::shared_ptr<OpenGLComputeShader> shader =
      std::dynamic_pointer_cast<OpenGLComputeShader>(computeShader);
    mStateTracker->SetShaderProgram(shader);
    glBindImageTexture(0,
                       framebuffer->GetColorAttachment(sourceAttachment)
                         .value()
                         .get()
                         .GetTextureID(),
                       0,
                       GL_FALSE,
                       0,
                       GL_READ_ONLY,
                       GL_RGBA8);
    glBindImageTexture(1,
                       framebuffer->GetColorAttachment(destinationAttachment)
                         .value()
                         .get()
                         .GetTextureID(),
                       0,
                       GL_FALSE,
                       0,
                       GL_WRITE_ONLY,
                       GL_RGBA8);

    glDispatchCompute(framebuffer->GetSpecification().Width / 16,
                      framebuffer->GetSpecification().Height / 16,
                      1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);*/
  }

  void
  OpenGLRendererApi::Blit(IFramebuffer& source,
                          IFramebuffer& destination,
                          uint32_t      sourceAttachment,
                          uint32_t      destinationAttachment,
                          uint32_t      width,
                          uint32_t      height)
  {
    auto* sourceFB = dynamic_cast<OpenGLFramebuffer*>(&source);
    auto* destinationFB = dynamic_cast<OpenGLFramebuffer*>(&destination);
    glBindFramebuffer(GL_READ_FRAMEBUFFER,
                      sourceFB->GetFramebufferRendererID());
    glReadBuffer(GL_COLOR_ATTACHMENT0 +
                 sourceAttachment); // Select the second attachment for reading
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer GL_READ_FRAMEBUFFER", "OpenGLRendererApi", mLogger);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
                      destinationFB->GetFramebufferRendererID());
    glDrawBuffer(GL_COLOR_ATTACHMENT0 +
                 destinationAttachment); // Usually the default for framebufferB
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer GL_DRAW_FRAMEBUFFER", "OpenGLRendererApi", mLogger);
    glBlitFramebuffer(0,
                      0,
                      width,
                      height,
                      0,
                      0,
                      width,
                      height,
                      GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);
    OpenGLUtilities::CheckOpenGLError(
      "glBlitFramebuffer", "OpenGLRendererApi", mLogger);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer Unbind", "OpenGLRendererApi", mLogger);
  }

  void
  OpenGLRendererApi::BlitDepth(IFramebuffer& source,
                               IFramebuffer& destination,
                               uint32_t      width,
                               uint32_t      height)
  {
    if (!source.GetDepthAttachment().has_value() ||
        !destination.GetDepthAttachment().has_value())
    {
      return;
    }

    auto* sourceFB = dynamic_cast<OpenGLFramebuffer*>(&source);
    auto* destinationFB = dynamic_cast<OpenGLFramebuffer*>(&destination);
    glBindFramebuffer(GL_READ_FRAMEBUFFER,
                      sourceFB->GetFramebufferRendererID());
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer GL_READ_FRAMEBUFFER", "OpenGLRendererApi", mLogger);
    OpenGLUtilities::CheckOpenGLError(
      "glReadBuffer GL_DEPTH_ATTACHMENT", "OpenGLRendererApi", mLogger);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
                      destinationFB->GetFramebufferRendererID());
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer GL_DRAW_FRAMEBUFFER", "OpenGLRendererApi", mLogger);
    glBlitFramebuffer(0,
                      0,
                      width,
                      height,
                      0,
                      0,
                      width,
                      height,
                      GL_DEPTH_BUFFER_BIT,
                      GL_NEAREST);
    OpenGLUtilities::CheckOpenGLError(
      "glBlitFramebuffer", "OpenGLRendererApi", mLogger);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer Unbind", "OpenGLRendererApi", mLogger);
  }

  void
  OpenGLRendererApi::CustomBlit(IFramebuffer& source,
                                IFramebuffer& destination,
                                uint32_t      sourceAttachment,
                                uint32_t      destinationAttachment,
                                IShader&      shader,
                                bool          srgb)
  {
    auto& oglMesh = dynamic_cast<OpenGLMesh&>(*mScreenQuad);

    OpenGLShader& oglShader = shader.IsCompiled()
                                ? dynamic_cast<OpenGLShader&>(shader)
                                : dynamic_cast<OpenGLShader&>(*mErrorShader);

    destination.Bind();
    if (srgb)
    {
      glEnable(GL_FRAMEBUFFER_SRGB);
    }
    mStateTracker->SetViewport(0,
                               0,
                               destination.GetSpecification().Width,
                               destination.GetSpecification().Height);

    OpenGLUtilities::CheckOpenGLError("glClear", "OpenGLRendererApi", mLogger);
    mStateTracker->SetShaderProgram(oglShader);

    OpenGLUtilities::CheckOpenGLError(
      "glUseProgram", "OpenGLRendererApi", mLogger);
    /*glActiveTexture(GL_TEXTURE0);
    OpenGLUtilities::CheckOpenGLError(
      "glActiveTexture", "OpenGLRendererApi", mLogger);
    glBindTexture(
      GL_TEXTURE_2D,
      static_cast<GLuint>(
        source.GetColorAttachment(sourceAttachment)->get().GetTextureID()));
    OpenGLUtilities::CheckOpenGLError(
      "glBindTexture", "OpenGLRendererApi", mLogger);
    // shader.SetUniform("hdrTexture", 0);
    shader.SetParameter("hdrTexture", 0);*/

    oglMesh.Bind();
    glDrawElements(GL_TRIANGLES, oglMesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glDrawElements", "OpenGLRendererApi", mLogger);
    if (srgb)
    {
      glDisable(GL_FRAMEBUFFER_SRGB);
    }
    oglMesh.Unbind();
    destination.Unbind();
  }

  void
  OpenGLRendererApi::ApplyPostProcess(IPingPongBuffer& buffer,
                                      ICamera&         camera,
                                      IShader&         shader,
                                      bool             srgb)
  {
    auto&         oglMesh = dynamic_cast<OpenGLMesh&>(*mScreenQuad);
    OpenGLShader& oglShader = shader.IsCompiled()
                                ? dynamic_cast<OpenGLShader&>(shader)
                                : dynamic_cast<OpenGLShader&>(*mErrorShader);

    if (srgb)
    {
      glEnable(GL_FRAMEBUFFER_SRGB);
    }
    mStateTracker->SetViewport(
      0,
      0,
      buffer.GetReadFramebuffer().lock()->GetSpecification().Width,
      buffer.GetReadFramebuffer().lock()->GetSpecification().Height);

    OpenGLUtilities::CheckOpenGLError("glClear", "OpenGLRendererApi", mLogger);
    mStateTracker->SetShaderProgram(oglShader);

    uint32_t textureCount = 0;
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer Unbind", "OpenGLRendererApi", mLogger);

    buffer.GetWriteFramebuffer().lock()->Bind();

    /*shader.SetUniform(
      "uInverseViewProjection",
      glm::inverse(camera.GetProjectionMatrix() * camera.GetViewMatrix()));
    shader.SetUniform("uInverseView", glm::inverse(camera.GetViewMatrix()));
    shader.SetUniform("uCameraPosition",
                      camera.GetProperties().Transform.GetPosition());*/
    shader.SetParameter(
      "uInverseViewProjection",
      glm::inverse(camera.GetProjectionMatrix() * camera.GetViewMatrix()));
    shader.SetParameter("uInverseView", glm::inverse(camera.GetViewMatrix()));
    shader.SetParameter("uCameraPosition",
                        camera.GetProperties().Transform.GetPosition());

    oglMesh.Bind();
    glDrawElements(GL_TRIANGLES, oglMesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glDrawElements", "OpenGLRendererApi", mLogger);
    if (srgb)
    {
      glDisable(GL_FRAMEBUFFER_SRGB);
    }
    oglMesh.Unbind();
    buffer.GetWriteFramebuffer().lock()->Unbind();
  }

  auto
  OpenGLRendererApi::QueryVRAMUsage() -> VRAMUsageBuffer
  {
    VRAMUsageBuffer result{};

    if (GLAD_GL_NVX_gpu_memory_info != 0)
    {
      GLint totalMemoryKb = 0;
      GLint availableMemoryKb = 0;
      glGetIntegerv(GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX,
                    &totalMemoryKb);
      glGetIntegerv(GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX,
                    &availableMemoryKb);

      result.totalMemoryMb = totalMemoryKb / 1024;
      result.usedMemoryMb = (totalMemoryKb - availableMemoryKb) / 1024;
    }
    else
    {
      mLogger->LogDebug(Log("GL_NVX_gpu_memory_info extension not supported.",
                            "OpenGLRendererApi"));
    }

    return result;
  }

  auto
  OpenGLRendererApi::GetMaxSamples() -> int32_t
  {
    GLint maxSamples = 0;
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);

    return maxSamples;
  }
}