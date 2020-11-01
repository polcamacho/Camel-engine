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
    void CreateCheckersImage();
    void CreateCheckerBuffer(uint& id);
    void CreateTextureBuffer(uint& id);
    void LoadTexture(const char* path_texture);
public:

    GLubyte checkerImage[64][64][4];
};
#endif // !__MODULELOADOBJECT_H__
