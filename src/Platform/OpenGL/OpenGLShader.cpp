#include "dpch.h"

#include "OpenGLShader.h"
#include "Utilities/FileHandler.h"
#include "Core/Asset/AssetDatabase.h"
#include "Core/Asset/AssetComponents.h"

namespace Dwarf
{
	OpenGLShader::OpenGLShader() {}
	OpenGLShader::~OpenGLShader() {}

	std::map<GLenum, ShaderParameterType> glTypeToDwarfShaderType =
		{
			{GL_BOOL, BOOLEAN},
			{GL_INT, INTEGER},
			{GL_FLOAT, FLOAT},
			{GL_FLOAT_VEC2, VEC2},
			{GL_FLOAT_VEC3, VEC3},
			{GL_FLOAT_VEC4, VEC4},
			{GL_SAMPLER_2D, TEX2D}};

	void OpenGLShader::Compile()
	{
		m_SuccessfullyCompiled = false;

		std::string vertexShaderSource;
		std::string tescShaderSource;
		std::string teseShaderSource;
		std::string geometryShaderSource;
		std::string fragmentShaderSource;

		if (m_VertexShaderAsset != nullptr && AssetDatabase::Exists(m_VertexShaderAsset))
		{
			std::filesystem::path vertexShaderPath = AssetDatabase::Retrieve<VertexShaderAsset>(m_VertexShaderAsset)->GetAsset()->m_Path;
			vertexShaderSource = FileHandler::ReadFile(vertexShaderPath);
			AssetDatabase::AddShaderWatch(vertexShaderPath, this);
		}
		else
		{
			vertexShaderSource = m_VertexShaderSource;
		}

		if (AssetDatabase::Exists(m_TessellationControlShaderAsset))
		{
			tescShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<TesselationControlShaderAsset>(m_TessellationControlShaderAsset)->GetAsset()->m_Path);
		}

		if (AssetDatabase::Exists(m_TessellationEvaluationShaderAsset))
		{
			teseShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<TesselationEvaluationShaderAsset>(m_TessellationEvaluationShaderAsset)->GetAsset()->m_Path);
		}

		if (AssetDatabase::Exists(m_GeometryShaderAsset))
		{
			geometryShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<GeometryShaderAsset>(m_GeometryShaderAsset)->GetAsset()->m_Path);
		}

		if (m_FragmentShaderAsset != nullptr && AssetDatabase::Exists(m_VertexShaderAsset))
		{
			std::filesystem::path fragmentShaderPath = AssetDatabase::Retrieve<FragmentShaderAsset>(m_FragmentShaderAsset)->GetAsset()->m_Path;
			fragmentShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<FragmentShaderAsset>(m_FragmentShaderAsset)->GetAsset()->m_Path);
			AssetDatabase::AddShaderWatch(fragmentShaderPath, this);
		}
		else
		{
			fragmentShaderSource = m_FragmentShaderSource;
		}

		if (vertexShaderSource.length() > 0 && fragmentShaderSource.length() > 0)
		{
			const char *vertexSource = vertexShaderSource.c_str();
			const char *fragmentSource = fragmentShaderSource.c_str();

			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexSource, NULL);
			glCompileShader(vertexShader);

			GLint vertex_compiled;
			glGetShaderInfoLog(vertexShader, 1024, &vert_log_length, vert_message);
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertex_compiled);
			if (vertex_compiled != GL_TRUE)
			{
				glDeleteShader(vertexShader);
				return;
			}

			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
			glCompileShader(fragmentShader);

			GLint fragment_compiled;
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragment_compiled);
			glGetShaderInfoLog(fragmentShader, 1024, &frag_log_length, frag_message);
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
				std::cout << "WTF???" << std::endl;
				const char *geometrySource = geometryShaderSource.c_str();

				GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
				glShaderSource(geometryShader, 1, &geometrySource, NULL);
				glCompileShader(geometryShader);

				GLint geometry_compiled;
				glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &geometry_compiled);
				glGetShaderInfoLog(geometryShader, 1024, &geom_log_length, geom_message);
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

			if (geometryShader != -1)
			{
				glDeleteShader(geometryShader);
			}

			m_SuccessfullyCompiled = true;
		}
		else
		{
			// TODO log missing shader error
		}
	}

	void OpenGLShader::SetVertexShader(Ref<UID> vertexShader)
	{
		m_VertexShaderAsset = vertexShader;
		m_VertexShaderSource = nullptr;
	}

	void OpenGLShader::SetVertexShader(std::string vertexShader)
	{
		m_VertexShaderAsset = nullptr;
		m_VertexShaderSource = vertexShader;
	}

	void OpenGLShader::SetFragmentShader(Ref<UID> fragmentShader)
	{
		m_FragmentShaderAsset = fragmentShader;
		m_FragmentShaderSource = nullptr;
	}

	void OpenGLShader::SetFragmentShader(std::string fragmentShader)
	{
		m_FragmentShaderAsset = nullptr;
		m_FragmentShaderSource = fragmentShader;
	}

	void OpenGLShader::SetTesselaltionControlShader(Ref<UID> tessellationControlShader)
	{
		m_TessellationControlShaderAsset = tessellationControlShader;
		m_TessellationControlShaderSource = nullptr;
	}

	void OpenGLShader::SetTesselaltionControlShader(std::string tessellationControlShader)
	{
		m_TessellationControlShaderAsset = nullptr;
		m_TessellationControlShaderSource = tessellationControlShader;
	}

	void OpenGLShader::SetTesselaltionEvaluationShader(Ref<UID> tessellationEvaluationShader)
	{
		m_TessellationEvaluationShaderAsset = tessellationEvaluationShader;
		m_TessellationEvaluationShaderSource = nullptr;
	}

	void OpenGLShader::SetTesselaltionEvaluationShader(std::string tessellationEvaluationShader)
	{
		m_TessellationEvaluationShaderAsset = nullptr;
		m_TessellationEvaluationShaderSource = tessellationEvaluationShader;
	}

	void OpenGLShader::SetGeometryShader(Ref<UID> geometryShader)
	{
		m_GeometryShaderAsset = geometryShader;
		m_GeometryShaderSource = nullptr;
	}

	void OpenGLShader::SetGeometryShader(std::string geometryShader)
	{
		m_GeometryShaderAsset = nullptr;
		m_GeometryShaderSource = geometryShader;
	}

	Ref<UID> OpenGLShader::GetVertexShader()
	{
		return m_VertexShaderAsset;
	}

	Ref<UID> OpenGLShader::GetFragmentShader()
	{
		return m_FragmentShaderAsset;
	}

	Ref<UID> OpenGLShader::GetGeometryShader()
	{
		return m_GeometryShaderAsset;
	}

	GLuint OpenGLShader::GetID()
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

	std::map<std::string, Ref<IShaderParameter>> OpenGLShader::GetParameters()
	{
		std::map<std::string, Ref<IShaderParameter>> parameters = std::map<std::string, Ref<IShaderParameter>>();
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

			parameters[std::string(name)] = Shader::CreateShaderParameter(glTypeToDwarfShaderType[type]);
		}

		return parameters;
	}
}