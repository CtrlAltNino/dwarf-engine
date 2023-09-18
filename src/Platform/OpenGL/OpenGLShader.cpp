#include "dpch.h"

#include "OpenGLShader.h"
#include "Utilities/FileHandler.h"
#include "Core/Asset/AssetDatabase.h"
#include "Core/Asset/AssetComponents.h"

namespace Dwarf {
    OpenGLShader::OpenGLShader() { }
    OpenGLShader::~OpenGLShader() { }

    void OpenGLShader::Compile() {
		m_SuccessfullyCompiled = false;

        std::string vertexShaderSource;
		std::string tescShaderSource;
		std::string teseShaderSource;
		std::string geometryShaderSource;
		std::string fragmentShaderSource;

		if(AssetDatabase::Exists(m_VertexShader)){
			vertexShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<VertexShaderAsset>(m_VertexShader)->GetAsset()->m_Path);
		}

		if(AssetDatabase::Exists(m_TessellationControlShader)){
			tescShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<TesselationControlShaderAsset>(m_TessellationControlShader)->GetAsset()->m_Path);
		}

		if(AssetDatabase::Exists(m_TessellationEvaluationShader)){
			teseShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<TesselationEvaluationShaderAsset>(m_TessellationEvaluationShader)->GetAsset()->m_Path);
		}

		if(AssetDatabase::Exists(m_GeometryShader)){
			geometryShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<GeometryShaderAsset>(m_GeometryShader)->GetAsset()->m_Path);
		}

		if(AssetDatabase::Exists(m_FragmentShader)){
			fragmentShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<FragmentShaderAsset>(m_FragmentShader)->GetAsset()->m_Path);
		}

        if(vertexShaderSource.length() > 0 && fragmentShaderSource.length() > 0) {
			const char* vertexSource = vertexShaderSource.c_str();
			const char* fragmentSource = fragmentShaderSource.c_str();

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

			//GLuint geometryShader = -1;
			GLuint geometryShader = -1;

			if(geometryShaderSource.length() > 0) {
				const char* geometrySource = geometryShaderSource.c_str();

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

			if(geometryShader != -1) {
				glDeleteShader(geometryShader);
			}

			m_SuccessfullyCompiled = true;
			std::cout << "Compiling shader" << std::endl;
		}else {
			// TODO log missing shader error
		}
    }

    void OpenGLShader::SetVertexShader(Ref<UID> vertexShader) {
		m_VertexShader = vertexShader;
	}

	void OpenGLShader::SetFragmentShader(Ref<UID> fragmentShader) {
		m_FragmentShader = fragmentShader;
	}

	void OpenGLShader::SetTesselaltionControlShader(Ref<UID> tessellationControlShader) {
		m_TessellationControlShader = tessellationControlShader;
	}

	void OpenGLShader::SetTesselaltionEvaluationShader(Ref<UID> tessellationEvaluationShader) {
		m_TessellationEvaluationShader = tessellationEvaluationShader;
	}

    void OpenGLShader::SetGeometryShader(Ref<UID> geometryShader) {
		m_GeometryShader = geometryShader;
	}

    Ref<UID> OpenGLShader::GetVertexShader() {
		return m_VertexShader;
	}

	Ref<UID> OpenGLShader::GetFragmentShader() {
		return m_FragmentShader;
	}

    Ref<UID> OpenGLShader::GetGeometryShader() {
		return m_GeometryShader;
	}

    GLuint OpenGLShader::GetID(){
        return m_ID;
    }

    Ref<OpenGLShader> OpenGLShader::CreateDefaultShader(){
        Ref<OpenGLShader> defaultShader = CreateRef<OpenGLShader>(OpenGLShader());
        //std::filesystem::path defaultShaderPath = "data/engine/shaders/default/opengl/";
        defaultShader->SetVertexShader(AssetDatabase::Import(Shader::GetDefaultShaderPath() / "default.vert"));
        defaultShader->SetFragmentShader(AssetDatabase::Import(Shader::GetDefaultShaderPath() / "default.frag"));
        defaultShader->Compile();
        return defaultShader;
    }

    Ref<OpenGLShader> OpenGLShader::CreateErrorShader(){
        Ref<OpenGLShader> errorShader = CreateRef<OpenGLShader>(OpenGLShader());
        std::filesystem::path errorShaderPath = "data/engine/shaders/error/opengl/";
        errorShader->SetVertexShader(AssetDatabase::Import(Shader::GetErrorShaderPath() / "error.vert"));
        errorShader->SetFragmentShader(AssetDatabase::Import(Shader::GetErrorShaderPath() / "error.frag"));
        errorShader->Compile();
        return errorShader;
    }

    Ref<OpenGLShader> OpenGLShader::CreateGridShader(){
        Ref<OpenGLShader> gridShader = CreateRef<OpenGLShader>(OpenGLShader());
        std::filesystem::path gridShaderPath = "data/engine/shaders/grid/opengl/";
        gridShader->SetVertexShader(AssetDatabase::Import(Shader::GetGridShaderPath() / "grid.vert"));
        gridShader->SetFragmentShader(AssetDatabase::Import(Shader::GetGridShaderPath() / "grid.frag"));
        gridShader->Compile();
        return gridShader;
    }
}