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

	// cleanUpObjects();

	// delete buffers
	// cleanupModels();

	// delete shaders
	// cleanupShaderPrograms();
}

void initApplication()
{
	Deserialize("./Prefabs/Scenes/garage.xml");

	if (!createGBuffer())
	{
		pgr::dieWithError("unable to create GBuffer");
	}


	glutSetCursor(GLUT_CURSOR_NONE);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
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
