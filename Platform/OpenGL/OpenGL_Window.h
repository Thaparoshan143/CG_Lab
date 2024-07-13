#pragma once

#include<map>

#include"../../includes/GLAD/glad.h"
#include"../../includes/GLFW/glfw3.h"

#include"../../Core/Window.h"
#include"./OpenGL_Shader.h"

namespace OpenGL
{
	class OpenGL_Window : public Interface::IWindow
	{
		public:
		OpenGL_Window(Interface::IApplication &targetApp, WindowInfo &wi);
		~OpenGL_Window() {	glfwTerminate();	}

		void SetActive() override {	glfwMakeContextCurrent(this->m_glfwWindow);	}
		inline bool ShouldCloseWindow() override {	return glfwWindowShouldClose(this->m_glfwWindow);	}
		void SetColor(float r, float  g, float b, float a = 1.0f) override;
		void SetColor(Color4 &col) override {	SetColor(col.r, col.g, col.b, col.a);	}
		void SwapFrameBuffer() override {	glfwSwapBuffers(this->m_glfwWindow);	}
		void SetWindowInfo(WindowInfo &wi);
		void* GetWindow() override {	return m_glfwWindow;	}

		void initializeOpenGLWindow(int w, int h, String t);

		GLFWwindow *m_glfwWindow;
	};
}

