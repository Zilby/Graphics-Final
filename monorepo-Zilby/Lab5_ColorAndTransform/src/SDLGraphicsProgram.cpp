#include "SDLGraphicsProgram.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

// This assert inserts a breakpoint in your code!
// That way we can get the line number and file.
// #define ASSERT(x) if(!(x)) __debugbreak(); (__debugbreak() is only available
// on MSVC)
#define GLCall(x) GLClearErrorStates(); x ; GLCheckError(#x,__LINE__);

// New error handling routine
static void GLClearErrorStates(){
    // Return all of the errors that occur.
    while(glGetError() != GL_NO_ERROR){
        ;
    }
}

// Returns false if an error occurs
static bool GLCheckError(const char* function, int line){
    while(GLenum error = glGetError()){
        // __LINE__ is a special preprocessor macro that
        // Can tell us what line any errors occurred on.
        std::cout << "[OpenGL Error]" << error << "|" << function << " Line: " << line << "\n";
        return false;
    }
    return true;
}



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
    errorStream << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
    std::string errors=errorStream.str();
    SDL_Log("%s\n",errors.c_str());
  }else{
    SDL_Log("SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, errors detected during initialization\n\n");
  }

	// SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra debug support!
	getOpenGLVersionInfo();

}

	// Proper shutdown and destroy initialized objects
	SDLGraphicsProgram::~SDLGraphicsProgram(){
	//Destroy window
	SDL_DestroyWindow( gWindow );
	// Point gWindow to NULL to ensure it points to nothing.
	gWindow = NULL;
	// Deallocate Program
	glDeleteProgram(shader);
	//Quit SDL subsystems
	SDL_Quit();
}




void SDLGraphicsProgram::GenerateBuffers(){

	// TODO: Add color data to the 'verts'
    //       One vertex should be red, one green, one blue.
    GLfloat verts[] = {
	0.0f,0.0f,0.0f, 1.0f, 0.0f, 0.0f,
	
	1.0f,1.0f,0.0f, 0.0f, 1.0f, 0.0f,

	-1.0f,1.0f,0.0f, 0.0f, 0.0f, 1.0f,
	
	-1.0f,-1.0f,0.0f, 1.0f, 0.0f, 0.0f,
    };

    


	// Add data to the indices as you see fit
	GLuint indices[] = {0,1,2, 0,3,4 };

	// VertexArrays
	glGenVertexArrays(1, &VAOId);
	glBindVertexArray(VAOId);

	// Vertex Buffer Object (VBO)
	// Create a buffer (note we’ll see this pattern of code often in OpenGL)
	// TODO: Read this and understand what is going on
	glGenBuffers(1, &vertexPositionBuffer); // selecting the buffer is
											// done by binding in OpenGL
											// We tell OpenGL then how we want to use our selected(or binded) buffer with the arguments passed into the function.
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
  	// Finally pass in our vertex data
	// TODO: Do you think this needs to be modified?
    glVertexAttribPointer(  0,   // Attribute 0, which will match layout in shader
							3,   // size (Number of components (2=x,y)  (3=x,y,z), etc.)
							GL_FLOAT, // Type of data
							GL_FALSE, // Is the data normalized
							sizeof(float)*6, // Stride - Amount of bytes between each vertex.
											// If we only have vertex data, then this is 0.
											// // That means our vertices(or whatever data) is tightly packed, one after the other.
											// // If we add in vertex color information(3 more floats), then this becomes 6, as we move 6*sizeof(float) to get to the next chunk of data.
											// If we have normals, then we need to jump 3*sizeof(GL_FLOAT) bytes to get
											// to our next vertex.
							0       // Pointer to the starting point of our data. If we are just grabbing vertices, this is 0. But if we have some other attribute, (stored in the same data structure), this may vary if the very first element is some different attribute.
			    					// If we had some data after (say normals), then we would have an offset of 3*sizeof(GL_FLOAT) for example
	);

    // TODO: Add a new glVertexAttrib here. Make sure to enable it first
    // - Make sure you use a new attribute(i.e. 0 is already used for position data!)
    // - Make sure the correct offset is set (i.e. the starting point of the color data)
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(  1,   // Attribute 0, which will match layout in shader
                          3,   // size (Number of components (2=x,y)  (3=x,y,z), etc.)
                          GL_FLOAT, // Type of data
                          GL_FALSE, // Is the data normalized
                      sizeof(float)*6, // Stride - Amount of bytes between each vertex.
                        (void*)(3*sizeof(GL_FLOAT))      
  );

	// Another Vertex Buffer Object (VBO)
	// This time for your index buffer.
	glGenBuffers(1, &indexBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::initGL(){
	//Success flag
	bool success = true;

	// Setup geometry
	GenerateBuffers();

  	// Setup shaders
	std::string vertexShader = LoadShader("./shaders/vert.glsl");
	std::string fragmentShader = LoadShader("./shaders/frag.glsl");

  	shader = CreateShader(vertexShader,fragmentShader);

	// Use our shader
	glUseProgram(shader);


    // Setup our OpenGL State machine
    // TODO: Read this
    // The below command is new!
    // What we are doing, is telling opengl to create a depth(or Z-buffer) 
    // for us that is stored every frame.
    glEnable(GL_DEPTH_TEST);


	return success;
}


// Update OpenGL
void SDLGraphicsProgram::update()
{
	// Here we apply our camera
    // This first transformation is applied to the object
    // This is the model transform matrix.
    // That is, 'how do I move our model'
    // Here we see I have translated the model -1.0f away from its original location.
    glm::mat4 modelTransformMatrix = glm::translate(glm::mat4(),glm::vec3(0.0f,0.0f,-1.0f));
    // Next we apply the 'view' matrix which creates perspective.
    // The first argument is 'field of view'
    // Then perspective
    // Then the near and far clipping plane.
    // Note I cannot see anything closer than 0.1f units from the screen.
    glm::mat4 projectionMatrix = glm::perspective(60.0f,(float)screenWidth/(float)screenHeight,0.1f,10.0f);

    // Note that we are now 'looking' inside the shader for a particular
    // variable. This means the name has to exactly match!
    GLint modelTransformMatrixUniformLocation =  glGetUniformLocation(shaderID,"modelTransformMatrix");
    GLint projectionMatrixUniformLocation = glGetUniformLocation(shaderID,"projectionMatrix");

    // Now update this information through our uniforms.
    // glUniformMatrix4v means a 4x4 matrix of floats
    glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE,&modelTransformMatrix[0][0]);
    glUniformMatrix4fv(projectionMatrixUniformLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
    
    // TODO: Modify the vertex shader so you can see perspective!
    // (See shaders/vert.glsl)
}



// Render
// The render function gets called once per loop
void SDLGraphicsProgram::render(){
    // Initialize clear color
    // This is the background of the screen.
    glViewport(0, 0, screenWidth, screenHeight);
    glClearColor( 0.2f, 0.2f, 0.2f, 1.f );

    // TODO: Read this
    // Clear color buffer and Depth Buffer
    // Remember that the 'depth buffer' is our
    // z-buffer that figures out how far away items are every frame
    // and we have to do this every frame!
  	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	//Render data
	GLCall(glDrawElements(GL_TRIANGLES,
					6, // The number of indicies, not triangles.
					GL_UNSIGNED_INT, // Make sure the data type matches
					nullptr);)        // Offset pointer to the data. nullptr because we are currently bound
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
      	} // End SDL_PollEvent loop.

		// Update our scene
		update();
		// Render using OpenGL
	    render(); 	// TODO: potentially move this depending on your logic
					// for how you handle drawing a triangle or rectangle.
      	//Update screen of our specified window
      	SDL_GL_SwapWindow(getSDLWindow());
    }

    //Disable text input
    SDL_StopTextInput();
}


