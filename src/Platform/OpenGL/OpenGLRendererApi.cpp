#include "pch.h"

#include "Core/Asset/Database/IAssetDatabase.h"
#include "Core/Rendering/RendererApi/IRendererApi.h"
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

  struct SetShaderParameterVisitor
  {
    std::reference_wrapper<OpenGLShader> mShader;
    std::string                          mParameterName;
    std::shared_ptr<IAssetDatabase>      mAssetDatabase;
    std::shared_ptr<IDwarfLogger>        mLogger;
    std::reference_wrapper<uint32_t>     mTextureCount;

    void
    operator()(bool& parameter) const
    {
      mShader.get().SetUniform(mParameterName, parameter);
    }
    void
    operator()(int& parameter) const
    {
      mShader.get().SetUniform(mParameterName, parameter);
    }
    void
    operator()(uint32_t& parameter) const
    {
      mShader.get().SetUniform(mParameterName, parameter);
    }
    void
    operator()(float& parameter) const
    {
      mShader.get().SetUniform(mParameterName, parameter);
    }
    void
    operator()(Texture2DAssetValue& parameter) const
    {
      if (parameter.has_value() && mAssetDatabase->Exists(parameter.value()))
      {
        auto& textureAsset = dynamic_cast<TextureAsset&>(
          mAssetDatabase->Retrieve(*parameter)->GetAsset());
        mShader.get().SetUniform(mParameterName, textureAsset, mTextureCount++);
      }
    }
    void
    operator()(glm::vec2& parameter) const
    {
      mShader.get().SetUniform(mParameterName, parameter);
    }
    void
    operator()(glm::vec3& parameter) const
    {
      mShader.get().SetUniform(mParameterName, parameter);
    }
    void
    operator()(glm::vec4& parameter) const
    {
      mShader.get().SetUniform(mParameterName, parameter);
    }
  };

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

    mStateTracker->SetShaderProgram(shader.GetID());

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
        ParameterValue& shaderParameterValue =
          material.GetShaderParameters()->GetParameter(identifier);
        std::visit(SetShaderParameterVisitor{ .mShader = shader,
                                              .mParameterName = identifier,
                                              .mAssetDatabase = mAssetDatabase,
                                              .mLogger = mLogger,
                                              .mTextureCount = textureCount },
                   shaderParameterValue);
      }
    }

    shader.SetUniform("modelMatrix", modelMatrix);
    shader.SetUniform("viewMatrix", camera.GetViewMatrix());
    shader.SetUniform("projectionMatrix", camera.GetProjectionMatrix());
    shader.SetUniform("_Time", (float)mEditorStats->GetTimeSinceStart());
    shader.SetUniform("viewPosition",
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
    std::shared_ptr<OpenGLComputeShader> shader =
      std::dynamic_pointer_cast<OpenGLComputeShader>(computeShader);
    mStateTracker->SetShaderProgram(shader->GetID());
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
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
  }

  void
  OpenGLRendererApi::Blit(IFramebuffer& source,
                          IFramebuffer& destination,
                          uint32_t      sourceAttachment,
                          uint32_t      destinationAttachment,
                          uint32_t      width,
                          uint32_t      height)
  {
    OpenGLFramebuffer* sourceFB = (OpenGLFramebuffer*)&source;
    OpenGLFramebuffer* destinationFB = (OpenGLFramebuffer*)&destination;
    glBindFramebuffer(GL_READ_FRAMEBUFFER,
                      sourceFB->GetFramebufferRendererID());
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer GL_READ_FRAMEBUFFER", "OpenGLRendererApi", mLogger);
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
                      GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);
    OpenGLUtilities::CheckOpenGLError(
      "glBlitFramebuffer", "OpenGLRendererApi", mLogger);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glBindFramebuffer Unbind", "OpenGLRendererApi", mLogger);
  }

  void
  OpenGLRendererApi::CustomBlit(IFramebuffer&            source,
                                IFramebuffer&            destination,
                                uint32_t                 sourceAttachment,
                                uint32_t                 destinationAttachment,
                                std::shared_ptr<IShader> shader)
  {
    auto& oglMesh = dynamic_cast<OpenGLMesh&>(*mScreenQuad);

    source.Bind();
    GLint srcTexture = 0;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER,
                                          GL_COLOR_ATTACHMENT0 +
                                            sourceAttachment,
                                          GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                          &srcTexture);

    OpenGLUtilities::CheckOpenGLError(
      "glGetFramebufferAttachmentParameteriv", "OpenGLRendererApi", mLogger);

    // mStateTracker->SetDepthTest(false);
    // mStateTracker->SetCullMode(false);

    destination.Bind();
    mStateTracker->SetViewport(0,
                               0,
                               destination.GetSpecification().Width,
                               destination.GetSpecification().Height);
    glClear(GL_COLOR_BUFFER_BIT);

    OpenGLUtilities::CheckOpenGLError("glClear", "OpenGLRendererApi", mLogger);

    OpenGLShader& oglShader = shader->IsCompiled()
                                ? dynamic_cast<OpenGLShader&>(*shader)
                                : dynamic_cast<OpenGLShader&>(*mErrorShader);
    mStateTracker->SetShaderProgram(oglShader.GetID());
    OpenGLUtilities::CheckOpenGLError(
      "glUseProgram", "OpenGLRendererApi", mLogger);
    glActiveTexture(GL_TEXTURE0);
    OpenGLUtilities::CheckOpenGLError(
      "glActiveTexture", "OpenGLRendererApi", mLogger);
    glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(srcTexture));
    OpenGLUtilities::CheckOpenGLError(
      "glBindTexture", "OpenGLRendererApi", mLogger);
    glUniform1i(oglShader.GetUniformLocation("hdrTexture"), 0);
    OpenGLUtilities::CheckOpenGLError(
      "glUniform1i", "OpenGLRendererApi", mLogger);

    oglMesh.Bind();
    glDrawElements(GL_TRIANGLES, oglMesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glDrawElements", "OpenGLRendererApi", mLogger);
    oglMesh.Unbind();
    destination.Unbind();
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
      // std::cout << "GL_NVX_gpu_memory_info extension not supported.\n";
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