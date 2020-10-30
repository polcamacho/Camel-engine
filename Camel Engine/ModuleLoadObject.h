#pragma once
#ifndef __MODULELOADOBJECT_H__
#define __MODULELOADOBJECT_H__
#include "Module.h"
#include "Globals.h"
#include "MeshHandler.h"
#include <vector>

class ModuleLoadObject : public Module
{
public:
    ModuleLoadObject(bool start_enabled);
    ~ModuleLoadObject();

    bool Start();
    update_status Update(float dt);
    bool CleanUp();
    std::vector<MeshPart*>* LoadObjectData(const char* path);
public:
   
};
#endif // !__MODULELOADOBJECT_H__
