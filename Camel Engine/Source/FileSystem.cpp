#include "FileSystem.h"
#include "Application.h"

#include "Mesh.h"
#include "GameObject.h"
#include "Material.h"
#include "Transform.h"

#include "SDL/include/SDL.h"
#include <fstream>
#include <iostream>
#include <Shlwapi.h>

#include "Timer.h"
#include "parson/parson.h"

#include "PhysFS/include/physfs.h"
#include "Assimp/Assimp/include/cimport.h"
#include "Assimp/Assimp/include/scene.h"
#include "Assimp/Assimp/include/postprocess.h"

#include "Devil/include/IL/il.h"
#include "Devil/include/IL/ilu.h"
#include "Devil/include/IL/ilut.h"

#pragma comment (lib, "Libs/PhysFS/libx86/physfs.lib")
#pragma comment (lib,"shlwapi.lib")
#pragma comment (lib, "Libs/Assimp/Assimp/libx86/assimp.lib")

#pragma comment (lib, "Libs/Devil/libx86/DevIL.lib")	
#pragma comment (lib, "Libs/Devil/libx86/ILU.lib")	
#pragma comment (lib, "Libs/Devil/libx86/ILUT.lib")	

#pragma region FileSystem

void FileSystem::Init()
{
	ilInit();
	iluInit();
	if (ilutRenderer(ILUT_OPENGL)) 
		LOG("DevIL initted correctly");

	char* base_path = SDL_GetBasePath();

	if (PHYSFS_init(nullptr) != 0)
		LOG("PhysFS initted correctly");
	
	SDL_free(base_path);

	AddPath("."); //Adding ProjectFolder (working directory)
	AddPath("Assets");

	if (PHYSFS_setWriteDir("./Assets") == 0)
		LOG_ERROR("File System error while creating write dir: %s\n", PHYSFS_getLastError());

	CreateLibraryDirectories();

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	std::string path = PHYSFS_getWriteDir();

	normalize_scales = true;
}

void FileSystem::CleanUp()
{
	PHYSFS_deinit();
	aiDetachAllLogStreams();
}

void FileSystem::GetPhysFSVersion(std::string& version_str)
{
	PHYSFS_Version version;
	PHYSFS_getLinkedVersion(&version);
	version_str = std::to_string(version.major) + "." + std::to_string(version.minor) + "." + std::to_string(version.patch);
}

void FileSystem::CreateLibraryDirectories()
{
	//CreateDir(LIBRARY_PATH);
	//CreateDir(FOLDERS_PATH);
	//CreateDir(MESHES_PATH);
	//CreateDir(TEXTURES_PATH);
	//CreateDir(MATERIALS_PATH);
	CreateDir("Config/");
	CreateDir("Textures/");
	CreateDir("Models/");
	//CreateDir("Materials/");
	//CreateDir(ANIMATIONS_PATH);
	//CreateDir(PARTICLES_PATH);
	//CreateDir(SHADERS_PATH);
	//CreateDir(SCENES_PATH);
}

// Add a new zip file or folder
bool FileSystem::AddPath(const char* path_or_zip)
{
	bool ret = false;

	if (PHYSFS_mount(path_or_zip, nullptr, 1) == 0)
	{
		LOG_ERROR("File System error while adding a path or zip: %s\n", PHYSFS_getLastError());
	}
	else
		ret = true;

	return ret;
}

// Check if a file exists
bool FileSystem::Exists(const char* file) 
{
	return PHYSFS_exists(file) != 0;
}

bool FileSystem::CreateDir(const char* dir)
{
	if (IsDirectory(dir) == false)
	{
		PHYSFS_mkdir(dir);
		return true;
	}
	return false;
}

// Check if a file is a directory
bool FileSystem::IsDirectory(const char* file) 
{
	return PHYSFS_isDirectory(file) != 0;
}

const char* FileSystem::GetWriteDir() 
{
	//TODO: erase first annoying dot (".")
	return PHYSFS_getWriteDir();
}

void FileSystem::DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) 
{
	char** rc = PHYSFS_enumerateFiles(directory);
	char** i;

	for (i = rc; *i != nullptr; i++)
	{
		std::string str = std::string(directory) + std::string("/") + std::string(*i);
		if (IsDirectory(str.c_str()))
			dir_list.push_back(*i);
		else
			file_list.push_back(*i);
	}

	PHYSFS_freeList(rc);
}

