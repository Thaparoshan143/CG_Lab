#include"./include_header.h"
#include"../Util/Utils.cpp"

namespace Extra
{
	#define SCR_WIDTH 800
	#define SCR_HEIGHT 800
	#define POINT_SIZE 4.0
	// #define INPUT_ENABLE

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
			// variable declaraiton for DDA algo
			fVec2 x1y1, x2y2, del, inc;
			#ifdef INPUT_ENABLE
			x1y1 = Util::get_2dpoint("# Inital:");
			x2y2 = Util::get_2dpoint("# Final:");
			#else
			x1y1 = fVec2(0, 0);
			x2y2 = fVec2(SCR_HEIGHT/4, SCR_WIDTH);
			#endif
			del = fVec2(x2y2.x - x1y1.x, x2y2.y - x1y1.y);

			int step = abs(del.x) > abs(del.y) ? abs(del.x) : abs(del.y);
			inc = fVec2(del.x/step, del.y/step);
			fVec2 xy = fVec2(x1y1.x, x1y1.y);

			std::vector<float> plotpoint;
			plotpoint.clear();

			// Calculation of the intermediate points
			while(step > 0)
			{
				xy = fVec2(xy.x + inc.x, xy.y + inc.y);
				plotpoint.push_back(xy.x);
				plotpoint.push_back(xy.y);
				step--;
			}

			// normalize from scren space to -1 to 1
			Util::normalizeM11_point2d(plotpoint.data(), plotpoint.size(), iVec2(SCR_WIDTH, SCR_HEIGHT));
			print_point2d(plotpoint.data(), plotpoint.size(), '\n');

			VertexArrayObject VAO(BufferLayout::PP);
			VertexBufferObject VBO;
			VAO.Bind();
			VBO.PushData(plotpoint.data(), plotpoint.size());
			VBO.OffloadData();
			VAO.SetVertexAttributePointer();

			Shader *temp = this->GetRawShader("../res/Shaders/line/");

			// main loop 
			while (!m_window->ShouldCloseWindow())
			{
				m_window->SetColor(1, 1, 1, 0);
				temp->UseProgram();	

				glPointSize(POINT_SIZE);
				glDrawArrays(GL_POINTS, 0, plotpoint.size()/2);

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