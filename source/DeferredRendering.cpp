#include "pgr.h"
#include "../include/DeferredRendering.h"
#include "../include/Shading/Shader.h"
#include "../include/Shading/Mesh.h"
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__
#include "../include/stb_image_write.h"
#include <ctime>

//Quad
float quadVertices[] = {
	//   positions     texture coordinates
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
};
int quadNAttribsPerVertex = 4;
int quadNVertices = 6;

GBuffer gBuffer;
GBuffer gBuffer1;
std::vector<std::pair<Shader*, Material*>> ppShaders;
std::pair<Shader*, Material*> toScreenShader;

void setupQuad()
{
	//vertex buffer
	glGenBuffers(1, &gBuffer.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, gBuffer.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * quadNVertices * quadNAttribsPerVertex, quadVertices, GL_STATIC_DRAW);
	CHECK_GL_ERROR();

	glGenVertexArrays(1, &gBuffer.VAO);
	glBindVertexArray(gBuffer.VAO);

	for (auto const& ppShader : ppShaders)
	{
		// vertex positions
		glEnableVertexAttribArray(ppShader.first->locations["position"]);
		glVertexAttribPointer(ppShader.first->locations["position"], 2, GL_FLOAT, GL_FALSE, quadNAttribsPerVertex * sizeof(float), (void*)(0));  // [xy][uv]
		CHECK_GL_ERROR();
		// vertex uv
		glEnableVertexAttribArray(ppShader.first->locations["vertexUV"]);
		glVertexAttribPointer(ppShader.first->locations["vertexUV"], 2, GL_FLOAT, GL_FALSE, quadNAttribsPerVertex * sizeof(float), (void*)(2 * sizeof(float)));
	}

	// vertex positions
	glEnableVertexAttribArray(toScreenShader.first->locations["position"]);
	glVertexAttribPointer(toScreenShader.first->locations["position"], 2, GL_FLOAT, GL_FALSE, quadNAttribsPerVertex * sizeof(float), (void*)(0));  // [xy][uv]
	CHECK_GL_ERROR();
	// vertex uv
	glEnableVertexAttribArray(toScreenShader.first->locations["vertexUV"]);
	glVertexAttribPointer(toScreenShader.first->locations["vertexUV"], 2, GL_FLOAT, GL_FALSE, quadNAttribsPerVertex * sizeof(float), (void*)(2 * sizeof(float)));
}

void setPostProcessingShaders(std::string fileName)
{
	//return;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(fileName.c_str());
	//std::cout << "Load result: " << result.description() << std::endl;
	pugi::xml_node pp = doc.child("postProcessing");

	ppShaders.clear();
	for (pugi::xml_node effectNode : pp.children())
	{
		Shader* s = new Shader(effectNode.attribute("shader").as_string());
		Material* m = new Material(effectNode.attribute("material").as_string());
		ppShaders.push_back(std::pair<Shader*, Material*>(s, m));
	}

	Shader* s0 = new Shader("./Prefabs/Shaders/toScreen.shader");
	Material* m0 = new Material("./Prefabs/Materials/gBuffer.mat");
	toScreenShader = std::pair<Shader*, Material*>(s0, m0);
}