void FileSystem::GetAllFilesWithExtension(const char* directory, const char* extension, std::vector<std::string>& file_list) 
{
	std::vector<std::string> files;
	std::vector<std::string> dirs;
	DiscoverFiles(directory, files, dirs);

	for (uint i = 0; i < files.size(); i++)
	{
		std::string ext;
		SplitFilePath(files[i].c_str(), nullptr, nullptr, &ext);

		if (ext == extension)
			file_list.push_back(files[i]);
	}
}

void FileSystem::GetRealDir(const char* path, std::string& output) 
{
	output = PHYSFS_getBaseDir();

	std::string baseDir = PHYSFS_getBaseDir();
	std::string searchPath = *PHYSFS_getSearchPath();
	std::string realDir = PHYSFS_getRealDir(path);

	output.append(*PHYSFS_getSearchPath()).append("/");
	output.append(PHYSFS_getRealDir(path)).append("/").append(path);
}

std::string FileSystem::GetPathRelativeToAssets(const char* originalPath) 
{
	std::string file_path = originalPath;
	
	std::size_t pos = file_path.find("Assets");

	if (pos > file_path.size())
	{
		LOG_WARNING("Trying to load a file out of the working directory");
		file_path.clear();
	}
	else
	{
		file_path = file_path.substr(pos);
	}

	return file_path;
}

bool FileSystem::HasExtension(const char* path) 
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	return ext != "";
}

bool FileSystem::HasExtension(const char* path, std::string extension) 
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	return ext == extension;
}

bool FileSystem::HasExtension(const char* path, std::vector<std::string> extensions) 
{
	std::string ext = "";
	SplitFilePath(path, nullptr, nullptr, &ext);
	if (ext == "")
		return true;
	for (uint i = 0; i < extensions.size(); i++)
	{
		if (extensions[i] == ext)
			return true;
	}
	return false;
}

std::string FileSystem::ProcessPath(const char* path)
{
	std::string final_path = path;
	
	final_path = NormalizePath(final_path.c_str());
	std::string tmp_path = GetPathRelativeToAssets(final_path.c_str());

	//The file is inside a directory
	if (tmp_path.size() > 0)
	{
		return tmp_path.c_str();
	}

	return final_path.c_str();
}

std::string FileSystem::NormalizePath(const char* full_path)
{
	std::string newPath(full_path);
	for (int i = 0; i < newPath.size(); ++i)
	{
		if (newPath[i] == '\\')
			newPath[i] = '/';
	}
	return newPath;
}

void FileSystem::SplitFilePath(const char* full_path, std::string* path, std::string* file, std::string* extension) 
{
	if (full_path != nullptr)
	{
		std::string full(full_path);
		size_t pos_separator = full.find_last_of("\\/");
		size_t pos_dot = full.find_last_of(".");

		if (path != nullptr)
		{
			if (pos_separator < full.length())
				*path = full.substr(0, pos_separator + 1);
			else
				path->clear();
		}

		if (file != nullptr)
		{
			if (pos_separator < full.length())
				*file = full.substr(pos_separator + 1, pos_dot - pos_separator - 1);
			else
				*file = full.substr(0, pos_dot);
		}

		if (extension != nullptr)
		{
			if (pos_dot < full.length())
				*extension = full.substr(pos_dot + 1);
			else
				extension->clear();
		}
	}
}

unsigned int FileSystem::Load(const char* path, const char* file, char** buffer) 
{
	std::string full_path(path);
	full_path += file;
	return Load(full_path.c_str(), buffer);
}

