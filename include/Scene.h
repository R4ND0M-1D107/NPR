#pragma once
#include <string>
#include <vector>
#include "../include/Components/GameObject.h"

void loadScene(std::string sceneName);

std::vector<GameObject*> GetScene();

void Deserialize(std::string fileName);