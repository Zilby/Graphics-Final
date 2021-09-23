#include "PPM.h"
#include <iostream>
#include <fstream>

// Constructor loads a filename with the .ppm extension
PPM::PPM(std::string fileName){
	std::ifstream inFile;
	inFile.open(fileName);

	if(inFile.is_open())
	{
		std::string line; 
		getline(inFile,line);
		bool gotWidth = false;
		bool gotHeight = false;
		bool gotRange = false;
		m_PixelData = new unsigned char[m_width * m_height * 3];
		memset(m_PixelData, 255, m_width * m_height * 3);
		int p = 0;
		while(getline(inFile,line))
		{
			if(line.at(0)=='#')
			{
				continue;
			}
			else
			{
				std::string temp = "";
				// Handle multiple inputs on one line
				for(int i = 0; i < line.length(); ++i) 
				{
					temp += line.at(i);
					if(line.at(i)==' ' || i == line.length() - 1)
					{
						if(!gotWidth) 
						{
							m_width = std::stoi(temp);
							gotWidth = true;
						} 
						else if (!gotHeight) 
						{
							m_height = std::stoi(temp);
							gotHeight = true;
						}
						else if (!gotRange) 
						{
							m_range = std::stoi(temp);
							gotRange = true;
						} 
						else 
						{
							// Scale different color ranges so that they will work with darken
							m_PixelData[p] = (unsigned char)(std::stoi(line) * (255 / m_range));
							++p;
						}
						temp = "";
					}
				}
			}
		}
	}

	inFile.close();
}

// Destructor clears any memory that has been allocated
PPM::~PPM(){
	delete[] m_PixelData;
}

// Saves a PPM Image to a new file.
void PPM::savePPM(std::string outputFileName){
	std::ofstream outFile;
	outFile.open(outputFileName);

	outFile << "P3" << std::endl;
	outFile << m_width << " " << m_height << std::endl;
	outFile << 255 << std::endl;
	for(int i = 0; i < m_width * m_height * 3; ++i) 
	{
		outFile << (int)m_PixelData[i] << std::endl;
	}

	outFile.close();
}

// Darken subtracts 50 from each of the red, green
// and blue color components of all of the pixels
// in the PPM. Note that no values may be less than
// 50 in a ppm.
void PPM::darken(){
	for(int i = 0; i < m_width * m_height * 3; ++i) 
	{
		if((int)m_PixelData[i] > 50) 
		{
			m_PixelData[i] -= (unsigned char)50;
		} 
		else 
		{
			m_PixelData[i] = (unsigned char)0;
		}
	}
}

// Sets a pixel to a specific R,G,B value 
void PPM::setPixel(int x, int y, int R, int G, int B){
	m_PixelData[((y * m_width + x) * 3)] = (unsigned char)R;
	m_PixelData[((y * m_width + x) * 3) + 1] = (unsigned char)G;
	m_PixelData[((y * m_width + x) * 3) + 2] = (unsigned char)B;
}

// Returns the raw pixel data in an array.
// You may research what 'inline' does.
inline unsigned char* PPM::pixelData(){
	return m_PixelData;
}
// Returns image width
inline int PPM::getWidth(){
	return m_width;
}
// Returns image height
inline int PPM::getHeight(){
	return m_height;
}

// Returns image range
inline int PPM::getRange(){
	return m_range;
}
