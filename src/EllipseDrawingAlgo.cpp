// Implementation of mid-point ellipse drawing algorithm

#include"./include_header.h"
#include"../Util/Utils.cpp"

namespace Extra
{
    #define SCR_WIDTH 800
    #define SCR_HEIGHT 800
    #define POINT_SIZE 3.0
    #define INPUT_ENABLE 
    #define FONT_SCALE 1.5

    ApplicationInfo appInfo(SCR_WIDTH, SCR_HEIGHT, "Mid-point Ellipse Drawing Alogrithm");

    class EDAApp : public OpenGL_Application
    {
        public:
        EDAApp(ApplicationInfo appInfo) : OpenGL_Application(appInfo), lineShader("../res/Shaders/line/"), textShader("../res/Shaders/Text/projbased/")  
        {
            targetText = new FreetypeText(displayText);
            targetText->SetAligment(Interface::TextAlignment::CENTER);
            targetText->SetTarget((GLFWwindow*)m_window->GetWindow()); 
            lineShader.CreateProgram();
            textShader.CreateProgram();
        }

        void Initialize() override
        {
            std::cout << "EDA Application instance created ! " << std::endl;

            #ifdef INPUT_ENABLE
            center = Util::get_2dpoint("Ellipse Center : ");
            radius = Util::get_2dpoint("Ellipse Radius : ");
            std::cout << "Enter the text to display in center : " << std::endl;
            std::cin >> displayText;
            #else
            center = fVec2(SCR_WIDTH/2, SCR_HEIGHT/2);
            radius = fVec2(250, 180);
            displayText = "078BCT036\n";
            #endif

            // For region - 1
            point = fVec2(0, radius.y);

            // recheck the formula for initial decsion parameter
            p1 = pow(radius.y, 2) + (pow(radius.x, 2))/4.0 - pow(radius.x, 2) * radius.y;

            bool condition1 = true;
            while(condition1)
            {
                plotPoints.push_back(point.x + center.x);
                plotPoints.push_back(point.y + center.y);

                plotPoints.push_back(-point.x + center.x);
                plotPoints.push_back(point.y + center.y);

                plotPoints.push_back(-point.x + center.x);
                plotPoints.push_back(-point.y + center.y);

                plotPoints.push_back(point.x + center.x);
                plotPoints.push_back(-point.y + center.y);

                if(p1 < 0)
                {
                    point = fVec2(point.x + 1, point.y);
                    p1 = p1 + 2 * pow(radius.y, 2)*point.x + pow(radius.y, 2);
                }
                else
                {
                    point = fVec2(point.x + 1, point.y - 1);
                    p1 = p1 + 2 * pow(radius.y, 2)*point.x - 2 * pow(radius.x, 2) * point.y + pow(radius.y, 2);
                }
                condition1 = (pow(radius.y, 2) * point.x) < (pow(radius.x, 2) * point.y) ? true : false;
            }

            // For region - 2
            point = fVec2(point.x, point.y);

            p2 = pow(radius.y, 2) * pow((point.x + 1/2.0), 2) + pow(radius.x, 2) * pow((point.y -1), 2) - pow(radius.x, 2) * pow(radius.y, 2);
            
            bool condition2 = true;
            while(condition2)
            {
                plotPoints.push_back(point.x + center.x);
                plotPoints.push_back(point.y + center.y);
                
                plotPoints.push_back(-point.x + center.x);
                plotPoints.push_back(point.y + center.y);

                plotPoints.push_back(-point.x + center.x);
                plotPoints.push_back(-point.y + center.y);

                plotPoints.push_back(point.x + center.x);
                plotPoints.push_back(-point.y + center.y);

                if(p2 <= 0)
                {
                    point = fVec2(point.x + 1, point.y - 1);
                    p2 = p2 + 2 * pow(radius.y, 2) * point.x - 2 * pow(radius.x, 2) * point.y + pow(radius.x, 2);
                }
                else
                {
                    point = fVec2(point.x, point.y - 1);
                    p2 = p2 - 2 * pow(radius.x, 2) * point.y + pow(radius.x, 2);
                }
                condition2 = (point.y >= 0) ? true : false;
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
                m_window->SetColor(1, 1, 1, 1.0);
                lineShader.UseProgram();

                VAO.Bind();
                glPointSize(POINT_SIZE);
                glDrawArrays(GL_POINTS, 0, plotPoints.size()/2);
                targetText->RenderText(textShader, center.x, center.y, FONT_SCALE, Color3(1, 0.5, 0.2), true);
        
                m_window->SwapFrameBuffer();
                glfwPollEvents();
            }
        }

        protected:
        // required variable for application
        fVec2 radius, center, point;
        float p1, p2;

        std::vector<float> plotPoints;
        VertexArrayObject VAO;
        VertexBufferObject VBO;
        Shader lineShader;
        Shader textShader;
        FreetypeText *targetText;
        String displayText;
    };

    EDAApp* export_app_instance()
    {
        EDAApp *temp = new EDAApp(appInfo);
        return temp;
    }
}

#define EXPORT_ENABLED