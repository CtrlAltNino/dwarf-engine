#pragma once

#include "../../utilities/dpch.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtx/quaternion.hpp>
#include<entt/entt.hpp>

#include"../editor/UID.h"
#include"../data structures/Mesh.h"
//#include"../data structures/Rotation.h"
#include"../rendering/Material.h"
//#include"../data structures/Quaternion.h"
//#include"../scene/Object.h"

enum LIGHT_TYPE {DIRECTIONAL, POINT_LIGHT, SPOT_LIGHT};
static const char* lightTypeNames[3] = {"Directional", "Point", "Spot"};

struct IDComponent{
    UID ID;
    IDComponent() = default;
    IDComponent(const IDComponent&) = default;
    IDComponent(const UID& other){
        ID = other;
    };
};

struct TagComponent{
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag)
        : Tag(tag) {}
};

struct TransformComponent{
    #define RAD_2_DEG ((float)(180.0f/std::_Pi))
    #define DEG_2_RAD ((float)(std::_Pi / 180.0f))
    public:
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
        entt::entity parent = entt::null;
        std::vector<entt::entity> children;
        
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(glm::vec3 pos)
            : position(pos) {}
        TransformComponent(glm::vec3 pos, glm::vec3 rot)
            : position(pos), rotation(rot) {}

        // ========== Getters ==========
        
        glm::vec3 getPosition() { return position; }
        glm::vec3 getEulerAngles() { return rotation; }
        glm::mat4 getRotationMatrix() {
            return glm::toMat4(glm::quat(DEG_2_RAD * rotation));
        }
        glm::vec3 getScale() { return scale; }
        glm::vec3 getForward() { return getRotationMatrix() * glm::vec4(0, 0, -1, 1); }
        glm::vec3 getUp() { return getRotationMatrix() * glm::vec4(0, 1, 0, 1); }
        glm::vec3 getRight() { return getRotationMatrix() * glm::vec4(1, 0, 0, 1); }
        glm::mat4x4 getModelMatrix(){
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
            glm::mat4 rotationMatrix = getRotationMatrix();

            return translationMatrix * rotationMatrix * scaleMatrix;
        }
};

struct LightComponent{
    LIGHT_TYPE type = DIRECTIONAL;
    glm::vec3 lightColor = glm::vec3(1.0f);
    float attenuation = 4.0f;
    float radius = 15.0f;
    float openingAngle = 33;

    LightComponent() = default;
};

struct MeshRendererComponent{
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    bool canCastShadow;

    MeshRendererComponent() = default;
    MeshRendererComponent(const MeshRendererComponent&) = default;
    MeshRendererComponent(std::vector<Mesh> meshes, std::vector<Material> materials)
        : meshes(meshes), materials(materials) {}
};