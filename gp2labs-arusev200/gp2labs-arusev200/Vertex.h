#ifndef Vertex_h
#define Vertex_h

#include <glm/glm.hpp>
using glm::vec4;
using glm::vec3;
using glm::vec2;

struct Vertex
{
	vec3 position;
	vec2 textCoords;
	vec4 Colours;
};

#define BUFFER_OFFSET(i) ((chat*)NULL + (i))
#endif