#include "ObjParser.h"


ObjParser::ObjParser(std::string fileName) {
	std::ifstream inFile;

	inFile.open(fileName);

	if(inFile.is_open()) {
		std::string line;
		while(getline(inFile, line)) {
			parseLine(line);
		}
	}

	inFile.close();	
}

ObjParser::~ObjParser() { }

void ObjParser::parseLine(std::string line) {
	if(line.length() > 1) {
		std::istringstream stream(line);
		std::string value;
		stream >> value;
		if(line[0] == 'v') {
			ObjParser::Vec3<float> vec;
			for(int i = 0; i < 3; ++i) {
				stream >> value;
				vec[i] = std::stof(value);
			}
			if(line[1] == 'n') {
				normals.push_back(vec);
			} else {
				verts.push_back(vec);
			}
		} else if(line[0] == 'f') {
			ObjParser::Vec3<unsigned int> verts;
			ObjParser::Vec3<unsigned int> norms;
			for(int i = 0; i < 3; ++i) {
				stream >> value;
				std::size_t found = value.find_first_of("//");
				verts[i] = std::stoi(value.substr(0, found));
				norms[i] = std::stof(value.substr(found + 2));
			}
			ObjParser::Face f;
			f.verts = verts;
			f.norms = norms;
			faces.push_back(f);
		} 
	}
}

std::vector<ObjParser::Vec3<float>> ObjParser::getVerts() {
	return verts;
}
	
std::vector<ObjParser::Vec3<float>> ObjParser::getNormals() {
	return normals;
}
	
std::vector<ObjParser::Face> ObjParser::getFaces() {
	return faces;
}

float* ObjParser::getVertsArray() 
{
	float* output = new float[verts.size() * 3];
	for(int i = 0; i < verts.size(); ++i) {
		for(int j = 0; j < 3; ++j) {
			output[(i * 3) + j] = verts[i][j];
		}
	}
	return output;
}

float* ObjParser::getNormalsArray() 
{
	float* output = new float[normals.size() * 3];
	for(int i = 0; i < normals.size(); ++i) {
		for(int j = 0; j < 3; ++j) {
			output[(i * 3) + j] = normals[i][j];
		}
	}
	return output;
}

unsigned int* ObjParser::getFacesArray() 
{
	unsigned int* output = new unsigned int[faces.size() * 3];
	for(int i = 0; i < faces.size(); ++i) {
		for(int j = 0; j < 3; ++j) {
			output[(i * 3) + j] = faces[i].verts[j] - 1;
		}
	}
	return output;
}

void ObjParser::debug() {
	for(int i = 0; i < verts.size(); ++i) {
		printf("%f, %f, %f\n", verts[i].x, verts[i].y, verts[i].z);
	}
	for(int i = 0; i < normals.size(); ++i) {
		printf("%f, %f, %f\n", normals[i].x, normals[i].y, normals[i].z);
	}
	for(int i = 0; i < faces.size(); ++i) {
		printf("%dll%d, %dll%d, %dll%d\n", faces[i].verts.x, faces[i].norms.x, 
			faces[i].verts.y, faces[i].norms.y, faces[i].verts.z, faces[i].norms.z);
	}
}