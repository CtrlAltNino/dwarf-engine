#include "dpch.h"
#include"Core/Rendering/Material.h"

namespace Dwarf {

	Ref<Material> Material::s_DefaultMaterial = nullptr;
	Ref<Material> Material::s_ErrorMaterial = nullptr;
	Ref<Material> Material::s_GridMaterial = nullptr;

	void Material::Init(){
		s_DefaultMaterial = CreateRef<Material>("Default Material", Shader::s_DefaultShader);
		s_ErrorMaterial = CreateRef<Material>("Error Material", Shader::s_ErrorShader);
		s_GridMaterial = CreateRef<Material>("grid material", Shader::s_GridShader);
		s_GridMaterial->SetTransparency(true);
	}

	Material::Material(std::string name) : m_Name(name) {
		m_Shader = Shader::s_DefaultShader;
	}

	Material::Material(std::string name, Ref<Shader> shader) : m_Name(name), m_Shader(shader){}

	Material::~Material(){ }

	std::string Material::GetName(){
		return m_Name;
	}

	void Material::SetShader(Ref<Shader> shader){
		m_Shader = shader;
	}

	Ref<Shader> Material::GetShader(){
		return m_Shader;
	}

	/*void Material::bind() {
		shader->Activate();
	}*/

	/*void Material::UpdateUniforms(){
		// Bind textures
		char textureInputCounter = 0;
		for(auto const& [key, val] : m_Textures){
			// std::cout << key << std::endl;
			glActiveTexture(GL_TEXTURE0+textureInputCounter);
			glBindTexture(GL_TEXTURE_2D, val.ID);

			GLuint uniformID = glGetUniformLocation(shader->ID, key.c_str());
			glUniform1i(uniformID, textureInputCounter);

			//glBindTexture(GL_TEXTURE_2D, 0);
			//glActiveTexture(0);

			textureInputCounter++;
		}

		GLuint uniformID;
		// Bind booleans
		for(auto const& [key, val] : m_BoolUniforms){
			uniformID = glGetUniformLocation(shader->ID, key.c_str());
			glUniform1f(uniformID, (float)val);
		}

		// Bind integers
		for(auto const& [key, val] : m_IntegerUniforms){
			uniformID = glGetUniformLocation(shader->ID, key.c_str());
			glUniform1i(uniformID, val);
		}

		// Bind floats
		for(auto const& [key, val] : floatUniforms){
			uniformID = glGetUniformLocation(shader->ID, key.c_str());
			glUniform1f(uniformID, val);
		}

		// Bind vector2
		for(auto const& [key, val] : m_Vector2Uniforms){
			uniformID = glGetUniformLocation(shader->ID, key.c_str());
			glUniform2f(uniformID, val.x, val.y);
		}

		// Bind vector3
		for(auto const& [key, val] : m_Vector3Uniforms){
			uniformID = glGetUniformLocation(shader->ID, key.c_str());
			glUniform3f(uniformID, val.x, val.y, val.z);
		}

		// Bind vector4
		for(auto const& [key, val] : m_Vector4Uniforms){
			uniformID = glGetUniformLocation(shader->ID, key.c_str());
			glUniform4f(uniformID, val.x, val.y, val.z, val.w);
		}
	}*/

	/*void Material::UpdateMVP(glm::mat4x4 modelMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix){
		GLuint mmID = glGetUniformLocation(shader->ID, "modelMatrix");
		GLuint vmID = glGetUniformLocation(shader->ID, "viewMatrix");
		GLuint pmID = glGetUniformLocation(shader->ID, "projectionMatrix");

		glUniformMatrix4fv(mmID, 1, GL_FALSE, &modelMatrix[0][0]);
		glUniformMatrix4fv(vmID, 1, GL_FALSE, &viewMatrix[0][0]);
		glUniformMatrix4fv(pmID, 1, GL_FALSE, &projectionMatrix[0][0]);
	}*/

	void Material::SetTexture(std::string uniformName, Ref<UID> val){
		m_Textures[uniformName] = val;
	}

	void Material::SetUniform(std::string uniformName, bool val){
		m_BoolUniforms[uniformName] = val;
	}

	void Material::SetUniform(std::string uniformName, int val){
		m_IntegerUniforms[uniformName] = val;
	}

	void Material::SetUniform(std::string uniformName, float val){
		m_FloatUniforms[uniformName] = val;
	}

	void Material::SetUniform(std::string uniformName, glm::vec2 val){
		m_Vector2Uniforms[uniformName] = val;
	}

	void Material::SetUniform(std::string uniformName, glm::vec3 val){
		m_Vector3Uniforms[uniformName] = val;
	}

	void Material::SetUniform(std::string uniformName, glm::vec4 val){
		m_Vector4Uniforms[uniformName] = val;
	}

	template<typename T>
	void Material::RemoveShaderInput(std::string uniformName){
		m_Textures.erase(uniformName);
		m_BoolUniforms.erase(uniformName);
		m_IntegerUniforms.erase(uniformName);
		m_FloatUniforms.erase(uniformName);
		m_Vector2Uniforms.erase(uniformName);
		m_Vector3Uniforms.erase(uniformName);
		m_Vector4Uniforms.erase(uniformName);
	}

	template<>
	void Material::RemoveShaderInput<Texture>(std::string uniformName){
		m_Textures.erase(uniformName);
	}

	template<>
	void Material::RemoveShaderInput<bool>(std::string uniformName){
		m_BoolUniforms.erase(uniformName);
	}

	template<>
	void Material::RemoveShaderInput<int>(std::string uniformName){
		m_IntegerUniforms.erase(uniformName);
	}

	template<>
	void Material::RemoveShaderInput<float>(std::string uniformName){
		m_FloatUniforms.erase(uniformName);
	}

	template<>
	void Material::RemoveShaderInput<glm::vec2>(std::string uniformName){
		m_Vector2Uniforms.erase(uniformName);
	}

	template<>
	void Material::RemoveShaderInput<glm::vec3>(std::string uniformName){
		m_Vector3Uniforms.erase(uniformName);
	}

	template<>
	void Material::RemoveShaderInput<glm::vec4>(std::string uniformName){
		m_Vector4Uniforms.erase(uniformName);
	}

	void Material::SetTransparency(bool transparent){
		m_Transparent = transparent;
	}

	bool Material::IsTransparent(){
		return m_Transparent;
	}

	std::map<std::string, Ref<UID>> Material::GetTextures(){
		return m_Textures;
	}

	/// @brief Boolean uniforms.
	std::map<std::string, bool> Material::GetBoolUniforms(){
		return m_BoolUniforms;
	}

	/// @brief Integer uniforms.
	std::map<std::string, int> Material::GetIntegerUniforms(){
		return m_IntegerUniforms;
	}

	/// @brief Float uniforms.
	std::map<std::string, float> Material::GetFloatUniforms(){
		return m_FloatUniforms;
	}

	/// @brief 2D vector uniforms.
	std::map<std::string, glm::vec2> Material::Get2DUniforms(){
		return m_Vector2Uniforms;
	}

	/// @brief 3D vector uniforms.
	std::map<std::string, glm::vec3> Material::Get3DUniforms(){
		return m_Vector3Uniforms;
	}

	/// @brief 4D vector uniforms.
	std::map<std::string, glm::vec4> Material::Get4DUniforms(){
		return m_Vector4Uniforms;
	}

	void Material::GenerateShaderInputs(){
		// Get all shader inputs from abstract Shader function and put them in the maps
	}
}