#pragma once

#include<string>
enum class MODULE_TYPE {PERFORMANCE, SCENE_GRAPH, CONSOLE, SCENE_VIEWER, ASSET_BROWSER, INSPECTOR};

class IModelListener{
    
};

class IViewListener{
    public:
        virtual void AddWindow(MODULE_TYPE moduleType)=0;
        virtual void RemoveWindow(int index)=0;
};