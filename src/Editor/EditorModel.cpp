#include "dpch.h"

#include "Editor/EditorModel.h"

namespace Dwarf
{
	EditorModel::EditorModel(std::string name, std::filesystem::path projectPath) : m_ProjectName(name), m_ProjectPath(projectPath) {}

	Ref<Scene> EditorModel::GetScene()
	{
		return this->m_Scene;
	}

	void EditorModel::SetScene(Ref<Scene> scene)
	{
		this->m_Scene = scene;
	}

	std::string EditorModel::GetName()
	{
		return m_ProjectName;
	}

	std::filesystem::path EditorModel::GetProjectPath()
	{
		return this->m_ProjectPath;
	}

	void EditorModel::SetDeltaTime(double deltaTime)
	{
		this->m_DeltaTime = deltaTime;
	}

	double EditorModel::GetDeltaTime()
	{
		return this->m_DeltaTime;
	}
}