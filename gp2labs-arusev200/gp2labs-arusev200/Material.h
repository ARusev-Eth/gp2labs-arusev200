#ifndef Material_h
#define Material_h
#include <GL/glew.h>
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include "Component.h"

class Material::public Component{
public: 
	Material();
	~Material();

	void destroy;
	void bind();
	bool loadShader(const std::string& vsFilename, const std::string& fsFileName);

	Glint getUnifromLocation(const std::string& name);

protected:
private:
	GLuint m_ShaderProgram;
};

#endif