#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/gl.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

#define PI 3.14159

/** @brief Angle variation for the x coordinate. */
constexpr static const float xDeltaAngle = 2 * PI / 90.0;

/** @brief Angle variation for the y coordinate. */
constexpr static const float yDeltaAngle = 2 * PI / 90.0;

/** @brief Angle variation for the z coordinate. */
constexpr static const float zDeltaAngle = 2 * PI / 90.0;

/** @brief GLFW window width. */
static const int s_windowWidth = 800;

/** @brief GLFW window height. */
static const int s_windowHeight = 800;

/**
 * @brief Triangle vertex positions in 4D clip-space. Each 4 entries represent a single
 * vertex.
 */
static float s_vboData[36] = {
    // clang-format off
    // Upper triangle
    0.0, 0.5, 0.0, 1.0,
    -0.25, 0.0, 0.0, 1.0,
    0.25, 0.0, 0.0, 1.0,
    // Down left triangle
    -0.25, 0.0, 0.0, 1.0,
    -0.5, -0.5, 0.0, 1.0,
    0.0, -0.5, 0.0, 1.0,
    // Down right triangle
    0.25, 0.0, 0.0, 1.0,
    0.0, -0.5, 0.0, 1.0,
    0.5, -0.5, 0.0, 1.0
    // clang-format on
};

/** @brief String representing the vertex shader. */
static const char* s_vertexShaderStr = "#version 460\n"
                                       "layout(location = 0) in vec4 pos;\n"
                                       "void main() {\n"
                                       "    gl_Position = pos;\n"
                                       "}\n";

/** @brief String representing the fragment shader. */
static const char* s_fragmentShaderStr = "#version 460\n"
                                         "out vec4 outCol;\n"
                                         "void main() {\n"
                                         "    outCol = vec4(1.0f, 0.843f, 0.0f, 1.0f);\n"
                                         "}\n";

/** @brief OpenGL program containing the vertex and fragment shader. */
static GLuint s_program;

/** @brief Vertex buffer object. */
static GLuint s_vbo;

/** @brief Vertex array object. */
static GLuint s_vao;

/**
 * @brief Given a shader type and a string containing the shader code, returns an OpenGL
 * shader object.
 */
GLuint createShader(const GLenum shaderType, const char* shaderStr) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderStr, nullptr);
    glCompileShader(shader);

    GLint status{0};
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        GLint infoLogLen{0};
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

        GLchar* infoLogStr = new GLchar[infoLogLen];
        glGetShaderInfoLog(shader, infoLogLen, nullptr, infoLogStr);

        const char* shaderTypeStr{};
        switch (shaderType) {
        case GL_VERTEX_SHADER: {
            shaderTypeStr = "vertex";
        } break;
        case GL_FRAGMENT_SHADER: {
            shaderTypeStr = "fragment";
        } break;
        default: {
            shaderTypeStr = "unknown";
        }
        }

        fprintf(
            stderr,
            "OpenGL failed to compile %s shader: %s\n",
            shaderTypeStr,
            infoLogStr);
        delete[] infoLogStr;
    }

    return shader;
}

/**
 * @brief Given an array of shader objects, attaches the shaders to `s_program` and
 * links the program.
 *
 * @param shaders Pointer to the array of shader objects to attach to the program.
 * @param numShaders Number of shader objects present in the array `shaders`.
 * @return Returns true if the linking was successful, false otherwise.
 */
bool createProgram(GLuint* shaders, size_t numShaders) {
    s_program = glCreateProgram();
    for (size_t idx = 0; idx < numShaders; idx++) {
        glAttachShader(s_program, shaders[idx]);
    }
    glLinkProgram(s_program);

    GLint isLinked{0};
    glGetProgramiv(s_program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint infoLogLen{0};
        glGetProgramiv(s_program, GL_INFO_LOG_LENGTH, &infoLogLen);
        GLchar* infoLogStr = new GLchar[infoLogLen];
        glGetProgramInfoLog(s_program, infoLogLen, nullptr, infoLogStr);
        fprintf(stderr, "OpenGL failed to link program: %s\n", infoLogStr);
        delete[] infoLogStr;
        glDeleteProgram(s_program);
        return false;
    }

    for (size_t idx = 0; idx < numShaders; idx++) {
        glDetachShader(s_program, shaders[idx]);
    }

    return true;
}

/**
 * @brief Initializes the OpenGL program object `s_program` by creating the shaders
 * from `s_vertexShaderStr` and `s_fragmentShaderStr`. If the creation of the program
 * fails, the function will toggle the GLFW window to close.
 *
 * @param window Window associated to the OpenGL context.
 */
