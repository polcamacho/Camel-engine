#include "JSONDataController.h"

JSON_Value* GetJsonValueFromPath(JSON_Value* main_root, const char* name)
{
	JSON_Value* root;
	JSON_Object* node = json_value_get_object(main_root);
	const char* path_name = json_object_get_string(node, name);
	root = json_parse_string(path_name);
	return root;
}