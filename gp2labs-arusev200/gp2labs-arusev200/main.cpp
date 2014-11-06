//Headers
#include <iostream>
#include <GL/glew.h> //include glew
//Maths
#include <glm/glm.hpp>
using glm::mat4;
using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//Important stuff
#include <SDL.h> //SDL functionality
#include <SDL_opengl.h> //OpenGL stuff
#include <SDL_image.h> //Pretty pictures
#include <SDL_ttf.h> 
#include <gl\GLU.h> //GLU
#include <vector>

//Paths
const std::string ASSET_PATH = "../assets/";
const std::string SHADER_PATH = "shaders/";
const std::string TEXTURE_PATH = "textures/";
const std::string FONT_PATH = "fonts/";

//Headers
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"


//Variables:

SDL_Window * window = NULL; //Pointer to our window
SDL_GLContext glcontext = NULL; //SDL Context

//Window creation properties
const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;
bool running = true; //Game loops/black magics
//GLuint	shaderProgram = 0; //global int to hold shader


std::vector<GameObject*> displayList;
GameObject * mainCamera;

Vertex triangleData[]=
{
	//Front
	{ vec3(-0.5f, 0.5f, 0.5f), vec2(0.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) }, //Top Left
	{ vec3(-0.5f, -0.5f, 0.5f), vec2(0.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) }, //Bottom Left
	{ vec3(0.5f, -0.5f, 0.5f), vec2(1.0f, 1.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) }, //Bottom Right
	{ vec3(0.5f, 0.5f, 0.5f), vec2(1.0f, 0.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) }, //Top Right

	//Back
	{ vec3(-0.5f, 0.5f, -0.5f), vec2(0.0f, 0.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f) }, //Top Left
	{ vec3(-0.5f, -0.5f, -0.5f), vec2(0.0f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f) }, //Bottom Left
	{ vec3(0.5f, -0.5f, -0.5f), vec2(1.0f, 1.0f), vec4(0.0f, 0.0f, 0.0f, 1.0f) }, //Bottom Right
	{ vec3(0.5f, 0.5f, -0.5f), vec2(1.0f, 0.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f) } //Top Right
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
//Variables\\

/*
GLuint triangleVBO; //To be filled when generating buffer/integer ID reffered to by the VBOs
GLuint triangleEBO;

SDL_GLContext glContext = NULL; //SDL GL Context
*/

//Error checks
void CheckForErrors()
{
	GLenum error;
	do{
		error = glGetError();
	} while  (error != GL_NO_ERROR);
}

//Functions:

//Window
void InitWindow(int width, int height, bool fullscreen)
{

	//some full screen action
	int flags=SDL_WINDOW_OPENGL;
	if (fullscreen)
	{
		flags=SDL_WINDOW_OPENGL|SDL_WINDOW_FULLSCREEN;
	}

	//Creating them windows, setting them parameters
	window = SDL_CreateWindow(
		"Lab 6",				//Window title
		SDL_WINDOWPOS_CENTERED, //x position of the window - centered
		SDL_WINDOWPOS_CENTERED, //y position of the window - againt centered
		width,					//Window width, in pixels
		height,					//Window height, in pixels
		flags);     //Flags ?? 
}



//Clean up
void CleanUp()
{
	//Destroy game objects in the list
	auto iter = displayList.begin();
	while (iter != displayList.end())
	{
		(*iter)->destroy();
		if ((*iter))
		{
			delete(*iter);
			(*iter) = NULL;
			iter = displayList.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	displayList.clear;

	SDL_GL_DeleteContext(glcontext); //Delete OGL context before you exit
	SDL_DestroyWindow(window);
	IMG_Quit;
	TTF_Quit;
	SDL_Quit;
}

//Initialise OpenGL
void initOpenGL()
{
	//Ask for version 3.2 of OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE	);

	glcontext = SDL_GL_CreateContext(window); //Create gl context

	if(!glcontext)
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

void Initialise()
{
	//Making the camera
	mainCamera = new GameObject(); 
	mainCamera->SetName("MainCamera"); 

	//Setting up the transformation
	Transform *t = new Transform();
	t->setPosition(0.0f, 0.0f, 10.0f);
	mainCamera->SetTransform(t);

	//Setting the camera's properties up
	Camera * c = new Camera();
	c->setAspectRatio((float)(WINDOW_WIDTH / WINDOW_HEIGHT));
	c->setFOV(45.0f);
	c->setNearClip(0.1f);
	c->setFarClip(1000.0f);
	mainCamera->setCamera(c);
	displayList.push_back(mainCamera);

	//Making the cube #Boxie
	GameObject * cube = new GameObject();
	cube->setName("BOXIE");

	//Setting the transform up
	Transform *transform = new Transform();
	transform->setPosition(0.0f, 0.0f, 0.0f);
	cube->setTransform(transform);

	//Material and shaders
	Material * material = new Material();
	std::string vsPath = ASSET_PATH + SHADER_PATH + "/simpleVS.glsl";
	std::string fsPath = ASSET_PATH + SHADER_PATH + "/simpleFS.glsl";
	material->loadShader(vsPath, fsPath);
	cube->setMaterial(material);

	//All of this for a cube?
	Mesh * mesh = new Mesh;
	cube->setMesh(mesh);
	displayList.push_back(cube); //Put it on the target list

	//Brian's alternate syntax, used for updating, rendering and disposing off stuff easily; saves typing
	//All it does is fill a list and then go through the list (cube,mainCamera)
	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->init();
	}

	mesh->copyVertexData(8, sizeof(Vertex), (void**)triangleData);
	mesh->copyIndexData(36, sizeof(int), (void**)indices);
}

//Let there be images
void render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Background to black
	glClearDepth(1.0f); //Make sure depth properly set
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the colour and depth buffers

	//Render everything on the list
	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->render();

		//Get the current object's(the one being rendered during this loop) mesh, transform and material
		Mesh * currentMesh = (*iter)->getMesh();
		Transform * currentTransform = (*iter)->getTransform();
		Material * currentMaterial = (*iter)->getMaterial;

		//If the object has a mesh, a transform and a material
		if (currentMesh && currentTransform && currentMaterial)
		{
			currentMaterial->bind();
			currentMesh->bind();

			GLint MVPLocation = currentMaterial->getUniformLocation("MVP");

			Camera * camera1 = mainCamera->getCamera();
			mat4 MVP = cam->getProjection() * camera1->getView * currentTransform->getModel();
			//Should look back at this later on
			glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, glm::value_ptr(MVP));

			glDrawElements(GL_TRIANGLES, currentMesh->getIndexCount(), GL_UNSIGNED_INT, 0);
		}
	}

	SDL_GL_SwapWindow(window);
	/*
	!!!!!!RIGHTY SO I SHOULD DEFO START REMOVING THESE AT SOME POINT, need to save them somewhere before I do!!!!!
	there's a swap window somewhere in here...
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
	Commented out for keeps sake

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
	/*
	projMatrix = glm::perspective(45.0f,	(float)WINDOW_WIDTH	/ (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	viewMatrix = glm::lookAt(vec3(0.0f, 0.0f, 10.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	worldMatrix = glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f));
	*/

	//Update everything on the list (Camera, Cube)
	for (auto iter = displayList.begin(); iter != displayList.end(); iter++)
	{
		(*iter)->update();
	}
}