void bindColorTexture(GBuffer buff)
{
	glBindTexture(GL_TEXTURE_2D, buff.texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, GL_RGB, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buff.texture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void bindMaskTexture(GBuffer buff)
{
	glBindTexture(GL_TEXTURE_2D, buff.maskTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, GL_RGB, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, buff.maskTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void bindPosTexture(GBuffer buff)
{
	glBindTexture(GL_TEXTURE_2D, buff.posTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, GL_RGB, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, buff.posTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void bindUVTexture(GBuffer buff)
{
	glBindTexture(GL_TEXTURE_2D, buff.UVTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, GL_RGB, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, buff.UVTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void bindNormalTexture(GBuffer buff)
{
	glBindTexture(GL_TEXTURE_2D, buff.normalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, GL_RGB, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, buff.normalTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void bindEmitTexture(GBuffer buff)
{
	glBindTexture(GL_TEXTURE_2D, buff.emissionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, GL_RGB, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, buff.emissionTexture, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void bindDepthTexture(GBuffer buff)
{
	glBindTexture(GL_TEXTURE_2D, buff.depthTexture); CHECK_GL_ERROR();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0); CHECK_GL_ERROR();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, buff.depthTexture, 0); CHECK_GL_ERROR();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void bindStencilTexture(GBuffer buff)
{
	glBindTexture(GL_TEXTURE_2D, buff.stencilTexture); CHECK_GL_ERROR();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0); CHECK_GL_ERROR();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, buff.stencilTexture, 0); CHECK_GL_ERROR();

	glBindTexture(GL_TEXTURE_2D, 0);
}

bool createGBuffer()
{
	glGenFramebuffers(1, &gBuffer.FBO);
	glGenRenderbuffers(1, &gBuffer.RBO);
	glGenTextures(1, &gBuffer.texture);
	glGenTextures(1, &gBuffer.normalTexture);
	glGenTextures(1, &gBuffer.depthTexture);
	glGenTextures(1, &gBuffer.emissionTexture);
	glGenTextures(1, &gBuffer.maskTexture);
	glGenTextures(1, &gBuffer.UVTexture);
	glGenTextures(1, &gBuffer.posTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.FBO);

	bindColorTexture(gBuffer);
	bindNormalTexture(gBuffer);
	bindDepthTexture(gBuffer);
	bindEmitTexture(gBuffer);
	bindMaskTexture(gBuffer);
	bindPosTexture(gBuffer);
	bindUVTexture(gBuffer);
	CHECK_GL_ERROR();

	glGenFramebuffers(1, &gBuffer1.FBO);
	glGenRenderbuffers(1, &gBuffer1.RBO);
	glGenTextures(1, &gBuffer1.texture);
	glGenTextures(1, &gBuffer1.normalTexture);
	glGenTextures(1, &gBuffer1.depthTexture);
	glGenTextures(1, &gBuffer1.emissionTexture);
	glGenTextures(1, &gBuffer1.maskTexture);
	glGenTextures(1, &gBuffer1.UVTexture);
	glGenTextures(1, &gBuffer1.posTexture);

	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer1.FBO);

	bindColorTexture(gBuffer1);
	bindNormalTexture(gBuffer1);
	bindDepthTexture(gBuffer1);
	bindEmitTexture(gBuffer1);
	bindMaskTexture(gBuffer1);
	bindPosTexture(gBuffer1);
	bindUVTexture(gBuffer1);
	CHECK_GL_ERROR();

	Shader* s0 = new Shader("./Prefabs/Shaders/toScreen.shader");
	Material* m0 = new Material("./Prefabs/Materials/gBuffer.mat");
	toScreenShader = std::pair<Shader*, Material*>(s0, m0);

	setupQuad();

	return true;
}


void renderIntoGBuffer()
{
	CHECK_GL_ERROR();
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer.FBO);	

	CHECK_GL_ERROR();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	CHECK_GL_ERROR();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	GLenum buffs[6] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
	glDrawBuffers(6, buffs);

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_STENCIL_TEST);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		pgr::dieWithError("Frame buffer setup failed");

	CHECK_GL_ERROR();
}

void swapGBuffer(GBuffer buff)
{
	CHECK_GL_ERROR();

	glBindFramebuffer(GL_FRAMEBUFFER, buff.FBO);

	GLenum buffs[6] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5 };
	glDrawBuffers(6, buffs);

	//glDisable(GL_STENCIL_TEST);
	glDisable(GL_DEPTH_TEST);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		pgr::dieWithError("Frame buffer setup failed");

	CHECK_GL_ERROR();
}

void SetupGBufferMaterial(Material* mat, GBuffer buff)
{
	mat->floats["height"] = glutGet(GLUT_WINDOW_HEIGHT);
	mat->floats["width"] = glutGet(GLUT_WINDOW_WIDTH);
	mat->textures["screenTexture"] = buff.texture;
	mat->textures["normalTexture"] = buff.normalTexture;
	mat->textures["depthTexture"] = gBuffer.depthTexture;
	mat->textures["emissionTexture"] = buff.emissionTexture;
	mat->textures["maskTexture"] = buff.maskTexture;
	mat->textures["posTexture"] = buff.posTexture;
	mat->textures["uvTexture"] = buff.UVTexture;
}

int postProcessing(int renderPass)
{
	for (auto const& ppShader : ppShaders)
	{
		if (renderPass % 2 == 1) swapGBuffer(gBuffer);
		else swapGBuffer(gBuffer1);

		glUseProgram(ppShader.first->program);
		SetupGBufferMaterial(ppShader.second, (renderPass % 2 == 1 ? gBuffer1 : gBuffer));
		ppShader.first->UseMaterial(ppShader.second);
		CHECK_GL_ERROR();
		glBindVertexArray(gBuffer.VAO);
		CHECK_GL_ERROR();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		CHECK_GL_ERROR();
		glBindVertexArray(0);
		CHECK_GL_ERROR();

		renderPass++;
	}
	return renderPass;
}


bool screenshotQueued = false;
void queueScreenshot()
{
	screenshotQueued = true;
}

void screenshot()
{
	if (!screenshotQueued) return;
	screenshotQueued = false;
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	static GLubyte* data = new GLubyte[3 * width * height];
	memset(data, 0, 3 * width * height);

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	int numOfComponents = 3; // RGB
	int strideInBytes = width * 3;
	time_t now = time(0);
	char utc[26];
    ctime_s(utc,sizeof utc,&now);
	std::string imgName = "screenshot" + std::string(utc);
	stbi_write_png(imgName.c_str(), width, height, 3, data, width * 3);
}

void executeDeferredShading()
{
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	CHECK_GL_ERROR();
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	int renderPass = 0;
	renderPass = postProcessing(renderPass);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(toScreenShader.first->program);
	SetupGBufferMaterial(toScreenShader.second, (renderPass % 2 == 1 ? gBuffer1 : gBuffer));
	toScreenShader.first->UseMaterial(toScreenShader.second);
	//screenshot();
	glBindVertexArray(gBuffer.VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	CHECK_GL_ERROR();
}