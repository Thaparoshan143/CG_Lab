#pragma once

#include"../../Core/Text.h"
#include"./OpenGL_Shader.h"
#include"./OpenGL_Window.h"
#include"./OpenGL_Buffer.h"

#include "../../includes/freetypeinc/ft2build.h"
#include "../../includes/freetypeinc/freetype/freetype.h"

#define BASE_FONT_SIZE 1/6.0

namespace OpenGL
{
    // Map 
    // String (font family name) as key and CharMap as value (which is also map..)
    using FontMap = std::map<String, Interface::CharMap>;

    class FreetypeText : public Interface::Text
    {
        public:
        FreetypeText(String &text, String fontFamily="Roboto");

        static void SetTarget(GLFWwindow *target)   {   m_target = target;  }
        void SetText(String &text) { m_text = text; }
        void SetFontFamily(String fontFamily)   {   m_fontFamily = fontFamily;  initFreetypeFont();  }

        void RenderText(uint shaderID, float x, float y, float scale, Color3 color, bool normalize=true);
        void RenderText(OpenGL_Shader &shader, float x, float y, float scale, Color3 color, bool normalize=true);
    
        private:
        void initFreetypeFont();
        VertexBufferObject m_VBO;
        VertexArrayObject m_VAO;
        // target window to render text, required for getting user pointer..
        static GLFWwindow *m_target;
        static FontMap m_fontFamilyList;
    };
}