#include "..\include\Scene.h"
#include <iostream>
#include "../include/pugixml/pugixml.hpp"
#include <map>
#include "../include/Shading/Shader.h"
#include "../include/Shading/Mesh.h"
#include "../include/Components/GameObject.h"
#include "../include/Components/Renderer.h"
#include "../include/Components/PlayerMovement.h"
#include "../include/Components/Camera.h"
#include "../include/Components/CameraController.h"
#include "../include/Components/Light.h"
#include "../include/Components/PointLight.h"
#include "../include/DeferredRendering.h"
#include "../include/RenderingManager.h"
#include "../include/UpdateManager.h"

std::map<std::string, Shader*> shaders;
std::map<std::string, Material*> materials;
std::map<std::string, Mesh*> meshes;

std::vector<GameObject*> cache;


void DeleteCache()
{
	while(meshes.size())
	{
		Mesh* back = prev(meshes.end())->second;
		meshes.erase(prev(meshes.end()));
		delete back;
	}
	while (meshes.size())
	{
		Material* back = prev(materials.end())->second;
		materials.erase(prev(materials.end()));
		delete back;
	}
	while (meshes.size())
	{
		Shader* back = prev(shaders.end())->second;
		shaders.erase(prev(shaders.end()));
		delete back;
	}
	while (cache.size())
	{
		GameObject* obj = cache.back();
		cache.pop_back();
		delete obj;
	}
	cache.clear();
	shaders.clear();
	materials.clear();
	meshes.clear();
}

void loadScene(std::string sceneName)
{
	DeleteCache();
	//RenderingManagerClear();
	//UpdateManagerClear();
	//Light::ClearLights();
	Deserialize(sceneName);

	if (!createGBuffer())
	{
		pgr::dieWithError("unable to create GBuffer");
	}
}

void DeserializeShaders(pugi::xml_node shadersNode)
{
	for (pugi::xml_node shaderNode : shadersNode.children())
	{
		std::string shaderName = shaderNode.attribute("name").as_string();
		Shader* shader;
		std::string geometryShader = shaderNode.attribute("gs").as_string("");
		if (geometryShader != "")
		{
			shader = new Shader(shaderNode.attribute("path").as_string(), geometryShader);
		}
		else
		{
			shader = new Shader(shaderNode.attribute("path").as_string());
		}
		shaders.insert(std::pair<std::string, Shader*>(shaderName, shader));
	}
}

void DeserializeMaterials(pugi::xml_node materialsNode)
{
	for (pugi::xml_node materialNode : materialsNode.children())
	{
		Material* material = new Material(materialNode.attribute("path").as_string());
		std::string materialName = materialNode.attribute("name").as_string();
		materials.insert(std::pair<std::string, Material*>(materialName, material));
	}
}

void DeserializeMeshes(pugi::xml_node meshesNode)
{
	for (pugi::xml_node meshNode : meshesNode.children())
	{
		Mesh* mesh = new Mesh(meshNode.attribute("path").as_string());
		std::string meshName = meshNode.attribute("name").as_string();
		meshes.insert(std::pair<std::string, Mesh*>(meshName, mesh));
	}
}

void DeserializeGameObjects(pugi::xml_node gameObjectsNode)
{
	for (pugi::xml_node gameObjectNode : gameObjectsNode.children())
	{
		GameObject* gameObject = new GameObject();
		cache.push_back(gameObject);

		for (pugi::xml_node component : gameObjectNode.children())
		{
			std::string type = component.name();
			if (type == "transform")
			{
				gameObject->transform->position = glm::vec3(component.child("position").attribute("x").as_float(), component.child("position").attribute("y").as_float(), component.child("position").attribute("z").as_float());
				gameObject->transform->rotation = glm::vec3(component.child("rotation").attribute("x").as_float(), component.child("rotation").attribute("y").as_float(), component.child("rotation").attribute("z").as_float());
				gameObject->transform->scale = glm::vec3(component.child("scale").attribute("x").as_float(), component.child("scale").attribute("y").as_float(), component.child("scale").attribute("z").as_float());
			}
			else if (type == "camera")
			{
				Camera* camera = new Camera(gameObject);
			}
			else if (type == "playerMovement")
			{
				PlayerMovement* playerMovement = new PlayerMovement(gameObject);
			}
			else if (type == "cameraController")
			{
				CameraController* cameraController = new CameraController(gameObject);
			}
			else if (type == "renderer")
			{
				Mesh* mesh = meshes[(std::string)component.attribute("mesh").as_string()];
				Shader* shader = shaders[(std::string)component.attribute("shader").as_string()];
				Material* material = materials[(std::string)component.attribute("material").as_string()];
				Renderer* renderer = new Renderer(gameObject, shader, material, mesh);
			}
			else if (type == "light")
			{
				std::string lightType = component.attribute("type").as_string();
				if (lightType == "point")
				{
					PointLight* light = new PointLight(gameObject, component);
				}
				else
				{
					printf("Would you kindly implement %s light before trying to use it", component.attribute("type").as_string());
				}

			}
			else if (type == "shadowRenderer")
			{
				Mesh* mesh = meshes[(std::string)component.attribute("mesh").as_string()];
				Shader* shader = shaders[(std::string)component.attribute("shader").as_string()];
				ShadowRenderer* renderer = new ShadowRenderer(gameObject, shader, mesh);
			}
			else
			{
				printf("Error: undefined component - %s\n", type);
			}
		}
	}
}

void Deserialize(std::string fileName)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(fileName.c_str());
	std::cout << "Load result: " << result.description() << std::endl;

	pugi::xml_node scene = doc.child("scene");
	setPostProcessingShaders(scene.child("postProcessing"));
	DeserializeShaders(scene.child("shaders"));
	DeserializeMaterials(scene.child("materials"));
	DeserializeMeshes(scene.child("meshes"));
	DeserializeGameObjects(scene.child("gameObjects"));
}