// Read a whole file and put it in a new buffer
uint FileSystem::Load(const char* file, char** buffer) 
{
	uint ret = 0;

	PHYSFS_file* fs_file = PHYSFS_openRead(file);

	if (fs_file != nullptr)
	{
		PHYSFS_sint32 size = (PHYSFS_sint32)PHYSFS_fileLength(fs_file);

		if (size > 0)
		{
			*buffer = new char[size + 1];
			uint readed = (uint)PHYSFS_read(fs_file, *buffer, 1, size);
			if (readed != size)
			{
				LOG_ERROR("File System error while reading from file %s: %s\n", file, PHYSFS_getLastError());
				RELEASE_ARRAY(buffer);
			}
			else
			{
				ret = readed;
				//Adding end of file at the end of the buffer. Loading a shader file does not add this for some reason
				(*buffer)[size] = '\0';
			}
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG_ERROR("File System error while closing file %s: %s\n", file, PHYSFS_getLastError());
	}
	else
		LOG_ERROR("File System error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

bool FileSystem::DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath)
{
	std::string fileStr, extensionStr;
	SplitFilePath(file, nullptr, &fileStr, &extensionStr);

	relativePath = relativePath.append(dstFolder).append("/") + fileStr.append(".") + extensionStr;
	std::string finalPath = std::string(*PHYSFS_getSearchPath()).append("/") + relativePath;

	return DuplicateFile(file, finalPath.c_str());

}

bool FileSystem::DuplicateFile(const char* srcFile, const char* dstFile)
{
	//TODO: Compare performance to calling Load(srcFile) and then Save(dstFile)
	std::ifstream src;
	src.open(srcFile, std::ios::binary);
	bool srcOpen = src.is_open();
	std::ofstream  dst(dstFile, std::ios::binary);
	bool dstOpen = dst.is_open();

	dst << src.rdbuf();

	src.close();
	dst.close();

	if (srcOpen && dstOpen)
	{
		LOG("[success] File Duplicated Correctly");
		return true;
	}
	else
	{
		LOG("[error] File could not be duplicated");
		return false;
	}
}

int close_sdl_rwops(SDL_RWops* rw)
{
	RELEASE_ARRAY(rw->hidden.mem.base);
	SDL_FreeRW(rw);
	return 0;
}

// Save a whole buffer to disk
uint FileSystem::Save(const char* file, const void* buffer, unsigned int size, bool append) 
{
	unsigned int ret = 0;

	bool overwrite = PHYSFS_exists(file) != 0;
	PHYSFS_file* fs_file = (append) ? PHYSFS_openAppend(file) : PHYSFS_openWrite(file);

	if (fs_file != nullptr)
	{
		uint written = (uint)PHYSFS_write(fs_file, (const void*)buffer, 1, size);
		if (written != size)
		{
			LOG("[error] File System error while writing to file %s: %s", file, PHYSFS_getLastError());
		}
		else
		{
			if (append == true)
			{
				LOG("Added %u data to [%s%s]", size, GetWriteDir(), file);
			}
			else if (overwrite == true)
			{
				LOG("File [%s%s] overwritten with %u bytes", GetWriteDir(), file, size);
			}
			else
			{
				LOG("New file created [%s%s] of %u bytes", GetWriteDir(), file, size);
			}

			ret = written;
		}

		if (PHYSFS_close(fs_file) == 0)
			LOG("[error] File System error while closing file %s: %s", file, PHYSFS_getLastError());
	}
	else
		LOG("[error] File System error while opening file %s: %s", file, PHYSFS_getLastError());

	return ret;
}

std::string FileSystem::GetUniqueName(const char* path, const char* name) 
{
	//TODO: modify to distinguix files and dirs?
	std::vector<std::string> files, dirs;
	DiscoverFiles(path, files, dirs);

	std::string finalName(name);
	bool unique = false;

	for (uint i = 0; i < 50 && unique == false; ++i)
	{
		unique = true;

		//Build the compare name (name_i)
		if (i > 0)
		{
			finalName = std::string(name).append("_");
			if (i < 10)
				finalName.append("0");
			finalName.append(std::to_string(i));
		}

		//Iterate through all the files to find a matching name
		for (uint f = 0; f < files.size(); ++f)
		{
			if (finalName == files[f])
			{
				unique = false;
				break;
			}
		}
	}
	return finalName;
}

void FileSystem::LoadFile(const char* file_path, bool drag_and_drop)
{
	std::string extension = GetFileFormat(file_path);
	std::string processed_path = ProcessPath(file_path);

	if (extension == ".fbx")
	{
		GameObject* imported_fbx = MeshImporter::LoadFBX(processed_path.c_str());

		if (imported_fbx != nullptr)
			App->scene->AddGameObject(imported_fbx);
	}
	else if (extension == ".png" || extension ==".jpg" || extension == ".bmp" || extension == ".tga")
	{
		if (drag_and_drop)
		{
			App->scene->SetDroppedTexture(TextureImporter::LoadTexture(processed_path.c_str()));
		}
	}
}

std::string FileSystem::GetFileFormat(const char* path)
{
	std::string format = PathFindExtensionA(path);
	std::transform(format.begin(), format.end(), format.begin(), [](unsigned char c) { return std::tolower(c); });
	return format;
}

std::string FileSystem::GetFile(const char* path)
{
	std::string file;
	std::string file_path;
	SplitFilePath(path, &file_path, &file);
	return file;
}

#pragma endregion 

#pragma region MeshImporter

GameObject* MeshImporter::LoadFBX(const char* path)
{
	Timer timer;
	timer.Start();

	GameObject* root = nullptr;

	std::string normalized_path = FileSystem::NormalizePath(path);
	std::string relative_path = FileSystem::GetPathRelativeToAssets(normalized_path.c_str());

	char* buffer = nullptr;
	uint size = FileSystem::Load(relative_path.c_str(), &buffer);

	if (buffer == nullptr)
	{
		if(relative_path.size() <= 0) {
			size = FileSystem::Load(normalized_path.c_str(), &buffer);
		}
		else {
			size = FileSystem::Load(relative_path.c_str(), &buffer);
		}
	}

	const aiScene* scene = nullptr;

	if (buffer != nullptr) 
	{
		scene = aiImportFileFromMemory(buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, NULL);
		
		if(scene == NULL)
		{
			LOG_ERROR("Error trying to load %s directly from memory", path);
			scene = aiImportFile(normalized_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
		}
	}
	else 
	{
		scene = aiImportFile(normalized_path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	}

	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* rootNode = scene->mRootNode;
		root = PreorderChildren(scene, rootNode, nullptr, nullptr, path);
		aiReleaseImport(scene);
		LOG("%s loaded in %.3f s", path, timer.ReadSec());
	}
	else
		LOG_ERROR("Error loading scene %s", path);

	RELEASE_ARRAY(buffer);

	//root->UpdateChildrenTransforms();

	return root;
}

GnMesh* MeshImporter::LoadMesh(const aiScene* scene, aiNode* node, const char* path) {
	GnMesh* currentMesh = new GnMesh();
	aiMesh* currentAiMesh = scene->mMeshes[*node->mMeshes];

	//vertex copying
	currentMesh->vertices_amount = currentAiMesh->mNumVertices;
	currentMesh->vertices = new float[currentMesh->vertices_amount * 3]();
	memcpy(currentMesh->vertices, currentAiMesh->mVertices, sizeof(float) * currentMesh->vertices_amount * 3);
	LOG("%s loaded with %d vertices", currentAiMesh->mName.C_Str(), currentMesh->vertices_amount);

	//indices copying
	if (currentAiMesh->HasFaces())
	{
		currentMesh->indices_amount = currentAiMesh->mNumFaces * 3;
		currentMesh->indices = new uint[currentMesh->indices_amount]();
		LOG("%s loaded with %d indices", currentAiMesh->mName.C_Str(), currentMesh->indices_amount);

		for (size_t f = 0; f < currentAiMesh->mNumFaces; f++)
		{
			if (currentAiMesh->mFaces[f].mNumIndices != 3)
			{
				LOG_WARNING("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&currentMesh->indices[f * 3], currentAiMesh->mFaces[f].mIndices, 3 * sizeof(uint));
			}
		}
	}

	currentMesh->texcoords = new float[currentMesh->vertices_amount * 2]();
	currentMesh->colors = new float[currentMesh->indices_amount * 4]();
	currentMesh->normals = new float[currentAiMesh->mNumVertices * 3]();

	int t = 0;

	//normals, color and texture coordinates
	for (size_t v = 0, n = 0, tx = 0, c = 0; v < currentAiMesh->mNumVertices; v++, n += 3, c += 4, tx += 2)
	{
		//normal copying
		if (currentAiMesh->HasNormals())
		{
			//normal copying
			currentMesh->normals[n] = currentAiMesh->mNormals[v].x;
			currentMesh->normals[n + 1] = currentAiMesh->mNormals[v].y;
			currentMesh->normals[n + 2] = currentAiMesh->mNormals[v].z;
		}
		//color copying
		if (currentAiMesh->HasVertexColors(v))
		{
			currentMesh->colors[c] = currentAiMesh->mColors[v]->r;
			currentMesh->colors[c + 1] = currentAiMesh->mColors[v]->g;
			currentMesh->colors[c + 2] = currentAiMesh->mColors[v]->b;
			currentMesh->colors[c + 3] = currentAiMesh->mColors[v]->a;
		}
		else
		{
			currentMesh->colors[c] = 0.0f;
			currentMesh->colors[c + 1] = 0.0f;
			currentMesh->colors[c + 2] = 0.0f;
			currentMesh->colors[c + 3] = 0.0f;
		}

		//texcoords copying
		if (currentAiMesh->mTextureCoords[0])
		{
			currentMesh->texcoords[tx] = currentAiMesh->mTextureCoords[0][v].x;
			currentMesh->texcoords[tx + 1] = currentAiMesh->mTextureCoords[0][v].y;
			//LOG("TexCoords: %.2f , %.2f", currentMesh->texcoords[tx], currentMesh->texcoords[tx + 1]);
		}
		else
		{
			currentMesh->texcoords[tx] = 0.0f;
			currentMesh->texcoords[tx + 1] = 0.0f;
		}
		t = tx;
	}
	//LOG("Texcoords loaded: %d", t);
	currentMesh->GenerateBuffers();
	//currentMesh->GenerateAABB(currentMesh);
	return currentMesh;
}

GameObject* MeshImporter::PreorderChildren(const aiScene* scene, aiNode* node, aiNode* parentNode, GameObject* parentGameObject, const char* path)
{
	GameObject* gameObject = new GameObject();

	if (parentGameObject != nullptr)
	{
		parentGameObject->AddChild(gameObject);
		gameObject->SetParent(parentGameObject);
	}

	if (node->mMeshes != nullptr)
	{
		gameObject->SetName(node->mName.C_Str());

		GnMesh* mesh = LoadMesh(scene, node, path);
		gameObject->AddComponent(mesh);

		GnTexture* texture = TextureImporter::GetAiMeshTexture(scene, node, path);
		Material* material = new Material(mesh, texture);
		gameObject->AddComponent(material);

		LoadTransform(node, gameObject->GetTransform());
	}
	else
	{
		std::string folder;
		std::string file;
		FileSystem::SplitFilePath(path, &folder, &file);
		gameObject->SetName(file.c_str());
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		PreorderChildren(scene, node->mChildren[i], node, gameObject, path);
	}

	return gameObject;
}

void MeshImporter::LoadTransform(aiNode* node, Transform* transform)
{
	aiVector3D position, scaling, eulerRotation;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, position);
	eulerRotation = rotation.GetEuler() * RADTODEG;

	transform->SetPosition(position.x, position.y, position.z);
	transform->SetRotation(eulerRotation.x, eulerRotation.y, eulerRotation.z);

	if (FileSystem::normalize_scales) {
		if (scaling.x == 100 && scaling.y == 100 && scaling.z == 100) 
		{
			scaling.x = scaling.y = scaling.z = 1.0f;
		}
	}

	transform->SetScale(scaling.x, scaling.y, scaling.z);
	transform->UpdateLocalTransform();
}

#pragma endregion 

#pragma region TextureImporter

GnTexture* TextureImporter::GetAiMeshTexture(const aiScene* scene, aiNode* node, const char* path)
{
	aiMesh* currentAiMesh = scene->mMeshes[*node->mMeshes];

	aiMaterial* material = scene->mMaterials[currentAiMesh->mMaterialIndex];
	aiString aiTexture_path;
	aiGetMaterialTexture(material, aiTextureType_DIFFUSE, currentAiMesh->mMaterialIndex, &aiTexture_path);

	GnTexture* texture = new GnTexture();

	if (aiTexture_path.length != 0)
	{
		std::string tmp_path = FindTexture(aiTexture_path.C_Str(), path);

		if (tmp_path.size() <= 0)
			tmp_path = aiTexture_path.C_Str();

		texture = LoadTexture(tmp_path.c_str());
		texture->name = aiTexture_path.C_Str();
		texture->path = tmp_path;
	}

	return texture;
}

GnTexture* TextureImporter::LoadTexture(const char* path)
{
	Timer timer;
	timer.Start();
	ILuint imageID = 0;

	std::string normalized_path = FileSystem::NormalizePath(path);
	std::string relative_path = FileSystem::GetPathRelativeToAssets(normalized_path.c_str());

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

	GnTexture* texture = new GnTexture();

	if (imageID == 0)
		LOG_ERROR("Could not create a texture buffer to load: %s, %d", path, ilGetError());

	char* buffer;
	uint size = FileSystem::Load(relative_path.c_str(), &buffer);

	ILenum file_format = IL_TYPE_UNKNOWN;

	std::string format = FileSystem::GetFileFormat(path);

	if (format == ".png")
		file_format = IL_PNG;
	else if (format == ".jpg")
		file_format = IL_JPG;
	else if (format == ".bmp")
		file_format = IL_BMP;


	if (ilLoadL(file_format, buffer, size) == IL_FALSE)
	{
		LOG_WARNING("Error trying to load the texture %s into buffer, %d: %s", path, ilGetError(), iluErrorString(ilGetError()));
		buffer = nullptr;
		ilGenImages(1, &imageID);
		ilBindImage(imageID);

		if (ilLoadImage(normalized_path.c_str()) == IL_FALSE)
		{
			LOG_ERROR("Error trying to load the texture directly from %s", path);
		}
	}

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	ILenum error;
	error = ilGetError();

	if (error != IL_NO_ERROR)
	{
		LOG_ERROR("%d: %s", error, iluErrorString(error));
	}
	else
	{
		LOG("Texture loaded successfully from: %s in %.3f s", path, timer.ReadSec());

		texture->id = (ILuint)imageID;
		texture->name = FileSystem::GetFile(path) + format;
		texture->data = ilGetData();
		texture->width = ilGetInteger(IL_IMAGE_WIDTH);
		texture->height = ilGetInteger(IL_IMAGE_HEIGHT);
		texture->path = normalized_path.c_str();
	}

	ilBindImage(0);

	if(buffer != NULL)
		RELEASE_ARRAY(buffer);

	return texture;
}

std::string TextureImporter::FindTexture(const char* texture_name, const char* model_directory)
{
	std::string path;
	FileSystem::SplitFilePath(model_directory, &path);
	std::string texture_path = path + texture_name;

	//Check if the texture is in the same folder
	if (FileSystem::Exists(texture_path.c_str()))
	{
		return texture_path.c_str();
	}
	else
	{
		//Check if the texture is in a sub folder
		texture_path = path + "Textures/" + texture_name;
		if (FileSystem::Exists(texture_path.c_str()))
		{
			return texture_path.c_str();
		}
		else
		{
			//Check if the texture is in the root textures folder
			texture_path = std::string("Assets/Textures/") + texture_name;
			if (FileSystem::Exists(texture_path.c_str()))
			{
				return texture_path.c_str();
			}
		}
	}

	texture_path.clear();
	return texture_path;
}

void TextureImporter::UnloadTexture(uint imageID)
{
	//ilBindImage(0);
	ilDeleteImages(1, &imageID);
}

#pragma endregion

#pragma region JSONParser

JSON_Array* JSONParser::LoadConfig(char* buffer, JSON_Value* root)
{
	root = json_parse_string(buffer);
	JSON_Object* config_object = json_value_get_object(root);
	JSON_Array* modules = json_object_get_array(config_object, "modules");

	if (root != NULL)
	{
		LOG("Config file loaded successfully");
	}
	else
	{
		LOG_ERROR("Error trying to load config file");
	}

	return modules;
}

JSON_Object* JSONParser::GetJSONObjectByName(const char* name, JSON_Array* modules_array)
{
	int modules = json_array_get_count(modules_array);

	for (size_t i = 0; i < modules; i++)
	{
		JSON_Object* object = json_array_get_object(modules_array, i);
		if (strcmp(name, json_object_get_string(object, "name")) == 0)
			return object;
	}

	LOG_ERROR("JSON object %s could not be found", name);
	return NULL;
}

#pragma endregion