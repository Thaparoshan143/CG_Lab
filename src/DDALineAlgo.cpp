#include"./include_header.h"

namespace Extra
{
	#define SCR_WIDTH 800
	#define SCR_HEIGHT 600

	ApplicationInfo appInfo(SCR_WIDTH, SCR_HEIGHT, "DDA Line Algorithm");

	class DDAApp : public OpenGL_Application
	{
		public:
		DDAApp(ApplicationInfo appinf): OpenGL_Application(appInfo) {}

		void Initialize() override
		{

		}

		void Loop() override
		{
			float *quadVerts = get_quad_bufferind(fVec2(0), fVec2(0.5), Color3(0.5, 1.0, 0.2));
			uint *quadIBO = get_quad_index();

			VertexArrayObject VAO(BufferLayout::PPP_RGB);
			VertexBufferObject VBO;
			IndexBufferObject IBO;
			VAO.Bind();
			IBO.PushData(quadIBO, 6);
			VBO.PushData(quadVerts, 24);
			VBO.OffloadData();
			IBO.OffloadData();
			VAO.SetVertexAttributePointer();

			Shader *temp = this->GetRawShader("../res/Shaders/");

			while (!m_window->ShouldCloseWindow())
			{
				m_window->SetColor(0, 0, 0, 0);
				temp->UseProgram();	

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

				m_window->SwapFrameBuffer();
				glfwPollEvents();
			}
		}
	};

	DDAApp* export_app_instance()
	{
		DDAApp *temp = new DDAApp(appInfo);
		return temp;
	}
}

#define EXPORT_ENABLED