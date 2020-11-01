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
#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#include "Devil/include/ilu.h"

#pragma comment (lib, "Assimp/Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )

ModuleLoadObject::ModuleLoadObject(bool start_enabled) : Module(start_enabled)
{
	checkerImage[64][64][4] = NULL;
}

ModuleLoadObject::~ModuleLoadObject()
{
}

bool ModuleLoadObject::Start()
{
	LOG("Starting Load Object");

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
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

std::vector<MeshPart*>* ModuleLoadObject::LoadObjectData(const char* path)
{
	// Load FBX
	const aiScene* scene = nullptr;
	char* buffer = nullptr;
	std::string file_path(path);
	aiString texture_path_to_read;

	// Shows directory of dropped file
	file_path.erase(0, file_path.find_last_of("\\") + 1);
	file_path = "Assets/Models/" + file_path;
	uint buffer_size = App->file_system->Load(file_path.c_str(), &buffer);
	
	if (buffer) {
		scene = aiImportFileFromMemory(buffer, buffer_size, aiProcessPreset_TargetRealtime_MaxQuality, NULL);
	}
	else {
		scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	}
		
	FullMesh* ret = new FullMesh;
	ret->id = path;
	if (scene != nullptr && scene->HasMeshes())
	{
		for (int num_meshes = 0; num_meshes < scene->mNumMeshes; ++num_meshes)
		{
			MeshData m;

			//Creating reference to game object mesh
			aiMesh* Object_mesh = scene->mMeshes[num_meshes];
			// Load / copy vertices
			m.num_vertex = Object_mesh->mNumVertices;
			m.vertex = new float[m.num_vertex * 3];
			memcpy(m.vertex, Object_mesh->mVertices, sizeof(float) * m.num_vertex * 3);
			LOG("New mesh with %d vertices", m.num_vertex);

			aiMaterial* Object_material = scene->mMaterials[Object_mesh->mMaterialIndex];
			aiGetMaterialTexture(Object_material, aiTextureType_DIFFUSE, Object_mesh->mMaterialIndex, &texture_path_to_read);
			std::string file_path(texture_path_to_read.C_Str());
			file_path = "//" + file_path;
			if (texture_path_to_read.length > 0) {
				LoadTexture(file_path.c_str());
			}
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
			LOG("New mesh with %d faces", m.num_index / 3);
			
			//Load / copy texture coords

			if (Object_mesh->HasTextureCoords(0)) {
				m.num_tex_coords = Object_mesh->mNumVertices * 2;
				m.tex_coords = new float[m.num_tex_coords];

				for (uint coords = 0; coords < Object_mesh->mNumVertices; ++coords)
				{
					m.tex_coords[coords * 2] = Object_mesh->mTextureCoords[0][coords].x;
					m.tex_coords[(coords * 2)+1] = Object_mesh->mTextureCoords[0][coords].y;
				}
			}
			LOG("New mesh with %d texture coords:", m.num_tex_coords);

			glGenBuffers(1, (GLuint*)&(m.id_vertex));
			glBindBuffer(GL_ARRAY_BUFFER, m.id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_vertex * 3, m.vertex, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, (GLuint*)&(m.id_tex_coords));
			glBindBuffer(GL_ARRAY_BUFFER, m.id_tex_coords);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_tex_coords, m.tex_coords, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, (GLuint*)&(m.id_index));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.id_index);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m.num_index, m.index, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			uint texture_id = 0;
			//CreateCheckerBuffer(texture_id);
			CreateTextureBuffer(texture_id);
			MeshPart* p = new MeshPart;
			p->id_index = m.id_index;
			p->id_vertex = m.id_vertex;
			p->num_index = m.num_index;
			p->id_tex_coords = m.id_tex_coords;
			p->texture_id = texture_id;
			ret->parts.push_back(p);
		}

		// Call after import data
		aiReleaseImport(scene);
	}

	
	else
		LOG("Error loading scene % s", scene);

	App->scene_intro->meshes.push_back(ret);

	return &ret->parts;
	
}

void ModuleLoadObject::CreateCheckersImage()
{
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int c = ((((i & 0x2) == 0) ^ (((j & 0x2)) == 0))) * 255;
			checkerImage[i][j][0] = (GLubyte)c;
			checkerImage[i][j][1] = (GLubyte)c;
			checkerImage[i][j][2] = (GLubyte)c;
			checkerImage[i][j][3] = (GLubyte)255;
		}
	}
}

void ModuleLoadObject::CreateCheckerBuffer(uint& id)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerImage);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ModuleLoadObject::LoadTexture(const char* path_texture)
{
	ILuint imageID = 0;
	ILenum type;
	uint buffer_size;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	char* buffer = nullptr;
	std::string file_path(path_texture);
	file_path.erase(0, file_path.find_last_of("\\") + 1);
	file_path = "Assets/Textures/" + file_path;
	buffer_size = App->file_system->Load(file_path.c_str(), &buffer);
	
	if (file_path.substr(file_path.find_last_of(".")) == ".jpg") {
		type = IL_JPG;
	}
	else if (file_path.substr(file_path.find_last_of(".")) == ".png"){
		type = IL_PNG;
	}
	ilLoadL(type, buffer, buffer_size);
	
	if (imageID != NULL) {
		LOG("path: %s", file_path);
	}
	else {
		LOG("Error loading image");
	}
}

void ModuleLoadObject::CreateTextureBuffer(uint& id)
{
	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), width, height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
	glBindTexture(GL_TEXTURE_2D, 0);
}
