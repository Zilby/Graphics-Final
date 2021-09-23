#include "SDLGraphicsProgram.h"

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h):screenWidth(w),screenHeight(h){
  // Initialization flag
	bool success = true;
  // String to hold any errors that occur.
  std::stringstream errorStream;
  // The window we'll be rendering to
  gWindow = NULL;
  // Render flag
  // By default, render all of the shapes as soon as possible.
  gRenderShapes = true;

  // Graphics program
  gProgramID = 0;
  gVertexPos2DLocation = -1;
  gVBO = 0;
  gIBO = 0;

	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO)< 0){
		errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";
		success = false;
	}
	else{
		//Use OpenGL 3.3 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    // We want to request a double buffer for smooth updating.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		//Create window
		gWindow = SDL_CreateWindow( "Lab",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                screenWidth,
                                screenHeight,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );

    // Check if Window did not create.
    if( gWindow == NULL ){
			errorStream << "Window could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

		//Create an OpenGL Graphics Context
		gContext = SDL_GL_CreateContext( gWindow );
		if( gContext == NULL){
			errorStream << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
			success = false;
		}

    // Initialize GLAD Library
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)){
        errorStream << "Failed to iniitalize GLAD\n";
        success = false;
    }

		//Initialize OpenGL
		if(!initGL()){
			errorStream << "Unable to initialize OpenGL!\n";
			success = false;
		}

  }

  // If initialization did not work, then print out a list of errors in the constructor.
  if(!success){
    errorStream << "Failed to initialize!\n";
    std::string errors=errorStream.str();
    std::cout << errors << "\n";
  }else{
    std::cout << "No SDL, GLAD, or OpenGL, errors Detected\n\n";
  }

}

// Proper shutdown and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
	//Deallocate program
	glDeleteProgram( gProgramID );
	//Destroy window
	SDL_DestroyWindow( gWindow );
    // Point gWindow to NULL to ensure it points to nothing.
	gWindow = NULL;
    // Delete our buffers and shader program
    glDeleteBuffers(1, &myBuffer);
    glDeleteVertexArrays(1, &VAOId);
    glDeleteProgram(shader);

	//Quit SDL subsystems
	SDL_Quit();
}


// Initialize OpenGL
// Setup any of our shaders here.
// Note that a shader is simply a 'source program'
// that gets compiled, and then executed on the GPU.
bool SDLGraphicsProgram::initGL(){
  	//Success flag
  	bool success = true;

    // Setup shaders
    std::string vertexShader =
      "#version 330 core\n"
      "in vec4 position;\n"
      "void main()\n"
      "{\n"
      "  gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
      "}\n";


    std::string fragmentShader =
        "#version 330 core\n"
        "out vec4 color;\n"
        "void main()\n"
        "{\n"
        "  color = vec4(0.4f, 0.1f, 0.9f, 1.0f);\n"
        "}\n";

      shader = CreateShader(vertexShader,fragmentShader);

      // VertexArrays
      glGenVertexArrays(1, &VAOId);
      glBindVertexArray(VAOId);

      // VBO data
      // This currently contains only vertexPositons for the data.
      static const GLfloat vertexPositions[] =
      {
       -0.3f, -0.7f, 0.0f, // Left vertex position
        0.5f, -0.5f, 0.0f,  // right vertex position
        0.8f,  0.6f, 0.0f,  // Top vertex position
      };

      // Vertex Buffer Object (VBO)
      // Create a buffer (note we’ll see this pattern of code often in OpenGL)
      glGenBuffers(1, &myBuffer);
			// selecting the buffer is done by binding in OpenGL
			// We tell OpenGL then how we want to use our selected(or binded) buffer with the arguments passed into the function.
      glBindBuffer(GL_ARRAY_BUFFER, myBuffer);
      glBufferData(GL_ARRAY_BUFFER, 9* sizeof(GL_FLOAT), vertexPositions, GL_STATIC_DRAW);

	return success;
}


// Update OpenGL
void SDLGraphicsProgram::update()
{
	// TBD
}


// Render
// The render function gets called once per loop
void SDLGraphicsProgram::render(){
    // Disable depth test and face culling.
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, screenWidth, screenHeight);
    glClearColor( 0.9f, 0.1f, 0.3f, 1.f );

    //Clear color buffer and Depth Buffer
  	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // Use our shader
	glUseProgram(shader);

    // Enable our attributes
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, myBuffer);
    // Finally pass in our vertex data
    glVertexAttribPointer(0,  // Attribute 0, which will match layout in shader
                          3,  			// size
                          GL_FLOAT, // Type
                          GL_FALSE, // Is the data normalized
                          0, 				// Stride
                          (void*)0
    );

    //Render data
    glDrawArrays(GL_TRIANGLES,0,3);

    // Remove our program
	glDisableVertexAttribArray(0);
    glUseProgram(NULL);
}


//Loops forever!
void SDLGraphicsProgram::loop(){
    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();
    // While application is running
    while(!quit){
      //Handle events on queue
      while(SDL_PollEvent( &e ) != 0){
        // User posts an event to quit
        // An example is hitting the "x" in the corner of the window.
        if(e.type == SDL_QUIT){
          quit = true;
        }
      }

      // Render using OpenGL
      render();
      //Update screen of our specified window
      SDL_GL_SwapWindow(getSDLWindow());
    }

    //Disable text input
    SDL_StopTextInput();
}

unsigned int SDLGraphicsProgram::CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource){

    // Create a new program
    unsigned int program = glCreateProgram();
    // Compile our shaders
    unsigned int myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    // Link our program
    glAttachShader(program,myVertexShader);
    glAttachShader(program,myFragmentShader);
    // Link our programs together
    glLinkProgram(program);
    glValidateProgram(program);

    // Once the shaders have been linked in, we can delete them.
    glDetachShader(program,myVertexShader);
    glDetachShader(program,myFragmentShader);

    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    return program;
}


unsigned int SDLGraphicsProgram::CompileShader(unsigned int type, const std::string& source){
  // Compile our shaders
  unsigned int id;

  if(type == GL_VERTEX_SHADER){
    id = glCreateShader(GL_VERTEX_SHADER);
  }else if(type == GL_FRAGMENT_SHADER){
    id = glCreateShader(GL_FRAGMENT_SHADER);
  }
  const char* src = source.c_str();
  // The source of our shader
  glShaderSource(id, 1, &src, nullptr);
  // Now compile our shader
  glCompileShader(id);

  // Retrieve the result of our compilation
  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE){
      int length;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
      char* errorMessages = new char[length]; // Could also use alloca here.
      glGetShaderInfoLog(id, length, &length, errorMessages);
      if(type == GL_VERTEX_SHADER){
        std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n" << errorMessages << "\n";
      }else if(type == GL_FRAGMENT_SHADER){
        std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n" << errorMessages << "\n";
      }
      // Reclaim our memory
      delete[] errorMessages;
      // Delete our broken shader
      glDeleteShader(id);
      return 0;
  }

  return id;
}



// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::getSDLWindow(){
  return gWindow;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::getOpenGLVersionInfo(){
  std::cout << "Vendor: " << glGetString(GL_VENDOR) << "\n";
  std::cout << "Renderer: " << glGetString(GL_RENDERER) << "\n";
  std::cout << "Version: " << glGetString(GL_VERSION) << "\n";
  std::cout << "Shading language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}
