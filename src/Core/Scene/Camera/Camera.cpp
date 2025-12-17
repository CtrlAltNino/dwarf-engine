#include "pch.hpp"

#include "Camera.hpp"

namespace Dwarf
{

  // ========== Makros ==========

#define DEFAULT_CAMERA_POSITION glm::vec3(0, 2, 15)
#define DEFAULT_CAMERA_ROTATION glm::vec3(30, 0, 0)

  // ========== Constructors ==========
  Camera::Camera(std::shared_ptr<IDwarfLogger> logger,
                 CameraProperties              properties)
    : mLogger(std::move(logger))
    , mProperties(properties)
  {
    mLogger->LogDebug(Log("Camera created.", "Camera"));
  }

  Camera::Camera(std::shared_ptr<IDwarfLogger> logger,
                 const nlohmann::json&         serializedCameraData)
    : mLogger(std::move(logger))
    , mProperties(CameraProperties())
  {
    if (serializedCameraData.contains("fov"))
    {
      mProperties.Fov = serializedCameraData["fov"];
    }

    if (serializedCameraData.contains("nearPlane"))
    {
      mProperties.NearPlane = serializedCameraData["nearPlane"];
    }

    if (serializedCameraData.contains("farPlane"))
    {
      mProperties.FarPlane = serializedCameraData["farPlane"];
    }

    mLogger->LogDebug(Log("Camera created.", "Camera"));
  }

  Camera::~Camera()
  {
    mLogger->LogDebug(Log("Camera destroyed.", "Camera"));
  }

  auto
  Camera::GetProjectionMatrix(float aspectRatio) const -> glm::mat4x4
  {
    return glm::perspective(glm::radians(mProperties.Fov),
                            aspectRatio,
                            mProperties.NearPlane,
                            mProperties.FarPlane);
  }

  auto
  Camera::GetProperties() -> CameraProperties&
  {
    return mProperties;
  }

  auto
  Camera::Serialize() -> nlohmann::json
  {
    nlohmann::json j;
    j["fov"] = mProperties.Fov;
    j["nearPlane"] = mProperties.NearPlane;
    j["farPlane"] = mProperties.FarPlane;
    return j;
  }
}