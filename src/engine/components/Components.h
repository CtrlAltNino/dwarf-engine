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
        //TransformComponent(entt::entity parentEntity):parent(parentEntity){};
        TransformComponent(glm::vec3 pos)
            : position(pos) {}
        TransformComponent(glm::vec3 pos, glm::vec3 rot)
            : position(pos), rotation(rot) {}
        /*TransformComponent(const glm::mat4& transform)
            : Transform(transform) {}

        operator glm::mat4 () { return Transform; }
        operator const glm::mat4& () const { return Transform; }*/

        // ========== Getters ==========
        
        glm::vec3 getPosition() { return position; }
        glm::vec3 getEulerAngles() { return rotation; }
        glm::mat4 getRotationMatrix() {
            /*return glm::rotate(glm::mat4(1.0f), DEG_2_RAD * rotation.z, glm::vec3(0,0,1))
                * glm::rotate(glm::mat4(1.0f), DEG_2_RAD * rotation.x, glm::vec3(1,0,0))
                * glm::rotate(glm::mat4(1.0f), DEG_2_RAD * rotation.y, glm::vec3(0,1,0));*/
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

        /*void rotate(glm::vec3 axis, float angle){
            float length2 = axis.length() * axis.length();
            if (length2 < FLT_EPSILON)
            {
                return;
            }

            glm::vec3 n = axis * (1.f / sqrtf(length2));
            float s = sinf(angle);
            float c = cosf(angle);
            float k = 1.f - c;

            float xx = n.x * n.x * k + c;
            float yy = n.y * n.y * k + c;
            float zz = n.z * n.z * k + c;
            float xy = n.x * n.y * k;
            float yz = n.y * n.z * k;
            float zx = n.z * n.x * k;
            float xs = n.x * s;
            float ys = n.y * s;
            float zs = n.z * s;

            m[0][0] = xx;
            m[0][1] = xy + zs;
            m[0][2] = zx - ys;
            m[0][3] = 0.f;
            m[1][0] = xy - zs;
            m[1][1] = yy;
            m[1][2] = yz + xs;
            m[1][3] = 0.f;
            m[2][0] = zx + ys;
            m[2][1] = yz - xs;
            m[2][2] = zz;
            m[2][3] = 0.f;
            m[3][0] = 0.f;
            m[3][1] = 0.f;
            m[3][2] = 0.f;
            m[3][3] = 1.f;
        }*/

        // ========== Setters ==========
        
        //void setPosition(glm::vec3 position) { this->position = position; }
        //void setRotation(glm::quat rotation) { this->rotation.setQuaternion(rotation); }
        //void setRotation(glm::vec3 eulerAngles) { this->rotation.setEulerAngles(eulerAngles); }
        /*void setRotation(glm::vec3 eulerAngles) {
            //glm::vec3 delta = eulerAngles - this->eulerAngles;
            //rotate({1,0,0}, delta.x);
            //rotate(glm::inverse(rotation) * glm::vec3(0,1,0), delta.y);
            //rotate({0,0,-1}, delta.z);
            //this->rotation = glm::quat(DEG_2_RAD * this->eulerAngles);
            this->eulerAngles = eulerAngles;
        }*/
        //void setScale(glm::vec3 scale) {this->scale = scale; }

        // ========== Transform Operations ==========
        
        //void translate(glm::vec3 translationVector) { this->position += translationVector; }
        /*void rotate(glm::vec3 eulerAngles) {
            //this->rotation = glm::tquat<float>(DEG_2_RAD * eulerAngles) * this->rotation;
            rotate(glm::vec3(1,0,0), eulerAngles.x);
            rotate(glm::vec3(0,0,-1), eulerAngles.z);
            rotate(glm::vec3(0,1,0), eulerAngles.y);
        }
        void rotate(glm::vec3 rotationAxis, float angle) {
            this->rotation = glm::rotate(this->rotation, DEG_2_RAD * angle, rotationAxis);
            this->eulerAngles = glm::eulerAngles(this->rotation);
        }
        void rotate(glm::quat rotation) {
            this->rotation = rotation * this->rotation;
            this->eulerAngles = glm::eulerAngles(this->rotation);
        }*/
        //void rotateGlobally(glm::vec3 eulerAngles) { /* TODO IMPLEMENT */ }
        //void lookAt() { /* TODO IMPLEMENT */ }
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