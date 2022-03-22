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

void EditorModel::InitSceneNew(std::string projectPath) {
	Entity cubeEntity = scene.CreateEntity("Popcat cube");
	Shader cubeShader = Shader("data/engine/shaders/default/opengl/default");
	Material cubeMat = Material(&cubeShader);
	cubeMat.UpdateShaderInput("useAlbedoMap", true);
	cubeMat.UpdateShaderInput("fogStart", 10.0f);
	cubeMat.UpdateShaderInput("fogEnd", 30.0f);
	cubeMat.UpdateShaderInput("color", {1, 1, 1, 1});
	cubeMat.UpdateShaderInput("shininess", 20.0f);
	cubeMat.UpdateShaderInput("fogColor", {0.1f, 0.1f, 0.1f, 0.1f});
	cubeMat.UpdateShaderInput("albedoMap", new Texture((projectPath + "/Assets/textures/popcat.jpg").c_str()));
	std::vector<Material> cubeMaterials;
	cubeMaterials.push_back(cubeMat);
	cubeEntity.AddComponent<MeshRendererComponent>(MeshLoader::meshFromFile((projectPath + "/Assets/models/cube.obj").c_str()), cubeMaterials);
	cubeEntity.GetComponent<TransformComponent>().setPosition({0,1,0});


	Entity monkeyEntity = scene.CreateEntity("Monkey");
	Shader monkeyShader = Shader("data/engine/shaders/error/opengl/error");
	Material monkeyMat = Material(&monkeyShader);
	monkeyMat.UpdateShaderInput("useAlbedoMap", true);
	monkeyMat.UpdateShaderInput("fogStart", 10.0f);
	monkeyMat.UpdateShaderInput("fogEnd", 30.0f);
	monkeyMat.UpdateShaderInput("shininess", 20.0f);
	monkeyMat.UpdateShaderInput("color", {1, 1, 1, 1});
	monkeyMat.UpdateShaderInput("fogColor", {0.1f, 0.1f, 0.1f, 0.1f});
	std::vector<Material> monkeyMaterials;
	monkeyMaterials.push_back(monkeyMat);
	monkeyEntity.AddComponent<MeshRendererComponent>(MeshLoader::meshFromFile((projectPath + "/Assets/models/suzanne.obj").c_str()), monkeyMaterials);
	monkeyEntity.GetComponent<TransformComponent>().setPosition({0,3,0});
	monkeyEntity.SetParent(cubeEntity.GetHandle());


	Entity r2d2Entity = scene.CreateEntity("R2D2");
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
	std::vector<Material> r2d2Materials;
	r2d2Materials.push_back(r2d2Mat);
	r2d2Entity.AddComponent<MeshRendererComponent>(MeshLoader::meshFromFile((projectPath + "/Assets/models/r2d2.obj").c_str()), r2d2Materials);
	r2d2Entity.GetComponent<TransformComponent>().setPosition({3,0,0});
	r2d2Entity.GetComponent<TransformComponent>().setScale({0.3,0.3,0.3});


	Entity sfEntity = scene.CreateEntity("Space Ship");
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
	std::vector<Material> sfMaterials;
	sfMaterials.push_back(sfMat);
	sfEntity.AddComponent<MeshRendererComponent>(MeshLoader::meshFromFile((projectPath + "/Assets/models/spaceship.fbx").c_str()), sfMaterials);
	//sf.transform.setScale(glm::vec3(2, 2, 2));
	sfEntity.GetComponent<TransformComponent>().setPosition({17,2,-20});
	sfEntity.GetComponent<TransformComponent>().setRotation({-80, -30, 0});
	sfEntity.GetComponent<TransformComponent>().setScale({2,2,2});


	Entity gridEntity = scene.CreateEntity("Grid");
	Shader gridShader = Shader("data/engine/shaders/grid/opengl/grid");
	Material gridMat = Material(&gridShader);
	gridMat.isTransparent = true;
	std::vector<Material> gridMaterials;
	gridMaterials.push_back(gridMat);
	gridEntity.AddComponent<MeshRendererComponent>(MeshLoader::meshFromFile((projectPath + "/Assets/models/plane.obj").c_str()), gridMaterials);
	//grid.transform.setScale(glm::vec3(50, 1, 50));
	gridEntity.GetComponent<TransformComponent>().setScale({50,1,50});
}

Scene* EditorModel::GetScene() {
	return &scene;
}