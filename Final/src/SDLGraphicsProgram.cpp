#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "SDLGraphicsProgram.h"


// State our scene objects
Object* candleObj;
SceneNode* Candle;
ParticleGenerator* Fire;
ParticleGenerator* Snow;
ParticleGenerator* Ice;
SceneNode* Root;

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h){
    std::cout << "(SDLGraphicsProgram.cpp) Constructor Called\n";
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
    gWindow = SDL_CreateWindow( "Snowy Particles",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                w,
                                h,
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
            errorStream << "Failed to iniitalize GLAD and OpenGL\n";
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


    // Setup our Renderer
    renderer = new Renderer(w,h);

    // Initialize our scene objects
    candleObj = new Object();
    candleObj->LoadObject("objects/Candle/CandleStick.obj");
    Candle = new SceneNode(candleObj, renderer->blender);
    // Set up all of our textures. 
    std::vector<std::string> fireTextures; 
    std::vector<std::string> snowTextures; 
    for(int i = 1; i < 5; ++i) 
    {
        fireTextures.push_back("./particles/fire" + std::to_string(i) + ".png");
        snowTextures.push_back("./particles/snow" + std::to_string(i) + ".png");
    }
    // Set up our particle generators
    Fire = new ParticleGenerator(renderer->blender, fireTextures, 120, glm::vec3(0.01f), 
        glm::vec3(-0.05f, 0.075f, -0.01f), glm::vec3(0.05f, 0.25f, 0.01f), 10, 50, true);
    Snow = new ParticleGenerator(renderer->blender, snowTextures, 200, glm::vec3(38.f, 0.f, 15.f), 
        glm::vec3(-0.01f, -0.1f, -0.01f), glm::vec3(0.01f, -0.2f, 0.01f), 100, 500, true);
    Ice = new ParticleGenerator(renderer->blender, snowTextures, 2000, glm::vec3(33.f, 19.f, 0.1f), 
        glm::vec3(0.f), glm::vec3(0.f), 100, 700, true, true, &(Fire->getWorldTransform()));
    Root = new SceneNode(nullptr, nullptr);
    // Render our scene starting from the root.
    renderer->setRoot(Root);
    Root->AddChild(Candle);
    Candle->AddChild(Fire);
    Root->AddChild(Snow);
    Root->AddChild(Ice);
}


// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram(){
    if(renderer!=nullptr){
        delete renderer;
    }
    
    //Destroy window
    SDL_DestroyWindow( gWindow );
    // Point gWindow to NULL to ensure it points to nothing.
    gWindow = NULL;
    //Quit SDL subsystems
    SDL_Quit();
}


//Loops forever!
void SDLGraphicsProgram::loop(){
    static float candleX = 0.0f;
    static float candleY = 0.0f;

    // Main loop flag
    // If this is quit = 'true' then the program terminates.
    bool quit = false;
    // Event handler that handles various events in SDL
    // that are related to input and output
    SDL_Event e;
    // Enable text input
    SDL_StartTextInput();

    // Set a default speed for the camera
    float cameraSpeed = 0.05f;

    // While application is running
    while(!quit){

        //Handle events on queue
        while(SDL_PollEvent( &e ) != 0){
            switch(e.type){
                case SDL_QUIT: {
                    quit = true;
                    break;
                }
                case SDL_MOUSEMOTION: {
                    // Handle mouse movements
                    int mouseX = e.motion.x;
                    int mouseY = e.motion.y;
                    int width;
                    int height;
                    SDL_GetWindowSize(gWindow, &width, &height);
                    float scalar = 0.0045f;
                    candleX = ((mouseX / (float)width) - .5f) * scalar * (float)width;
                    candleY = ((mouseY / (float)height) - .5f) * -scalar * (float)height;
                    break;
                }
                // Handle keyboard presses
                case SDL_KEYDOWN: {
                    switch(e.key.keysym.sym){
                        case SDLK_w: {
                            renderer->wireFrame = !renderer->wireFrame;
                            break;
                        }
                        case SDLK_q: {
                            quit = true;
                            break;
                        }
                        case SDLK_p: {
                            Fire->TogglePaused();
                            Snow->TogglePaused();
                            Ice->TogglePaused();
                            break;
                        }
                        default: {
                            break;
                        }
                    }
                }
                default: {
                    break;
                }
            }
        } // End SDL_PollEvent loop.   

        // Set object transforms
        Candle->getLocalTransform().loadIdentity();
        Candle->getLocalTransform().translate(0.1f + candleX, -0.5f + candleY, -3.0f);
        Candle->getLocalTransform().scale(0.1f,0.1f,0.1f);
        Candle->getLocalTransform().rotate(-1.5f,1.0f,0.0f,0.0f);
        Candle->getLocalTransform().rotate(-3.f,0.0f,0.0f,1.0f);

        Fire->getLocalTransform().loadIdentity();
        Fire->getLocalTransform().rotate(3.f,0.0f,0.0f,1.0f);
        Fire->getLocalTransform().rotate(1.5f,1.0f,0.0f,0.0f);
        Fire->getLocalTransform().translate(-1.1f, 8.2f, 1.5f);
        Fire->getLocalTransform().scale(0.5f,0.5f,0.5f);

        Snow->getLocalTransform().loadIdentity();
        Snow->getLocalTransform().translate(0.f, 8.f, -10.f);
        Snow->getLocalTransform().scale(0.2f,0.2f,0.2f);

        Ice->getLocalTransform().loadIdentity();
        Ice->getLocalTransform().translate(0.f, 0.f, -2.65f);
        Ice->getLocalTransform().scale(0.08f,0.08f,0.08f);

        // Update our scene through our renderer
        renderer->Update();
        // Render our scene using our selected renderer
        renderer->Render();
        // Delay to slow things down just a bit!
        SDL_Delay(25); 
        //Update screen of our specified window
        SDL_GL_SwapWindow(getSDLWindow());
    }

    //Disable text input
    SDL_StopTextInput();
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
