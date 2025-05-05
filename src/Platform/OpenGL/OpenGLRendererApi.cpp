#include "pch.hpp"

#include "Core/Asset/Database/AssetComponents.hpp"
#include "Core/Asset/Database/IAssetDatabase.hpp"
#include "Core/Rendering/RendererApi/IRendererApi.hpp"
#include "Core/Rendering/Shader/ShaderParameterCollection/IShaderParameterCollection.hpp"
#include "OpenGLFramebuffer.hpp"
#include "Platform/OpenGL/OpenGLComputeShader.hpp"
#include "Platform/OpenGL/OpenGLFramebuffer.hpp"
#include "Platform/OpenGL/OpenGLMeshBuffer.hpp"
#include "Platform/OpenGL/OpenGLRendererApi.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"
#include "Platform/OpenGL/OpenGLUtilities.hpp"
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

    std::shared_ptr<IMesh> screenQuad = mMeshFactory->CreateFullscreenQuad();
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
  OpenGLRendererApi::RenderIndexed(const IMeshBuffer* mesh,
                                   IMaterial&         material,
                                   ICamera&           camera,
                                   glm::mat4          modelMatrix)
  {
    OpenGLUtilities::CheckOpenGLError(
      "Before rendering", "OpenGLRendererApi", mLogger);
    const auto*   oglMesh = dynamic_cast<const OpenGLMeshBuffer*>(mesh);
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

    for (auto const& identifier :
         material.GetShaderParameters()->GetParameterIdentifiers())
    {
      if (material.GetShaderParameters()->HasParameter(identifier))
      {
        std::visit(
          [&shader, identifier, this](auto&& value)
          {
            using T = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<T, std::optional<UUID>>)
            {
              // std::optional return static_cast<std::optional<UUID>>(value);
              auto uid = static_cast<std::optional<UUID>>(value);

              if (uid.has_value())
              {
                auto& textureAsset = static_cast<TextureAsset&>(
                  mAssetDatabase->Retrieve(uid.value())->GetAsset());
                shader.SetParameter(identifier, textureAsset.GetTexture());
              }
            }
            else
            {
              // return value;
              shader.SetParameter(identifier, value);
            }
          },
          material.GetShaderParameters()->GetParameter(identifier));
      }
    }

    shader.SetParameter("modelMatrix", modelMatrix);
    shader.SetParameter("viewMatrix", camera.GetViewMatrix());
    shader.SetParameter("projectionMatrix", camera.GetProjectionMatrix());
    shader.SetParameter("_Time", (float)mEditorStats->GetTimeSinceStart());
    shader.SetParameter("viewPosition",
                        camera.GetProperties().Transform.GetPosition());

    shader.UploadParameters();

    oglMesh->Bind();

    glDrawElements(GL_TRIANGLES, oglMesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
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
    auto& oglMesh = dynamic_cast<OpenGLMeshBuffer&>(*mScreenQuad);

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

    oglShader.UploadParameters();

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
                                      IShader&         shader,
                                      bool             srgb)
  {
    auto&         oglMesh = dynamic_cast<OpenGLMeshBuffer&>(*mScreenQuad);
    OpenGLShader& oglShader = shader.IsCompiled()
                                ? dynamic_cast<OpenGLShader&>(shader)
                                : dynamic_cast<OpenGLShader&>(*mErrorShader);

    if (srgb)
    {
      glEnable(GL_FRAMEBUFFER_SRGB);
      OpenGLUtilities::CheckOpenGLError(
        "glEnable GL_FRAMEBUFFER_SRGB", "OpenGLRendererApi", mLogger);
    }
    mStateTracker->SetViewport(
      0,
      0,
      buffer.GetReadFramebuffer().lock()->GetSpecification().Width,
      buffer.GetReadFramebuffer().lock()->GetSpecification().Height);
    mStateTracker->SetShaderProgram(oglShader);
    oglShader.UploadParameters();
    buffer.GetWriteFramebuffer().lock()->Bind();
    oglMesh.Bind();
    glDrawElements(GL_TRIANGLES, oglMesh.GetIndexCount(), GL_UNSIGNED_INT, 0);
    OpenGLUtilities::CheckOpenGLError(
      "glDrawElements", "OpenGLRendererApi", mLogger);
    oglMesh.Unbind();

    if (srgb)
    {
      glDisable(GL_FRAMEBUFFER_SRGB);
      OpenGLUtilities::CheckOpenGLError(
        "glDisable GL_FRAMEBUFFER_SRGB", "OpenGLRendererApi", mLogger);
    }
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

  auto
  OpenGLRendererApi::IsAnisoSupported() -> bool
  {
    GLint numExtensions = 0;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);

    for (GLint i = 0; i < numExtensions; ++i)
    {
      const char* ext =
        reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
      if (std::string(ext) == "GL_EXT_texture_filter_anisotropic")
      {
        return true;
      }
    }
    return false;
  }

  auto
  OpenGLRendererApi::GetMaxAnisoLevel() -> uint8_t
  {
    GLfloat maxAniso = 1.0F;
    if (IsAnisoSupported())
    {
      glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
    }
    return static_cast<uint8_t>(maxAniso);
  }
}