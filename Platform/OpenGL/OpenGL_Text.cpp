#include"./OpenGL_Text.h"

namespace OpenGL
{
	GLFWwindow* FreetypeText::m_target = nullptr;
	FontMap FreetypeText::m_fontFamilyList = FontMap();

    FreetypeText::FreetypeText(String &text, String fontFamily) : Text(text, fontFamily)
    {
        initFreetypeFont();
		std::cout << "The given text is : " << this->m_text << std::endl;
    }

    void FreetypeText::initFreetypeFont()
    {
		// if doesnt exist's in the fontlist map then load the character mapping info.. and add to map
		if(m_fontFamilyList.count(m_fontFamily)==0)
		{
			FT_Library ft;
			if (FT_Init_FreeType(&ft))
			{
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
				exit(1);
			}

			FT_Face face;
			String fontFamilyPath = "../res/Fonts/" + m_fontFamily + ".ttf";

			if (FT_New_Face(ft, fontFamilyPath.c_str(), 0, &face)) {
				std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
				exit(1);
			}
			else {
				FT_Set_Pixel_Sizes(face, 0, 48);

				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				std::map<char, Interface::Character> charMap;
				for (unsigned char c = 0; c < 128; c++)
				{
					if (FT_Load_Char(face, c, FT_LOAD_RENDER))
					{
						std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
						continue;
					}

					unsigned int texture;
					glGenTextures(1, &texture);
					glBindTexture(GL_TEXTURE_2D, texture);
					glTexImage2D(
						GL_TEXTURE_2D,
						0,
						GL_RED,
						face->glyph->bitmap.width,
						face->glyph->bitmap.rows,
						0,
						GL_RED,
						GL_UNSIGNED_BYTE,
						face->glyph->bitmap.buffer
					);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					Interface::Character character = {
						texture,
						iVec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
						iVec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
						static_cast<uint>(face->glyph->advance.x)
					};
					charMap.insert(std::pair<char, Interface::Character>(c, character));
				}
				m_fontFamilyList.insert({m_fontFamily, charMap});
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			FT_Done_Face(face);
			FT_Done_FreeType(ft);
		}
		else
		{
			std::cout << "Font familt exist on the list!!" << std::endl;
		}

		m_VAO.Bind();
		m_VBO.Bind();
		// For text rendering we require only position (x,y) and texture (u,v).. for 6*4 (i.e using two triangle method to draw quad to draw text on top)
		m_VBO.ReserveBuffer(6*4, GL_DYNAMIC_DRAW);
		// Here 4 is the total vertex buffer size i.e two position and two texture data 
		m_VAO.SetVertexAttributePointerMan(4);
		// unbinding is not manditory, optional
		m_VAO.Unbind();
		m_VBO.Unbind();

		m_textAlign = Interface::CENTER;
		m_activeCharacters = &m_fontFamilyList[m_fontFamily];
    }

    void FreetypeText::RenderText(uint shaderID, float x, float y, float scale, Color3 color, bool normalize)
    {
		if(m_target==nullptr)
		{
			std::cout << "Window target not updated to render" << std::endl;
			return; 
		}
		OpenGL_Window *targetWindow = (OpenGL_Window*)glfwGetWindowUserPointer(m_target);
        iVec2 winDim = targetWindow->GetWindowSize();
		scale *= ((float(winDim.x + winDim.y)/(400+300))) * BASE_FONT_SIZE;

		glUseProgram(shaderID);
		uint location = glGetUniformLocation(shaderID, "textColor");
		glUniform3f(location, color.r, color.g, color.b);

		m_VAO.Bind();
		m_VBO.Bind();
		location = glGetUniformLocation(shaderID, "projection");
		glm::mat4 proj = glm::ortho<float>(0, winDim.x, 0, winDim.y);
 		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(proj));
		glActiveTexture(GL_TEXTURE0);

		if(!normalize)
		{
			x = x * (winDim.x/2.0) + winDim.x/2.0;
			y = -1 * y * (winDim.y/2.0) + winDim.y/2.0;
		}
		// the position based are different in projection and OpenGL working so adjusting for y
		y = winDim.y - y;
		// For now adjusting the x and y alignment 
		fVec2 alignmentOffset = GetAlignmentOffset();
		x = x - (alignmentOffset.x * scale);
		y = y - (alignmentOffset.y * scale);

		// iterate through all characters
		std::string::const_iterator c;
		for (c = m_text.begin(); c != m_text.end(); c++) 
		{
			
			Interface::Character ch = (*m_activeCharacters)[*c];
			float xpos = x + ch.Bearing.x * scale;
			float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;

			float vertices[6][4] = {
				{ xpos,     ypos + h,   0.0f, 0.0f },            
				{ xpos,     ypos,       0.0f, 1.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },

				{ xpos,     ypos + h,   0.0f, 0.0f },
				{ xpos + w, ypos,       1.0f, 1.0f },
				{ xpos + w, ypos + h,   1.0f, 0.0f }
			};
			// render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// update content of m_VBO memory
			m_VBO.LoadSubBuffer(6*4, *vertices);
			m_VBO.Unbind();
			// render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			x += ((ch.Advance >> 6) * scale); // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		}
		m_VAO.Unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
    }

	void FreetypeText::RenderText(OpenGL_Shader &shader, float x, float y, float scale, Color3 color, bool normalize)
	{
		this->RenderText(shader.GetProgramID(), x, y, scale, color, normalize);
	}
}