/* I should really start getting rid of these things
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
	std::string	vsPath	= ASSET_PATH + SHADER_PATH+"/simpleVS.glsl";	
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
*/

//Main method - entry point
int main(int argc, char * arg[])
{
	//Initialise everything, if it's not 0 - all is good
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Error SDL_Init derp" << SDL_GetError() << std::endl;
		return -1;
	}


	int imageInitFlags = IMG_INIT_JPG | IMG_INIT_PNG;
	int returnInitFlags = IMG_Init(imageInitFlags);
	//Let me know/handle if SDL_Image doesn't get initialised properly
	if (((returnInitFlags)& (imageInitFlags)) != imageInitFlags)
	{
		std::cout << "Error SDL_Image Init" << IMG_GetError() << std::endl;
	}
	//Let me know/handle if SDL_ttf doesn't get initialised properly
	if (TTF_Init() == -1)
	{
		std::cout << "TTF_Init: " << TTF_GetError();
	}


	//Initialise window
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false);

	initOpenGL(); //Call initOpenGL function
	CheckForErrors();
	setViewPort(WINDOW_WIDTH, WINDOW_HEIGHT); //set viewport

	//Init Camera and cube 
	Initialise();

	//Where my event at/Handle event generator
	SDL_Event event;

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

	CleanUp();

	return 0;
}

