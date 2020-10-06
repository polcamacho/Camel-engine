#pragma once
#ifndef __JSON_DATA_CONTROL_H__
#define __JSON_DATA_CONTROL_H__

#include "parson/parson.h"

void GetJsonValueFromPath(JSON_Value* main_root, const char* name, JSON_Value** container);

#endif // !__JSON_DATA_CONTROL_H__