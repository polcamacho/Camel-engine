#include "JSONDataController.h"
#include <string>

void GetJsonValueFromPath(JSON_Value* main_root, const char* name, JSON_Value** container)
{
	const char* path = json_object_get_string(json_value_get_object(main_root), name);
	(*container) = json_parse_file(path);
}