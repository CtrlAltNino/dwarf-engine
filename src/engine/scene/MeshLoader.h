#pragma once

#include "../../utilities/dpch.h"

//#include<glad/glad.h>
#include<glm/glm.hpp>
// Material abstrahieren
#include<assimp/cimport.h>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include<assimp/material.h>

#include"../data structures/Mesh.h"

class MeshLoader {
    public:
        static std::vector<Mesh> meshFromFile(const char* fileName){
            const aiScene* scene = aiImportFile(fileName, aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs);
            const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

            if ((scene == nullptr) || !scene) {
                std::cerr << "Could not load file " << fileName << ": " << aiGetErrorString() << std::endl;
            }

            std::vector<Mesh> meshes;
            meshes.reserve(scene->mNumMeshes);

            for (int meshIdx = 0; meshIdx < scene->mNumMeshes; meshIdx++) {
                aiMesh* mesh = scene->mMeshes[meshIdx];

                std::vector<Vertex> vertices;
                vertices.reserve(mesh->mNumVertices);

                for (int vertIdx = 0; vertIdx < mesh->mNumVertices; vertIdx++) {
                    aiVector3D pos = mesh->mVertices[vertIdx];
                    aiVector3D norm = mesh->mNormals[vertIdx];

                    aiVector3D aiTangent = mesh->HasTangentsAndBitangents() ? mesh->mTangents[vertIdx] : Zero3D;
                    aiVector3D aiBiTangent = mesh->HasTangentsAndBitangents() ? mesh->mBitangents[vertIdx] : Zero3D;

                    aiVector3D uv = mesh->HasTextureCoords(0) ? mesh->mTextureCoords[0][vertIdx] : Zero3D;
                    
                    vertices.push_back(
                        {
                            glm::vec3(pos.x, pos.y, pos.z),
                            glm::vec3(norm.x, norm.y, norm.z),
                            glm::vec3(aiTangent.x, aiTangent.y, aiTangent.z),
                            glm::vec3(aiBiTangent.x, aiBiTangent.y, aiBiTangent.z),
                            glm::vec2(uv.x,uv.y)
                        }
                    );
                }

                if (!mesh->HasTangentsAndBitangents()) {
                    for (int vertIdx = 0; vertIdx < vertices.size(); vertIdx += 3) {
                        // Shortcuts for vertices
                        glm::vec3 v0 = vertices[vertIdx + 0].pos;
                        glm::vec3 v1 = vertices[vertIdx + 1].pos;
                        glm::vec3 v2 = vertices[vertIdx + 2].pos;

                        // Shortcuts for UVs
                        glm::vec2 uv0 = vertices[vertIdx + 0].uv;
                        glm::vec2 uv1 = vertices[vertIdx + 1].uv;
                        glm::vec2 uv2 = vertices[vertIdx + 2].uv;

                        // Edges of the triangle : position delta
                        glm::vec3 deltaPos1 = v1 - v0;
                        glm::vec3 deltaPos2 = v2 - v0;

                        // UV delta
                        glm::vec2 deltaUV1 = uv1 - uv0;
                        glm::vec2 deltaUV2 = uv2 - uv0;

                        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
                        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
                        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

                        vertices[vertIdx].tangent = tangent;
                        vertices[vertIdx+1].tangent = tangent;
                        vertices[vertIdx+2].tangent = tangent;
                        
                        vertices[vertIdx].biTangent = bitangent;
                        vertices[vertIdx + 1].biTangent = bitangent;
                        vertices[vertIdx + 2].biTangent = bitangent;
                    }
                }

                std::vector<GLuint> indices;
                indices.reserve(mesh->mNumFaces * 3);
                for (int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++) {
                    indices.push_back(mesh->mFaces[faceIdx].mIndices[0]);
                    indices.push_back(mesh->mFaces[faceIdx].mIndices[1]);
                    indices.push_back(mesh->mFaces[faceIdx].mIndices[2]);
                }
                
                meshes.push_back(Mesh(vertices, indices));
            }

            aiReleaseImport(scene);
            return meshes;
        }
};