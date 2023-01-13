#include "..\..\include\Shading\Material.h"
#include <fstream>
#include <pgr.h>

Material::Material(std::string txt)
{
	std::ifstream file(txt, std::ios::in);
	if (!file)
	{
		pgr::dieWithError("material file doesn't exist");
	}
	else
	{
		std::string type;
		std::string name;
		std::string value;
		while (file >> type >> name >> value)
		{
			if (type == "texture")
			{
				GLuint texture = pgr::createTexture(value);
				if (texture == 0)
					pgr::dieWithError("Texture loading failed.");

				textures.insert(std::pair<std::string, GLuint>(name, texture));
			}
			else if (type == "float")
			{
				floats.insert(std::pair<std::string, float>(name, std::stof(value)));
			}
			else if (type == "vec2")
			{
				std::string separator = ";";
				float v1 = std::stof(value.substr(0, value.find(separator)));
				float v2 = std::stof(value.erase(0, value.find(separator) + separator.length()));
				vec2s.insert(std::pair<std::string, glm::vec2>(name, glm::vec2(v1,v2)));
			}
			else
			{
				pgr::dieWithError("Invalid shader definition");
			}
		}
	}
}

Material::~Material()
{
}
