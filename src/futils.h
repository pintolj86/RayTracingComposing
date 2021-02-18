#ifndef FUtils_H
#define FUtils_H

#include <iostream>
#include <fstream>
#include <string>
#include "vec3.h"

class FUtils {

public:
	void close() { file.close(); }
	void init(std::string imgPath, const double mp) { file.open(imgPath.c_str(), std::ios::out);  MP = mp;}
	void writeToFile(std::string buffer) { file << buffer << std::endl; }
	void writeColor(Color pixelColor) {
		// Write the translated [0,255] value of each color component.
		file << static_cast<int>(MP * pixelColor.x()) << ' '
			<< static_cast<int>(MP * pixelColor.y()) << ' '
			<< static_cast<int>(MP * pixelColor.z()) << std::endl;
	}
private:
	std::ofstream file;
	double MP;
};

#endif
