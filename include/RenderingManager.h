#pragma once
#include <vector>
#include "Components/Renderer.h"
#include "Components/ShadowRenderer.h"
#include "Components/Camera.h"

void onRender();
void AddRenderer(Renderer* renderer);
void RemoveRenderer(Renderer* renderer);
void AddShadowRenderer(ShadowRenderer* renderer);
void RemoveShadowRenderer(ShadowRenderer* renderer);
void RenderingManagerClear();
void onReshape(int width, int height);
void SetMainCamera(Camera* camera);
void RenderShadowCasters(std::vector<glm::mat4> projViewMats, glm::vec3 lightPos);