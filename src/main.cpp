
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

int g_windowSizeX = 640;
int g_windowSizeY = 480;

/* ����������� ���� ��� ��������� �������� */
void glfwWindowSizeCallback(GLFWwindow* pwindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);             // ����� ��� ������ � ���� (���������� ������ ������� ����) � �������
}

/* ��������� ������� ������ */
void glfwKeyCallback(GLFWwindow* pwindow, int key, int scancode, int action, int mode)
{
    /* ������� �� ESC ������� �� ��������� */
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pwindow, GL_TRUE);
    }
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }
    /* ������������� ���������� ���������� ������ OpenGL (� ������ ������ 4.6) */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                          
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);          // ������������� core profile ��� openGL

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pwindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "BattleCity", nullptr, nullptr);
    if (!pwindow)
    {
        std::cout << "glfwTerminate failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* ������������ ������� ��������� ������ ��� ����������� ���� ��� ��������� �������� */
    glfwSetWindowSizeCallback(pwindow, glfwWindowSizeCallback);

    /* ������������ callback ������� �������� ���� */
    glfwSetKeyCallback(pwindow, glfwKeyCallback);

    /* Make the window's context current (���� � �������� ���������� openGL) */ 
    glfwMakeContextCurrent(pwindow);

    if (!gladLoadGL())      // �������������� ���������� GLAD
    {
        std::cout << "Can't load GLAD" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL_version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    std::cout << "OpenGL: " << GLVersion.major << "." << GLVersion.minor << std::endl;

    glClearColor(1, 1, 0, 1);

    /* Loop until the user closes the window (������� ���� ���������, ����������� ���� ���� �� ����� �������) */
    while (!glfwWindowShouldClose(pwindow))
    {
        /* Render here (������� ����� �����)*/
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers (������ ������ ������� ��� ������� �����������) */
        glfwSwapBuffers(pwindow);

        /* Poll for and process events (��������� ������� (������� ������, ���� �.�.�))*/
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
