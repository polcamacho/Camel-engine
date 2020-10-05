#pragma once
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "parson/parson.h"
#include "Module.h"

class Config
{
public:

	// Config Constructor
	Config() {}

	// Config Destructor
	~Config() {}

	static void LoadConfigFile(Module(*mod), JSON_Object* root) 
	{
		mod->LoadModuleConfig(root);
	}

	static void SaveConfigFile(Module(*mod), JSON_Object* root)
	{
		mod->SaveModuleConfig(root);
	}

public:


};

#endif // !__CONFIG_H__