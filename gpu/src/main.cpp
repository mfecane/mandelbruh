#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>


#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

long double map(long double x,
    long double in_min,
    long double in_max,
    long double out_min,
    long double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};


float ratio = 1.0f;
glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

float zoom = 1.0f;

float offsetx = 0.0f;
float offsety = 0.0f;

double xmin = -1.0f;
double xmax = 1.0f;
double ymin = -1.0f;
double ymax = 1.0f;


void calculateMVP(GLFWwindow* window);

void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void checkMousePressed(GLFWwindow* window, double time)
{
    int state;
    int buttonPressed = -1;
    state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS) {
        buttonPressed = GLFW_MOUSE_BUTTON_LEFT;
    }
    state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    if (state == GLFW_PRESS) {
        buttonPressed = GLFW_MOUSE_BUTTON_RIGHT;
    }

    if (buttonPressed != -1)
    {
        float factor = 1.0f;
        float step = 1.0f * time;

        if (buttonPressed == GLFW_MOUSE_BUTTON_RIGHT) {
            factor += step;
        }
        if (buttonPressed == GLFW_MOUSE_BUTTON_LEFT) {
            factor -= step;
        }

        int width, height;
        glfwGetWindowSize(window, &width, &height);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        //zoom point in final coords
        xpos = map(xpos, 0, width, xmin, xmax);
        ypos = map(ypos, height, 0, ymin, ymax);

        long double offsetx = xpos * (1 - factor);
        long double offsety = ypos * (1 - factor);

        xmin = offsetx + xmin * factor;
        xmax = offsetx + xmax * factor;
        ymin = offsety + ymin * factor;
        ymax = offsety + ymax * factor;
    }
}


void calculateMVP(GLFWwindow* window)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    ratio = 1.0f * (float)height / width;
    proj = glm::ortho(-1.0f, 1.0f, -ratio, ratio, -1.0f, 1.0f);
}


void window_size_callback(GLFWwindow* window, int width, int height)
{
    calculateMVP(window);
}


static void GLClearError() 
{
    while (glGetError() != GL_NO_ERROR);
}


static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL error] (" << error << "): " << function <<
            " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}


static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if(line.find("#shader") != std::string::npos) {
            if(line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}


static unsigned int CompileShader(unsigned int type, const std::string& source) 
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id); 
        return 0;
    }

    return id;
}


static int CreateShader(const std::string& vertexShader, const std::string& fragmantShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmantShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}


int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(800, 600, "Mandelbrot", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -1.0f, -1.0f,  0.0f,
         1.0f, -1.0f,  0.0f,
         1.0f,  1.0f,  0.0f,
        -1.0f,  1.0f,  0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(float), positions, GL_STATIC_DRAW));

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    ShaderProgramSource source = ParseShader("d:/dev/mandelbruh/gpu/res/Mandelbrot.shader");

	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    GLCall(glUseProgram(shader));
    
    int location = glGetUniformLocation(shader, "u_MVP");
    int lBounds = glGetUniformLocation(shader, "u_bounds");

    calculateMVP(window);

    double previousTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        GLCall(glUseProgram(shader));

        GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, &proj[0][0]));

        GLCall(glUniform4d(lBounds, xmin, xmax, ymin, ymax));

        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        glfwSwapBuffers(window);

        double currentTime = glfwGetTime();
        checkMousePressed(window, currentTime - previousTime);
        previousTime = currentTime;

        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
