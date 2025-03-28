#pragma once
#include <string>
#include <map>
#include <pgr.h>

class Material
{
public:
	Material(std::string txt);
	~Material();
	std::map<std::string, float> floats;
	std::map<std::string, GLuint> textures;
	std::map<std::string, glm::vec2> vec2s;
};