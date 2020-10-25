#pragma once
#ifndef __MODULELOADOBJECT_H__
#define __MODULELOADOBJECT_H__
#include "Module.h"
#include "Globals.h"
#include <vector>

struct Mesh{
    uint id_index; // index in VRAM
    uint num_index;
    uint* index;
    uint id_vertex; // unique vertex in VRAM
    uint num_vertex;
    float* vertex;
};

class ModuleLoadObject : public Module
{
public:
    ModuleLoadObject(Application* app, bool start_enabled);
    ~ModuleLoadObject();

    bool Start();
    update_status Update(float dt);
    bool CleanUp();
    void LoadObjectData(const char* path);

public:
    Mesh *m;
   
};
#endif // !__MODULELOADOBJECT_H__
