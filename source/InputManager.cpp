#include "../include/InputManager.h"
#include <pgr.h>

Keys keys;
bool pressedKeys[256];
glm::vec2 mouseDir;
int lastMouseX = 250;
int lastMouseY = 250;
float lastMouseUpdate = 0.0f;
std::vector<Component*> mouseListeners;

void keyDown(unsigned char keyPressed, int mouseX, int mouseY)
{
	pressedKeys[keyPressed] = true;
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