void initShaderProgram(GLFWwindow* window) {
    GLuint shaders[2] = {
        createShader(GL_VERTEX_SHADER, s_vertexShaderStr),
        createShader(GL_FRAGMENT_SHADER, s_fragmentShaderStr)};
    if (!createProgram(shaders, 2)) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    glDeleteShader(shaders[0]);
    glDeleteShader(shaders[1]);
}

/**
 * @brief Takes the data from `s_vboData` and copies it into an OpenGL buffer
 * `s_vbo` in the GPU.
 */
void initBufferObject() {
    glGenBuffers(1, &s_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_vboData), s_vboData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void copyToVBO() {
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_vboData), s_vboData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
 * @brief Rotate the vertices in `s_vboData` by given angles.
 *
 * @param a Rotation angle for the x coordinate.
 * @param b Rotation angle for the y coordinate.
 * @param c Rotation angle for the z coordinate.
 */
void rotateVertices(float a, float b, float c) {
    for (size_t vertexIdx = 0; vertexIdx < 9; vertexIdx++) {
        size_t idx = 4 * vertexIdx;
        float x = s_vboData[idx];
        float y = s_vboData[idx + 1];
        float z = s_vboData[idx + 2];

        s_vboData[idx] = (2 * y * cos(a) + 2 * x * cos(a + b) - y * cos(a - 2 * b) +
                          2 * x * cos(a - b) - y * cos(a + 2 * b) - 2 * z * cos(a + c) +
                          2 * z * cos(a - c) - 2 * y * sin(a + c) + z * sin(a + b + c) -
                          z * sin(a - b + c) - 2 * y * sin(a - c) + z * sin(a + b - c) -
                          z * sin(a - b - c)) /
                         4;
        s_vboData[idx + 1] =
            (2 * y * cos(a + c) - z * cos(a + b + c) + z * cos(a - b + c) +
             2 * y * cos(a - c) - z * cos(a + b - c) + z * cos(a - b - c) +
             2 * x * sin(a + b) + 2 * x * sin(a - b) - 2 * z * sin(a + c) -
             y * sin(a + b + c) + y * sin(a - b + c) + 2 * z * sin(a - c) +
             y * sin(a + b - c) - y * sin(a - b - c)) /
            4;
        s_vboData[idx + 2] = (z * cos(b + c) + z * cos(b - c) - 2 * x * sin(b) +
                              y * sin(b + c) - y * sin(b - c)) /
                             2;
    }
}

/**
 * @brief Clears the display, and using the `s_program` program object and the
 * `s_vbo` buffer object, draws to the screen. This function uses double
 * buffering.
 */
void updateDisplay(GLFWwindow* window) {
    glClearColor(0.0F, 0.0F, 0.0F, 0.0F);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(s_program);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);

    rotateVertices(xDeltaAngle, yDeltaAngle, zDeltaAngle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(s_vboData), s_vboData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 9);

    glDisableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);

    glfwSwapBuffers(window);
}

/**
 * @brief If the use presses the escape key, the GLFW window is issued to close.
 */
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

/**
 * @brief Makes a viewport transformation whenever the GLFW window is resized.
 */
void resizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

/**
 * @brief Clean up the OpenGL objects when closing the window, and destroy the window.
 */
void windowCloseCallback(GLFWwindow* window) {
    printf("Closing window...");
    glDeleteProgram(s_program);
    glDeleteBuffers(1, &s_vbo);
    glDeleteVertexArrays(1, &s_vao);
    glfwDestroyWindow(window);
}

/** @brief Report GLFW errors. */
void errorCallback(int error, const char* desc) {
    fprintf(stderr, "GLFW error %d: %s\n", error, desc);
}

int main() {
    glfwSetErrorCallback(errorCallback);

    if (glfwInit() == GLFW_FALSE) {
        printf("GLFW failed to initialize...\n");
    }

    glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    GLFWwindow* window =
        glfwCreateWindow(s_windowWidth, s_windowHeight, "Triforce", nullptr, nullptr);
    if (window == nullptr) {
        printf("GLFW failed to create window...\n");
        return -1;
    }

    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        printf("Glad failed to initialize OpenGL context\n");
        return -1;
    }
    printf(
        "Loaded OpenGL version: %d.%d\n",
        GLAD_VERSION_MAJOR(version),
        GLAD_VERSION_MINOR(version));

    glfwSetWindowSizeCallback(window, resizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetWindowCloseCallback(window, windowCloseCallback);

    initShaderProgram(window);
    initBufferObject();

    glGenVertexArrays(1, &s_vao);
    glBindVertexArray(s_vao);

    while (glfwWindowShouldClose(window) == GLFW_FALSE) {
        updateDisplay(window);
        glfwPollEvents();
        usleep(6000);
    }

    glfwTerminate();
    return 0;
}
