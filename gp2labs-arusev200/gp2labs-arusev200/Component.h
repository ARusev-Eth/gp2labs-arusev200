#ifndef Component_h
#define Component_h

#include <string>

class GameObject;

class Component
{
public:
	Component();
	virtual ~Component();

	virtual void init();
	virtual void update();
	virtual void render();
	virtual void destroy();
	bool isActive();
	void toggleActive();
	void setParent(GameObject * object);

	const std::string& getType();

protected:
	std::string m_Type;
	bool m_Active;
	GameObject * m_Parent;
};

#endif