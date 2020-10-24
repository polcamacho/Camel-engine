#include "Globals.h"
#include "Application.h"
#include "ModuleLoadObject.h"
#include "glmath.h"
#include "glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Assimp/Assimp/include/cimport.h"
#include "Assimp/Assimp/include/scene.h"
#include "Assimp/Assimp/include/postprocess.h"

#pragma comment (lib, "Assimp/Assimp/libx86/assimp.lib")

ModuleLoadObject::ModuleLoadObject(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	m.id_index = 0;
	m.num_index = 0;
	m.index = nullptr;

	m.id_vertex = 0;
	m.num_vertex = 0;
	m.vertex = nullptr;
}

ModuleLoadObject::~ModuleLoadObject()
{}

bool ModuleLoadObject::Start()
{
	LOG("Starting Load Object");

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleLoadObject::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}

update_status ModuleLoadObject::Update(float dt)
{
	return UPDATE_CONTINUE;
}

void ModuleLoadObject::LoadObjectData(const char* path)
{
	// Load FBX
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int num_meshes = 0; num_meshes < scene->mNumMeshes; ++num_meshes)
		{
			//Creating reference to game object mesh
			aiMesh* Object_mesh = scene->mMeshes[num_meshes];

			// Load / copy vertices
			m.num_vertex = Object_mesh->mNumVertices;
			m.vertex = new float[m.num_vertex * 3];
			memcpy(m.vertex, Object_mesh->mVertices, sizeof(float) * m.num_vertex * 3);
			LOG("New mesh with %d vertices", m.num_vertex);

			// Load / copy faces
			if (Object_mesh->HasFaces())
			{
				m.num_index = Object_mesh->mNumFaces * 3;
				m.index = new uint[m.num_index]; // assume each face is a triangle
				for (uint i = 0; i < Object_mesh->mNumFaces; ++i)
				{
					if (Object_mesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else {
						memcpy(&m.index[i * 3], Object_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}
			}
		}

		// Call after import data
		aiReleaseImport(scene);
	}
	else
		LOG("Error loading scene % s", scene);

	
}