#ifndef GameObject_h
#define GameObject_h

#include <string>
#include <vector>

class Component;
class Transform;
class Mesh;
class Material;
class Camera;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void init();
	void update();
	void render();
	void destroy();

	void addComponent(Component * component);
	void setname(const std::string& name);
	const std::string& getName();

	void setTransform(Transform * transform);
	void setMesh(Mesh * mesh);
	void setMaterial(Material * material);
	void setCamera(Camera * camera);

	Transform * getTransform();
	Material * getMaterial();
	Mesh* getMesh();
	Camera* getCamera();

protected:
private:
	std::vector<Component*> m_components;
	std::string m_Name;

	Transform *m_Transform;
	Mesh *m_Mesh;
	Material *m_Material;
	Camera *m_Camera;
};

#endif

#endif