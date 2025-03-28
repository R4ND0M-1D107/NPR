#include "..\..\include\Components\PointLight.h"
#include "..\..\include\Components\GameObject.h"
#include "..\..\include\Components\Transform.h"
#include "..\..\include\Components\ShadowRenderer.h"
#include "../../include/RenderingManager.h"
#include "pgr.h"
#include "..\..\libs\imgui\imgui.h"

PointLight::PointLight(GameObject* owner, pugi::xml_node xmlNode) : Light(owner, xmlNode)
{
    glGenFramebuffers(1, &shadowFBO);

    glGenTextures(1, &shadowCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubemap);

    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowCubemap, 0);
   
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        pgr::dieWithError("Shadow framebuffer is incomplete");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    CHECK_GL_ERROR();

    if (exponent < 0.05) exponent = 1;
}

PointLight::~PointLight()
{
}

void PointLight::ComputeShadows()
{
    glEnable(GL_DEPTH_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    std::vector<glm::mat4> projViewMats;
    float aspect = (float)SHADOW_WIDTH/(float)SHADOW_HEIGHT;
    for (unsigned int i = 0; i < 6; i++)
    {
        glm::mat4 projMat = glm::perspective(glm::radians(90.0f), aspect, 1.0f, 25.0f);
        glm::mat4 viewMat = glm::lookAt(this->gameObject->transform->position, this->gameObject->transform->position + directions[i].front, directions[i].up);
        projViewMats.push_back(projMat*viewMat);
    }

    RenderShadowCasters(projViewMats, this->gameObject->transform->position);
    CHECK_GL_ERROR();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PointLight::OnGUIDraw()
{
    Light::OnGUIDraw();
    ImGui::DragFloat("Range", &exponent, 1.0, 0.05f, 20.0f);
}
