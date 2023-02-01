#pragma once

#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>
#include<glm/gtx/quaternion.hpp>

/// @brief Class representing a rotation in 3D space.
class Rotation {
    #define RAD_2_DEG ((float)(180.0f/std::numbers::pi))
    #define DEG_2_RAD ((float)(std::numbers::pi / 180.0f))
    private:
        /// @brief Euler representation of the rotation.
        glm::vec3 eulerAngles;

        /// @brief Quaternion representation of the rotation.
        glm::quat quaternion;

        /// @brief 
        void updateEulerAngles(){
            glm::vec3 result = RAD_2_DEG * glm::eulerAngles(this->quaternion);

            //this->eulerAngles = result;

            //(x, y, z, w) = (q[0], q[1], q[2], q[3])
            float t0 = +2.0 * (this->quaternion.w * this->quaternion.x + this->quaternion.y * this->quaternion.z);
            float t1 = +1.0 - 2.0 * (this->quaternion.x * this->quaternion.x + this->quaternion.y * this->quaternion.y);
            float roll = atan2(t0, t1);
            float t2 = +2.0 * (this->quaternion.w * this->quaternion.y - this->quaternion.z * this->quaternion.x);
            //t2 = +1.0 if t2 > +1.0 else t2;
            //t2 = -1.0 if t2 < -1.0 else t2;
            t2 = t2 > +1.0 ? +1.0 : t2;
            t2 =  t2 < -1.0 ? -1.0 : t2;
            float pitch = asin(t2);
            float t3 = +2.0 * (this->quaternion.w * this->quaternion.z + this->quaternion.x * this->quaternion.y);
            float t4 = +1.0 - 2.0 * (this->quaternion.y * this->quaternion.y + this->quaternion.z * this->quaternion.z);
            float yaw = atan2(t3, t4);

            this->eulerAngles = RAD_2_DEG * glm::vec3(yaw, pitch, roll);

            this->eulerAngles.x -= this->eulerAngles.z;
            this->eulerAngles.z = 0;

            //return [yaw, pitch, roll]
        }

        void updateQuaternion(){
            this->quaternion = glm::quat(1,0,0,0);
            
            glm::quat xRot = glm::rotate(glm::quat(1,0,0,0), DEG_2_RAD * this->eulerAngles.x, {1,0,0});
            glm::quat yRot = glm::rotate(glm::quat(1,0,0,0), DEG_2_RAD * this->eulerAngles.y, {0,1,0});
            glm::quat zRot = glm::rotate(glm::quat(1,0,0,0), DEG_2_RAD * this->eulerAngles.z, {0,0,-1});

            this->quaternion = zRot * yRot * xRot;
        }
    public:
        Rotation(){
            setEulerAngles({0,0,0});
        }
        Rotation(glm::vec3 eulerAngles){
            setEulerAngles(eulerAngles);
        }
        Rotation(glm::quat quaternion){
            setQuaternion(quaternion);
        }
        
        glm::vec3 getEulerAngles(){
            return this->eulerAngles;
        }
        glm::quat getQuaternion(){
            return this->quaternion;
        }
        glm::mat4 getRotationMatrix(){
            return glm::mat4_cast(quaternion);
        }
        
        void setEulerAngles(glm::vec3 eulerAngles){
            this->eulerAngles = eulerAngles;
            updateQuaternion();
        }
        void setQuaternion(glm::quat quaternion){
            this->quaternion = quaternion;
            updateEulerAngles();
        }

        void rotate(glm::vec3 axis, float angle){
            //this->quaternion = glm::angleAxis(DEG_2_RAD * angle, axis) * this->quaternion;
            this->quaternion = glm::angleAxis(DEG_2_RAD * angle, axis) * this->quaternion;
            updateEulerAngles();
        }
        void rotate(glm::quat quaternion){
            this->quaternion = quaternion * this->quaternion;
            updateEulerAngles();
        }
};