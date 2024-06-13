#pragma once

#include"../includes/GLAD/glad.h"
#include"../includes/GLFW/glfw3.h"

#include"../Platform/OpenGL/OpenGL_Shader.h"
#include"../Platform/OpenGL/OpenGL_Window.h"
#include"../Platform/OpenGL/OpenGL_Application.h"
#include"../Platform/OpenGL/OpenGL_Buffer.h"
#include"../Platform/OpenGL/OpenGL_Text.h"

#define TESTMODE
#include"../Util/VertexGenerator.cpp"

using namespace OpenGL;
using Window = OpenGL_Window;
using Shader = OpenGL_Shader;
using Application = OpenGL_Application;