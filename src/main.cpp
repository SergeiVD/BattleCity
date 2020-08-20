
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

/* Задаем координаты вершин */
GLfloat point[] = {
    0.0f, 0.5f,  0.0f,
    0.5f, -0.5f, 0.0f,
   -0.5f, -0.5f, 0.0f
};

/* Задаем цвет для вершин */
GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

/* Вершинный шейдер */
const char* vertex_shader =
"#version 460\n"                                    // Версия шейдера (460 т.к. версия OpenGL 4.6)
"layout(location = 0) in vec3 vertex_position;"     // Данные о вершинах на вход шейдера 
"layout(location = 1) in vec3 vertex_color;"        // Данные о цвете на вход шейдера
"out vec3 color;"                                    // Выходная переменная для цвета
"void main() {"
"color = vertex_color;"
"gl_Position = vec4(vertex_position, 1.0);"         // Выходная переменная для позиций вершин (встроенная)
"}";

/* Фрагментный шейдер */
const char* fragment_shader =
"#version 460\n"
"in vec3 color;"                                    // Входной параметр
"out vec4 frag_color;"                              // Выходной параметр
"void main() {"
"frag_color = vec4(color, 1.0);"                    // Передаем выходной переменной интерполированное значение входного параметра
"}";

int g_windowSizeX = 640;
int g_windowSizeY = 480;

/* Перерисовка окна при изменении размеров */
void glfwWindowSizeCallback(GLFWwindow* pwindow, int width, int height)
{
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);             // место где рисуем в окне (координаты левого нижнего угла) и размеры
}

/* Обработка нажатия клавиш */
void glfwKeyCallback(GLFWwindow* pwindow, int key, int scancode, int action, int mode)
{
    /* Нажатие на ESC выходит из программы */
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
    /* Устанавливаем минимально допустимую версию OpenGL (в данном случае 4.6) */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);                          
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);          // Использование core profile для openGL
    
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pwindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "BattleCity", nullptr, nullptr);
    if (!pwindow)
    {
        std::cout << "glfwTerminate failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* регистрируем функцию обратного вызова для перерисовки окно при изменении размеров */
    glfwSetWindowSizeCallback(pwindow, glfwWindowSizeCallback);

    /* регистрируем callback функцию закрытия окна */
    glfwSetKeyCallback(pwindow, glfwKeyCallback);

    /* Make the window's context current (окно к которому применится openGL) */ 
    glfwMakeContextCurrent(pwindow);

    if (!gladLoadGL())      // Инициализируем библиотеку GLAD
    {
        std::cout << "Can't load GLAD" << std::endl;
        return -1;
    }

    /* Выведем в консоль версии, модель видеокарты, OpenGL, GLSL */
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL_version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    std::cout << "OpenGL: " << GLVersion.major << "." << GLVersion.minor << std::endl;

    glClearColor(1, 1, 0, 1);

    /*================================== Подключаем шейдеры =======================================*/

    /* Получаем идентификатор вершинного шейдера */
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    /* Связываем идентификатор вершинного шейдера (vs) с кодом шейдера (&vertex_shader) */
    glShaderSource(vs, 1, &vertex_shader, nullptr);
    /* Компилируем шейдер */
    glCompileShader(vs);

    /* Получаем идентификатор фрагментного шейдера */
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    /* Связываем идентификатор фрагментного шейдера (vs) с кодом шейдера (&vertex_shader) */
    glShaderSource(fs, 1, &fragment_shader, nullptr);
    /* Компилируем шейдер */
    glCompileShader(fs);

    /* Линкуем шейдеры (тоесть связываем выходные значения вершинного шейдера с входными значениями фрагментного шейдера) */

    /* Создаем шейдерную программу */
    GLuint shader_program = glCreateProgram();          
    
    /* Прикрепляем шейдеры к шейдерной программе */
    glAttachShader(shader_program, vs);                 // Прикрепили вершинный шейдер
    glAttachShader(shader_program, fs);                 // Прикрепили фрагментный шейдер

    glLinkProgram(shader_program);                        // Линкуем шейдеры между собой

    /* Удаляем объекты шейдеров т.к. они больше ненужны */
    glDeleteShader(vs);
    glDeleteShader(fs);
    /*==============================================================================================*/

    /******************** Передаем информацию необходимую для шейдеров (позицию и цвет) в память видеокарты ************************/
    /* Создаем вершинный буфер */
    GLuint points_vbo = 0;                                                  // Переменная для идентефикатора объекта вершинного буфера (vbo)
    glGenBuffers(1, &points_vbo);                                           // Создаем буфер (один буфер) и записываем его идентификатор в переменную points_vbo
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);                              // Подключаем буфер и делаем его текущим
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);    // Перемещаем данные из оперативной памяти в память видеокарты


    /* Создаем фрагментный буфер */
    GLuint colors_vbo = 0;                                                      // Переменная для идентефикатора объекта вершинного буфера (vbo)
    glGenBuffers(1, &colors_vbo);                                               // Создаем буфер (один буфер) и записываем его идентификатор в переменную points_vbo
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);                                  // Подключаем буфер и делаем его текущим
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);      // Перемещаем данные из оперативной памяти в память видеокарты

    /* Создаем объект массива вершин */
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);                                                 // Создаем массив вершин
    glBindVertexArray(vao);                                                     // Подключаем массив и делаем его текущим

    /* Активируем массив вершин */
    glEnableVertexAttribArray(0);                                               // Координаты вершин
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);                                  // Делаем текущим буфер вершин
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);                // Связываем данные
    
    /* Активируем массив фрагментов */
    glEnableVertexAttribArray(1);                                               // Координаты вершин
    glBindBuffer(GL_ARRAY_BUFFER,  colors_vbo);                                  // Делаем текущим буфер вершин
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);                // Связываем данные


    
    //glEnableVertexAttribArray(1);               // Цвет вершин

    /* Loop until the user closes the window (главный цикл отрисовки, продолжаетс пока окно не будет закрыто) */
    while (!glfwWindowShouldClose(pwindow))
    {
        /* Render here (очищаем буфер цвета)*/
        glClear(GL_COLOR_BUFFER_BIT);

        /* Подключаем шейдер для рисования */
        glUseProgram(shader_program);

        /* Подключаем vao (вертекс атрибут объетк) */
        glBindVertexArray(vao);

        /* Команда отрисовки текущего vao (вертекс атрибут объект) */
        glDrawArrays(GL_TRIANGLES, 0, 3);


        /* Swap front and back buffers (меняем буферы местами при двойной буферизации) */
        glfwSwapBuffers(pwindow);

        /* Poll for and process events (обработка событий (нажатие клавиш, мышь и.и.д))*/
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
