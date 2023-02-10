#pragma once
#include "./pugixml/pugixml.hpp"

typedef struct _GBuffer
{
	unsigned int FBO;
	unsigned int RBO;
	unsigned int texture;
	unsigned int normalTexture;
	unsigned int depthTexture;
	unsigned int stencilTexture;
	unsigned int emissionTexture;
	unsigned int maskTexture;
	unsigned int posTexture;
	unsigned int UVTexture;
	GLuint VBO;
	GLuint VAO;
} GBuffer;

bool createGBuffer();
void renderIntoGBuffer();
void queueScreenshot();
void executeDeferredShading();
glm::vec3 GetCameraPosition();
void setPostProcessingShaders(std::string fileName);