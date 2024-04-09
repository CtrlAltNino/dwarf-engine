#include "dpch.h"

#include <utility>

#include "OpenGLShader.h"
#include "Utilities/FileHandler.h"
#include "Core/Asset/AssetDatabase.h"
#include "Core/Asset/AssetComponents.h"

#define GL_SHADER_LOG_LENGTH (1024)

namespace Dwarf
{
	OpenGLShader::OpenGLShader() = default;
	OpenGLShader::~OpenGLShader() = default;

	const std::array<std::string, 3> OpenGLShader::ReservedUniformNames = {
		"modelMatrix",
		"viewMatrix",
		"projectionMatrix",
	};

	const std::map<GLenum, ShaderParameterType> glTypeToDwarfShaderType =
		{
			{GL_BOOL, ShaderParameterType::BOOLEAN},
			{GL_INT, ShaderParameterType::INTEGER},
			{GL_UNSIGNED_INT, ShaderParameterType::UNSIGNED_INTEGER},
			{GL_FLOAT, ShaderParameterType::FLOAT},
			{GL_FLOAT_VEC2, ShaderParameterType::VEC2},
			{GL_FLOAT_VEC3, ShaderParameterType::VEC3},
			{GL_FLOAT_VEC4, ShaderParameterType::VEC4},
			{GL_SAMPLER_2D, ShaderParameterType::TEX2D}};

	void OpenGLShader::Compile()
	{
		SetIsCompiled(false);

		std::string vertexShaderSource;
		std::string tescShaderSource;
		std::string teseShaderSource;
		std::string geometryShaderSource;
		std::string fragmentShaderSource;

		if (m_ShaderAssets.m_VertexShaderAsset != nullptr && AssetDatabase::Exists(m_ShaderAssets.m_VertexShaderAsset))
		{
			std::filesystem::path vertexShaderPath = AssetDatabase::Retrieve<VertexShaderAsset>(m_ShaderAssets.m_VertexShaderAsset)->GetAsset()->m_Path;
			vertexShaderSource = FileHandler::ReadFile(vertexShaderPath);
			// TODO: This needs to move somewhere else
			AssetDatabase::AddShaderWatch(vertexShaderPath, CreateRef<OpenGLShader>(*this));
		}
		else
		{
			vertexShaderSource = m_ShaderSources.m_VertexShaderSource;
		}

		if (AssetDatabase::Exists(m_ShaderAssets.m_TessellationControlShaderAsset))
		{
			tescShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<TesselationControlShaderAsset>(m_ShaderAssets.m_TessellationControlShaderAsset)->GetAsset()->m_Path);
		}

