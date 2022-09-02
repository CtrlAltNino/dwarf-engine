#include"EditorModel.h"

EditorModel::EditorModel(IModelListener* listener) : modelListener(listener), scene("Test scene"){
    //InitScene(listener->GetProjectPath());
}

void EditorModel::InitScene(std::string projectPath) {
	Entity cubeEntity = scene.CreateEntity("Popcat cube");
	Shader cubeShader = Shader("data/engine/shaders/default/opengl/default");
	Material cubeMat = Material(&cubeShader);
	cubeMat.SetUniform("useAlbedoMap", true);
	cubeMat.SetUniform("fogStart", 10.0f);
	cubeMat.SetUniform("fogEnd", 30.0f);
	cubeMat.SetUniform("color", {1, 1, 1, 1});
	cubeMat.SetUniform("shininess", 20.0f);
	cubeMat.SetUniform("fogColor", {0.1f, 0.1f, 0.1f, 0.1f});
	cubeMat.SetUniform("albedoMap", new Texture((projectPath + "/Assets/textures/popcat.jpg").c_str()));
	std::vector<Material> cubeMaterials;
	cubeMaterials.push_back(cubeMat);
	cubeEntity.AddComponent<MeshRendererComponent>(MeshLoader::meshFromFile((projectPath + "/Assets/models/cube.obj").c_str()), cubeMaterials);
	cubeEntity.GetComponent<TransformComponent>().setPosition({0,1,0});


	Entity monkeyEntity = scene.CreateEntity("Monkey");
	Shader monkeyShader = Shader("data/engine/shaders/error/opengl/error");
	Material monkeyMat = Material(&monkeyShader);
	monkeyMat.SetUniform("useAlbedoMap", true);
	monkeyMat.SetUniform("fogStart", 10.0f);
	monkeyMat.SetUniform("fogEnd", 30.0f);
	monkeyMat.SetUniform("shininess", 20.0f);
	monkeyMat.SetUniform("color", {1, 1, 1, 1});
	monkeyMat.SetUniform("fogColor", {0.1f, 0.1f, 0.1f, 0.1f});
	std::vector<Material> monkeyMaterials;
	monkeyMaterials.push_back(monkeyMat);
	monkeyEntity.AddComponent<MeshRendererComponent>(MeshLoader::meshFromFile((projectPath + "/Assets/models/suzanne.obj").c_str()), monkeyMaterials);
	monkeyEntity.GetComponent<TransformComponent>().setPosition({0,3,0});
	monkeyEntity.SetParent(cubeEntity.GetHandle());


	Entity r2d2Entity = scene.CreateEntity("R2D2");
	Shader r2d2Shader = Shader("data/engine/shaders/default/opengl/default");
	Material r2d2Mat = Material(&r2d2Shader);
	r2d2Mat.SetUniform("useAlbedoMap", true);
	r2d2Mat.SetUniform("useNormalMap", true);
	r2d2Mat.SetUniform("useSpecularMap", true);
	r2d2Mat.SetUniform("fogStart", 10.0f);
	r2d2Mat.SetUniform("fogEnd", 30.0f);
	r2d2Mat.SetUniform("shininess", 20.0f);
	r2d2Mat.SetUniform("color", {1, 1, 1, 1});
	r2d2Mat.SetUniform("fogColor", {0.1f, 0.1f, 0.1f, 0.1f});
	r2d2Mat.SetUniform("albedoMap", new Texture((projectPath + "/Assets/textures/r2d2_diffuse.png").c_str()));
	r2d2Mat.SetUniform("specularMap", new Texture((projectPath + "/Assets/textures/r2d2_specular.png").c_str()));
	r2d2Mat.SetUniform("normalMap", new Texture((projectPath + "/Assets/textures/r2d2_normalmap.png").c_str(), GL_LINEAR, GL_REPEAT, GL_RGBA, GL_UNSIGNED_BYTE));
	std::vector<Material> r2d2Materials;
	r2d2Materials.push_back(r2d2Mat);
	r2d2Entity.AddComponent<MeshRendererComponent>(MeshLoader::meshFromFile((projectPath + "/Assets/models/r2d2.obj").c_str()), r2d2Materials);
	r2d2Entity.GetComponent<TransformComponent>().setPosition({3,0,0});
	r2d2Entity.GetComponent<TransformComponent>().setScale({0.3,0.3,0.3});


	Entity sfEntity = scene.CreateEntity("Space Ship");
	Shader sfShader = Shader("data/engine/shaders/default/opengl/default");
	Material sfMat = Material(&sfShader);
	sfMat.SetUniform("fogStart", 10.0f);
	sfMat.SetUniform("fogEnd", 30.0f);
	sfMat.SetUniform("shininess", 20.0f);
	sfMat.SetUniform("useAlbedoMap", true);
	sfMat.SetUniform("useNormalMap", true);
	sfMat.SetUniform("useSpecularMap", true);
	sfMat.SetUniform("color", {1, 1, 1, 1});
	sfMat.SetUniform("fogColor", {0.1f, 0.1f, 0.1f, 0.1f});
	sfMat.SetUniform("albedoMap", new Texture((projectPath + "/Assets/textures/spaceship_diffuse.jpg").c_str()));
	sfMat.SetUniform("specularMap", new Texture((projectPath + "/Assets/textures/spaceship_specular.jpg").c_str()));
	sfMat.SetUniform("normalMap", new Texture((projectPath + "/Assets/textures/spaceship_normalmap.jpg").c_str()));
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