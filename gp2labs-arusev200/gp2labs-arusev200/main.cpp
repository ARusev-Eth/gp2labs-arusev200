//Headers
#include <iostream>
#include <GL/glew.h> //include glew
#include <SDL.h> //SDL functionality
#include <SDL_opengl.h> //OpenGL stuff
#include <gl\GLU.h> //GLU

//Variables:

//Pointer to our window
SDL_Window * window;

//Window creation properties
const int WINDOW_HEIGHT = 480;
const int WINDOW_WIDTH = 640;

bool full = false; //Fullscreen?
bool running = true; //Game loops/black magics

//Triangle coordinates
float triangleData[] = {0.0f, 1.0f, 0.0f, //Top
						-1.0f, -1.0f, 0.0f, //Bottom Left
						1.0f, -1.0f, 0.0f}; //Bottom Right

GLuint triangleVBO; //To be filled when generating buffer/integer ID reffered to by the VBOs

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
	glDeleteBuffers(1,&triangleVBO); //Reclame memory alocated when #cleaningup
	SDL_GL_DeleteContext(glContext); //Delete OGL context before you exit
	SDL_DestroyWindow(window);
	SDL_Quit;
}

//Initialise OpenGL
void initOpenGL()
{
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

	//Change to project matrix mode
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity;

	gluPerspective( 45.0f, ratio, 0.1f, 100.0f); //calc perspective matrix, using glu lib functions
	
	glMatrixMode (GL_MODELVIEW); //Switch to modelview
	glLoadIdentity(); //Reset using Identity Matrix
}

//Let there be images
void render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //Background to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear the colour and depth buffers

	//To be removed soon...
	
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
	//To be removed soon...\\

	SDL_GL_SwapWindow(window); //Swap buffers
}

//Let it move
void update()
{

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

