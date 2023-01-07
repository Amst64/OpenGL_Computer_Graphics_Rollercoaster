#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/FreeflyCamera.hpp> // Class implemented by Askar SEYADOUMOUGAMMADOU during TP

int window_width  = 1280;
int window_height = 720;

glimac::FreeflyCamera camera;
bool mouseFirstMove = true;
float mouseLastX;
float mouseLastY;

void moveCameraWithKeyInput(GLFWwindow* window, float speed);

static void key_callback(GLFWwindow* /*window*/, int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/)
{
}

static void mouse_button_callback(GLFWwindow* /*window*/, int /*button*/, int /*action*/, int /*mods*/)
{
}

static void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double /*yoffset*/)
{
}

static void cursor_position_callback(GLFWwindow* /*window*/, double xpos, double ypos)
{
    if (mouseFirstMove) {
        mouseLastX = (float)xpos;
        mouseLastY = (float)ypos;
        mouseFirstMove = false;
    }

    float xoffset = (float)xpos - mouseLastX;
    float yoffset = mouseLastY - (float)ypos;
    mouseLastX = (float)xpos;
    mouseLastY = (float)ypos;

    float sensitivity = 0.5f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.rotateLeft(-xoffset);
    camera.rotateUp(yoffset);
}

static void size_callback(GLFWwindow* /*window*/, int width, int height)
{
    window_width  = width;
    window_height = height;
}

int main(int argc, char* argv[])
{
    int a = argc;
    a++;
    /* Initialize the library */
    if (!glfwInit()) {
        return -1;
    }

    /* Create a window and its OpenGL context */
#ifdef __APPLE__
    /* We need to explicitly ask for a 3.3 context on Mac */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Rollercoaster", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Intialize glad (loads the OpenGL functions) */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    /* Hook input callbacks */
    glfwSetKeyCallback(window, &key_callback);
    glfwSetMouseButtonCallback(window, &mouse_button_callback);
    glfwSetScrollCallback(window, &scroll_callback);
    glfwSetCursorPosCallback(window, &cursor_position_callback);
    glfwSetWindowSizeCallback(window, &size_callback);

    //chargement des shaders
    glimac::FilePath applicationPath(argv[0]);
    glimac::Program  program = loadProgram(applicationPath.dirPath() + "Rollercoaster/shaders/3D.vs.glsl", applicationPath.dirPath() + "Rollercoaster/shaders/3D.fs.glsl");
    program.use();

    GLuint uMVPMatrix = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
    GLuint uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");

    glimac::Sphere sphere(1, 16, 32);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount() * sizeof(glimac::ShapeVertex), sphere.getDataPointer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const GLuint VERTEX_ATTRIB_POS = 0;
    const GLuint VERTEX_ATTRIB_NORMAL = 1;
    const GLuint VERTEX_ATTRIB_UV = 2;

    glEnableVertexAttribArray(VERTEX_ATTRIB_POS);
    glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTRIB_UV);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (GLvoid*)offsetof(glimac::ShapeVertex, position));
    glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(VERTEX_ATTRIB_UV, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 ProjMatrix, ModelMatrix, ViewMatrix, NormalMatrix;

    ProjMatrix = glm::perspective(glm::radians(70.0f), ((float)window_width / (float)window_height), 0.1f, 100.0f);
    ModelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
    NormalMatrix = glm::transpose(glm::inverse(ModelMatrix));

    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glEnable(GL_DEPTH_TEST);

    float currentFrame = 0;
    float lastFrame = 0;
    float deltaTime = 0;

    float cameraSpeed = 1.5f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(1.000f, 0.992f, 0.735f, 1.000f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        moveCameraWithKeyInput(window, cameraSpeed * deltaTime);

        ViewMatrix = camera.getViewMatrix();
        glm::mat4 MVPMatrix = ProjMatrix * ViewMatrix * ModelMatrix;

        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(MVPMatrix));

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    return 0;
}

void moveCameraWithKeyInput(GLFWwindow* window, float speed)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveFront(speed);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveFront(-speed);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveLeft(speed);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveLeft(-speed);
}