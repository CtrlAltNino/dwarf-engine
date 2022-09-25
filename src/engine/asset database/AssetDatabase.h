#pragma once
#include<entt/entt.hpp>
#include"../editor/UID.h"
#include"AssetReference.h"

class AssetDatabase {
    private:
        entt::registry m_Registry;
    public:
        AssetDatabase();
        ~AssetDatabase();

        void Init();
        void Clear();
        AssetReference Retrieve(UID id);
};