#include "Object.h"
#include "Camera.h"

#include <iostream>


Object::Object(){
    std::cout << "(Object.cpp) Constructor called\n";    
}

Object::~Object(){
    
}

// TODO: In the future it may be good to 
// think about loading a 'default' texture
// if the user forgets to do this action!
void Object::LoadTexture(std::string fileName){
    if(fileName != "") 
    {
        int last = fileName.find_last_of(".");
        if(fileName.substr(last) == ".png") 
        {
            diffuseMap.LoadPNGTexture(fileName);
        } 
        else 
        {
            // Load our actual textures
            diffuseMap.LoadTexture(fileName);
        }
    }
}


// Initialization of object
// This could be called in the constructor, or
// it is more typicaly to 'explicitly' call this
// so we create our objects at the correct time
void Object::LoadObject(std::string objPath){
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
}

void Object::render(){
     Bind();
    //Render data
    glDrawElements(GL_TRIANGLES,
                    geometry.getIndicesSize(),               // The number of indicies, not triangles.
                    GL_UNSIGNED_INT, // Make sure the data type matches
                    nullptr);       // Offset pointer to the data. nullptr because we are currently bound:	
}

std::vector<std::string> Object::GetShaders(){
    std::vector<std::string> shaders; 
    shaders.push_back("./shaders/vert.glsl");
    shaders.push_back("./shaders/frag.glsl");
    return shaders;
}

