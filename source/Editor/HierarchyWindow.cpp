#include "../../include/Editor/HierarchyWindow.h"
#include <vector>
#include <Components/GameObject.h>
#include <Scene.h>
#include "../../libs/imgui/imgui.h"

namespace Editor
{
	GameObject* selected;

	void RecurseNode(GameObject* obj)
	{
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
		if (obj == selected) node_flags |= ImGuiTreeNodeFlags_Selected;
		if (obj->children.size() == 0) node_flags |= ImGuiTreeNodeFlags_Leaf;

		bool node_open = ImGui::TreeNodeEx(obj->name.c_str(), node_flags);

		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
		{
			selected = obj;
		}

		if (node_open)
		{
			for (GameObject* obj1 : obj->children)
			{
				RecurseNode(obj1);
			}
			ImGui::TreePop();
		}
	}

	void DrawHierarchyWindow()
	{
		ImGui::Begin("Hierarchy");
		std::vector<GameObject*> scene = GetScene();
		for (GameObject* obj : scene)
		{
			RecurseNode(obj);
		}
		ImGui::End();
	}

	GameObject* GetSelectedGameObject()
	{
		return selected;
	}
}