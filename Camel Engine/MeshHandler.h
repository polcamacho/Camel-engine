#pragma once
#ifndef __MESHHANDLER_H__
#define __MESHHANDLER_H__

#include "Globals.h"
#include <vector>

struct MeshPart {   //all the data needed to print a mesh
    uint id_index;
    uint id_vertex;
    uint id_tex_coords;
    uint id_texture;
    uint num_index;
    //uint checkers_id;
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
    uint id_tex_coords; // tex coords in VRAM
    uint num_tex_coords;
    float* tex_coords;
    uint id_texture;
    
};

#endif // !__MESHHANDLER_H__