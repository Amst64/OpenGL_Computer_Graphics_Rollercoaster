#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/Cube.hpp>
#include <glimac/FreeflyCamera.hpp> // Class implemented by Askar SEYADOUMOUGAMMADOU during TP
#include <glimac/Mesh.hpp>
#include <glimac/Image.hpp>
#include <glimac/Texture.hpp>

int window_width  = 1280;
int window_height = 720;

glimac::FreeflyCamera camera;
bool mouseFirstMove = true;
float mouseLastX;
float mouseLastY;

void moveCameraWithKeyInput(GLFWwindow* window, float speed);

int loadTexture(glimac::FilePath& dir, const char* imageName, std::vector<glimac::Texture>& textures);

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


    glimac::FilePath applicationPath(argv[0]); //get application dir

    //load shaders
    glimac::Program  program = loadProgram(applicationPath.dirPath() + "Rollercoaster/shaders/3D.vs.glsl", applicationPath.dirPath() + "Rollercoaster/shaders/3D.fs.glsl");

    //load shaders
    glimac::Program  light_cube_program = loadProgram(applicationPath.dirPath() + "Rollercoaster/shaders/3D.vs.glsl", applicationPath.dirPath() + "Rollercoaster/shaders/light_cube.fs.glsl");


    glimac::Sphere sphere(1, 16, 32);
    std::vector<glimac::Texture> sphereTexture;
    if(!loadTexture(applicationPath, "EarthMap.jpg", sphereTexture))
    {
        return -1;
    }
    glimac::Mesh spehereMesh(sphere.getVertices(), sphere.getIndices(), sphereTexture);

    glimac::Cube cube;
    std::vector<glimac::Texture> cubeTexture;
    if (!loadTexture(applicationPath, "container2.png", cubeTexture)) 
    {
        return -1;
    }
    glimac::Mesh cubeMesh(cube.getVertices(), cube.getIndices(), cubeTexture);

    glm::mat4 ProjMatrix, ModelMatrix, ViewMatrix, NormalMatrix;

    ProjMatrix = glm::perspective(glm::radians(70.0f), ((float)window_width / (float)window_height), 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    float currentFrame = 0;
    float lastFrame = 0;
    float deltaTime = 0;

    float cameraSpeed = 1.5f;

    glm::vec3 lightPos(1.5f, 0, -3);
    glm::vec3 viewPos;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {

        currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        moveCameraWithKeyInput(window, cameraSpeed * deltaTime);

        ViewMatrix = camera.getViewMatrix();
        viewPos = camera.getPosition();

        lightPos = glm::vec3(1.5f, glm::cos((float)glfwGetTime()), -3);

        glUniform3fv(glGetUniformLocation(program.getGLId(), "uLightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(program.getGLId(), "uViewPos"), 1, glm::value_ptr(viewPos));

        ModelMatrix = glm::translate(glm::mat4(1), lightPos);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        NormalMatrix = glm::transpose(glm::inverse(ModelMatrix));
        cubeMesh.Draw(light_cube_program, ModelMatrix, ViewMatrix, ProjMatrix, NormalMatrix);

        ModelMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, -5));
        NormalMatrix = glm::transpose(glm::inverse(ModelMatrix));
        spehereMesh.Draw(program, ModelMatrix, ViewMatrix, ProjMatrix, NormalMatrix);

        ModelMatrix = glm::translate(glm::mat4(1), glm::vec3(3, 0, -5));
        NormalMatrix = glm::transpose(glm::inverse(ModelMatrix));
        cubeMesh.Draw(program, ModelMatrix, ViewMatrix, ProjMatrix, NormalMatrix);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

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

int loadTexture(glimac::FilePath& dir, const char* imageName, std::vector<glimac::Texture>& textures)
{
    glimac::FilePath imagePath(dir.dirPath() + "assets/textures/" + imageName);
    std::unique_ptr<glimac::Image> image = glimac::loadImage(imagePath);
    if (image == NULL) {
        std::cout << "image not found\n"
            << std::endl;
        return 0;
    }
    textures.push_back(glimac::Texture(image, "no need of type for the moment", 0, GL_RGBA));
    return 1;
}