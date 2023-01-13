#include "../include/RenderingManager.h"
#include "../include/Components/Renderer.h"
#include <pgr.h>
#include "../include/DeferredRendering.h"
#include "../include/Components/Camera.h"
#include "../include/Components/GameObject.h"
#include "../include/Components/Transform.h"


std::vector<Renderer*> renderers;
Camera* mainCamera;

void onRender()
{
	CHECK_GL_ERROR();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CHECK_GL_ERROR();

	renderIntoGBuffer();
	
	for (Renderer* renderer : renderers)
	{
		renderer->Render(mainCamera->GetViewMatrix(), mainCamera->GetPerspectiveMatrix());
	}

	CHECK_GL_ERROR();
	executeDeferredShading();
	CHECK_GL_ERROR();
	glutSwapBuffers();
	CHECK_GL_ERROR();
	glutPostRedisplay();
}

void AddRenderer(Renderer* renderer)
{
	renderers.push_back(renderer);
}

void RemoveRenderer(Renderer* renderer)
{
	renderers.erase(std::remove(renderers.begin(), renderers.end(), renderer), renderers.end());
}

void RenderingManagerClear()
{
	renderers.clear();
}

void onReshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
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