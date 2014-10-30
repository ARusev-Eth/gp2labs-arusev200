//Headers
#include <iostream>
#include <GL/glew.h> //include glew
#include <SDL.h> //SDL functionality
#include <SDL_opengl.h> //OpenGL stuff
#include <gl\GLU.h> //GLU
#include "Vertex.h" //Include the vertex header, changes how our vertices are represented
#include "Shader.h"

//maths	headers
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load it from a memory buffer
GLuint loadShaderFromMemory(const char * pMem, SHADER_TYPE shaderType)
{
	GLuint program = glCreateShader(shaderType);
	glShaderSource(program, 1, &pMem, NULL);
	glCompileShader(program);
	return program;
}

//Variables:

//matrices
mat4	viewMatrix;
mat4	projMatrix;
mat4	worldMatrix;

//Pointer to our window
SDL_Window * window;

//Window creation properties
const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;

bool full = false; //Fullscreen?
bool running = true; //Game loops/black magics
GLuint	shaderProgram=0; //global int to hold shader


//Triangle coordinates
/*float triangleData[] = {0.0f, 1.0f, 0.0f, //Top
						-1.0f, -1.0f, 0.0f, //Bottom Left
						1.0f, -1.0f, 0.0f}; //Bottom Right
*/

Vertex triangleData[]=
{
	
	//Front
	{ -0.5f, 0.5f, 0.5f,
	 1.0f, 0.0f, 1.0f, 1.0f },// Top Left
	{ -0.5f, -0.5f, 0.5f,
	 1.0f, 1.0f, 0.0f, 1.0f },// Bottom Left
	{ 0.5f, -0.5f, 0.5f,
	 0.0f, 1.0f, 1.0f, 1.0f }, //Bottom Right
	{ 0.5f, 0.5f, 0.5f,
	 1.0f, 0.0f, 1.0f, 1.0f },// Top Right
	
	 //back
	{ -0.5f, 0.5f, -0.5f,
	 1.0f, 0.0f, 1.0f, 1.0f },// Top Left
	{ -0.5f, -0.5f, -0.5f,
	 1.0f, 1.0f, 0.0f, 1.0f },// Bottom Left
	{ 0.5f, -0.5f, -0.5f,
	 0.0f, 1.0f, 1.0f, 1.0f }, //Bottom Right
	{ 0.5f, 0.5f, -0.5f,
	 1.0f, 0.0f, 1.0f, 1.0f },// Top Right
};

GLuint indices[]={
	 //front
	 0,1,2,
	 0,3,2,
 
	 //left
	 4,5,1,
	 4,1,0,
 
	 //right
	 3,7,2,
	 7,6,2,
 
	 //bottom
	 1,5,2,
	 6,2,1,
 
	 //top
	 5,0,7,
	 5,7,3,
 
	 //back
	 4,5,6,
	 4,7,6
};

GLuint triangleVBO; //To be filled when generating buffer/integer ID reffered to by the VBOs
GLuint triangleEBO;

SDL_GLContext glContext = NULL; //SDL GL Context

//Variables\\

//Functions:

//Window
void InitWindow(int width, int height, bool fullscreen)
{
	int flags=SDL_WINDOW_OPENGL;
	if (fullscreen)
	{
		flags=SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN;
	}

	window = SDL_CreateWindow(
		"Lab 1",				//Window title
		SDL_WINDOWPOS_CENTERED, //x position of the window - centered
		SDL_WINDOWPOS_CENTERED, //y position of the window - againt centered
		width,					//Window width, in pixels
		height,					//Window height, in pixels
		flags);     //Flags ?? 
}



//Clean up
void CleanUp()
{
	//Reclame memory alocated when #cleaningup
	glDeleteProgram(shaderProgram);
	//glDeleteBuffers(number of buffers specified, actual buffers passed)
	glDeleteBuffers(1,&triangleVBO); 
	glDeleteBuffers(1, &triangleEBO);
	SDL_GL_DeleteContext(glContext); //Delete OGL context before you exit
	SDL_DestroyWindow(window);
	SDL_Quit;
}

//Initialise OpenGL
void initOpenGL()
{
	//Ask for version 3.2 of OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE	);

	glContext = SDL_GL_CreateContext(window); //Create gl context

	if(!glContext)
	{
		std::cout << "Error Creating OpenGL context" << SDL_GetError() << std::endl;
	}

	glShadeModel(GL_SMOOTH);						   //Smooth shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);			   //Clear the background color/set it to black
	glClearDepth(1.0f);								   //Set buffer to 1.0
	glEnable(GL_DEPTH_TEST);						   //Enable depth testing
	glDepthFunc(GL_LEQUAL);							   //Which depth test to use
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //Turn on best perspective correction
	glewExperimental = GL_TRUE;

	GLenum err = glewInit(); //Initialise GLEW #works
	if(GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong */
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}
}

//Set and reset the viewport
void setViewPort(int width, int height)
{
	GLfloat ratio; //Screen ratio

	//Make sure height is always above 0
	if(height == 0)
		height = 1;

	ratio = (GLfloat)width / (GLfloat)height; //calc screen ratio
	glViewport(0, 0, (GLsizei)width, (GLsizei)height); //Setup Viewport
	

}

