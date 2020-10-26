#include "Globals.h"
#include "EngineUI.h"
#include "FileSystem.h"
//#include "PathNode.h"

#include "PhysFS/include/physfs.h"
#include <fstream>
#include <filesystem>

#include "Assimp/Assimp/include/cfileio.h"
#include "Assimp/Assimp/include/types.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

//m_filesystem::m_filesystem(bool start_enabled) : module("filesystem", true)
//{
//	// needs to be created before init so other modules can use it
//	char* base_path = sdl_getbasepath();
//	physfs_init(nullptr);
//	sdl_free(base_path);
//
//	//setting the working directory as the writing directory
//	if (physfs_setwritedir(".") == 0)
//		log("file system error while creating write dir: %s\n", physfs_getlasterror());
//
//	addpath("."); //adding projectfolder (working directory)
//	addpath("assets");
//	createlibrarydirectories();
//}
//
//// destructor
//m_filesystem::~m_filesystem()
//{
//	physfs_deinit();
//}
//
//// called before render is available
//bool m_filesystem::init(config& config)
//{
//	log("loading file system");
//	bool ret = true;
//
//	// ask sdl for a write dir
//	char* write_path = sdl_getprefpath(engine->getorganizationname(), engine->gettitlename());
//
//	// trun this on while in game mode
//	//if(physfs_setwritedir(write_path) == 0)
//	//	log("file system error while creating write dir: %s\n", physfs_getlasterror());
//
//	sdl_free(write_path);
//
//	return ret;
//}
//
//// called before quitting
//bool m_filesystem::cleanup()
//{
//	//log("freeing file system subsystem");
//
//	return true;
//}
//
//void m_filesystem::createlibrarydirectories()
//{
//	createdir(library_path);
//	createdir(folders_path);
//	createdir(meshes_path);
//	createdir(textures_path);
//	createdir(materials_path);
//	createdir(gameobjects_path);
//	createdir(animations_path);
//	createdir(particles_path);
//	createdir(shaders_path);
//
//}
//
//// add a new zip file or folder
//bool m_filesystem::addpath(const char* path_or_zip)
//{
//	bool ret = false;
//
//	if (physfs_mount(path_or_zip, nullptr, 1) == 0)
//	{
//		log("file system error while adding a path or zip: %s\n", physfs_getlasterror());
//	}
//	else
//		ret = true;
//
//	return ret;
//}
//
//// check if a file exists
//bool m_filesystem::exists(const char* file) const
//{
//	return physfs_exists(file) != 0;
//}
//
//bool m_filesystem::createdir(const char* dir)
//{
//	if (isdirectory(dir) == false)
//	{
//		physfs_mkdir(dir);
//		return true;
//	}
//	return false;
//}
//
//// check if a file is a directory
//bool m_filesystem::isdirectory(const char* file) const
//{
//	return physfs_isdirectory(file) != 0;
//}
//
//const char* m_filesystem::getwritedir() const
//{
//	//todo: erase first annoying dot (".")
//	return physfs_getwritedir();
//}
//
//void m_filesystem::discoverfiles(const char* directory, std::vector<std::string>& file_list, std::vector<std::string>& dir_list) const
//{
//	char** rc = physfs_enumeratefiles(directory);
//	char** i;
//
//	for (i = rc; *i != nullptr; i++)
//	{
//		std::string str = std::string(directory) + std::string("/") + std::string(*i);
//		if (isdirectory(str.c_str()))
//			dir_list.push_back(*i);
//		else
//			file_list.push_back(*i);
//	}
//
//	physfs_freelist(rc);
//}
//
//void m_filesystem::getallfileswithextension(const char* directory, const char* extension, std::vector<std::string>& file_list) const
//{
//	std::vector<std::string> files;
//	std::vector<std::string> dirs;
//	discoverfiles(directory, files, dirs);
//
//	for (uint i = 0; i < files.size(); i++)
//	{
//		std::string ext;
//		splitfilepath(files[i].c_str(), nullptr, nullptr, &ext);
//
//		if (ext == extension)
//			file_list.push_back(files[i]);
//	}
//}
//
//pathnode m_filesystem::getallfiles(const char* directory, std::vector<std::string>* filter_ext, std::vector<std::string>* ignore_ext) const
//{
//	pathnode root;
//	if (exists(directory))
//	{
//		root.path = directory;
//		engine->filesystem->splitfilepath(directory, nullptr, &root.localpath);
//		if (root.localpath == "")
//			root.localpath = directory;
//
//		std::vector<std::string> file_list, dir_list;
//		discoverfiles(directory, file_list, dir_list);
//
//		//adding all child directories
//		for (uint i = 0; i < dir_list.size(); i++)
//		{
//			std::string str = directory;
//			str.append("/").append(dir_list[i]);
//			root.children.push_back(getallfiles(str.c_str(), filter_ext, ignore_ext));
//		}
//		//adding all child files
//		for (uint i = 0; i < file_list.size(); i++)
//		{
//			//filtering extensions
//			bool filter = true, discard = false;
//			if (filter_ext != nullptr)
//			{
//				filter = hasextension(file_list[i].c_str(), *filter_ext);
//			}
//			if (ignore_ext != nullptr)
//			{
//				discard = hasextension(file_list[i].c_str(), *ignore_ext);
//			}
//			if (filter == true && discard == false)
//			{
//				std::string str = directory;
//				str.append("/").append(file_list[i]);
//				root.children.push_back(getallfiles(str.c_str(), filter_ext, ignore_ext));
//			}
//		}
//		root.isfile = hasextension(root.path.c_str());
//		root.isleaf = root.children.empty() == true;
//	}
//	return root;
//}
//
//void m_filesystem::getrealdir(const char* path, std::string& output) const
//{
//	output = physfs_getbasedir();
//
//	std::string basedir = physfs_getbasedir();
//	std::string searchpath = *physfs_getsearchpath();
//	std::string realdir = physfs_getrealdir(path);
//
//	output.append(*physfs_getsearchpath()).append("/");
//	output.append(physfs_getrealdir(path)).append("/").append(path);
//}
//
//std::string m_filesystem::getpathrelativetoassets(const char* originalpath) const
//{
//	std::string ret;
//	getrealdir(originalpath, ret);
//
//	return ret;
//}
//
//bool m_filesystem::hasextension(const char* path) const
//{
//	std::string ext = "";
//	splitfilepath(path, nullptr, nullptr, &ext);
//	return ext != "";
//}
//
//bool m_filesystem::hasextension(const char* path, std::string extension) const
//{
//	std::string ext = "";
//	splitfilepath(path, nullptr, nullptr, &ext);
//	return ext == extension;
//}
//
//bool m_filesystem::hasextension(const char* path, std::vector<std::string> extensions) const
//{
//	std::string ext = "";
//	splitfilepath(path, nullptr, nullptr, &ext);
//	if (ext == "")
//		return true;
//	for (uint i = 0; i < extensions.size(); i++)
//	{
//		if (extensions[i] == ext)
//			return true;
//	}
//	return false;
//}
//
//std::string m_filesystem::normalizepath(const char* full_path) const
//{
//	std::string newpath(full_path);
//	for (int i = 0; i < newpath.size(); ++i)
//	{
//		if (newpath[i] == '\\')
//			newpath[i] = '/';
//	}
//	return newpath;
//}
//
//void m_filesystem::splitfilepath(const char* full_path, std::string* path, std::string* file, std::string* extension) const
//{
//	if (full_path != nullptr)
//	{
//		std::string full(full_path);
//		size_t pos_separator = full.find_last_of("\\/");
//		size_t pos_dot = full.find_last_of(".");
//
//		if (path != nullptr)
//		{
//			if (pos_separator < full.length())
//				*path = full.substr(0, pos_separator + 1);
//			else
//				path->clear();
//		}
//
//		if (file != nullptr)
//		{
//			if (pos_separator < full.length())
//				*file = full.substr(pos_separator + 1, pos_dot - pos_separator - 1);
//			else
//				*file = full.substr(0, pos_dot);
//		}
//
//		if (extension != nullptr)
//		{
//			if (pos_dot < full.length())
//				*extension = full.substr(pos_dot + 1);
//			else
//				extension->clear();
//		}
//	}
//}
//
//unsigned int m_filesystem::load(const char* path, const char* file, char** buffer) const
//{
//	std::string full_path(path);
//	full_path += file;
//	return load(full_path.c_str(), buffer);
//}
//
//// read a whole file and put it in a new buffer
//uint m_filesystem::load(const char* file, char** buffer) const
//{
//	uint ret = 0;
//
//	physfs_file* fs_file = physfs_openread(file);
//
//	if (fs_file != nullptr)
//	{
//		physfs_sint32 size = (physfs_sint32)physfs_filelength(fs_file);
//
//		if (size > 0)
//		{
//			*buffer = new char[size + 1];
//			uint readed = (uint)physfs_read(fs_file, *buffer, 1, size);
//			if (readed != size)
//			{
//				log("file system error while reading from file %s: %s\n", file, physfs_getlasterror());
//				release(buffer);
//			}
//			else
//			{
//				ret = readed;
//				//adding end of file at the end of the buffer. loading a shader file does not add this for some reason
//				(*buffer)[size] = '\0';
//			}
//		}
//
//		if (physfs_close(fs_file) == 0)
//			log("file system error while closing file %s: %s\n", file, physfs_getlasterror());
//	}
//	else
//		log("file system error while opening file %s: %s\n", file, physfs_getlasterror());
//
//	return ret;
//}
//
//bool m_filesystem::duplicatefile(const char* file, const char* dstfolder, std::string& relativepath)
//{
//	std::string filestr, extensionstr;
//	splitfilepath(file, nullptr, &filestr, &extensionstr);
//
//	relativepath = relativepath.append(dstfolder).append("/") + filestr.append(".") + extensionstr;
//	std::string finalpath = std::string(*physfs_getsearchpath()).append("/") + relativepath;
//
//	return duplicatefile(file, finalpath.c_str());
//
//}
//
//bool m_filesystem::duplicatefile(const char* srcfile, const char* dstfile)
//{
//	//todo: compare performance to calling load(srcfile) and then save(dstfile)
//	std::ifstream src;
//	src.open(srcfile, std::ios::binary);
//	bool srcopen = src.is_open();
//	std::ofstream  dst(dstfile, std::ios::binary);
//	bool dstopen = dst.is_open();
//
//	dst << src.rdbuf();
//
//	src.close();
//	dst.close();
//
//	if (srcopen && dstopen)
//	{
//		log("[success] file duplicated correctly");
//		return true;
//	}
//	else
//	{
//		log("[error] file could not be duplicated");
//		return false;
//	}
//}
//
//int close_sdl_rwops(sdl_rwops* rw)
//{
//	release_array(rw->hidden.mem.base);
//	sdl_freerw(rw);
//	return 0;
//}
//
//// save a whole buffer to disk
//uint m_filesystem::save(const char* file, const void* buffer, unsigned int size, bool append) const
//{
//	unsigned int ret = 0;
//
//	bool overwrite = physfs_exists(file) != 0;
//	physfs_file* fs_file = (append) ? physfs_openappend(file) : physfs_openwrite(file);
//
//	if (fs_file != nullptr)
//	{
//		uint written = (uint)physfs_write(fs_file, (const void*)buffer, 1, size);
//		if (written != size)
//		{
//			log("[error] file system error while writing to file %s: %s", file, physfs_getlasterror());
//		}
//		else
//		{
//			if (append == true)
//				log("added %u data to [%s%s]", size, getwritedir(), file);
//			else if (overwrite == true)
//				log("file [%s%s] overwritten with %u bytes", getwritedir(), file, size);
//			else
//				log("new file created [%s%s] of %u bytes", getwritedir(), file, size);
//
//			ret = written;
//		}
//
//		if (physfs_close(fs_file) == 0)
//			log("[error] file system error while closing file %s: %s", file, physfs_getlasterror());
//	}
//	else
//		log("[error] file system error while opening file %s: %s", file, physfs_getlasterror());
//
//	return ret;
//}
//
//bool m_filesystem::remove(const char* file)
//{
//	bool ret = false;
//
//	if (file != nullptr)
//	{
//		//if it is a directory, we need to recursively remove all the files inside
//		if (isdirectory(file))
//		{
//			std::vector<std::string> containedfiles, containeddirs;
//			pathnode rootdirectory = getallfiles(file);
//
//			for (uint i = 0; i < rootdirectory.children.size(); ++i)
//				remove(rootdirectory.children[i].path.c_str());
//		}
//
//		if (physfs_delete(file) != 0)
//		{
//			log("file deleted: [%s]", file);
//			ret = true;
//		}
//		else
//			log("file system error while trying to delete [%s]: %s", file, physfs_getlasterror());
//	}
//
//	return ret;
//}
//
//uint64 m_filesystem::getlastmodtime(const char* filename)
//{
//	return physfs_getlastmodtime(filename);
//}
//
//std::string m_filesystem::getuniquename(const char* path, const char* name) const
//{
//	//todo: modify to distinguix files and dirs?
//	std::vector<std::string> files, dirs;
//	discoverfiles(path, files, dirs);
//
//	std::string finalname(name);
//	bool unique = false;
//
//	for (uint i = 0; i < 50 && unique == false; ++i)
//	{
//		unique = true;
//
//		//build the compare name (name_i)
//		if (i > 0)
//		{
//			finalname = std::string(name).append("_");
//			if (i < 10)
//				finalname.append("0");
//			finalname.append(std::to_string(i));
//		}
//
//		//iterate through all the files to find a matching name
//		for (uint f = 0; f < files.size(); ++f)
//		{
//			if (finalname == files[f])
//			{
//				unique = false;
//				break;
//			}
//		}
//	}
//	return finalname;
//}