#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "SDLGraphicsProgram.h"
#include "Sphere.h"

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
    gWindow = SDL_CreateWindow( "Lab",
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



// ====================== Create the planets =============
// Create 3 moons for Planet
Object* planetMoon1Sphere;
SceneNode* PlanetMoon1;
Object* planetMoon2Sphere;
SceneNode* PlanetMoon2;
Object* planetMoon3Sphere;
SceneNode* PlanetMoon3;
// Create 2 moons for Mercury
Object* mercuryMoon1Sphere;
SceneNode* MercuryMoon1;
Object* mercuryMoon2Sphere;
SceneNode* MercuryMoon2;
// Create "Planet"
Object* planetSphere;
SceneNode* Planet;
// Create Mercury
Object* mercurySphere;
SceneNode* Mercury;
// Create the Moon
Object* earthMoon1Sphere;
SceneNode* EarthMoon1;
// Create the Earth
Object* earthSphere;
SceneNode* Earth;
// Create the Sun
Object* sunSphere;
SceneNode* Sun;

// ====================== Create the planets =============

//Loops forever!
void SDLGraphicsProgram::loop(){

    // ================== Initialize the planets ===============
    static float rotate = 0.0f;

    // Create 3 Planet moons
    planetMoon1Sphere = new Sphere();
    PlanetMoon1 = new SceneNode(planetMoon1Sphere);

    planetMoon2Sphere = new Sphere();
    PlanetMoon2 = new SceneNode(planetMoon2Sphere);

    planetMoon3Sphere = new Sphere();
    PlanetMoon3 = new SceneNode(planetMoon3Sphere);

    // Create "Planet"
    planetSphere = new Sphere();
    Planet = new SceneNode(planetSphere);

    // Create 2 Mercury moons
    mercuryMoon1Sphere = new Sphere();
    MercuryMoon1 = new SceneNode(mercuryMoon1Sphere);

    mercuryMoon2Sphere = new Sphere();
    MercuryMoon2 = new SceneNode(mercuryMoon2Sphere);
    
    // Create Mercury
    mercurySphere = new Sphere();
    Mercury = new SceneNode(mercurySphere);
    
    // Create Earth's Moon
    earthMoon1Sphere = new Sphere();
    EarthMoon1 = new SceneNode(earthMoon1Sphere);
    
    // Create the Earth
    earthSphere = new Sphere();
    Earth = new SceneNode(earthSphere);
    
    // Create the Sun
    sunSphere = new Sphere();
    Sun = new SceneNode(sunSphere);

    planetSphere->LoadTexture("planet.ppm");
    mercurySphere->LoadTexture("mercury.ppm");
    earthSphere->LoadTexture("earth.ppm");
    sunSphere->LoadTexture("sun.ppm");

    earthMoon1Sphere->LoadTexture("rock.ppm");
    mercuryMoon1Sphere->LoadTexture("rock.ppm");
    mercuryMoon2Sphere->LoadTexture("rock.ppm");
    planetMoon1Sphere->LoadTexture("rock.ppm");
    planetMoon2Sphere->LoadTexture("rock.ppm");
    planetMoon3Sphere->LoadTexture("rock.ppm");


    // Render our scene starting from the sun.
    renderer->setRoot(Sun);
    // Make the Earth a child of the Sun
    Sun->AddChild(Earth);
    // Make the Moon a child of the Earth
    Earth->AddChild(EarthMoon1);
    // Make Mercury a child of the Sun
    Sun->AddChild(Mercury);
    // Make the moons of mercury children of mercury
    Mercury->AddChild(MercuryMoon1);
    Mercury->AddChild(MercuryMoon2);
    // Make the moons of planet children of planet
    Planet->AddChild(PlanetMoon1);
    Planet->AddChild(PlanetMoon2);
    Planet->AddChild(PlanetMoon3);
    // Make Planet a child of the Sun
    Sun->AddChild(Planet);

    // ================== Use the planets ===============
    
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

        // =============== Move the planets =============================
        // Set up a little scene of objects
        rotate+=0.01f;

        Sun->getLocalTransform().loadIdentity();
        Sun->getLocalTransform().translate(0.0f,0.0f,-3.0f);
        Sun->getLocalTransform().rotate(rotate,0.0f,1.0f,0.0f);
        
        Earth->getLocalTransform().loadIdentity();
        Earth->getLocalTransform().translate(2.0f,0.0f,0.0f);
        Earth->getLocalTransform().scale(0.25f,0.25f,0.25f);
        Earth->getLocalTransform().rotate(rotate,0.0f,1.0f,0.0f);

        EarthMoon1->getLocalTransform().loadIdentity();
        EarthMoon1->getLocalTransform().translate(1.75f,0.5f,0.5f);
        EarthMoon1->getLocalTransform().scale(0.5f,0.5f,0.5f);
        EarthMoon1->getLocalTransform().rotate(-20*rotate,0.0f,1.0f,0.0f);

        Mercury->getLocalTransform().loadIdentity();
        Mercury->getLocalTransform().translate(-3.0f,2.0f,0.0f);
        Mercury->getLocalTransform().scale(0.15f,0.15f,0.15f);
        Mercury->getLocalTransform().rotate(6*rotate,0.0f,1.5f,0.0f);

        MercuryMoon1->getLocalTransform().loadIdentity();
        MercuryMoon1->getLocalTransform().translate(-1.0f,1.0f,0.0f);
        MercuryMoon1->getLocalTransform().scale(0.35f,0.35f,0.35f);
        MercuryMoon1->getLocalTransform().rotate(15*rotate,0.0f,1.5f,0.0f);

        MercuryMoon2->getLocalTransform().loadIdentity();
        MercuryMoon2->getLocalTransform().translate(2.0f,0.0f,0.4f);
        MercuryMoon2->getLocalTransform().scale(0.2f,0.2f,0.2f);
        MercuryMoon2->getLocalTransform().rotate(-12*rotate,0.0f,1.5f,0.0f);

        Planet->getLocalTransform().loadIdentity();
        Planet->getLocalTransform().translate(5.0f,0.0f,0.0f);
        Planet->getLocalTransform().scale(0.8f, 0.8f, 0.8f);
        Planet->getLocalTransform().rotate(-2*rotate,0.0f,1.5f,0.0f);

        PlanetMoon1->getLocalTransform().loadIdentity();
        PlanetMoon1->getLocalTransform().translate(1.7f,0.2f,1.0f);
        PlanetMoon1->getLocalTransform().scale(0.2f, 0.2f, 0.2f);
        PlanetMoon1->getLocalTransform().rotate(-5*rotate,0.0f,1.5f,0.0f);

        PlanetMoon2->getLocalTransform().loadIdentity();
        PlanetMoon2->getLocalTransform().translate(2.0f,-0.75f,0.0f);
        PlanetMoon2->getLocalTransform().scale(0.15f, 0.15f, 0.15f);
        PlanetMoon2->getLocalTransform().rotate(-2.5*rotate,0.0f,1.5f,0.0f);

        PlanetMoon3->getLocalTransform().loadIdentity();
        PlanetMoon3->getLocalTransform().translate(-1.5f,-1.0f,0.0f);
        PlanetMoon3->getLocalTransform().scale(0.1f, 0.1f, 0.1f);
        PlanetMoon3->getLocalTransform().rotate(5*rotate,0.0f,1.5f,0.0f);
        
        // =============== Move the planets =============================

        //Handle events on queue
        while(SDL_PollEvent( &e ) != 0){
            // User posts an event to quit
            // An example is hitting the "x" in the corner of the window.
            if(e.type == SDL_QUIT){
                quit = true;
            }
            // Handle keyboad input for the camera class
            if(e.type==SDL_MOUSEMOTION){
                // Handle mouse movements
                int mouseX = e.motion.x;
                int mouseY = e.motion.y;
                renderer->camera->mouseLook(mouseX, mouseY);
            }
            switch(e.type){
                // Handle keyboard presses
                case SDL_KEYDOWN:
                    switch(e.key.keysym.sym){
                        case SDLK_LEFT:
                            renderer->camera->moveLeft(cameraSpeed);
                            break;
                        case SDLK_RIGHT:
                            renderer->camera->moveRight(cameraSpeed);
                            break;
                        case SDLK_UP:
                            renderer->camera->moveForward(cameraSpeed);
                            break;
                        case SDLK_DOWN:
                            renderer->camera->moveBackward(cameraSpeed);
                            break;
                        case SDLK_RSHIFT:
                            renderer->camera->moveUp(cameraSpeed);
                            break;
                        case SDLK_RCTRL:
                            renderer->camera->moveDown(cameraSpeed);
                            break;
                        case SDLK_q:
                            quit = true;
                            break;
                    }
                break;
            }
        } // End SDL_PollEvent loop.

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
