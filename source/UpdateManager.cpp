#include "../include/UpdateManager.h"
#include <vector>
#include "pgr.h"

std::vector<Component*> updatables;
float lastUpdate = 0.0f;
void onUpdate(int time)
{
	float currentTime = 0.001f * (float)glutGet(GLUT_ELAPSED_TIME);
	for (Component* updatable : updatables)
	{
		updatable->Update(currentTime-lastUpdate);
	}
	lastUpdate = currentTime;
	glutTimerFunc(16, onUpdate, 0);
}

void AddUpdatable(Component* updatable)
{
	updatables.push_back(updatable);
}

void RemoveUpdatable(Component* updatable)
{
	updatables.erase(std::remove(updatables.begin(), updatables.end(), updatable), updatables.end());
}
