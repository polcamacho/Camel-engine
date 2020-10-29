#pragma once
#ifndef __COMPONENT_MESH_H__

#include "Component.h"
#include "GameObject.h"
#include "MeshHandler.h"

class ComponentMesh : public Component {
public:

	//Constructor
	ComponentMesh(GameObject* owner, bool active = true);

	//Destructor
	~ComponentMesh();


	//Update
	void Update() override;

	//Enable component
	void Enable() override;

	//Disable component
	void Disable() override;

	//Check if component is enabled
	bool IsEnabled() override;

	
	//Own mesh functions ----------
	std::vector<MeshPart*>* GetMesh();
	//-----------------------------

public:
	bool requesting_mesh;

private:
	std::vector<MeshPart*>* mesh;
	char* path;
};

#endif // !__COMPONENT_MESH_H__