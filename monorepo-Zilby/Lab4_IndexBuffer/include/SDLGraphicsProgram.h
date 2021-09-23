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
#include <fstream>

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
    // Test link status
    bool CheckLinkStatus(GLuint programID);

    // Loads a file and returns it as a string
    std::string LoadShader(const std::string& fname);
    // Generate any vertex buffers
    void GenerateBuffers();

    // Get Pointer to Window
    SDL_Window* getSDLWindow();
    // Shader loading utility programs
    void printProgramLog( GLuint program );
    void printShaderLog( GLuint shader );
    // Helper Function to Query OpenGL information.
    void getOpenGLVersionInfo();

private:
    // Screen dimension constants
    int screenHeight;
    int screenWidth;

    // The window we'll be rendering to
    SDL_Window* gWindow ;
    // OpenGL context
    SDL_GLContext gContext;
    // For now, we can have one shader.
    unsigned int shader;
    // Vertex Array Object
    GLuint VAOId;
    // Vertex Buffer
    GLuint vertexPositionBuffer;
    // Index Buffer Object
    GLuint indexBufferObject;
    // Number of indices used for shape. 
    int indiceNumber;

};

#endif