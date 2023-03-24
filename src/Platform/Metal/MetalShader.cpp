#include "dpch.h"

#include "MetalShader.h"
#include "Utilities/FileHandler.h"
#include "Core/Asset/AssetDatabase.h"
#include "Core/Asset/AssetComponents.h"


namespace Dwarf {
    MetalShader::MetalShader() { }
    MetalShader::~MetalShader() { }

    void MetalShader::Compile() {
        
    }
    
    void MetalShader::SetShader(Ref<UID> shaderAsset) {
		m_ShaderAsset = shaderAsset;
	}

    Ref<UID> MetalShader::GetShader() {
		return m_ShaderAsset;
	}

    /*GLuint MetalShader::GetID(){
        return m_ID;
    }*/

    Ref<MetalShader> MetalShader::CreateDefaultShader(){
        Ref<MetalShader> defaultShader = CreateRef<MetalShader>(MetalShader());
        std::filesystem::path defaultShaderPath = "data/engine/shaders/grid/metal/default.metal";
        defaultShader->SetShader(AssetDatabase::Import(defaultShaderPath));
        defaultShader->Compile();
        return defaultShader;
    }

    Ref<MetalShader> MetalShader::CreateErrorShader(){
        Ref<MetalShader> errorShader = CreateRef<MetalShader>(MetalShader());
        std::filesystem::path errorShaderPath = "data/engine/shaders/grid/metal/error.metal";
        errorShader->SetShader(AssetDatabase::Import(errorShaderPath));
        errorShader->Compile();
        return errorShader;
    }

    Ref<MetalShader> MetalShader::CreateGridShader(){
        Ref<MetalShader> gridShader = CreateRef<MetalShader>(MetalShader());
        std::filesystem::path gridShaderPath = "data/engine/shaders/grid/metal/grid.metal";
        gridShader->SetShader(AssetDatabase::Import(gridShaderPath));
        gridShader->Compile();
        return gridShader;
    }
}