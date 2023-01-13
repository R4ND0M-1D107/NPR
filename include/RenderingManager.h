#pragma once
#include <vector>
#include "Components/Renderer.h"
#include "Components/Camera.h"

void onRender();
void AddRenderer();
void AddRenderer(Renderer* renderer);
void RemoveRenderer(Renderer* renderer);
void RenderingManagerClear();
void onReshape(int width, int height);
void SetMainCamera(Camera* camera);