// Loads a shader and returns a string
std::string SDLGraphicsProgram::LoadShader(const std::string& fname){
		std::string result;
		// 1.) Get every line of data
		std::string line;
		std::ifstream myFile(fname.c_str());

		if(myFile.is_open()){
			while(getline(myFile,line)){
					result += line + '\n';
					// SDL_Log(line); 	// Uncomment this if you want to see
										// the shader code get printed out.
			}
		}
		else{
			SDL_Log("SDLGraphicsProgram::LoadShader - file not found. Try an absolute file path to see if the file exists.\n");
		}
		// Close file
		myFile.close();
		return result;
}


unsigned int SDLGraphicsProgram::CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource){

    // Create a new program
    unsigned int program = glCreateProgram();
    // Compile our shaders
    unsigned int myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    // Link our program
    // These have been compiled already.
    glAttachShader(program,myVertexShader);
    glAttachShader(program,myFragmentShader);
    // Link our programs that have been 'attached'
    glLinkProgram(program);
    glValidateProgram(program);

    // Once the shaders have been linked in, we can delete them.
    glDetachShader(program,myVertexShader);
    glDetachShader(program,myFragmentShader);

    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    if(!CheckLinkStatus(program)){
        SDL_Log("SDLGraphicsProgram::CreateShader - ERROR, shader did not link! Were there compile errors in the shader?\n");
    }

    shaderID = program;
    return program;
}


unsigned int SDLGraphicsProgram::CompileShader(unsigned int type, const std::string& source){
  // Compile our shaders
  // id is the type of shader (Vertex, fragment, etc.)
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
  // This code is returning any compilation errors that may have occurred!
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE){
      int length;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
      char* errorMessages = new char[length]; // Could also use alloca here.
      glGetShaderInfoLog(id, length, &length, errorMessages);
      if(type == GL_VERTEX_SHADER){
		SDL_Log("SDLGraphicsProgram::CompileShader ERROR: GL_VERTEX_SHADER compilation failed!\n");
		SDL_Log("%s\n",errorMessages);
      }else if(type == GL_FRAGMENT_SHADER){
        SDL_Log("SDLGraphicsProgram::CompileShader ERROR: GL_FRAGMENT_SHADER compilation failed!\n");
		SDL_Log("%s\n",errorMessages);
      }
      // Reclaim our memory
      delete[] errorMessages;
      // Delete our broken shader
      glDeleteShader(id);
      return 0;
  }

  return id;
}

// Check to see if linking was successful
bool SDLGraphicsProgram::CheckLinkStatus(GLuint programID){

  #if defined(MINGW)
	// do nothing
	SDL_Log("WARNING: CheckLinkStatus is not implemented for windows folks"); 
  #else
	// Link our shaders
	// id is the type of shader (Vertex, fragment, etc.)
	unsigned int id;

    // Retrieve the result of our compilation
    int result;
    // This code is returning any Linker errors that may have occurred!
    glGetProgramiv(id, GL_LINK_STATUS, &result);
    if(result == GL_FALSE){
      int length;
      glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);
      char* errorMessages = new char[length]; // Could also use alloca here.
      glGetProgramInfoLog(id, length, &length, errorMessages);
      // Reclaim our memory
      SDL_Log("ERROR in linking process\n");
	  SDL_Log("%s\n",errorMessages);
      delete[] errorMessages;
      return false;
    }
	#endif

    return true;
}


// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::getSDLWindow(){
  return gWindow;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::getOpenGLVersionInfo(){
	SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

}
