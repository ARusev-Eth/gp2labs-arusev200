#include "Material.h"
#include "Shader.h"

Material::Material()
{
	m_ShaderProgram = -1;
	m_Type = "Material";
}

Material::~Material()
{
}

//Used in GameObject, memory matters
void Material::destroy()
{
	glDeleteProgram(m_ShaderProgram);
}

void Material::bind()
{
	glUseProgram(m_ShaderProgram);
}

bool Material::loadShader(const std::string &vsFileName, const std::string &fsFileName)
{
	GLuint vertexShaderProgram = 0;
	vertexShaderProgram = loadShaderFromFile(const_cast<std::string&>(vsFileName), VERTEX_SHADER);

	GLuint fragmentShaderProgram = 0;
	fragmentShaderProgram = loadShaderFromFile(const_cast<std::string&>(fsFileName), FRAGMENT_SHADER);

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, fragmentShaderProgram);
	glAttachShader(m_ShaderProgram, vertexShaderProgram);
	//Couldn't find proper explenation on the webz, assuming it compiles shaders into our program
	glLinkProgram(m_ShaderProgram);

	//MUCHO IMPORTANTE!
	checkForCompilerErrors(m_ShaderProgram);

	//Hence we no longer need the shaders
	glDeleteShader(vertexShaderProgram);
	glDeleteShader(fragmentShaderProgram);

	//Vertex info?
	glBindAttribLocation(m_ShaderProgram, 0, "vertexPosition");
	glBindAttribLocation(m_ShaderProgram, 1, "vertexTextCords");
	glBindAttribLocation(m_ShaderProgram, 2, "vertexColor");

	return true;
}

GLint Material::getUnifromLocation(const std::string &name)
{
	return glGetUniformLocation(m_ShaderProgram, name.c_str());
}
