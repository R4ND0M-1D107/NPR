#include "../include/InputManager.h"
#include <pgr.h>
#include "../include/Scene.h"
#include "../include/pugixml/pugixml.hpp"
#include "../include/DeferredRendering.h"

Keys keys;
bool pressedKeys[256];
glm::vec2 mouseDir;
int lastMouseX = 250;
int lastMouseY = 250;
float lastMouseUpdate = 0.0f;
std::vector<Component*> mouseListeners;

bool centering = true;
std::string currentScene = "./Prefabs/Scenes/garage.xml";
void keyDown(unsigned char keyPressed, int mouseX, int mouseY)
{
	if (keyPressed == 'z') centering = !centering;

	pressedKeys[keyPressed] = true;

	if(keyPressed == 'r') loadScene(currentScene);

	if(pressedKeys['p'])
	{
		if(keyPressed == '1')
		{
			setPostProcessingShaders("./Prefabs/PostProcessing/real.xml");
		}
		else if(keyPressed == '2')
		{
			setPostProcessingShaders("./Prefabs/PostProcessing/hatch.xml");
		}
		else if(keyPressed == '3')
		{
			setPostProcessingShaders("./Prefabs/PostProcessing/stipple.xml");
		}
		else if (keyPressed == '4')
		{
			setPostProcessingShaders("./Prefabs/PostProcessing/cel.xml");
		}
		else if (keyPressed == '5')
		{
			setPostProcessingShaders("./Prefabs/PostProcessing/voronoi.xml");
		}
	}

	if(pressedKeys['o'])
	{
		if(keyPressed == '1')
		{
			loadScene("./Prefabs/Scenes/garage.xml");
			currentScene = "./Prefabs/Scenes/garage.xml";
		}
		else if(keyPressed == '2')
		{
			loadScene("./Prefabs/Scenes/car.xml");
			currentScene = "./Prefabs/Scenes/car.xml";
		}
		else if(keyPressed == '3')
		{
			loadScene("./Prefabs/Scenes/teapot.xml");
			currentScene = "./Prefabs/Scenes/teapot.xml";
		}
	}
};

void keyUp(unsigned char keyReleased, int mouseX, int mouseY)
{
	pressedKeys[keyReleased] = false;
};

void specialKeyUp(int specKeyReleased, int mouseX, int mouseY)
{
	switch (specKeyReleased)
	{
	case GLUT_KEY_UP:
		keys.up = false;
		break;
	case GLUT_KEY_DOWN:
		keys.down = false;
		break;
	case GLUT_KEY_LEFT:
		keys.left = false;
		break;
	case GLUT_KEY_RIGHT:
		keys.right = false;
		break;
	}

	keys.shift = glutGetModifiers() == GLUT_ACTIVE_SHIFT;
};

void specialKeyDown(int specKeyPressed, int mouseX, int mouseY)
{
	switch (specKeyPressed)
	{
	case GLUT_KEY_UP:
		keys.up = true;
		break;
	case GLUT_KEY_DOWN:
		keys.down = true;
		break;
	case GLUT_KEY_LEFT:
		keys.left = true;
		break;
	case GLUT_KEY_RIGHT:
		keys.right = true;
		break;
	}

	keys.shift = glutGetModifiers() == GLUT_ACTIVE_SHIFT;
};

void mouseClick(int buttonPressed, int buttonState, int mouseX, int mouseY)
{

}

void AddMouseListener(Component* listener)
{
	mouseListeners.push_back(listener);
}

void RemoveMouseListener(Component* listener)
{
	mouseListeners.erase(std::remove(mouseListeners.begin(), mouseListeners.end(), listener), mouseListeners.end());
}

void mouseMove(int mouseX, int mouseY)
{
	float currentTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);

	if (currentTime - lastMouseUpdate < 0.025f) return;

	mouseDir = glm::vec2(lastMouseX - mouseX, lastMouseY - mouseY);

	for (Component* mouseListener : mouseListeners)
	{
		mouseListener->Update(currentTime - lastMouseUpdate);
	}
	
	lastMouseX = mouseX;
	lastMouseY = mouseY;
	lastMouseUpdate = currentTime;

	glutPostRedisplay();


	//center cursor if it's too far
	if (centering)
	{
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		int width = glutGet(GLUT_WINDOW_WIDTH);
		if (mouseX < 100 || mouseX > width - 100) {
			lastMouseX = width / 2;
			lastMouseY = height / 2;
			glutWarpPointer(width / 2, height / 2);
		}
		else if (mouseY < 100 || mouseY > height - 100) {
			lastMouseX = width / 2;
			lastMouseY = height / 2;
			glutWarpPointer(width / 2, height / 2);
		}
	}
	
}


bool IsKeyDown(char key)
{
	return pressedKeys[key];
}

Keys AreSpecialKeyDown()
{
	return keys;
}

glm::vec2 GetMouseDelta()
{
	return mouseDir;
}
