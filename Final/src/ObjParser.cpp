#include "ObjParser.h"
#include <algorithm>


ObjParser::ObjParser(std::string fileName) {
	// Get filepath of the object file's directory
	std::size_t endOfPath = fileName.find_last_of("/");
	relFilePath = fileName.substr(0, endOfPath + 1);

	std::ifstream objFile;

	objFile.open(fileName);

	// Parse obj file
	if(objFile.is_open()) {

		std::string line;
		while(getline(objFile, line)) {
			parseObjLine(line);
		}
	}

	objFile.close();

	std::ifstream mtlFile;
	mtlFile.open(relFilePath + mtlFileName);

	// Parse obj file's mtl file
	if(mtlFile.is_open()) {

		std::string line;
		while(getline(mtlFile, line)) {
			parseMtlLine(line);
		}
	}

	mtlFile.close();
}

ObjParser::~ObjParser() { 
}

void ObjParser::parseObjLine(std::string line) {
	if(line.length() > 1) {
		std::istringstream stream(line);
		std::string value;
		stream >> value;
		if(line[0] == 'v') {
			if (line[1] == 't') {
				ObjParser::Vec2<float> vec;
				for(int i = 0; i < 2; ++i) {

					stream >> value;
					vec[i] = std::stof(value);
				}
				vertTextures.push_back(vec);
			}
			else {
				ObjParser::Vec3<float> vec;
				for(int i = 0; i < 3; ++i) {
					stream >> value;
					vec[i] = std::stof(value);
				}
				if(line[1] == 'n') {
					normals.push_back(vec);
				}
				else {
					verts.push_back(vec);
				}
			}
		} else if(line[0] == 'f') {
			size_t n = std::count(line.begin(), line.end(), '/');
			if (n == 6) {
				ObjParser::Vec3<unsigned int> verts;
				ObjParser::Vec3<unsigned int> vertTextures;
				ObjParser::Vec3<unsigned int> norms;

				for(int i = 0; i < 3; ++i) {
					stream >> value;
					std::size_t foundVert = value.find("/", 0);
					verts[i] = std::stoi(value.substr(0, foundVert)) - 1;
					
					std::size_t foundVertTexture = value.find("/", foundVert + 1);				
					vertTextures[i] = std::stoi(value.substr(foundVert + 1, foundVertTexture)) - 1;
					
					std::size_t foundVertNorm = value.find("/", foundVertTexture + 1);				
					norms[i] = std::stoi(value.substr(foundVertTexture + 1)) - 1;
				}

				ObjParser::Face f;
				f.verts = verts;
				f.vertTextures = vertTextures;
				f.norms = norms;
				faces.push_back(f);
			// Subdivide quads into two faces
			} else if (n==8) {
				ObjParser::Vec3<unsigned int> verts1;
				ObjParser::Vec3<unsigned int> vertTextures1;
				ObjParser::Vec3<unsigned int> norms1;
				ObjParser::Vec3<unsigned int> verts2;
				ObjParser::Vec3<unsigned int> vertTextures2;
				ObjParser::Vec3<unsigned int> norms2;

				for(int i = 0; i < 4; ++i) {
					stream >> value;
					std::size_t foundVert = value.find("/", 0);
					std::size_t foundVertTexture = value.find("/", foundVert + 1);	
					std::size_t foundVertNorm = value.find("/", foundVertTexture + 1);

					if (i == 0 || i == 2) {
						verts1[i] = std::stoi(value.substr(0, foundVert)) - 1;
						vertTextures1[i] = std::stoi(value.substr(foundVert + 1, foundVertTexture)) - 1;
						norms1[i] = std::stoi(value.substr(foundVertTexture + 1)) - 1;
						verts2[i] = std::stoi(value.substr(0, foundVert)) - 1;
						vertTextures2[i] = std::stoi(value.substr(foundVert + 1, foundVertTexture)) - 1;
						norms2[i] = std::stoi(value.substr(foundVertTexture + 1)) - 1;
					} else if (i == 1) {
						verts1[i] = std::stoi(value.substr(0, foundVert)) - 1;
						vertTextures1[i] = std::stoi(value.substr(foundVert + 1, foundVertTexture)) - 1;
						norms1[i] = std::stoi(value.substr(foundVertTexture + 1)) - 1;
					} else if (i == 3) {
						verts1[i-2] = std::stoi(value.substr(0, foundVert)) - 1;
						vertTextures1[i-2] = std::stoi(value.substr(foundVert + 1, foundVertTexture)) - 1;
						norms1[i-2] = std::stoi(value.substr(foundVertTexture + 1)) - 1;
					}
				}

				ObjParser::Face f1;
				f1.verts = verts1;
				f1.vertTextures = vertTextures1;
				f1.norms = norms1;
				ObjParser::Face f2;
				f2.verts = verts2;
				f2.vertTextures = vertTextures2;
				f2.norms = norms2;
				faces.push_back(f1);
				faces.push_back(f2);
			}
		} else if(value == "mtllib") {
			stream >> value;
			mtlFileName = value;			
		}
	}
}

void ObjParser::parseMtlLine(std::string line) {
	if(line.length() > 1) {
		std::istringstream stream(line);
		std::string value;
		stream >> value;

		if(value == "map_Kd") {
			stream >> value;
			diffuseMapName = value;			
		}
		else if(value == "map_Bump") {
			stream >> value;
			normalMapName = value;
		}
		else if(value == "map_Ks") {
			stream >> value;
			spectralMapName = value;			
		}
	}
}

std::vector<ObjParser::Vec3<float>> ObjParser::getVerts() {
	return verts;
}
	
std::vector<ObjParser::Vec3<float>> ObjParser::getNormals() {
	return normals;
}

std::vector<ObjParser::Vec2<float>> ObjParser::getVertTextures() {
	return vertTextures;
}
	
std::vector<ObjParser::Face> ObjParser::getFaces() {
	return faces;
}

std::string ObjParser::getDiffuseMap()
{
	if (diffuseMapName == "") {
		return diffuseMapName;
	}
	return relFilePath + diffuseMapName;
}

void ObjParser::debug()
{
	for(int i = 0; i < verts.size(); ++i) {
		printf("v %f, %f, %f\n", verts[i].x, verts[i].y, verts[i].z);
	}

	for(int i = 0; i < normals.size(); ++i) {
		printf("vn %f, %f, %f\n", normals[i].x, normals[i].y, normals[i].z);
	}

	for(int i = 0; i < vertTextures.size(); ++i) {
		printf("vt %f, %f\n", vertTextures[i].x, vertTextures[i].y);
	}

	for(int i = 0; i < faces.size(); ++i) {
		printf("f %d/%d/%d, %d/%d/%d, %d/%d/%d\n", 
			faces[i].verts.x, faces[i].vertTextures.x, faces[i].norms.x, 
			faces[i].verts.y, faces[i].vertTextures.y, faces[i].norms.y, 
			faces[i].verts.z, faces[i].vertTextures.z, faces[i].norms.z);
	}

	std::cout << "mtlFileName: " << mtlFileName << std::endl;
	std::cout << "diffuse map: " << diffuseMapName << std::endl;
	std::cout << "normal map:  " << normalMapName << std::endl;
	std::cout << "spec map:    " << spectralMapName << std::endl;

}