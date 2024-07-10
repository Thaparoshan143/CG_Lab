// Implementation of different transformation technique

#include"./include_header.h"
#include"../Util/Utils.cpp"

namespace Extra
{
    #define SCR_WIDTH 800
    #define SCR_HEIGHT 800
    #define POINT_SIZE 3.0
    #define INPUT_ENABLE 
    #define FONT_SCALE 1.5

    ApplicationInfo appInfo(SCR_WIDTH, SCR_HEIGHT, "Transformation");

    Mat4 _get_mat4_translation(fVec2 translation)
    {
        return Mat4(1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    translation.x, translation.y, 0, 1);
    }

    // Works for only 2d and about z axis
    Mat4 _get_mat4_rotation(float rotAngle)
    {
        return Mat4(cos(rotAngle), sin(rotAngle), 0, 0,
                    -sin(rotAngle), cos(rotAngle), 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1);
    }

    Mat4 _get_mat4_scale(fVec2 scaleFactor)
    {
        return Mat4(scaleFactor.x, 0.0, 0.0, 0.0,
                    0.0, scaleFactor.y, 0.0, 0.0,
                    0.0, 0.0, 1, 0,
                    0.0, 0.0, 0.0, 1.0);
    }

    class TransformApp : public OpenGL_Application
    {
        public:
        TransformApp(ApplicationInfo appInfo) : OpenGL_Application(appInfo), objectShader("../res/Shaders/proj/"), textShader("../res/Shaders/Text/")
        {
            targetText = new FreetypeText(displayText);
            targetText->SetAligment(Interface::TextAlignment::CENTER);
            targetText->SetTarget((GLFWwindow*)m_window->GetWindow()); 
            objectShader.CreateProgram();
            textShader.CreateProgram();
        }

        void Initialize() override
        {
            std::cout << "Transformation Application instance created ! " << std::endl;

            float *tempVert;
            Color3 mainColor(1, 0.5, 0.2);
            int shape = 1;
            pos = fVec2(0.5, 0.5);
            dim = fVec2(1, 1);

            // int shape = Util::get_title_input<int>("Enter the shape construct\n1. For Rectangle\n2. For Triangle\n");
            #ifdef INPUT_ENABLE
            if (shape == 1)
            {
                pos = Util::get_2dpoint("Enter position of Rectangle");
                dim = Util::get_2dpoint("Enter Dim of Rectangle");
            }
            else
            {
                pos = Util::get_2dpoint("Enter position of Triangle");
                dim = Util::get_2dpoint("Enter Dim of Triangle");
            }
            translation = Util::get_2dpoint("Enter the translation to perform : ");
            scaleFactor = Util::get_2dpoint("Enter the scale : ");
            rotAngle = Util::get_title_input<float>("Enter the rotation radian : ");
            #else
            translation = fVec2(-0.5, -0.5);
            scaleFactor = fVec3(0.5);
            rotAngle = 3.1415/2.5;
            #endif

            if(shape == 1)
            {
                tempVert = get_quad_buffer(pos, dim, mainColor);

                for(int i=0;i<QUAD_VERTEX_COUNT;i++)
                {
                    plotPoints.push_back(*(tempVert+i));
                }
            }
            else if(shape == 2)
            {
                tempVert = get_triangle_buffer(pos, dim, mainColor);

                for(int i=0;i<TRIANGLE_VERTEX_COUNT;i++)
                {
                    plotPoints.push_back(*(tempVert+i));
                }
            }

            print_point2d(plotPoints.data(), plotPoints.size());
            VAO.SetBufferLayout(BufferLayout::PPP_RGB);
            VAO.Bind();
            VBO.PushData(plotPoints.data(), plotPoints.size());
            VBO.OffloadData();
            VAO.SetVertexAttributePointer();
        }

        void Loop() override
        {
            std::cout << "====================================================================================" << std::endl;
            std::cout << "--Position--" << std::endl;
            std::cout << pos.x << ", " << pos.y << std::endl;
            std::cout << "--Translation--" << std::endl;
            std::cout << translation.x << ", " << translation.y << std::endl;
            std::cout << "--Rotation--" << std::endl;
            std::cout << rotAngle<< std::endl;
            std::cout << "--Scale--" << std::endl;
            std::cout << scaleFactor.x << ", " << scaleFactor.y << std::endl;
            std::cout << "====================================================================================" << std::endl;

            Mat4 translationMat, rotationMat, scaleMat, totalMat;
            translationMat = _get_mat4_translation(translation);
            rotationMat = _get_mat4_rotation(rotAngle);
            scaleMat = _get_mat4_scale(scaleFactor);

            if(pos.x != 0 || pos.y !=0)
            {
                totalMat = _get_mat4_rotation(rotAngle);
                totalMat *= _get_mat4_scale(scaleFactor);
                totalMat *= _get_mat4_translation(translation);
            }
            else
            {
                totalMat = scaleMat * rotationMat * translationMat;
            }

            while(!m_window->ShouldCloseWindow())
            {
                m_window->SetColor(1, 1, 1, 1.0);
                objectShader.UseProgram();
                objectShader.SetUniformMat4("proj", totalMat);

                VAO.Bind();
                glDrawArrays(GL_TRIANGLES, 0, plotPoints.size()/2);
                targetText->RenderText(textShader, pos.x, pos.y, FONT_SCALE, Color3(1, 1, 0), true);
        
                m_window->SwapFrameBuffer();
                glfwPollEvents();
            }
        }

        protected:
        // required variable for application
        fVec2 translation, rotation, scale, pos, dim, scaleFactor;
        float rotAngle;

        std::vector<float> plotPoints;
        VertexArrayObject VAO;
        VertexBufferObject VBO;
        Shader objectShader;
        Shader textShader;
        FreetypeText *targetText;
        String displayText;
    };

    TransformApp* export_app_instance()
    {
        TransformApp *temp = new TransformApp(appInfo);
        return temp;
    }
}

#define EXPORT_ENABLED