		if (AssetDatabase::Exists(m_ShaderAssets.m_TessellationEvaluationShaderAsset))
		{
			teseShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<TesselationEvaluationShaderAsset>(m_ShaderAssets.m_TessellationEvaluationShaderAsset)->GetAsset()->m_Path);
		}

		if (AssetDatabase::Exists(m_ShaderAssets.m_GeometryShaderAsset))
		{
			geometryShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<GeometryShaderAsset>(m_ShaderAssets.m_GeometryShaderAsset)->GetAsset()->m_Path);
		}

		if (m_ShaderAssets.m_FragmentShaderAsset != nullptr && AssetDatabase::Exists(m_ShaderAssets.m_FragmentShaderAsset))
		{
			std::filesystem::path fragmentShaderPath = AssetDatabase::Retrieve<FragmentShaderAsset>(m_ShaderAssets.m_FragmentShaderAsset)->GetAsset()->m_Path;
			fragmentShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<FragmentShaderAsset>(m_ShaderAssets.m_FragmentShaderAsset)->GetAsset()->m_Path);
			AssetDatabase::AddShaderWatch(fragmentShaderPath, CreateRef<OpenGLShader>(*this));
		}
		else
		{
			fragmentShaderSource = m_ShaderSources.m_FragmentShaderSource;
		}

		if (vertexShaderSource.length() > 0 && fragmentShaderSource.length() > 0)
		{
			const char *vertexSource = vertexShaderSource.c_str();
			const char *fragmentSource = fragmentShaderSource.c_str();

			GLsizei vert_log_length = 0;
			GLchar vert_message[1024] = "";

			GLsizei frag_log_length = 0;
			GLchar frag_message[1024] = "";

			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexSource, nullptr);
			glCompileShader(vertexShader);

			GLint vertex_compiled;
			glGetShaderInfoLog(vertexShader, 1024, &vert_log_length, vert_message);
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertex_compiled);

			if (vert_log_length > 0)
			{
				m_ShaderLogs.m_VertexShaderLog = std::string(vert_message);
			}
			if (vertex_compiled != GL_TRUE)
			{
				glDeleteShader(vertexShader);
				return;
			}

			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
			glCompileShader(fragmentShader);

			GLint fragment_compiled;
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragment_compiled);
			glGetShaderInfoLog(fragmentShader, 1024, &frag_log_length, frag_message);

			if (frag_log_length > 0)
			{
				m_ShaderLogs.m_FragmentShaderLog = std::string(frag_message);
			}

			if (fragment_compiled != GL_TRUE)
			{
				glDeleteShader(vertexShader);
				glDeleteShader(fragmentShader);
				return;
			}

			m_ID = glCreateProgram();

			glAttachShader(m_ID, vertexShader);
			glAttachShader(m_ID, fragmentShader);

			GLuint geometryShader = -1;

			if (geometryShaderSource.length() > 0)
			{
				const char *geometrySource = geometryShaderSource.c_str();

				GLsizei geom_log_length = 0;
				GLchar geom_message[1024] = "";

				geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
				glShaderSource(geometryShader, 1, &geometrySource, nullptr);
				glCompileShader(geometryShader);

				GLint geometry_compiled;
				glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &geometry_compiled);
				glGetShaderInfoLog(geometryShader, 1024, &geom_log_length, geom_message);

				if (geom_log_length > 0)
				{
					m_ShaderLogs.m_GeometryShaderLog = std::string(geom_message);
				}

				if (geometry_compiled != GL_TRUE)
				{
					glDeleteShader(vertexShader);
					glDeleteShader(fragmentShader);
					glDeleteShader(geometryShader);
					return;
				}
			}

			glLinkProgram(m_ID);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			if (std::cmp_not_equal(geometryShader, -1))
			{
				glDeleteShader(geometryShader);
			}

			SetIsCompiled(true);
		}
		else
		{
			// TODO: log missing shader error
		}
	}

	void OpenGLShader::SetVertexShader(Ref<UID> vertexShader)
	{
		m_ShaderAssets.m_VertexShaderAsset = vertexShader;
		m_ShaderSources.m_VertexShaderSource = "";
	}

	void OpenGLShader::SetVertexShader(std::string_view vertexShader)
	{
		m_ShaderAssets.m_VertexShaderAsset = nullptr;
		m_ShaderSources.m_VertexShaderSource = vertexShader;
	}

	void OpenGLShader::SetFragmentShader(Ref<UID> fragmentShader)
	{
		m_ShaderAssets.m_FragmentShaderAsset = fragmentShader;
		m_ShaderSources.m_FragmentShaderSource = "";
	}

	void OpenGLShader::SetFragmentShader(std::string_view fragmentShader)
	{
		m_ShaderAssets.m_FragmentShaderAsset = nullptr;
		m_ShaderSources.m_FragmentShaderSource = fragmentShader;
	}

	void OpenGLShader::SetTesselaltionControlShader(Ref<UID> tessellationControlShader)
	{
		m_ShaderAssets.m_TessellationControlShaderAsset = tessellationControlShader;
		m_ShaderSources.m_TessellationControlShaderSource = "";
	}

	void OpenGLShader::SetTesselaltionControlShader(std::string_view tessellationControlShader)
	{
		m_ShaderAssets.m_TessellationControlShaderAsset = nullptr;
		m_ShaderSources.m_TessellationControlShaderSource = tessellationControlShader;
	}

	void OpenGLShader::SetTesselaltionEvaluationShader(Ref<UID> tessellationEvaluationShader)
	{
		m_ShaderAssets.m_TessellationEvaluationShaderAsset = tessellationEvaluationShader;
		m_ShaderSources.m_TessellationEvaluationShaderSource = "";
	}

	void OpenGLShader::SetTesselaltionEvaluationShader(std::string_view tessellationEvaluationShader)
	{
		m_ShaderAssets.m_TessellationEvaluationShaderAsset = nullptr;
		m_ShaderSources.m_TessellationEvaluationShaderSource = tessellationEvaluationShader;
	}

	void OpenGLShader::SetGeometryShader(Ref<UID> geometryShader)
	{
		m_ShaderAssets.m_GeometryShaderAsset = geometryShader;
		m_ShaderSources.m_GeometryShaderSource = "";
	}

	void OpenGLShader::SetGeometryShader(std::string_view geometryShader)
	{
		m_ShaderAssets.m_GeometryShaderAsset = nullptr;
		m_ShaderSources.m_GeometryShaderSource = geometryShader;
	}

	Ref<UID> OpenGLShader::GetVertexShader() const
	{
		return m_ShaderAssets.m_VertexShaderAsset;
	}

	Ref<UID> OpenGLShader::GetFragmentShader() const
	{
		return m_ShaderAssets.m_FragmentShaderAsset;
	}

	Ref<UID> OpenGLShader::GetGeometryShader() const
	{
		return m_ShaderAssets.m_GeometryShaderAsset;
	}

	GLuint OpenGLShader::GetID() const
	{
		return m_ID;
	}

	Ref<OpenGLShader> OpenGLShader::CreateDefaultShader()
	{
		Ref<OpenGLShader> defaultShader = CreateRef<OpenGLShader>(OpenGLShader());
		defaultShader->SetVertexShader(FileHandler::ReadFile(Shader::GetDefaultShaderPath() / "default.vert"));
		defaultShader->SetFragmentShader(FileHandler::ReadFile(Shader::GetDefaultShaderPath() / "default.frag"));
		defaultShader->Compile();
		return defaultShader;
	}

	Ref<OpenGLShader> OpenGLShader::CreateErrorShader()
	{
		Ref<OpenGLShader> errorShader = CreateRef<OpenGLShader>(OpenGLShader());
		errorShader->SetVertexShader(FileHandler::ReadFile(Shader::GetErrorShaderPath() / "error.vert"));
		errorShader->SetFragmentShader(FileHandler::ReadFile(Shader::GetErrorShaderPath() / "error.frag"));
		errorShader->Compile();
		return errorShader;
	}

	Ref<OpenGLShader> OpenGLShader::CreateGridShader()
	{
		Ref<OpenGLShader> gridShader = CreateRef<OpenGLShader>(OpenGLShader());
		gridShader->SetVertexShader(FileHandler::ReadFile(Shader::GetGridShaderPath() / "grid.vert"));
		gridShader->SetFragmentShader(FileHandler::ReadFile(Shader::GetGridShaderPath() / "grid.frag"));
		gridShader->Compile();
		return gridShader;
	}

	Ref<OpenGLShader> OpenGLShader::CreatePreviewShader()
	{
		Ref<OpenGLShader> previewShader = CreateRef<OpenGLShader>(OpenGLShader());
		previewShader->SetVertexShader(FileHandler::ReadFile(Shader::GetPreviewShaderPath() / "preview.vert"));
		previewShader->SetFragmentShader(FileHandler::ReadFile(Shader::GetPreviewShaderPath() / "preview.frag"));
		previewShader->Compile();
		return previewShader;
	}

	Ref<OpenGLShader> OpenGLShader::CreateIdShader()
	{
		Ref<OpenGLShader> idShader = CreateRef<OpenGLShader>(OpenGLShader());
		idShader->SetVertexShader(FileHandler::ReadFile(Shader::GetIdShaderPath() / "id.vert"));
		idShader->SetFragmentShader(FileHandler::ReadFile(Shader::GetIdShaderPath() / "id.frag"));
		idShader->Compile();
		return idShader;
	}

	Ref<OpenGLShader> OpenGLShader::CreateWhiteShader()
	{
		Ref<OpenGLShader> whiteShader = CreateRef<OpenGLShader>(OpenGLShader());
		whiteShader->SetVertexShader(FileHandler::ReadFile(Shader::GetOutlineShaderPath() / "white.vert"));
		whiteShader->SetFragmentShader(FileHandler::ReadFile(Shader::GetOutlineShaderPath() / "white.frag"));
		whiteShader->Compile();
		return whiteShader;
	}

	std::map<std::string, Ref<IShaderParameter>, std::less<>> OpenGLShader::GetParameters()
	{
		auto parameters = std::map<std::string, Ref<IShaderParameter>, std::less<>>();
		GLint i;
		GLint count;

		GLint size;	 // size of the variable
		GLenum type; // type of the variable (float, vec3 or mat4, etc)

		const GLsizei bufSize = 64; // maximum name length
		GLchar name[bufSize];		// variable name in GLSL
		GLsizei length;				// name length

		glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &count);

		for (i = 0; i < count; i++)
		{
			glGetActiveUniform(m_ID, (GLuint)i, bufSize, &length, &size, &type, name);

			if (std::find(ReservedUniformNames.begin(), ReservedUniformNames.end(), name) == ReservedUniformNames.end())
			{
				parameters[std::string(name)] = Shader::CreateShaderParameter(glTypeToDwarfShaderType.find(type)->second);
			}
		}

		return parameters;
	}

	const ShaderLogs &OpenGLShader::GetShaderLogs() const
	{
		return m_ShaderLogs;
	}

	ShaderAssets &OpenGLShader::GetShaderAssets()
	{
		return m_ShaderAssets;
	}
}