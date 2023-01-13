#pragma once
#include "Components/Component.h"

void onUpdate(int time);
void AddUpdatable(Component* updatable);
void UpdateManagerClear();
void RemoveUpdatable(Component* updatable);