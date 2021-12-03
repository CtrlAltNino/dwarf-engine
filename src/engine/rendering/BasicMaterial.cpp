#include"BasicMaterial.h"

// ========== Constructors ==========

BasicMaterial::BasicMaterial(const char* shaderName)
	: shader(shaderName)
	{
	//GLuint tex0Uni = glGetUniformLocation(this->shader.ID, "tex0");
	//glUniform1i(tex0Uni, 0);
	//albedo(texturePath, GL_NEAREST, GL_REPEAT, GL_RGB, GL_UNSIGNED_BYTE)
}

// ========== Getters ==========

Color BasicMaterial::getColor() {
	return color;
}

Shader BasicMaterial::getShader() {
	return shader;
}

Texture BasicMaterial::getAlbedoMap() {
	return *albedoMap;
}

Texture BasicMaterial::getSpecularMap() {
	return *specularMap;
}

Texture BasicMaterial::getNormalMap() {
	return *normalMap;
}

// ========== Setters ==========

void BasicMaterial::setColor(Color color) {
	this->color = color;
}

void BasicMaterial::setColor(float r, float g, float b) {
	this->color = Color(r, g, b, 1);
}

void BasicMaterial::setColor(float r, float g, float b, float a) {
	this->color = Color(r, g, b, a);
}

void BasicMaterial::setShader(Shader shader) {
	this->shader = shader;
}

void BasicMaterial::setAlbedoMap(const char* texturePath) {
	if (std::regex_search(texturePath, std::regex(".png$"))) {
		this->albedoMap = new Texture(texturePath, GL_LINEAR, GL_REPEAT, GL_RGB, GL_UNSIGNED_BYTE);
	}
	else if(std::regex_search(texturePath, std::regex(".jpg$"))){
		this->albedoMap = new Texture(texturePath, GL_LINEAR, GL_REPEAT, GL_RGB, GL_UNSIGNED_BYTE);
	}
}

void BasicMaterial::setAlbedoMap(Texture texture) {
	this->albedoMap = &texture;
}

void BasicMaterial::setSpecularMap(const char* texturePath) {
	this->specularMap = new Texture(texturePath, GL_LINEAR, GL_REPEAT, GL_RGB, GL_UNSIGNED_BYTE);
}

void BasicMaterial::setSpecularMap(Texture texture) {
	this->specularMap = &texture;
}

void BasicMaterial::setNormalMap(const char* texturePath) {
	if (std::regex_search(texturePath, std::regex(".png$"))) {
		this->normalMap = new Texture(texturePath, GL_LINEAR, GL_REPEAT, GL_RGBA, GL_UNSIGNED_BYTE);
	}
	else if (std::regex_search(texturePath, std::regex(".jpg$"))) {
		this->normalMap = new Texture(texturePath, GL_LINEAR, GL_REPEAT, GL_RGB, GL_UNSIGNED_BYTE);
	}
}

void BasicMaterial::setNormalMap(Texture texture) {
	this->normalMap = &texture;
}

// ========== Material Functions ==========

void BasicMaterial::bind() {
	shader.Activate();
}

void BasicMaterial::free() {
	shader.Delete();

	albedoMap->Delete();
	specularMap->Delete();
	normalMap->Delete();
}

void BasicMaterial::updateShaderParameters(glm::mat4x4 modelMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix) {
	// TODO implement completely
	GLuint mmID = glGetUniformLocation(shader.ID, "modelMatrix");
	GLuint vmID = glGetUniformLocation(shader.ID, "viewMatrix");
	GLuint pmID = glGetUniformLocation(shader.ID, "projectionMatrix");
	GLuint colorId = glGetUniformLocation(shader.ID, "color");
	GLuint shininessId = glGetUniformLocation(shader.ID, "shininess");
	GLuint fogStartID = glGetUniformLocation(shader.ID, "fogStart");
	GLuint fogEndID = glGetUniformLocation(shader.ID, "fogEnd");
	GLuint fogColorID = glGetUniformLocation(shader.ID, "fogColor");

	GLuint albedoUni = glGetUniformLocation(this->shader.ID, "useAlbedoMap");
	GLuint specularUni = glGetUniformLocation(this->shader.ID, "useSpecularMap");
	GLuint normalUni = glGetUniformLocation(this->shader.ID, "useNormalMap");
	
	if (albedoMap != nullptr) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, albedoMap->ID);
		GLuint tex0Uni = glGetUniformLocation(this->shader.ID, "albedoMap");
		glUniform1i(tex0Uni, 0);
		glUniform1f(albedoUni, 1);
	}
	else {
		glUniform1f(albedoUni, 0);
	}

	if (specularMap != nullptr) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap->ID);
		GLuint tex1Uni = glGetUniformLocation(this->shader.ID, "specularMap");
		glUniform1i(tex1Uni, 1);
		glUniform1f(specularUni, 1);
	}
	else {
		glUniform1f(specularUni, 0);
	}

	if (normalMap != nullptr) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, normalMap->ID);
		GLuint tex2Uni = glGetUniformLocation(this->shader.ID, "normalMap");
		glUniform1i(tex2Uni, 2);
		glUniform1f(normalUni, 1);
	}
	else {
		glUniform1f(normalUni, 0);
	}

	std::cout << settings.fogColor.x << std::endl;
	
	glUniform1f(fogStartID, settings.fogStart);
	glUniform1f(fogEndID, settings.fogEnd);
	glUniform4f(fogColorID, settings.fogColor.x, settings.fogColor.y, settings.fogColor.z, settings.fogColor.w);
	glUniform1f(shininessId, settings.shininess);
	glUniform4f(colorId, color.r, color.g, color.b, color.a);
	glUniformMatrix4fv(mmID, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(vmID, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(pmID, 1, GL_FALSE, &projectionMatrix[0][0]);
}