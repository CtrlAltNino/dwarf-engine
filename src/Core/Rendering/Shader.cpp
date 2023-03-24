#include <string>

#include "Core/Rendering/Shader.h"
#include "Core/Rendering/Renderer.h"

#ifdef _WIN32
	//#include "Platform/D3D11/D3D11Shader.h"
	//#include "Platform/D3D12/D3D12Shader.h"
	#include "Platform/OpenGL/OpenGLShader.h"
	//#include "Platform/Vulkan/VulkanShader.h"
#elif __linux__
	#include "Platform/OpenGL/OpenGLShader.h"
	//#include "Platform/Vulkan/VulkanShader.h"
#elif __APPLE__
	#include "Platform/Metal/MetalShader.h"
#endif

namespace Dwarf {

	Ref<Shader> Shader::s_DefaultShader = nullptr;
	Ref<Shader> Shader::s_ErrorShader = nullptr;
	Ref<Shader> Shader::s_GridShader = nullptr;

	/*std::string get_file_contents(std::string filename) {
		std::ifstream in(filename, std::ios::binary);
		if (in) {
			std::string contents;
			in.seekg(0, std::ios::end);
			contents.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&contents[0], contents.size());
			in.close();
			return(contents);
		}
		throw(errno);
	}*/

	Ref<Shader> Shader::Create(){
		switch(Renderer::GetAPI()){
#ifdef _WIN32
			//case GraphicsApi::D3D11: return CreateRef<D3D11Shader>(D3D11Shader());
			//case GraphicsApi::D3D12: return CreateRef<D3D12Shader>(D3D12Shader());
			case GraphicsApi::OpenGL:
					return CreateRef<OpenGLShader>(OpenGLShader());
				break;
			//case GraphicsApi::Vulkan: return CreateRef<VulkanShader>(VulkanShader());
#elif __linux__
			case GraphicsApi::OpenGL: return CreateRef<OpenGLShader>(OpenGLShader());
			//case GraphicsApi::Vulkan: return CreateRef<VulkanShader>(VulkanShader());
#elif __APPLE__
			//case GraphicsApi::Metal: return CreateRef<MetalShader>(MetalShader());
#endif
			default: return nullptr;
		}
	}

	Shader::Shader(){}
	Shader::~Shader(){}

	void Shader::Init(){
		switch(Renderer::GetAPI()){
#ifdef _WIN32
			/*case GraphicsApi::D3D11:
					s_DefaultShader = D3D11Shader::CreateDefaultShader();
					s_ErrorShader = D3D11Shader::CreateErrorShader();
					s_GridShader = D3D11Shader::CreateGridShader();
				break;*/
			/*case GraphicsApi::D3D12:
					s_DefaultShader = D3D12Shader::CreateDefaultShader();
					s_ErrorShader = D3D12Shader::CreateErrorShader();
					s_GridShader = D3D12Shader::CreateGridShader();
				break;*/
			case GraphicsApi::OpenGL:
					s_DefaultShader = OpenGLShader::CreateDefaultShader();
					s_ErrorShader = OpenGLShader::CreateErrorShader();
					s_GridShader = OpenGLShader::CreateGridShader();
				break;
			/*case GraphicsApi::Vulkan:
					s_DefaultShader = VulkanShader::CreateDefaultShader();
					s_ErrorShader = VulkanShader::CreateErrorShader();
					s_GridShader = VulkanShader::CreateGridShader();
				break;*/
#elif __linux__
			case GraphicsApi::OpenGL:
					s_DefaultShader = OpenGLShader::CreateDefaultShader();
					s_ErrorShader = OpenGLShader::CreateErrorShader();
					s_GridShader = OpenGLShader::CreateGridShader();
				break;
			/*case GraphicsApi::Vulkan:
					s_DefaultShader = VulkanShader::CreateDefaultShader();
					s_ErrorShader = VulkanShader::CreateErrorShader();
					s_GridShader = VulkanShader::CreateGridShader();
				break;*/
#elif __APPLE__
			case GraphicsApi::Metal:
					s_DefaultShader = MetalShader::CreateDefaultShader();
					s_ErrorShader = MetalShader::CreateErrorShader();
					s_GridShader = MetalShader::CreateGridShader();
				break;
#endif
		}

		s_DefaultShader->Compile();
		s_ErrorShader->Compile();
		s_GridShader->Compile();
	}

	/*void Shader::AddGeometryShader(std::string filePath) {
		this->geometryShaderSource = get_file_contents(filePath);
	}*/

	/*void Shader::CreateShaderProgram() {
		if(this->vertexShaderSource.length() > 0 && this->fragmentShaderSource.length() > 0) {
			const char* vertexSource = vertexShaderSource.c_str();
			const char* fragmentSource = fragmentShaderSource.c_str();

			GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShader, 1, &vertexSource, NULL);
			glCompileShader(vertexShader);

			GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
			glCompileShader(fragmentShader);

			ID = glCreateProgram();

			glAttachShader(ID, vertexShader);
			glAttachShader(ID, fragmentShader);

			GLuint geometryShader = -1;

			if(geometryShaderSource.length() > 0) {
				const char* geometrySource = geometryShaderSource.c_str();

				GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
				glShaderSource(geometryShader, 1, &geometrySource, NULL);
				glCompileShader(geometryShader);
				
				glAttachShader(ID, geometryShader);
			}

			glLinkProgram(ID);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			if(geometryShader != -1) {
				glDeleteShader(geometryShader);
			}
		}else {
			// TODO log missing shader error
		}
	}*/

	/*void Shader::Activate() {
		glUseProgram(ID);
	}

	void Shader::Delete() {
		glDeleteProgram(ID);
	}*/
}