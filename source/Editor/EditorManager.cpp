#include "..\..\include\Editor\EditorManager.h"
#include "../../libs/imgui/imgui.h"
#include "../../libs/imgui/imgui_impl_opengl3.h"
#include "../../libs/imgui/imgui_impl_glut.h"
#include <Editor/HierarchyWindow.h>
#include "pgr.h"
#include <Editor/InspectorWindow.h>
#include <Editor/MenuBar.h>

namespace Editor
{
	void DrawEditor()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGLUT_NewFrame();

		DrawMenuBar();

		bool show_demo_window = true;
		ImGui::ShowDemoWindow(&show_demo_window);

		DrawHierarchyWindow();
		DrawInspectorWindow();

		ImGui::Render();
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDrawCursor = true;
		glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound, but prefer using the GL3+ code.
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