//Let there be images
void render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Background to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the colour and depth buffers

	//-------------------------------------------------------------------------------------------------------------------
	//Bind and describe contents of the buffer

	//Make the new vertex buffer object active. Repeat here as a sanity check (may have changed since initialisation)
	glBindBuffer(GL_ARRAY_BUFFER,triangleVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
	glUseProgram(shaderProgram);
	
	//Tell	the	shader	that	0	is	the	position	element
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint),GL_UNSIGNED_INT,0);
	SDL_GL_SwapWindow(window); //Swap buffers
	//Establish it's 3 coordinates per vertex with zero space between elements
	//in array and contain floating point numbers
	//AFTER STRUCT: pipeline now needs to know the size of each vertex
	//glVertexPointer(3,GL_FLOAT,sizeof(Vertex),NULL);

	//Sets the color, last parameter says color values(rgba) start 3 floats into each element of the array
	//glColorPointer(4,GL_FLOAT,sizeof(Vertex),(void**)(3 * sizeof(float)));

	//Establish array contains vertices & colours
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	//-------------------------------------------------------------------------------------------------------------------

	//-------------------------------------------------------------------------------------------------------------------
	//Render the bloody triangle

	//glMatrixMode(GL_MODELVIEW); //Switch to ModelView
	//glLoadIdentity(); //Reset using the identity matrix
	//gluLookAt(0.0f,0.0f,0.0f,0.0f,0.0f,-1.0f,0.0,1.0,0.0); //Move from 2D to 3D; first 3 params = x,y,z, second 3 params = look at point = x,y,z; last 3 params are the Up Axis of the cam = x,y,z
	//glTranslatef(4.0f,0.0f,-10.0f);//Translate

	//Draw the triangle, giving the number of vertices provided
	//glDrawArrays(type of primitive we're drawing, start index of the first index in the array buffer(can be used as offset), amount of vertices we're drawing)
	//this is calculated from the total size of the vertices (9) devided by the size of one vertex(3 * sizeof(float)
	//glDrawArrays(GL_TRIANGLES,0,6); 
	//-------------------------------------------------------------------------------------------------------------------

	//To be removed soon...
	/* Commented out for keeps sake

	glMatrixMode (GL_MODELVIEW); //Switch to modelview
	glLoadIdentity(); //Reset using the ID matrix
	glTranslatef(0.0f,0.0f,-5.0f); //Translate -5.0f on the Z-axis
	glBegin(GL_TRIANGLES);
		glColor3f(1.0f,	0.0f, 0.0f); //Color of the vertices
		glVertex3f(0.0f,1.0f,0.0f); //Top
		glColor3f(0.0f,1.0f,0.0f); //EXPERIMENTO
		glVertex3f(-1.0f,-1.0f,0.0f); //Bottom left
		glColor3f(0.0f,1.0f,1.0f); //EXPERIMENTO
		glVertex3f(1.0f,-1.0f,0.0f); //Bottom Right
	glEnd();

	*/
	//To be removed soon...
}

//Let it move
void update()
{
	projMatrix = glm::perspective(45.0f,	(float)WINDOW_WIDTH	/ (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	worldMatrix = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
}

//This fills the Vertex Buffer Object with stuff
//VBO is used to hold vertex data /arrays of vertices/
void initGeometry()
{
	//Create buffer
	//glGenBuffers(Number of buffers to generate, pointer to one or an array of ints)
	glGenBuffers(1, &triangleVBO);
	
	//Make the new VBO active
	//glBindBuffer(type of buffer we're binding, buffer to be bound to pipeline)
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

	//Copy vertex Data to VBO/bound buffer
	//glBufferData(type of buffer we're copying, size of data being copied into the buffer, actual data copied,hint to OGL static/dynamic - in this case static
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleData),triangleData, GL_STATIC_DRAW);

	//create buffer
	glGenBuffers(1, &triangleEBO);
	
	//Make the EBO active
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);
	
	//Copy Index data to the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void	createShader()
{
	GLuint	vertexShaderProgram=0;	
	std::string	vsPath	=	ASSET_PATH	+	SHADER_PATH+"/simpleVS.glsl";	
	vertexShaderProgram	=	loadShaderFromFile(vsPath,	VERTEX_SHADER);
					
	GLuint	fragmentShaderProgram=0;
	std::string	fsPath	=	ASSET_PATH	+	SHADER_PATH	+	"/simpleFS.glsl";
	fragmentShaderProgram	= loadShaderFromFile(fsPath,	FRAGMENT_SHADER);

	shaderProgram	=	glCreateProgram();
	glAttachShader(shaderProgram,	vertexShaderProgram);
	glAttachShader(shaderProgram,	fragmentShaderProgram);
	glLinkProgram(shaderProgram);
	checkForLinkErrors(shaderProgram);
	glBindAttribLocation(shaderProgram,	0,	"vertexPosition");

	//now	we	can	delete	the	VS	&	FS	Programs
	glDeleteShader(vertexShaderProgram);
	glDeleteShader(fragmentShaderProgram);
}

//Main method - entry point
int main(int argc, char * arg[])
{
	//Initialise window
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, full);

	initOpenGL(); //Call initOpenGL function
	initGeometry(); //Call initGeometry function
	setViewPort(WINDOW_WIDTH, WINDOW_HEIGHT); //set viewport

	//Where my event at
	SDL_Event event;

	createShader();
	//Game loop/Black magic
	while (running)
	{
		//Moar magics / event handler / Poll takes event off event queue
		while(SDL_PollEvent(&event))
		{
			//Check event type
			if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
			{
				//Kill the game loop
				running = false;
			}
		}


		update(); //Check if anything's changed
		render(); //Draw the scene/flip the buffers etc.
	}


	//Initiate SDL
	if(SDL_Init (SDL_INIT_EVERYTHING) != 0)
	{
		std::cout <<"ERROR SDL_Init" <<SDL_GetError() << std::endl;

		//If it fucks up ...
		return -1;
	}

	CleanUp();
	return 0;
}

