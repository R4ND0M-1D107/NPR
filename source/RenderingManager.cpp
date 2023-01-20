#include "../include/RenderingManager.h"
#include "../include/Components/Renderer.h"
#include <pgr.h>
#include "../include/DeferredRendering.h"
#include "../include/Components/Camera.h"
#include "../include/Components/GameObject.h"
#include "../include/Components/Transform.h"
#include "../include/Components/Light.h"

std::vector<Renderer*> renderers;
std::vector<ShadowRenderer*> shadowRenderers;
Camera* mainCamera;

float clockToMS(clock_t ticks)
{
	return (ticks / (float)CLOCKS_PER_SEC) * 1000.0f;
}
int frames = 0;
clock_t deltaTime = 0;
float frameRate = 30;
void FPS(clock_t start, clock_t end)
{
	frames++;
	deltaTime += end - start;
	if (clockToMS(deltaTime) > 1000.0f)
	{
		frameRate = frames * 0.5 + frameRate * 0.5;
		frames = 0;
		deltaTime -= CLOCKS_PER_SEC;
		
		printf("%f\n", frameRate);
	}
}

int shadowCooldown = -1;
void onRender()
{
	clock_t start = clock();
	CHECK_GL_ERROR();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CHECK_GL_ERROR();

	renderIntoGBuffer();
	
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	for (Renderer* renderer : renderers)
	{
		renderer->Render(mainCamera->GetViewMatrix(), mainCamera->GetPerspectiveMatrix());
	}

	if(shadowCooldown < 0)
	{
		shadowCooldown = 100;
		glViewport(0, 0, 1024, 1024);
		for (Light* light : Light::GetAllLights())
		{
			light->ComputeShadows();
		}
		glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		CHECK_GL_ERROR();
	}else
	{
		shadowCooldown--;
	}


	executeDeferredShading();
	CHECK_GL_ERROR();
	glutSwapBuffers();
	CHECK_GL_ERROR();
	glutPostRedisplay();
	clock_t end = clock();
	FPS(start, end);
}

void RenderShadowCasters(std::vector<glm::mat4> projViewMats, glm::vec3 lightPos)
{
	for (ShadowRenderer* renderer : shadowRenderers)
	{
		renderer->Render(projViewMats, lightPos);
	}
}

void RenderShadowCasters(glm::mat4 projMat, glm::mat4 viewMat, glm::vec3 lightPos)
{
	for (ShadowRenderer* renderer : shadowRenderers)
	{
		renderer->Render(projMat, viewMat, lightPos);
	}
}

void AddRenderer(Renderer* renderer)
{
	renderers.push_back(renderer);
}

void RemoveRenderer(Renderer* renderer)
{
	renderers.erase(std::remove(renderers.begin(), renderers.end(), renderer), renderers.end());
}

void AddShadowRenderer(ShadowRenderer* renderer)
{
	shadowRenderers.push_back(renderer);
}

void RemoveShadowRenderer(ShadowRenderer* renderer)
{
	shadowRenderers.erase(std::remove(shadowRenderers.begin(), shadowRenderers.end(), renderer), shadowRenderers.end());
}

void RenderingManagerClear()
{
	renderers.clear();
}

void onReshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
	if (!createGBuffer())
	{
		pgr::dieWithError("unable to reshape GBuffer");
	}
}

void SetMainCamera(Camera* camera)
{
	if (mainCamera != nullptr)
	{
		printf("Replacing main camera. There can be only one active camera, please don't make more than one camera it might result in unwanted behaviour.");
	}
	mainCamera = camera;
}

glm::vec3 GetCameraPosition()
{
	return mainCamera->gameObject->transform->position;
}