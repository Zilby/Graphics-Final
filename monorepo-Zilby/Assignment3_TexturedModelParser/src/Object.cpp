#include "Object.h"
#include "Camera.h"

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


Object::Object(std::string objPath){
    init(objPath); 
}

Object::~Object(){
    
}

// TODO: In the future it may be good to 
// think about loading a 'default' texture
// if the user forgets to do this action!
void Object::LoadTexture(std::string fileName){
        // Load our actual textures
        diffuseMap.LoadTexture(fileName);
}


// Initialization of object
// This could be called in the constructor, or
// it is more typicaly to 'explicitly' call this
// so we create our objects at the correct time
void Object::init(std::string objPath){
        objParser = new ObjParser(objPath); 

        std::vector<ObjParser::Vec3<unsigned int>> seenIndicies; 

        // Iterate through each face
        for (unsigned int i = 0; i < objParser->getFaces().size(); ++i) {
            ObjParser::Face face = objParser->getFaces()[i];
            ObjParser::Vec3<unsigned int> vertexIndices;
            // Iterate through each vertex of the face
            for (unsigned int k = 0; k < 3; ++k) {
                ObjParser::Vec3<unsigned int> faceValues;
                faceValues.x = face.verts[k];
                faceValues.y = face.vertTextures[k];
                faceValues.z = face.norms[k];
            
                int foundDup = -1;
                // Find duplicate vertices and reuse them
                for (unsigned int j = 0; j < seenIndicies.size(); ++j) {
                    if (faceValues == seenIndicies[j]) { 
                        foundDup = j;
                        break;
                    }
                }

                // If no duplicates found, add the vertex and its texture to the array
                if (foundDup < 0) {
                    ObjParser::Vec3<float> vert = objParser->getVerts()[faceValues.x];
                    geometry.addVertex(vert.x, vert.y, vert.z);

                    ObjParser::Vec2<float> vertTex = objParser->getVertTextures()[faceValues.y];
                    geometry.addTexture(vertTex.x, vertTex.y);

                    vertexIndices[k] = seenIndicies.size();

                    seenIndicies.push_back(faceValues);
                } 
                else 
                {
                    vertexIndices[k] = foundDup;
                }
            }
            // Make the triangle
            geometry.makeTriangle(vertexIndices.x, vertexIndices.y, vertexIndices.z);

        }

		geometry.gen();

        // Create a buffer and set the stride of information
        myBuffer.CreateBufferNormalMapLayout(14,
											geometry.getSize(),
											geometry.getIndicesSize(),
											geometry.getData(),
											geometry.getIndicesData());

        
        // Setup shaders
        std::string vertexShader = myShader.LoadShader("./shaders/vert.glsl");
        std::string fragmentShader = myShader.LoadShader("./shaders/frag.glsl");
        // Actually create our shader
        myShader.CreateShader(vertexShader,fragmentShader);

        //TODO get path from objPath, add file name
        LoadTexture(objParser->getDiffuseMap());

        
}

// Bind everything we need in our object
// Generally this is called in update() and render()
// before we do any actual work with our object
void Object::Bind(){
        // Make sure we are updating the correct 'buffers'
        myBuffer.Bind();
		// Diffuse map is 0 by default, but it is good to set it explicitly
        diffuseMap.Bind(0);
  		// Select our appropriate shader
	    myShader.Bind();
}

void Object::update(unsigned int screenWidth, unsigned int screenHeight){
		Bind();
		// TODO: Read and understand
        // For our object, we apply the texture in the following way
        // Note that we set the value to 0, because we have bound
        // our texture to slot 0.
        myShader.setUniform1i("u_DiffuseMap",0);  
         // Here we apply the 'view' matrix which creates perspective.
        // The first argument is 'field of view'
        // Then perspective
        // Then the near and far clipping plane.
        // Note I cannot see anything closer than 0.1f units from the screen.
        // TODO: In the future this type of operation would be abstracted away
        //       in a camera class.
        projectionMatrix = glm::perspective(45.0f,((float)screenWidth)/((float)screenHeight),0.1f,20.0f);
        // Set the uniforms in our current shader

        // Set the MVP Matrix for our object
        // Send it into our shader
        myShader.setUniformMatrix4fv("model", &transform.getInternalMatrix()[0][0]);
        myShader.setUniformMatrix4fv("view",
&Camera::instance().getWorldToViewmatrix()[0][0]);
        myShader.setUniformMatrix4fv("projection", &projectionMatrix[0][0]);

        // Create a first 'light'
        myShader.setUniform3f("pointLights[0].lightColor",1.0f,1.0f,1.0f);
        myShader.setUniform3f("pointLights[0].lightPos",
                               Camera::instance().getEyeXPosition() + Camera::instance().getViewXDirection(),
                               Camera::instance().getEyeYPosition() + Camera::instance().getViewYDirection(),
                               Camera::instance().getEyeZPosition() + Camera::instance().getViewZDirection());
        myShader.setUniform1f("pointLights[0].ambientIntensity",0.5f);
        myShader.setUniform1f("pointLights[0].specularStrength",0.5f);
        myShader.setUniform1f("pointLights[0].constant",1.0f);
        myShader.setUniform1f("pointLights[0].linear",0.09f);
        myShader.setUniform1f("pointLights[0].quadratic",0.032f);

}

void Object::render(){
	    Bind();
		//Render data
    	glDrawElements(GL_TRIANGLES,
					geometry.getIndicesSize(),               // The number of indicies, not triangles.
					GL_UNSIGNED_INT, // Make sure the data type matches
					nullptr);       // Offset pointer to the data. nullptr because we are currently bound:	
}

// Returns the actual transform stored in our object
// which can then be modified
Transform& Object::getTransform(){
    return transform; 
}
