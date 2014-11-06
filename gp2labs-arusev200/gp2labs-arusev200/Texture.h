#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

GLuint convertSDLSurfaceToGLTexture(SDL_Surface * surface);
GLuint loadTextureFromFile(const std::string& filename);
//Note: load on startup
GLuint loadTextureFromFont(const std::string& fontFilename, int pointSize, const std::string& text);


#endif