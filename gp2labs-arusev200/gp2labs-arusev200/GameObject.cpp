#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

GameObject::GameObject()
{
	m_Transform = NULL;
	m_Mesh = NULL;
	m_Material = NULL;
	m_Camera = NULL;
	m_Name = "GameObject";
}

GameObject::~GameObject()
{
}

void GameObject::init()
{
	//Calls all components' update functions
	//So confused when this wasn't working... turns out I just spelt it with a non-capital c X_X
	for (auto iter = m_Components.begin(); iter != m_Components.end(); iter++)
	{
		(*iter)->init();
	}
}

void GameObject::update()
{
	for (auto iter = m_Components.begin(); iter != m_Components.end(); iter++)
	{
		(*iter)->update();
	}
}

void GameObject::render()
{
	//Analogical to the previous function
	for (auto iter = m_Components.begin(); iter != m_Components.end(); iter++)
	{
		(*iter)->render();
	}
}

void GameObject::destroy()
{
	auto iter = m_Components.begin();
	while (iter != m_Components.end())
	{
		if ((*iter))
		{
			delete(*iter); //Dereference
			(*iter) = NULL; //Clean up
			iter = m_Components.erase(iter); //Kill... kinda works backwards in criminal series X_X
		}
		else
		{
			iter++;
		}
	}
	m_Components.clear();
}

//Does what it says on the box
void GameObject::addComponent(Component *component)
{
	component->setParent(this);
	m_Components.push_back(component);
}

//Does what it says on the box
void GameObject::setName(const std::string& name)
{
	m_Name = name;
}

//Does what it says on the box
const std::string& GameObject::getName()
{
	return m_Name;
}

//Does what it says on the box
void GameObject::setTransform(Transform *transform)
{
	m_Transform = transform;
	addComponent(transform);
}

//Is there even a point to these comments
void GameObject::setMesh(Mesh *mesh)
{
	m_Mesh = mesh;
	addComponent(mesh);
}

//I don't even know anymore
void GameObject::setMaterial(Material *material)
{
	m_Material = material;
	addComponent(material);
}

//This is here for the sole purpose of entertainment 
void GameObject::setCamera(Camera *camera)
{
	m_Camera = camera;
	addComponent(camera);
}

//Time for getters now that setters are done with
Transform * GameObject::getTransform()
{
	return m_Transform;
}

//MOAR GETTERS
Mesh * GameObject::getMesh()
{
	return m_Mesh;
}

//Why... I don't know
Material * GameObject::getMaterial()
{
	return m_Material;
}

//WHO BE THE CAMERA! STEP UP
Camera * GameObject::getCamera()
{
	return m_Camera;
}


