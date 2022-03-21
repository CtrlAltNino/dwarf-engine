#include"Material.h"

Material::Material(Shader* shader) : shader(*shader){}

Material::~Material(){
	//shader.Delete();
}

void Material::bind() {
	shader.Activate();
}

void Material::UpdateUniforms(){
	// Bind textures
	char textureInputCounter = 0;
	for(auto const& [key, val] : textures){
		std::cout << key << std::endl;
		glActiveTexture(GL_TEXTURE0+textureInputCounter);
		glBindTexture(GL_TEXTURE_2D, val.ID);
		
		GLuint uniformID = glGetUniformLocation(shader.ID, key.c_str());
		glUniform1i(uniformID, textureInputCounter);
		
		//glBindTexture(GL_TEXTURE_2D, 0);
		//glActiveTexture(0);

		textureInputCounter++;
	}

	GLuint uniformID;
	// Bind booleans
	for(auto const& [key, val] : boolUniforms){
		uniformID = glGetUniformLocation(shader.ID, key.c_str());
		glUniform1f(uniformID, (float)val);
	}

	// Bind integers
	for(auto const& [key, val] : integerUniforms){
		uniformID = glGetUniformLocation(shader.ID, key.c_str());
		glUniform1i(uniformID, val);
	}

	// Bind floats
	for(auto const& [key, val] : floatUniforms){
		uniformID = glGetUniformLocation(shader.ID, key.c_str());
		glUniform1f(uniformID, val);
	}

	// Bind vector2
	for(auto const& [key, val] : vector2Uniforms){
		uniformID = glGetUniformLocation(shader.ID, key.c_str());
		glUniform2f(uniformID, val.x, val.y);
	}

	// Bind vector3
	for(auto const& [key, val] : vector3Uniforms){
		uniformID = glGetUniformLocation(shader.ID, key.c_str());
		glUniform3f(uniformID, val.x, val.y, val.z);
	}

	// Bind vector4
	for(auto const& [key, val] : vector4Uniforms){
		uniformID = glGetUniformLocation(shader.ID, key.c_str());
		glUniform4f(uniformID, val.x, val.y, val.z, val.w);
	}
}

void Material::UpdateMVP(glm::mat4x4 modelMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix){
	GLuint mmID = glGetUniformLocation(shader.ID, "modelMatrix");
	GLuint vmID = glGetUniformLocation(shader.ID, "viewMatrix");
	GLuint pmID = glGetUniformLocation(shader.ID, "projectionMatrix");

	glUniformMatrix4fv(mmID, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(vmID, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(pmID, 1, GL_FALSE, &projectionMatrix[0][0]);
}

void Material::UpdateShaderInput(const char* uniformName, Texture* val){
	textures[uniformName] = *val;
}

void Material::UpdateShaderInput(const char* uniformName, bool val){
	boolUniforms[uniformName] = val;
}

void Material::UpdateShaderInput(const char* uniformName, int val){
	integerUniforms[uniformName] = val;
}

void Material::UpdateShaderInput(const char* uniformName, float val){
	floatUniforms[uniformName] = val;
}

void Material::UpdateShaderInput(const char* uniformName, glm::vec2 val){
	vector2Uniforms[uniformName] = val;
}

void Material::UpdateShaderInput(const char* uniformName, glm::vec3 val){
	vector3Uniforms[uniformName] = val;
}

void Material::UpdateShaderInput(const char* uniformName, glm::vec4 val){
	vector4Uniforms[uniformName] = val;
}

template<typename T>
void Material::RemoveShaderInput(const char* uniformName){
	textures.erase(uniformName);
	boolUniforms.erase(uniformName);
	integerUniforms.erase(uniformName);
	floatUniforms.erase(uniformName);
	vector2Uniforms.erase(uniformName);
	vector3Uniforms.erase(uniformName);
	vector4Uniforms.erase(uniformName);
}

template<>
void Material::RemoveShaderInput<Texture>(const char* uniformName){
	textures.erase(uniformName);
}

template<>
void Material::RemoveShaderInput<bool>(const char* uniformName){
	boolUniforms.erase(uniformName);
}

template<>
void Material::RemoveShaderInput<int>(const char* uniformName){
	integerUniforms.erase(uniformName);
}

template<>
void Material::RemoveShaderInput<float>(const char* uniformName){
	floatUniforms.erase(uniformName);
}

template<>
void Material::RemoveShaderInput<glm::vec2>(const char* uniformName){
	vector2Uniforms.erase(uniformName);
}

template<>
void Material::RemoveShaderInput<glm::vec3>(const char* uniformName){
	vector3Uniforms.erase(uniformName);
}

template<>
void Material::RemoveShaderInput<glm::vec4>(const char* uniformName){
	vector4Uniforms.erase(uniformName);
}