// Implementation of different moving object

#include"./include_header.h"
#include"../Util/Utils.cpp"

namespace Extra
{
    fVec2 movement = fVec2(0, 0), lastPosition;
    float angle = 0, lastAngle;

    #define SCR_WIDTH 800
    #define SCR_HEIGHT 800
    #define POINT_SIZE 3.0
    #define FONT_SCALE 1
    #define MULTIPLER 0.03f

    ApplicationInfo appInfo(SCR_WIDTH, SCR_HEIGHT, "Moving Object");

    void static_key_callback(GLFWwindow *window, int key, int scancode, int action, int mod)
    {
        std::cout << char(key) << std::endl;
        if(key == GLFW_KEY_W)
        {
            movement = Util::normalize_vec2(fVec2(glm::sin(angle), glm::cos(angle)));
        }
        else if(key == GLFW_KEY_S)
        {
            movement = Util::normalize_vec2(fVec2(glm::sin(angle), -glm::cos(angle)));
        }
        else if(key == GLFW_KEY_A)
        {
            movement = Util::normalize_vec2(fVec2(-glm::cos(angle), glm::sin(angle)));
        }
        else if(key == GLFW_KEY_D)
        {
            movement = Util::normalize_vec2(fVec2(glm::cos(angle), glm::sin(angle)));
        }
        else if(key == GLFW_KEY_Q)
        {
            angle -= 0.05;
        }
        else if(key == GLFW_KEY_E)
        {
            angle += 0.05;
        }
    }


    class MovingObjectApp : public OpenGL_Application
    {
        public:
        MovingObjectApp(ApplicationInfo appInfo) : OpenGL_Application(appInfo), objectShader("../res/Shaders/"), textShader("../res/Shaders/Text/projbased/")
        {
            targetText = new FreetypeText(displayText);
            targetText->SetAligment(Interface::TextAlignment::CENTER);
            targetText->SetTarget((GLFWwindow*)m_window->GetWindow()); 
            objectShader.CreateProgram();
            textShader.CreateProgram();
        }

        void Initialize() override
        {
            float *tempVBO = get_quad_bufferind(fVec2(0,0), fVec2(1,1), Color3(1, 0.5, 0.2));
            uint *tempIBO = get_quad_index();

            for(int i=0;i<QUAD_IND_VERTEX_COUNT;i++)
            {
                plotPoints.push_back(*(tempVBO+i) * 0.5);
            }

            print_point2d(plotPoints.data(), plotPoints.size());
            VAO.SetBufferLayout(BufferLayout::PPP_RGB);
            VAO.Bind();
            VBO.PushData(plotPoints.data(), plotPoints.size());
            IBO.PushData(tempIBO, QUAD_INDEX_COUNT);
            IBO.OffloadData();
            VBO.OffloadData();
            VAO.SetVertexAttributePointer();

            glfwSetKeyCallback((GLFWwindow*)this->m_window->GetWindow(), static_key_callback);
        }

        void Loop() override
        {
            float temp = 0.5;
            translationMat4 = Mat4(1);

            while(!m_window->ShouldCloseWindow())
            {
                if(temp>1.4)
                {
                    temp = 0.5;
                }
                temp += 0.003;
                
                displayText = "Pos : (" + std::to_string(lastPosition.x) + ", " + std::to_string(lastPosition.y) + ") | Angle : " + std::to_string(lastAngle) + " rad";

                m_window->SetColor(1, 1, 1, 1.0);
                objectShader.UseProgram();
                translationMat4 = glm::translate(translationMat4, fVec3(movement.x * MULTIPLER, movement.y * MULTIPLER, 0));
                translationMat4 = glm::rotate(translationMat4, angle, fVec3(0, 0, 1));
                totalMat = glm::scale(translationMat4, fVec3(temp));
                objectShader.SetUniformMat4("model", totalMat);

                VAO.Bind();
                glDrawElements(GL_TRIANGLES, plotPoints.size()/2, GL_UNSIGNED_INT, 0);
                
                totalMat = glm::translate(translationMat4, fVec3((15 * sin(temp) - movement.x) * MULTIPLER, (15 * cos(temp) - movement.y) * MULTIPLER, 0));
                totalMat = glm::scale(totalMat, fVec3(1.4 - temp));
                objectShader.SetUniformMat4("model", totalMat);
                glDrawElements(GL_TRIANGLES, plotPoints.size()/2, GL_UNSIGNED_INT, 0);

                totalMat = glm::translate(translationMat4, fVec3((-10 * cos(temp) - movement.x) * MULTIPLER, (25 * sin(temp) + movement.y) * MULTIPLER, 0));
                totalMat = glm::scale(totalMat, fVec3(glm::sin(temp)));
                objectShader.SetUniformMat4("model", totalMat);
                glDrawElements(GL_TRIANGLES, plotPoints.size()/2, GL_UNSIGNED_INT, 0);

                targetText->RenderText(textShader, 0, 0.9, FONT_SCALE, Color3(1, 0, 0), false);
                lastPosition = fVec2(translationMat4[3][0], translationMat4[3][1]);
                lastAngle += angle;
                movement = fVec2(0, 0);
                angle = 0;
                m_window->SwapFrameBuffer();
                glfwPollEvents();
            }
        }

        protected:
        // required variable for application
        Mat4 translationMat4, totalMat;

        std::vector<float> plotPoints;
        VertexArrayObject VAO;
        VertexBufferObject VBO;
        IndexBufferObject IBO;
        Shader objectShader, textShader;
        FreetypeText *targetText;
        String displayText;
    };

    MovingObjectApp* export_app_instance()
    {
        MovingObjectApp *temp = new MovingObjectApp(appInfo);
        return temp;
    }
}

#define EXPORT_ENABLED