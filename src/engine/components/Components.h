#pragma once

#include "../../utilities/dpch.h"

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<entt/entt.hpp>

#include"../editor/UID.h"
#include"../data structures/Mesh.h"
#include"../rendering/Material.h"
#include"../data structures/Quaternion.h"
#include"../scene/Object.h"
#include"../scene/Camera.h"

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
	private:
        glm::vec3 position = glm::vec3(0), scale = glm::vec3(1);
        Quaternion rotation = Quaternion::getIdentity();

    public:
        entt::entity parent = entt::null;
        std::vector<entt::entity> children;
        
        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(glm::vec3 pos)
            : position(pos) {}
        TransformComponent(glm::vec3 pos, Quaternion quat)
            : position(pos), rotation(quat) {}
        TransformComponent(glm::vec3 pos, glm::vec3 rot)
            : position(pos), rotation(Quaternion(rot)) {}
        /*TransformComponent(const glm::mat4& transform)
            : Transform(transform) {}

        operator glm::mat4 () { return Transform; }
        operator const glm::mat4& () const { return Transform; }*/

        // ========== Getters ==========
        
        glm::vec3 getPosition() { return position; }
        Quaternion getRotation() { return rotation; }
        glm::vec3 getScale() { return scale; }
        glm::vec3 getForward() { return rotation * glm::vec3(0, 0, -1); }
        glm::vec3 getUp() { return rotation * glm::vec3(0, 1, 0); }
        glm::vec3 getRight() { return rotation * glm::vec3(1, 0, 0); }
        glm::mat4x4 getModelMatrix(){
            // TODO Fix scale and rotation part of the model matrix
            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
            glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
            //glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotation.GetAngle(), rotation.GetAxis());
            glm::mat4 rotationMatrix = rotation.getMatrix();
            //std::cout << "Angle: " << rotation.GetAngle() << " | Axis: [" << rotation.GetAxis().x << ", " << rotation.GetAxis().y << ", " << rotation.GetAxis().z << "]" << std::endl;
            //std::cout << "Quaterion: X = " << rotation.x << " | Y = " << rotation.y << " | Z = " << rotation.z << " | R = " << rotation.r << std::endl;
            //std::cout << "Euler Angles: X = " << rotation.GetEulerAngles().x << " | Y = " << rotation.GetEulerAngles().y << " | Z = " << rotation.GetEulerAngles().z << std::endl;
            /*float angle = glm::radians(15.0f);
            glm::mat4 yeet = {
                cos(angle), 0, -sin(angle), 0,
                0, 1, 0, 0,
                sin(angle), 0, cos(angle), 0,
                0, 0, 0, 1
            };*/

            return translationMatrix * rotationMatrix * scaleMatrix;
        }

        // ========== Setters ==========
        
        void setPosition(glm::vec3 position) { this->position = position; }
        void setRotation(Quaternion rotation) { this->rotation = rotation; }
        void setRotation(glm::vec3 eulerAngles) {this->rotation = Quaternion(eulerAngles); }
        void setScale(glm::vec3 scale) {this->scale = scale; }

        // ========== Transform Operations ==========
        
        void reset(){
            setPosition(glm::vec3(0));
            setRotation(glm::vec3(0));
            setScale(glm::vec3(1));
        }
        void translate(glm::vec3 translationVector) { this->position += translationVector; }
        void rotate(glm::vec3 eulerAngles) { this->rotation.rotate(eulerAngles); }
        void rotate(glm::vec3 rotationAxis, float angle) { this->rotation.rotate(rotationAxis, angle); }
        void rotateGlobally(glm::vec3 eulerAngles) { /* TODO IMPLEMENT */ }
        void lookAt() { /* TODO IMPLEMENT */ }
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
    MeshRendererComponent(const MeshRendererComponent&) = default;
    MeshRendererComponent(std::vector<Mesh> meshes, std::vector<Material> materials)
        : meshes(meshes), materials(materials) {}
};