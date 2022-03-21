#include"EditorModel.h"

EditorModel::EditorModel(IModelListener* listener) : modelListener(listener), scene("Test scene"){
    //InitScene(listener->GetProjectPath());
}

void EditorModel::InitScene(std::string projectPath) {
    Shader cubeShader = Shader("data/engine/shaders/default/opengl/default");
	Material cubeMat = Material(&cubeShader);
	cubeMat.UpdateShaderInput("useAlbedoMap", true);
	cubeMat.UpdateShaderInput("fogStart", 10.0f);
	cubeMat.UpdateShaderInput("fogEnd", 30.0f);
	cubeMat.UpdateShaderInput("color", {1, 1, 1, 1});
	cubeMat.UpdateShaderInput("fogColor", {0.1f, 0.1f, 0.1f, 0.1f});
	cubeMat.UpdateShaderInput("albedoMap", new Texture((projectPath + "/Assets/textures/popcat.jpg").c_str()));
	Transform cubeTransform = Transform(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0));
	Renderer cubeRenderer = Renderer(&cubeMat, (projectPath + "/Assets/models/cube.obj").c_str());
	Object cube = Object("Popcat cube", &cubeTransform, &cubeRenderer);
	
	Shader monkeyShader = Shader("data/engine/shaders/error/opengl/error");
	Material monkeyMat = Material(&monkeyShader);
	monkeyMat.UpdateShaderInput("useAlbedoMap", true);
	monkeyMat.UpdateShaderInput("fogStart", 10.0f);
	monkeyMat.UpdateShaderInput("fogEnd", 30.0f);
	monkeyMat.UpdateShaderInput("color", {1, 1, 1, 1});
	monkeyMat.UpdateShaderInput("fogColor", {0.1f, 0.1f, 0.1f, 0.1f});
	Renderer monkeyRenderer = Renderer(&monkeyMat, (projectPath + "/Assets/models/suzanne.obj").c_str());
	Transform monkeyTransform = Transform(glm::vec3(0, 3, 0), glm::vec3(0, 0, 0));
	Object monkey = Object("Monkey", &monkeyTransform, &monkeyRenderer);

	Shader r2d2Shader = Shader("data/engine/shaders/default/opengl/default");
	Material r2d2Mat = Material(&r2d2Shader);
	r2d2Mat.UpdateShaderInput("useAlbedoMap", true);
	r2d2Mat.UpdateShaderInput("useNormalMap", true);
	r2d2Mat.UpdateShaderInput("useSpecularMap", true);
	r2d2Mat.UpdateShaderInput("fogStart", 10.0f);
	r2d2Mat.UpdateShaderInput("fogEnd", 30.0f);
	r2d2Mat.UpdateShaderInput("shininess", 20.0f);
	r2d2Mat.UpdateShaderInput("color", {1, 1, 1, 1});
	r2d2Mat.UpdateShaderInput("fogColor", {0.1f, 0.1f, 0.1f, 0.1f});
	r2d2Mat.UpdateShaderInput("albedoMap", new Texture((projectPath + "/Assets/textures/r2d2_diffuse.png").c_str()));
	r2d2Mat.UpdateShaderInput("specularMap", new Texture((projectPath + "/Assets/textures/r2d2_specular.png").c_str()));
	r2d2Mat.UpdateShaderInput("normalMap", new Texture((projectPath + "/Assets/textures/r2d2_normalmap.png").c_str(), GL_LINEAR, GL_REPEAT, GL_RGBA, GL_UNSIGNED_BYTE));
	Renderer r2d2Renderer = Renderer(&r2d2Mat, (projectPath + "/Assets/models/r2d2.obj").c_str());
	Transform r2d2Transform = Transform(glm::vec3(3, 0, 0), glm::vec3(0, 0, 0));
	Object r2d2 = Object("R2D2", &r2d2Transform, &r2d2Renderer);
	r2d2.transform.setScale(glm::vec3(0.3, 0.3, 0.3));

	Shader sfShader = Shader("data/engine/shaders/default/opengl/default");
	Material sfMat = Material(&sfShader);
	sfMat.UpdateShaderInput("fogStart", 10.0f);
	sfMat.UpdateShaderInput("fogEnd", 30.0f);
	sfMat.UpdateShaderInput("shininess", 20.0f);
	sfMat.UpdateShaderInput("useAlbedoMap", true);
	sfMat.UpdateShaderInput("useNormalMap", true);
	sfMat.UpdateShaderInput("useSpecularMap", true);
	sfMat.UpdateShaderInput("color", {1, 1, 1, 1});
	sfMat.UpdateShaderInput("fogColor", {0.1f, 0.1f, 0.1f, 0.1f});
	sfMat.UpdateShaderInput("albedoMap", new Texture((projectPath + "/Assets/textures/spaceship_diffuse.jpg").c_str()));
	sfMat.UpdateShaderInput("specularMap", new Texture((projectPath + "/Assets/textures/spaceship_specular.jpg").c_str()));
	sfMat.UpdateShaderInput("normalMap", new Texture((projectPath + "/Assets/textures/spaceship_normalmap.jpg").c_str()));
	Renderer sfRenderer = Renderer(&sfMat, (projectPath + "/Assets/models/spaceship.fbx").c_str());
	Transform sfTransform = Transform(glm::vec3(17, 2, -20), glm::vec3(-80, -30, 0));
	Object sf = Object("Space Ship", &sfTransform, &sfRenderer);
	sf.transform.setScale(glm::vec3(2, 2, 2));

	Shader gridShader = Shader("data/engine/shaders/grid/opengl/grid");
	Material gridMat = Material(&gridShader);
	gridMat.isTransparent = true;
	Transform gridTransform = Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
	Renderer gridRenderer = Renderer(&gridMat, (projectPath + "/Assets/models/plane.obj").c_str());
	Object grid = Object("Grid", &gridTransform, &gridRenderer);
	grid.transform.setScale(glm::vec3(50, 1, 50));
	
	scene.sceneObjects.push_back(std::move(cube));
	scene.sceneObjects.push_back(std::move(monkey));
	scene.sceneObjects.push_back(std::move(r2d2));
	scene.sceneObjects.push_back(std::move(sf));
	scene.sceneObjects.push_back(std::move(grid));
}

Scene* EditorModel::GetScene() {
	return &scene;
}