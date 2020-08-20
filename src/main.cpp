
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

/* ������ ���������� ������ */
GLfloat point[] = {
    0.0f, 0.5f,  0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f
};

/* ������ ���� ��� ������ */
GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

/* ��������� ������ */
const char* vertex_shader =
"#version 460\n"                                    // ������ ������� (460 �.�. ������ OpenGL 4.6)
"layout(location = 0) in vec3 vertex_position;"     // ������ � �������� �� ���� ������� 
"layout(location = 1) in vec3 vertex_color;"        // ������ � ����� �� ���� �������
"out vec3 color;"                                    // �������� ���������� ��� �����
"void main() {"
"color = vertex_color;"
"gl_Position = vec4(vertex_position, 1.0);"         // �������� ���������� ��� ������� ������ (����������)
"}";

/* ����������� ������ */
const char* fragment_shader =
"#version 460\n"
"in vec3 color;"                                    // ������� ��������
"out vec4 frag_color;"                              // �������� ��������
"void main() {"
"frag_color = vec4(color, 1.0);"                    // �������� �������� ���������� ����������������� �������� �������� ���������
"}";

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

    /* ������� � ������� ������, ������ ����������, OpenGL, GLSL */
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL_version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    std::cout << "OpenGL: " << GLVersion.major << "." << GLVersion.minor << std::endl;

    glClearColor(1, 1, 0, 1);

    /*================================== ���������� ������� =======================================*/

    /* �������� ������������� ���������� ������� */
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    /* ��������� ������������� ���������� ������� (vs) � ����� ������� (&vertex_shader) */
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    /* ����������� ������ */
    glCompileShader(vs);

    /* �������� ������������� ������������ ������� */
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    /* ��������� ������������� ������������ ������� (vs) � ����� ������� (&vertex_shader) */
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    /* ����������� ������ */
    glCompileShader(fs);

    /* ������� ������� (������ ��������� �������� �������� ���������� ������� � �������� ���������� ������������ �������) */

    /* ������� ��������� ��������� */
    GLuint shader_program = glCreateProgram();          
    
    /* ����������� ������� � ��������� ��������� */
    glAttachShader(shader_program, vs);                 // ���������� ��������� ������
    glAttachShader(shader_program, fs);                 // ���������� ����������� ������

    glLinkProgram(shader_program);                        // ������� ������� ����� �����

    /* ������� ������� �������� �.�. ��� ������ ������� */
    glDeleteShader(vs);
    glDeleteShader(fs);
    /*==============================================================================================*/

    /******************** �������� ���������� ����������� ��� �������� (������� � ����) � ������ ���������� ************************/
    /* ������� ��������� ����� */
    GLuint points_vbo = 0;                                                  // ���������� ��� �������������� ������� ���������� ������ (vbo)
    glGenBuffers(1, &points_vbo);                                           // ������� ����� (���� �����) � ���������� ��� ������������� � ���������� points_vbo
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);                              // ���������� ����� � ������ ��� �������
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);    // ���������� ������ �� ����������� ������ � ������ ����������


    /* ������� ����������� ����� */
    GLuint colors_vbo = 0;                                                      // ���������� ��� �������������� ������� ���������� ������ (vbo)
    glGenBuffers(1, &colors_vbo);                                               // ������� ����� (���� �����) � ���������� ��� ������������� � ���������� points_vbo
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);                                  // ���������� ����� � ������ ��� �������
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);      // ���������� ������ �� ����������� ������ � ������ ����������

    /* ������� ������ ������� ������ */
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);                                                 // ������� ������ ������
    glBindVertexArray(vao);                                                     // ���������� ������ � ������ ��� �������

    /* ���������� ������ ������ */
    glEnableVertexAttribArray(0);                                               // ���������� ������
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);                                  // ������ ������� ����� ������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);                // ��������� ������
    
    /* ���������� ������ ���������� */
    glEnableVertexAttribArray(1);                                               // ���������� ������
    glBindBuffer(GL_ARRAY_BUFFER,  colors_vbo);                                  // ������ ������� ����� ������
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);                // ��������� ������


    
    //glEnableVertexAttribArray(1);               // ���� ������

    /* Loop until the user closes the window (������� ���� ���������, ����������� ���� ���� �� ����� �������) */
    while (!glfwWindowShouldClose(pwindow))
    {
        /* Render here (������� ����� �����)*/
        glClear(GL_COLOR_BUFFER_BIT);

        /* ���������� ������ ��� ��������� */
        glUseProgram(shader_program);

        /* ���������� vao (������� ������� ������) */
        glBindVertexArray(vao);

        /* ������� ��������� �������� vao (������� ������� ������) */
        glDrawArrays(GL_TRIANGLES, 0, 3);


        /* Swap front and back buffers (������ ������ ������� ��� ������� �����������) */
        glfwSwapBuffers(pwindow);

        /* Poll for and process events (��������� ������� (������� ������, ���� �.�.�))*/
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
