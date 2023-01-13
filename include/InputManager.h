#pragma once
#include "pgr.h"
#include "Components/Component.h"

typedef struct _Keys
{
	bool up;
	bool down;
	bool left;
	bool right;
	bool shift;
}Keys;

void keyDown(unsigned char keyPressed, int mouseX, int mouseY);
void keyUp(unsigned char keyReleased, int mouseX, int mouseY);
void specialKeyUp(int specKeyPressed, int mouseX, int mouseY);
void specialKeyDown(int specKeyReleased, int mouseX, int mouseY);
void mouseClick(int buttonPressed, int buttonState, int mouseX, int mouseY);
void mouseMove(int mouseX, int mouseY);
bool IsKeyDown(char key);
Keys AreSpecialKeyDown();
glm::vec2 GetMouseDelta();
void AddMouseListener(Component* listener);

void RemoveMouseListener(Component* listener);
