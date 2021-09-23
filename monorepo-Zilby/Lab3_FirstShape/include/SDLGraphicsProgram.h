#ifndef SDLGRAPHICSPROGRAM
#define SDLGRAPHICSPROGRAM

// ==================== Libraries ==================
// Depending on the operating system we use
// The paths to SDL are actually different.
// The #define statement should be passed in
// when compiling using the -D argument.
// This gives an example of how a programmer
// may support multiple platforms with different
// dependencies.
#ifdef LINUX
    #include <SDL2/SDL.h>
#else // This works for Mac and Linux
    #include <SDL.h>
#endif

// The glad library helps setup OpenGL extensions.
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <sstream>


// This class sets up a full graphics program
class SDLGraphicsProgram{
public:

    // Constructor
    SDLGraphicsProgram(int w, int h);
    // Desctructor
    ~SDLGraphicsProgram();
    // Setup OpenGL
    bool initGL();
    // Per frame update
    void update();
    // Renders shapes to the screen
    void render();
    // loop that runs forever
    void loop();
    // Shader helpers
    unsigned int CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
    unsigned int CompileShader(unsigned int type, const std::string& source);

    // Get Pointer to Window
    SDL_Window* getSDLWindow();
    // Shader loading utility programs
    void printProgramLog( GLuint program );
    void printShaderLog( GLuint shader );
    // Helper Function to Query OpenGL information
    void getOpenGLVersionInfo();

private:
    // Screen dimension constants
    int screenHeight;
    int screenWidth;

    // The window we'll be rendering to
    SDL_Window* gWindow ;
    // OpenGL context
    SDL_GLContext gContext;
    // Render flag
    // By default, render all of the shapes as soon as possible.
    bool gRenderShapes;
    // Graphics program
    GLuint gProgramID;
    GLint gVertexPos2DLocation;
    GLuint gVBO;
    GLuint gIBO;
    // shader
    // For now, we can have one shader.
    unsigned int shader;
    // Vertex Array
    // Currently one allowed in this lab.
    GLuint VAOId;
    // Vertex Buffer
    // For now, one vertex buffer object
    GLuint myBuffer;
};

#endif
