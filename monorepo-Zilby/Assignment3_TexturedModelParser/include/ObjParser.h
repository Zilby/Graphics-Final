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

		bool operator==(const Vec3& o)
		{
			return x == o.x && y == o.y && z == o.z;
		}
	};

	template<class T>
	struct Vec2 { //TODO inheritance?
		T x, y;

		T& operator[] (int i) {
			return ((&x)[i]);
		}

		const T& operator[] (int i) const {
			return ((&x)[i]);
		}

		bool operator==(const Vec2& o)
		{
			return x == o.x && y == o.y;
		}
	};

	// Represents a Face of an object
	struct Face {
		ObjParser::Vec3<unsigned int> verts, vertTextures, norms;
	};

	// Constructor
    ObjParser(std::string fileName);
    // Desctructor
    ~ObjParser();

    // Parse a line of a obj file
    void parseObjLine(std::string line);
    void parseMtlLine(std::string line);

    // Get the verticies of this obj model
    std::vector<ObjParser::Vec3<float>> getVerts();
    // Get the normals of this obj model
	std::vector<ObjParser::Vec3<float>> getNormals();
    // Get the vertex textures of this obj model
	std::vector<ObjParser::Vec2<float>> getVertTextures();
	// Get the face of this obj model
	std::vector<ObjParser::Face> getFaces();

	// Print debugging goes here!
	void debug();

	std::string getDiffuseMap();

private:
	// The verticies of this obj model
	std::vector<ObjParser::Vec3<float>> verts;
	// The normals of this obj model
	std::vector<ObjParser::Vec3<float>> normals;
	// The vertex textures of this obj model
	std::vector<ObjParser::Vec2<float>> vertTextures;
	// The faces of this obj model
	std::vector<ObjParser::Face> faces;
	
	// Number of indicies to keep track of
	int idxNumber;

	// Corresponding mtlFile
	std::string mtlFileName;
	// Diffuse map for corresponding mtl file
	std::string diffuseMapName;
	// Normal map for corresponding mtl file
	std::string normalMapName;
	// Spectral map for corresponding mtl file
	std::string spectralMapName;

	// File path to the directory of this objParser
	std::string relFilePath;
};

#endif