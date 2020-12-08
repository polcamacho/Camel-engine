#pragma once
#include "Globals.h"

#include <vector>
#include <string>

#pragma region ForwardDeclarations

class GameObject;
class GnMesh;
class Transform;
class Material;
struct GnTexture;

class aiScene;
class aiMesh;
class aiMaterial;
struct aiNode;

struct json_array_t;
typedef json_array_t JSON_Array;

struct json_value_t;
typedef json_value_t JSON_Value;

struct json_object_t;
typedef json_object_t JSON_Object;

#pragma endregion

namespace FileSystem 
{
	static bool normalize_scales;

	void Init();
	void CleanUp();

	void GetPhysFSVersion(std::string& version_str);

	void CreateLibraryDirectories();

	bool AddPath(const char* path_or_zip);
	bool Exists(const char* file);
	bool CreateDir(const char* dir);
	bool IsDirectory(const char* file);
	const char* GetWriteDir();

	void DiscoverFiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list);
	void GetAllFilesWithExtension(const char* directory, const char* extension, std::vector<std::string>& file_list);
	//PathNode GetAllFiles(const char* directory, std::vector<std::string>* filter_ext = nullptr, std::vector<std::string>* ignore_ext = nullptr);
	void GetRealDir(const char* path, std::string& output);
	std::string GetPathRelativeToAssets(const char* originalPath);

	bool HasExtension(const char* path);
	bool HasExtension(const char* path, std::string extension);
	bool HasExtension(const char* path, std::vector<std::string> extensions);

	std::string ProcessPath(const char* path);
	std::string NormalizePath(const char* path);
	void SplitFilePath(const char* full_path, std::string* path, std::string* file = nullptr, std::string* extension = nullptr);

	// Open for Read/Write
	unsigned int Load(const char* path, const char* file, char** buffer);
	unsigned int Load(const char* file, char** buffer);

	bool DuplicateFile(const char* file, const char* dstFolder, std::string& relativePath);
	bool DuplicateFile(const char* srcFile, const char* dstFile);

	unsigned int Save(const char* file, const void* buffer, unsigned int size, bool append = false);
	bool Remove(const char* file);

	//uint64 GetLastModTime(const char* filename);
	std::string GetUniqueName(const char* path, const char* name);

	void LoadFile(const char* file_path, bool drag_and_drop = false);
	std::string GetFileFormat(const char* path);
	std::string GetFile(const char* path);
}

namespace MeshImporter 
{
	// Model import method:
	// ImportModel will use the other functions in order to create the new game object
	GameObject* ImportModel(const char* path);
	// --------------------

	// Mesh Data Import and Managemenet
	void Import(const aiMesh* imp_mesh, GnMesh* own_mesh);
	uint64 Save(const GnMesh* own_mesh, char** file_buffer);
	void Load(const char* file_buffer, GnMesh* own_mesh);
	// --------------------------------

	// GameObject Creation Block
	GameObject* PreorderChildren(const aiScene* scene, aiNode* node, aiNode* parentNode, GameObject* parentGameObject, const char* path);
	void LoadTransform(aiNode* node, Transform* transform);
	// -------------------------
}

namespace MaterialImporter
{
	void Import(const aiMaterial* imp_material, Material* our_material, const char* path);
	uint64 Save(const Material* our_material, char** file_buffer);
}

namespace TextureImporter
{
	std::string FindTexture(const char* texture_name, const char* model_directory);
	GnTexture* LoadTexture(const char* path);

	void UnloadTexture(uint imageID);
}

namespace JSONParser
{
	JSON_Array* LoadConfig(char* buffer, JSON_Value* root);
	JSON_Object* GetJSONObjectByName(const char* name, JSON_Array* modules_array);
}


