#include "JSONDataController.h"
#include "Globals.h"

void GetJsonValueFromPath(JSON_Value* main_root, const char* name, JSON_Value** container)
{
	const char* path = json_object_get_string(json_value_get_object(main_root), name);
	if (DoesJsonFileExist(path))
	{
		LOG("JSON file with path %c, successfully loaded...", path);
		(*container) = json_parse_file(path);
	}
	else
	{
		LOG("JSON file with path %c, couldn't be loaded...", path);
		(*container) = NULL;
	}
}

inline bool DoesJsonFileExist(const std::string& file) {
	struct stat buffer;
	return (stat(file.c_str(), &buffer) == 0);
}