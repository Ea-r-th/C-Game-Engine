#include <sepch.h>
#include "MiscTools.h"
#include "ShmingoApp.h"


//Disables cursor in order to use first person camera
void disableGLFWCursor(){

	glfwSetInputMode(se_application.getWindow()->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(se_application.getWindow()->getGLFWwindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

}

//Re enables normal cursor behavior
void enableGLFWCursor(){

	glfwSetInputMode(se_application.getWindow()->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void centerCursor(){

	glfwSetCursorPos(se_application.getWindow()->getGLFWwindow(), se_application.getWindow()->getWidth() / 2, se_application.getWindow()->getHeight() / 2);

}