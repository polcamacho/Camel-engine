#pragma once
#ifndef __COMPONENT_MESH_H__

#include "Component.h"
#include "GameObject.h"

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

	//-----------------------------
};

#endif // !__COMPONENT_MESH_H__