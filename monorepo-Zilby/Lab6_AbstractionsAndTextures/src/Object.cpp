#include "Object.h"



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


Object::Object(){
    init(); 
}

Object::~Object(){
    
}

// Initialization of object
// This could be called in the constructor, or
// it is more typicaly to 'explicitly' call this
// so we create our objects at the correct time
void Object::init(){

        // Setup geometry
        // Be careful not to forget comma's after each line
        // (except the last line of course)!
        GLfloat verts[] = {
            -1.0f,-1.0f,0.0f,   // Position
            0.0f, 0.0f, 
            1.0f,-1.0f,0.0f,    // Position
            1.0f, 0.0f,
            1.0f,1.0f,0.0f,     // Position
            1.0f, 1.0f,
            -1.0f,1.0f,0.0f,    // Position
            0.0f, 1.0f,
        };

        // Because verts is local, it returns the proper size
        // otherwise, the behavior of 'sizeof' returns the number of
        // bytes taken up by an object.
        std::cout << "sizeof(verts):" << sizeof(verts) << "\n";

        // Add data to the indices as you see fit
        GLuint indices[] = {0,1,2, 2,3,0};

        // Create a buffer and set the stride of information
        myBuffer.CreateBufferTextureLayout(5,sizeof(verts),6,verts,indices);
        //myBuffer.Bind();

        // Load our actual texture
        myTexture.LoadTexture("cat3.ppm");
        
        // Setup shaders
        std::string vertexShader = myShader.LoadShader("./shaders/vert.glsl");
        std::string fragmentShader = myShader.LoadShader("./shaders/frag.glsl");
        // Actually create our shader
        myShader.CreateShader(vertexShader,fragmentShader);
       
}


void Object::update(unsigned int screenWidth, unsigned int screenHeight){
        myTexture.Unbind();
        // Make sure we are updating the correct 'buffers'
        myBuffer.Bind();
        myTexture.Bind();  
        myShader.Bind();
        // For our object, we apply the texture in the following way
        // Note that we set the value to 0, because we have bound
        // our texture to slot 0.
        myShader.setUniformMatrix1i("u_Texture",0);
 
         // Here we apply the 'view' matrix which creates perspective.
        // The first argument is 'field of view'
        // Then perspective
        // Then the near and far clipping plane.
        // Note I cannot see anything closer than 0.1f units from the screen.
        // TODO: In the future this type of operation would be abstracted away
        //       in a camera class.
//        modelTransformMatrix = glm::translate(glm::mat4(),glm::vec3(0.0f,0.0f,-1.0f));
        projectionMatrix = glm::perspective(45.0f,(float)screenWidth/(float)screenHeight,0.1f,10.0f);
        // Set the uniforms in our current shader
        myShader.setUniformMatrix4fv("modelTransformMatrix",
transform.getTransformMatrix());
        myShader.setUniformMatrix4fv("projectionMatrix", &projectionMatrix[0][0]);
}

void Object::render(){
        // Select this objects buffer to render
        myBuffer.Bind();
        // Select this objects texture to render
        myTexture.Bind();
        // Select this objects shader to render
        myShader.Bind();
	    //Render data
    	glDrawElements(GL_TRIANGLES,
					6,               // The number of indicies, not triangles.
					GL_UNSIGNED_INT, // Make sure the data type matches
					nullptr);       // Offset pointer to the data. nullptr because we are currently bound:

}

// Returns the actual transform stored in our object
// which can then be modified
Transform& Object::getTransform(){
    return transform; 
}
