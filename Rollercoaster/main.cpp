#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glimac/FilePath.hpp>
#include <glimac/Program.hpp>

#include <glimac/Sphere.hpp>
#include <glimac/Cube.hpp>
#include <glimac/Quad.hpp>

#include <glimac/FreeflyCamera.hpp> // Class implemented by Askar SEYADOUMOUGAMMADOU during TP
#include <glimac/Mesh.hpp>
#include <glimac/Image.hpp>
#include <glimac/Texture.hpp>
#include <glimac/Spline.hpp>
#include <glimac/Track.hpp>
#include <glimac/Model.hpp>
#include <glimac/Wagon.hpp>

int window_width  = 1280;
int window_height = 720;

glimac::FreeflyCamera camera;
bool mouseFirstMove = true;
float mouseLastX;
float mouseLastY;

void moveCameraWithKeyInput(GLFWwindow* window, float speed);

bool loadTexture(glimac::FilePath& dir, const char* imageName, std::string type, std::vector<glimac::Texture>& textures, int number);

std::vector<glimac::Spline> splinesTrack1();

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
    glimac::Program  cube_program = loadProgram(applicationPath.dirPath() + "Rollercoaster/shaders/3D.vs.glsl", applicationPath.dirPath() + "Rollercoaster/shaders/basic_lighting.fs.glsl");
    //glimac::Program  sphere_program = loadProgram(applicationPath.dirPath() + "Rollercoaster/shaders/3D.vs.glsl", applicationPath.dirPath() + "Rollercoaster/shaders/basic_lighting.fs.glsl");
    glimac::Program  track1_program = loadProgram(applicationPath.dirPath() + "Rollercoaster/shaders/3D.vs.glsl", applicationPath.dirPath() + "Rollercoaster/shaders/basic_lighting.fs.glsl");
    glimac::Program  light_cube_program = loadProgram(applicationPath.dirPath() + "Rollercoaster/shaders/3D.vs.glsl", applicationPath.dirPath() + "Rollercoaster/shaders/light_cube.fs.glsl");
    glimac::Program  model_program = loadProgram(applicationPath.dirPath() + "Rollercoaster/shaders/3D.vs.glsl", applicationPath.dirPath() + "Rollercoaster/shaders/basic_lighting.fs.glsl");
    glimac::Program  ground_program = loadProgram(applicationPath.dirPath() + "Rollercoaster/shaders/3D.vs.glsl", applicationPath.dirPath() + "Rollercoaster/shaders/basic_lighting.fs.glsl");
    glimac::Program  fountain_program = loadProgram(applicationPath.dirPath() + "Rollercoaster/shaders/3D.vs.glsl", applicationPath.dirPath() + "Rollercoaster/shaders/basic_lighting.fs.glsl");
    glimac::Program  wall_program = loadProgram(applicationPath.dirPath() + "Rollercoaster/shaders/3D.vs.glsl", applicationPath.dirPath() + "Rollercoaster/shaders/basic_lighting.fs.glsl");


    //glimac::Geometry testGeo;
    


    glimac::Sphere sphere(1, 16, 32);
    std::vector<glimac::Texture> sphereTexture;
    if(!loadTexture(applicationPath, "EarthMap.jpg", "texture_diffuse", sphereTexture, 0))
    {
        return -1;
    }
    glimac::Mesh sphereMesh(sphere.getVertices(), sphere.getIndices(), sphereTexture);

    glimac::Cube cube;
    std::vector<glimac::Texture> cubeTexture;
    if (!loadTexture(applicationPath, "container2.png", "texture_diffuse", cubeTexture, 0)) 
    {
        return -1;
    }
    if (!loadTexture(applicationPath, "container2_specular.png", "texture_specular", cubeTexture, 1))
    {
        return -1;
    }
    glimac::Mesh cubeMesh(cube.getVertices(), cube.getIndices(), cubeTexture);

    glimac::Quad ground;
    std::vector<glimac::Texture> groundTexture;
    if (!loadTexture(applicationPath, "ground.png", "texture_diffuse", groundTexture, 0))
    {
        return -1;
    }
    glimac::Mesh groundMesh(ground.getVertices(), ground.getIndices(), groundTexture);

    std::vector<glimac::Spline> splines = splinesTrack1();
    glimac::Track track1(splines);
    std::vector<glimac::ShapeVertex> track1Vertices = track1.getTrackVertices();
    std::vector<uint32_t> track1Indices = track1.getTrackIndices();
    std::vector<glimac::Texture> track1Texture;
    if (!loadTexture(applicationPath, "red.png", "texture_diffuse", track1Texture, 0))
    {
        return -1;
    }
    if (!loadTexture(applicationPath, "red.png", "texture_specular", track1Texture, 1))
    {
        return -1;
    }
    glimac::Mesh track1Mesh(track1Vertices, track1Indices, track1Texture);


    glimac::Model streetLight(applicationPath.dirPath() + "assets/models/streetlight/streetlight.obj");
    glimac::Model fountain(applicationPath.dirPath() + "assets/models/Fountain/Fountain.obj");
    glimac::Model wall(applicationPath.dirPath() + "assets/models/wall/wall.obj");

    glm::mat4 ProjMatrix, ModelMatrix, ViewMatrix, MVMatrix, NormalMatrix, ModelTrackMatrix, WagonMatrix;

    ProjMatrix = glm::perspective(glm::radians(70.0f), ((float)window_width / (float)window_height), 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    float currentFrame = 0;
    float lastFrame = 0;
    float deltaTime = 0;

    float cameraSpeed = 1.5f;

    glm::vec3 cube_lightPos(1.8f - 3.6f, 0.0f, -5.5f);

    std::vector<glm::vec3> lightsPosition;

    lightsPosition.push_back(glm::vec3(1.8f, 3.0f, -6.5f));
    lightsPosition.push_back(glm::vec3(1.8f - 3.6f -5.0f, 3.0f, -6.5f));

    /* Loop until the user closes the window */
    glm::vec3 wagonSplinePos = splines[0].getPosition(0.06f);
    glm::vec3 wagonSplinePos1 = splines[0].getPosition(0.0f);
    glm::vec3 wagonPos;
    glimac::Wagon wagon1;
    glimac::Wagon wagon2;

    float ratio = 0.06f;
    int splineIndex = 0;
    while (!glfwWindowShouldClose(window)) {

        currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        moveCameraWithKeyInput(window, cameraSpeed * deltaTime);

        ViewMatrix = camera.getViewMatrix();

        //lightPos = glm::vec3(glm::sin((float)glfwGetTime()) + 3, 1, glm::cos((float)glfwGetTime()) * 2- 5);

        //std::cout << deltaTime << std::endl;

        
        
        if(ratio >= 1 && splineIndex <= splines.size())
        {
            ratio = 0;
            splineIndex += 1;
        }

        if(splineIndex == splines.size())
        {
            splineIndex = 0;
        }
        ratio += 1.3f * deltaTime;
        

        ModelTrackMatrix = glm::translate(glm::mat4(1), glm::vec3(5, -0.7f, -5));
        MVMatrix = ViewMatrix * ModelTrackMatrix;
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        track1Mesh.SetMatrix(track1_program, ModelTrackMatrix, ViewMatrix, ProjMatrix, NormalMatrix, lightsPosition);
        track1Mesh.Draw(track1_program, 32);

        //Wagon
        ModelMatrix = glm::translate(ModelTrackMatrix, wagonSplinePos + splines[splineIndex].GetCurveNormal(ratio) * 0.5f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
        wagonPos = glm::vec3(ModelMatrix[3][0], ModelMatrix[3][1], ModelMatrix[3][2] + 2);
        wagon1.move(wagonPos, splines[splineIndex].GetCurveTangent(ratio), splines[splineIndex].GetCurveNormal(ratio));
        WagonMatrix = wagon1.getModelMatrix();
        WagonMatrix[3][0] = 0;
        WagonMatrix[3][1] = 0;
        WagonMatrix[3][2] = 0;
        WagonMatrix = ModelMatrix * WagonMatrix;
        MVMatrix = ViewMatrix * WagonMatrix;
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        cubeMesh.SetMatrix(light_cube_program, WagonMatrix, ViewMatrix, ProjMatrix, NormalMatrix, lightsPosition);
        cubeMesh.Draw(light_cube_program, 0);

        ModelMatrix = glm::translate(ModelTrackMatrix, wagonSplinePos1 + splines[splineIndex].GetCurveNormal(ratio) * 0.5f);
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
        wagonPos = glm::vec3(ModelMatrix[3][0], ModelMatrix[3][1], ModelMatrix[3][2] + 2);
        wagon2.move(wagonPos, splines[splineIndex].GetCurveTangent(ratio), splines[splineIndex].GetCurveNormal(ratio));
        WagonMatrix = wagon2.getModelMatrix();
        WagonMatrix[3][0] = 0;
        WagonMatrix[3][1] = 0;
        WagonMatrix[3][2] = 0;
        WagonMatrix = ModelMatrix * WagonMatrix;
        MVMatrix = ViewMatrix * WagonMatrix;
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        cubeMesh.SetMatrix(light_cube_program, WagonMatrix, ViewMatrix, ProjMatrix, NormalMatrix, lightsPosition);
        cubeMesh.Draw(light_cube_program, 0);

        wagonSplinePos1 = splines[splineIndex].getPosition(ratio - 0.06f);
        wagonSplinePos = splines[splineIndex].getPosition(ratio);
        //

        ModelMatrix = glm::translate(glm::mat4(1), glm::vec3(3, 0, -5));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.2f, 1.2f, 1.2f));
        MVMatrix = ViewMatrix * ModelMatrix;
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        cubeMesh.SetMatrix(cube_program, ModelMatrix, ViewMatrix, ProjMatrix, NormalMatrix, lightsPosition);
        cubeMesh.Draw(cube_program, 32);
        
        glm::vec3 lampePosition = glm::vec3(2.5f, -0.7f, -0.5f);
        ModelMatrix = glm::translate(glm::mat4(1), glm::vec3(lampePosition.x, lampePosition.y, lampePosition.z - 6.0f));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        MVMatrix = ViewMatrix * ModelMatrix;
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        streetLight.SetMatrix(model_program, ModelMatrix, ViewMatrix, ProjMatrix, NormalMatrix, lightsPosition);
        streetLight.Draw(model_program, 32);

        ModelMatrix = glm::translate(glm::mat4(1), glm::vec3(lampePosition.x - 3.6f - 5.0f, lampePosition.y, lampePosition.z - 6.0f));
        ModelMatrix = glm::rotate(ModelMatrix, glm::radians(180.0f), glm::vec3(0, 1, 0));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        MVMatrix = ViewMatrix * ModelMatrix;
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        streetLight.SetMatrix(model_program, ModelMatrix, ViewMatrix, ProjMatrix, NormalMatrix, lightsPosition);
        streetLight.Draw(model_program, 32);

        ModelMatrix = glm::translate(glm::mat4(1), glm::vec3(1, -0.7f, -5));
        ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
        MVMatrix = ViewMatrix * ModelMatrix;
        NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        fountain.SetMatrix(fountain_program, ModelMatrix, ViewMatrix, ProjMatrix, NormalMatrix, lightsPosition);
        fountain.Draw(fountain_program, 32);


        for(int i = 0; i < 4; i++)
        {
            ModelMatrix = glm::translate(glm::mat4(1), glm::vec3(-i * 2.8f, -0.15f, 2));
            ModelMatrix = glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0, 1, 0));
            ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
            MVMatrix = ViewMatrix * ModelMatrix;
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            wall.SetMatrix(wall_program, ModelMatrix, ViewMatrix, ProjMatrix, NormalMatrix, lightsPosition);
            wall.Draw(wall_program, 1);
        }
        
        for(int i = 0; i < 5; i++)
        {
            ModelMatrix = glm::translate(glm::mat4(1), glm::vec3(-3 * 2.8f - 1.4f, -0.15f, 0.5f - i * 2.8f));
            ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
            MVMatrix = ViewMatrix * ModelMatrix;
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            wall.SetMatrix(wall_program, ModelMatrix, ViewMatrix, ProjMatrix, NormalMatrix, lightsPosition);
            wall.Draw(wall_program, 1);
        }

        for (int i = 0; i < 4; i++)
        {
            ModelMatrix = glm::translate(glm::mat4(1), glm::vec3(-i * 2.8f, -0.15f, 0.5f - 4 * 2.8f - 1.4f));
            ModelMatrix = glm::rotate(ModelMatrix, glm::radians(90.0f), glm::vec3(0, 1, 0));
            ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
            MVMatrix = ViewMatrix * ModelMatrix;
            NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
            wall.SetMatrix(wall_program, ModelMatrix, ViewMatrix, ProjMatrix, NormalMatrix, lightsPosition);
            wall.Draw(wall_program, 1);
        }
        

        for(int i = -15; i < 15; i++)
        {
            for(int j = -10; j < 10; j++)
            {
                ModelMatrix = glm::translate(glm::mat4(1), glm::vec3(-2.5f + j, -0.7f, 0 - i));
                //ModelMatrix = glm::scale(ModelMatrix, glm::vec3(50.0f, 15.0f, 15.0f));
                MVMatrix = ViewMatrix * ModelMatrix;
                NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
                groundMesh.SetMatrix(ground_program, ModelMatrix, ViewMatrix, ProjMatrix, NormalMatrix, lightsPosition);
                groundMesh.Draw(ground_program, 1);
            }
            
        }
        
        

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

