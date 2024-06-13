// Implementation of mid-point circle drawing algorithm

#include"./include_header.h"
#include"../Util/Utils.cpp"

namespace Extra
{
    #define SCR_WIDTH 800
	#define SCR_HEIGHT 800
	#define POINT_SIZE 3.0
	// #define INPUT_ENABLE
    #define FONT_SCALE 1.5

	ApplicationInfo appInfo(SCR_WIDTH, SCR_HEIGHT, "Mid-point Circle Drawing Alogrithm");

    class CDAApp : public OpenGL_Application
    {
        public:
        CDAApp(ApplicationInfo appInfo) : OpenGL_Application(appInfo), lineShader("../res/Shaders/line/"), textShader("../res/Shaders/Text/projbased/")   
        {   
            targetText = new FreetypeText(displayName);
            targetText->SetAligment(Interface::TextAlignment::CENTER);
            targetText->SetTarget((GLFWwindow*)m_window->GetWindow()); 
            lineShader.CreateProgram();
            textShader.CreateProgram();
        }

        void Initialize() override
        {
            std::cout << "CDA Application instance created ! " << std::endl;

            #ifdef INPUT_ENABLE
            std::cout << "Enter the center of circle : " << std::endl;
            std::cin >> center.x >> center.y;
            std::cout << "Enter the circle radius : " << std::endl;
            std::cin >> radius;

            std::cout << "Enter text to display" << std::endl;
            getchar();
            getline(std::cin, displayName);
            #else
            center = fVec2(SCR_WIDTH/2, SCR_HEIGHT/2);
            radius = SCR_WIDTH/8.0;
            displayName = "078BCT036";
            #endif

            point = fVec2(0, radius);
            // decision parameter
            p = 1 - radius;

            while(point.x <= point.y)
            {
                // Octant pointing
                for(int i=0;i<4;++i)
                {
                    plotPoints.push_back(i%2==0?center.x + point.x : center.x - point.x);
                    plotPoints.push_back(i>1?center.y - point.y : center.y + point.y);
                }

                for(int i=0;i<4;++i)
                {
                    plotPoints.push_back(i%2==0?center.x + point.y : center.x - point.y);
                    plotPoints.push_back(i>1?center.y - point.x : center.y + point.x);
                }

                if(p < 0)
                {
                    p = p + 2*point.x + 3;
                    point.x += 1;
                }
                else
                {
                    p = p + 2*point.x - 2*point.y + 5;
                    point.x += 1;
                    point.y -=1;
                }
            }

            print_point2d(plotPoints.data(), plotPoints.size());
            Util::normalizeM11_point2d(plotPoints.data(), plotPoints.size(), iVec2(SCR_WIDTH, SCR_HEIGHT));
            std::cout << "After normalization " << std::endl;
            print_point2d(plotPoints.data(), plotPoints.size());

            VAO.SetBufferLayout(BufferLayout::PP);
            VAO.Bind();
            VBO.PushData(plotPoints.data(), plotPoints.size());
            VBO.OffloadData();
            VAO.SetVertexAttributePointer();
        }

        void Loop() override
        {

            while(!m_window->ShouldCloseWindow())
            {
                m_window->SetColor(1, 1, 1, 1);
                lineShader.UseProgram();

                VAO.Bind();
                glPointSize(POINT_SIZE);
                glDrawArrays(GL_POINTS, 0, plotPoints.size()/2);

                targetText->RenderText(textShader, center.x, center.y, FONT_SCALE, Color3(1, 0.5, 0.2), true);

                m_window->SwapFrameBuffer();
                glfwPollEvents();
            }
        }

        private:
        fVec2 center, point;
        float p, radius;
        std::vector<float> plotPoints;
        VertexArrayObject VAO;
        VertexBufferObject VBO;
        Shader lineShader;
        Shader textShader;
        FreetypeText *targetText;
        String displayName;
    };

    CDAApp* export_app_instance()
    {
        CDAApp *temp = new CDAApp(appInfo);
        return temp;
    }
}

#define EXPORT_ENABLED