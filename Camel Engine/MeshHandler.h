#pragma once
#ifndef __MESHHANDLER_H__
#define __MESHHANDLER_H__

#include "Globals.h"
#include <vector>

struct MeshPart {   //all the data needed to print a mesh
    uint id_index;
    uint id_vertex;
    uint num_index;
};

struct FullMesh {
    const char* id;
    std::vector<MeshPart*> parts;
};

struct MeshData {
    uint id_index; // index in VRAM
    uint num_index;
    uint* index;
    uint id_vertex; // unique vertex in VRAM
    uint num_vertex;
    float* vertex;
};

#endif // !__MESHHANDLER_H__