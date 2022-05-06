/*****************************************************************//**
 * \file   Input.cpp
 * \brief  Source code of the Input class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/
#include "Input.hpp"

InputKey keys[1024] = { InputKey::Released };
int lastIndex = -1;

bool mouseMoved = false;
glm::dvec2 lastMousePos;
glm::vec2 mousePosOffset(0.0f, 0.0f);

bool mouseScrollMoved = false;
glm::vec2 mouseScrollOffset(0.0f, 0.0f);

void Input::Init(GLFWwindow * window)
{
	glfwGetCursorPos(window, &lastMousePos.x, &lastMousePos.y);
}

InputKey Input::key(unsigned int index) const
{
	if (index >= 1024) return InputKey::InputKeyError;
	return keys[index];
}

glm::dvec2 Input::mousePosition() const
{
	return lastMousePos;
}

glm::vec2 Input::mousePositionOffset() const
{
	return mousePosOffset;
}

glm::vec2 Input::mouseWheelOffset() const
{
	return mouseScrollOffset;
}

bool Input::mouseHasMoved() const
{
	return mouseMoved;
}

bool Input::mouseScrollHasMoved() const
{
	return mouseScrollMoved;
}

void InputCallbackCorrection()
{
	if (lastIndex >= 0 && lastIndex < 1024)
	{
		keys[lastIndex] = static_cast<InputKey>(static_cast<unsigned char>(keys[lastIndex]) + 1 % 4);
		lastIndex = -1;
	}
	mouseScrollMoved = mouseMoved = false;
	mouseScrollOffset = mousePosOffset = glm::vec2(0.0f, 0.0f);
}

void InputMouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	mouseMoved = true;

	mousePosOffset.x = xpos - lastMousePos.x;
	mousePosOffset.y = lastMousePos.y - ypos;
	lastMousePos.x = xpos;
	lastMousePos.y = ypos;
}

void InputScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
	mouseScrollMoved = true;

	mouseScrollOffset.x = xoffset;
	mouseScrollOffset.y = yoffset;
}

void InputKeyCallback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		switch (action)
		{
			case GLFW_PRESS:
				keys[key] = InputKey::JustPressed;
				lastIndex = key;
				break;
			case GLFW_RELEASE:
				keys[key] = InputKey::JustReleased;
				lastIndex = key;
				break;
			case GLFW_REPEAT:
				break;
		}
	}
}
