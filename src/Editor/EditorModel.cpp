#include "dpch.h"

#include "Editor/EditorModel.h"

namespace Dwarf
{
	EditorModel::EditorModel(std::string_view name, std::filesystem::path const &projectPath) : m_ProjectName(name), m_ProjectPath(projectPath) {}

	Ref<Scene> EditorModel::GetScene() const
	{
		return m_Scene;
	}

	void EditorModel::SetScene(Ref<Scene> scene)
	{
		m_Scene = scene;
	}

	std::string EditorModel::GetName() const
	{
		return m_ProjectName;
	}

	std::filesystem::path EditorModel::GetProjectPath() const
	{
		return m_ProjectPath;
	}

	void EditorModel::SetDeltaTime(double deltaTime)
	{
		m_DeltaTime = deltaTime;
	}

	double EditorModel::GetDeltaTime() const
	{
		return m_DeltaTime;
	}

	bool EditorModel::GetCloseSignal() const
	{
		return m_CloseSignal;
	}

	bool EditorModel::GetReturnToLauncher() const
	{
		return m_ReturnToLauncher;
	}

	SelectionContainer &EditorModel::GetSelection()
	{
		return m_Selection;
	}

	void EditorModel::CloseEditor(bool returnToLauncher)
	{
		m_CloseSignal = true;
		m_ReturnToLauncher = returnToLauncher;
	}
}