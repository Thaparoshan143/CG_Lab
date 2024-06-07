// Implementation of Bresenham's Line drawing algorithm

#include"./include_header.h"
#include"../Util/Utils.cpp"

namespace Extra
{
	#define SCR_WIDTH 800
	#define SCR_HEIGHT 800
	#define POINT_SIZE 4.0
	// #define INPUT_ENABLE

	ApplicationInfo appInfo(SCR_WIDTH, SCR_HEIGHT, "Bresenham's Line Algorithm");

	class BresenhamApp : public OpenGL_Application
	{
		public:
		BresenhamApp(ApplicationInfo appinf): OpenGL_Application(appinf) {}

		void Initialize() override
		{
            std::cout << "Bresenham Application Running !" << std::endl;
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
			x2y2 = fVec2(SCR_WIDTH, SCR_HEIGHT/3);
			#endif

            // compute incremental for x and y
			inc = fVec2(x2y2.x > x1y1.x ? 1 : -1, x2y2.y > x1y1.y ? 1 : -1);
            // compute delx and dely for xinc and yinc
			del = fVec2(abs(x2y2.x - x1y1.x), abs(x2y2.y - x1y1.y));

			std::vector<float> plotpoint;
			plotpoint.clear();

            if(del.x > del.y)
            {
                // slope is greater than 1 case
                std::cout << "delx is greater than dely" << std::endl;

                // computing initial decision parameter 
                float p = 2*del.y - del.x;
                float steps = del.x;
			    fVec2 xy = fVec2(x1y1.x, x1y1.y);

                while(steps >= 0)
                {
                    plotpoint.push_back(xy.x);
                    plotpoint.push_back(xy.y);
                    if(p < 0)
                    {
                        xy = fVec2(xy.x + inc.x, xy.y);
                        p = p + 2*del.y;
                    }
                    else
                    {
                        xy = fVec2(xy.x + inc.x, xy.y + inc.y);
                        p = p + 2*del.y - 2*del.x;
                    }
                    steps--;
                }
            }
            else
            {
                // slope is lese or equal to 1 case
                std::cout << "del y is greater than delx" << std::endl;

                // computing initial decision parameter
                float p = 2*del.x - del.y;
                float step = del.y;
			    fVec2 xy = fVec2(x1y1.x, x1y1.y);

                while(step >= 0)
                {
                    plotpoint.push_back(xy.x);
                    plotpoint.push_back(xy.y);
                    if(p < 0)
                    {
                        xy = fVec2(xy.x, xy.y + inc.y);
                        p = p + 2*del.x;
                    }
                    else
                    {
                        xy = fVec2(xy.x + inc.x, xy.y + inc.y);
                        p = p + 2*del.x - 2*del.y;
                    }
                    step--;
                }
            }

			// normalize from scren space to -1 to 1
			print_point2d(plotpoint.data(), plotpoint.size(), '\n');
			Util::normalizeM11_point2d(plotpoint.data(), plotpoint.size(), iVec2(SCR_WIDTH, SCR_HEIGHT));

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

	BresenhamApp* export_app_instance()
	{
		BresenhamApp *temp = new BresenhamApp(appInfo);
		return temp;
	}
}

#define EXPORT_ENABLED