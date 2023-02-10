#include "pgr.h"
#include "../include/RenderingManager.h"
#include "../include/UpdateManager.h"
#include "../include/InputManager.h"
#include "../include/Components/Renderer.h"
#include "../include/DeferredRendering.h"
#include "../include/Components/GameObject.h"
#include "../Models/sphere.h"
#include "../include/Components/PlayerMovement.h"
#include "../include/Components/CameraController.h"
#include "../include/Scene.h"
#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_glut.h"
#include "../libs/imgui/imgui_impl_opengl3.h"

void createWindow()
{
	glutInitWindowSize(1280, 720);
	glutCreateWindow("NPR examples");

	// Callbacks - use only those you need
	glutDisplayFunc(onRender);
	glutReshapeFunc(onReshape);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specialKeyDown);     // key pressed
	glutSpecialUpFunc(specialKeyUp); // key released
	glutMouseFunc(mouseClick);
	glutPassiveMotionFunc(mouseMove);
	glutTimerFunc(16, onUpdate, 0);
}

void finalizeApplication(void) {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGLUT_Shutdown();
	ImGui::DestroyContext();
}

void initApplication()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	// FIXME: Consider reworking this example to install our own GLUT funcs + forward calls ImGui_ImplGLUT_XXX ones, instead of using ImGui_ImplGLUT_InstallFuncs().
	ImGui_ImplGLUT_Init();
	ImGui_ImplGLUT_InstallFuncs();
	ImGui_ImplOpenGL3_Init();

	loadScene("./Prefabs/Scenes/garage.xml");
	setPostProcessingShaders("./Prefabs/PostProcessing/real.xml");

	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	createWindow();

	// initialize PGR framework (GL, DevIl, etc.)
	if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR))
		pgr::dieWithError("pgr init failed, required OpenGL not supported?");

	initApplication();

	// handle window close by the user
	glutCloseFunc(finalizeApplication);

	// Infinite loop handling the events
	glutMainLoop();

	// code after glutLeaveMainLoop()
	// cleanup

	return 0;
}