//number is the index of texture
bool loadTexture(glimac::FilePath& dir, const char* imageName, std::string type, std::vector<glimac::Texture>& textures, int number)
{
    glimac::FilePath imagePath(dir.dirPath() + "assets/textures/" + imageName);
    std::unique_ptr<glimac::Image> image = glimac::loadImage(imagePath);
    if (image == NULL) {
        std::cout << "image not found\n"
            << std::endl;
        return false;
    }
    GLenum textureUnit = GL_TEXTURE0 + number;
    textures.push_back(glimac::Texture(image, type, textureUnit, GL_RGBA));
    return true;
}

std::vector<glimac::Spline> splinesTrack1()
{
    glm::vec3 A0(0, 0, 6);
    glm::vec3 B0(0, 0, 6);
    glm::vec3 C0(0, 0, 3);
    glm::vec3 D0(0, 0, 3);

    glm::vec3 A1(0, 0, 3);
    glm::vec3 B1(0, 0, 3);
    glm::vec3 C1(0, 0, 0);
    glm::vec3 D1(0, 0, 0);

    //<loop>
    glm::vec3 A2(0, 0, 0);
    glm::vec3 B2(0.5f, 0, -2);
    glm::vec3 C2(0.5f, 3, -2);
    glm::vec3 D2(1, 3, 0);

    glm::vec3 A3(1, 3, 0);
    glm::vec3 B3(1.5f, 3, 2);
    glm::vec3 C3(1.5f, 0, 2);
    glm::vec3 D3(2, 0, 0);
    //</loop>

    glm::vec3 A4(2, 0, 0);
    glm::vec3 B4(2, 0, -4);
    glm::vec3 C4(6, 0, -4);
    glm::vec3 D4(6, 0, 0);

    glm::vec3 A5(6, 0, 0);
    glm::vec3 B5(6, 0, 0);
    glm::vec3 C5(6, 0, 6);
    glm::vec3 D5(6, 0, 6);

    glm::vec3 A6(6, 0, 6);
    glm::vec3 B6(6, 0, 10);
    glm::vec3 C6(3, 0, 10);
    glm::vec3 D6(3, 0, 10);

    glm::vec3 A7(3, 0, 10);
    glm::vec3 B7(0, 0, 10);
    glm::vec3 C7(0, 0, 8);
    glm::vec3 D7(0, 0, 6);

    glimac::Spline spline1(A0, B0, C0, D0, false);
    glimac::Spline spline2(A1, B1, C1, D1, false);
    glimac::Spline spline3(A2, B2, C2, D2, true);
    glimac::Spline spline4(A3, B3, C3, D3, true);
    glimac::Spline spline5(A4, B4, C4, D4, false);
    glimac::Spline spline6(A5, B5, C5, D5, false);
    glimac::Spline spline7(A6, B6, C6, D6, false);
    glimac::Spline spline8(A7, B7, C7, D7, false);

    std::vector<glimac::Spline> splines;

    splines.push_back(spline1);
    splines.push_back(spline2);
    splines.push_back(spline3);
    splines.push_back(spline4);
    splines.push_back(spline5);
    splines.push_back(spline6);
    splines.push_back(spline7);
    splines.push_back(spline8);

    return splines;
}