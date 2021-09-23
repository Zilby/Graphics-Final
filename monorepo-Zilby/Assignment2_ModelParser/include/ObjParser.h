#ifndef OBJPARSER_H
#define OBJPARSER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>

/*
	Represents a Parsed OBJ file. Saves vertex, normal, and face info.
*/
class ObjParser {
public:
	// Generic Vector3 object
	template<class T>
	struct Vec3 {
		T x, y, z;

		T& operator[] (int i) {
			return ((&x)[i]);
		}

		const T& operator[] (int i) const {
			return ((&x)[i]);
		}
	};

	// Represents a Face of an object
	struct Face {
		ObjParser::Vec3<unsigned int> verts, norms;
	};

	// Constructor
    ObjParser(std::string fileName);
    // Desctructor
    ~ObjParser();

    // Parse a line of a obj file
    void parseLine(std::string line);

    // Get the verticies of this obj model
    std::vector<ObjParser::Vec3<float>> getVerts();
    // Get the normals of this obj model
	std::vector<ObjParser::Vec3<float>> getNormals();
	// Get the face of this obj model
	std::vector<ObjParser::Face> getFaces();


    // Get an array of the verticies of this obj model
	float* getVertsArray();
    // Get an array of the normals of this obj model
	float* getNormalsArray();
	// Get an array of the face of this obj model
	unsigned int* getFacesArray();

	// Print debugging goes here!
	void debug();

private:
	// The verticies of this obj model
	std::vector<ObjParser::Vec3<float>> verts;
	// The normals of this obj model
	std::vector<ObjParser::Vec3<float>> normals;
	// The faces of this obj model
	std::vector<ObjParser::Face> faces;
};

#endif