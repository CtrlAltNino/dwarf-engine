#include "dpch.h"

#include "OpenGLShader.h"
#include "Utilities/FileHandler.h"
#include "Core/Asset/AssetDatabase.h"
#include "Core/Asset/AssetComponents.h"

namespace Dwarf {
    OpenGLShader::OpenGLShader() { }
    OpenGLShader::~OpenGLShader() { }

    void OpenGLShader::Compile() {
        std::string vertexShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<VertexShaderAsset>(m_VertexShader)->GetAsset().m_Path);
        std::string fragmentShaderSource = FileHandler::ReadFile(AssetDatabase::Retrieve<FragmentShaderAsset>(m_FragmentShader)->GetAsset().m_Path);
        if(vertexShaderSource.length() > 0 && fragmentShaderSource.length() > 0) {
			const char* vertexSource = vertexShaderSource.c_str();
			const char* fragmentSource = fragmentShaderSource.c_str();

			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexSource, NULL);
			glCompileShader(vertexShader);

			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
			glCompileShader(fragmentShader);

			m_ID = glCreateProgram();

			glAttachShader(m_ID, vertexShader);
			glAttachShader(m_ID, fragmentShader);

			//GLuint geometryShader = -1;

			/*if(geometryShaderSource.length() > 0) {
				const char* geometrySource = geometryShaderSource.c_str();

				GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
				glShaderSource(geometryShader, 1, &geometrySource, NULL);
				glCompileShader(geometryShader);
				
				glAttachShader(ID, geometryShader);
			}*/

			glLinkProgram(m_ID);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			/*if(geometryShader != -1) {
				glDeleteShader(geometryShader);
			}*/
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
        std::filesystem::path defaultShaderPath = "data/engine/shaders/default/opengl/";
        defaultShader->SetVertexShader(AssetDatabase::Import(defaultShaderPath / "default.vert"));
        defaultShader->SetFragmentShader(AssetDatabase::Import(defaultShaderPath / "default.frag"));
        defaultShader->Compile();
        return defaultShader;
    }

    Ref<OpenGLShader> OpenGLShader::CreateErrorShader(){
        Ref<OpenGLShader> errorShader = CreateRef<OpenGLShader>(OpenGLShader());
        std::filesystem::path errorShaderPath = "data/engine/shaders/error/opengl/";
        errorShader->SetVertexShader(AssetDatabase::Import(errorShaderPath / "error.vert"));
        errorShader->SetFragmentShader(AssetDatabase::Import(errorShaderPath / "error.frag"));
        errorShader->Compile();
        return errorShader;
    }

    Ref<OpenGLShader> OpenGLShader::CreateGridShader(){
        Ref<OpenGLShader> gridShader = CreateRef<OpenGLShader>(OpenGLShader());
        std::filesystem::path gridShaderPath = "data/engine/shaders/grid/opengl/";
        gridShader->SetVertexShader(AssetDatabase::Import(gridShaderPath / "grid.vert"));
        gridShader->SetFragmentShader(AssetDatabase::Import(gridShaderPath / "grid.frag"));
        gridShader->Compile();
        return gridShader;
    }
}