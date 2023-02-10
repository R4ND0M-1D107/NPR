#include "..\..\include\Editor\MenuBar.h"
#include "../../libs/imgui/imgui.h"
#include <windows.h>
#include <string.h>
#include <iostream>
#include "../../include/Scene.h"
#include <filesystem>

namespace Editor
{
	void FileMenu()
	{
		std::filesystem::path curr = std::filesystem::current_path();
		printf("curr %s\n", curr.c_str());
		if (ImGui::MenuItem("Open Scene"))
		{
			OPENFILENAME ofn;
			char fileName[MAX_PATH] = "";
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = "";

			std::string fileNameStr;

			if (GetOpenFileName(&ofn))
			{
				fileNameStr = fileName;
				 curr = std::filesystem::current_path();
				printf("curr %s\n", curr.c_str());
				std::filesystem::path filePath = std::filesystem::path(fileNameStr);
				printf("fp %s\n", filePath.c_str());
				fileNameStr = std::filesystem::relative(curr, filePath).generic_string();
				printf("Loading %s\n", fileNameStr);
				//loadScene(fileNameStr);
			}
		}
	}

	void DrawMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				FileMenu();
				ImGui::EndMenu();
			}
		}
	}
}