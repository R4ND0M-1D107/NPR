#include "..\..\include\Editor\InspectorWindow.h"
#include "../../libs/imgui/imgui.h"
#include <Components/GameObject.h>
#include <Editor/HierarchyWindow.h>

namespace Editor
{
	void DrawInspectorWindow()
	{
		ImGui::Begin("Inspector");

		GameObject* selected = GetSelectedGameObject();
		if (selected != nullptr)
		{
			ImGui::Checkbox(selected->name.c_str(), &selected->active);

			for (Component* component : selected->components)
			{
				ImGui::Checkbox("", &component->enabled);
				ImGui::SameLine();
				if (ImGui::CollapsingHeader(component->name.c_str()))
				{
					component->OnGUIDraw();
				}
			}
		}

		ImGui::End();
	}
}