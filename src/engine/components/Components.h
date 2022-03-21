#pragma once

#include<vector>
#include<glm/glm.hpp>
#include"../editor/UID.h"
#include"../data structures/Mesh.h"
#include"../rendering/Material.h"

struct IDComponent{
    UID ID;
    IDComponent() = default;
    IDComponent(const IDComponent&) = default;
};

struct TagComponent{
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag)
        : Tag(tag) {}
};

struct TransformComponent{
    glm::mat4 Transform = glm::mat4(1.0f);

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::mat4& transform)
        : Transform(transform) {}

    operator glm::mat4 () { return Transform; }
    operator const glm::mat4& () const { return Transform; }
};

struct LightComponent{
    enum LIGHT_TYPE {DIRECTIONAL, POINT, SPOT};
    LIGHT_TYPE type = DIRECTIONAL;
    glm::vec3 lightColor = glm::vec3(1.0f);
    double attenuation = 4.0f;
    double radius = 15.0f;
    glm::vec3 direction = glm::vec3(0.5773502691896258, -0.5773502691896258, 0.5773502691896258);
    double openingAngle = 33;

    LightComponent() = default;
};

struct MeshRendererComponent{
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    bool canCastShadow;

    MeshRendererComponent() = default;
};