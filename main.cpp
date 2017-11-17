#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SOIL.h"
#include "Shader.h"
#include "Camera.h"
#include "Mouse.h"
#include "Light.h"
#include "Material.h"
#include "GLProgram.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void click_callback(GLFWwindow*,int,int,int);
void input_process(GLFWwindow* window);
GLuint load_texture(const char* path);
GLuint generate_buffer_texture();
void renderFloor(GLProgram *program, GLuint floorVAO);
void renderCube(GLProgram *program, GLuint cubeVAO, glm::vec3* cubePositions);
void RenderQuad();

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
Mouse* mouse;

double deltaTime = 0.0f;
double lastFrame = 0.0f;

bool Keys[1024] = {0};

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

// The MAIN function, from here we start the application and run the game loop
int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    mouse = new Mouse(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, click_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwMakeContextCurrent(window);
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Define the viewport dimensions
    //glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, WIDTH, HEIGHT);

    Shader *shader = new Shader("./shaders/vertexShader.vs", "./shaders/fragment.frag");
    Shader *lightShader = new Shader("./shaders/lightVertexShader.vs", "./shaders/lightFragment.frag");
    Shader *depthShader = new Shader("./shaders/depthVertexShader.vs", "./shaders/depthFragment.frag");
    Shader *debugShader = new Shader("./shaders/depthDebug.vs", "./shaders/depthDebug.frag");

    GLfloat vertices[] = {
            // Positions          // Normal           // Texture Coords
            0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Top Right
            0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
            -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // Top Left

            0.5f,  0.5f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,
            0.5f, -0.5f, -1.0f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
            -0.5f, -0.5f, -1.0f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
            -0.5f,  0.5f, -1.0f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,

            0.5f,  0.5f, -1.0f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
            0.5f,  0.5f, 0.0f,    0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
            -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, -1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,

            0.5f, -0.5f, 0.0f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
            0.5f, -0.5f, -1.0f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
            -0.5f, -0.5f, -1.0f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,

            0.5f,  0.5f, -1.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
            0.5f, -0.5f, -1.0f,   1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
            0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,    0.0f, 0.0f,
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,    0.0f, 1.0f,

            -0.5f,  0.5f, 0.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
            -0.5f, -0.5f, 0.0f,   -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
            -0.5f, -0.5f, -1.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
            -0.5f,  0.5f, -1.0f,   -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
    };

    GLfloat floorVertices[] = {
            5.0f, -0.5f, -5.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // Top Right
            5.0f, -0.5f,  5.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
            -5.0f, -0.5f, 5.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Bottom Left

            5.0f, -0.5f, -5.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // Top Right
            -5.0f, -0.5f, 5.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, // Bottom Left
            -5.0f, -0.5f, -5.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f, // Top Left
    };

    GLuint indices[] = {  // Note that we start from 0!
            0, 1, 3, // First Triangle
            1, 2, 3, // Second Triangle

            4, 5, 7,
            5, 6, 7,

            8, 9, 11,
            9, 10, 11,

            12, 13, 15,
            13, 14, 15,

            16, 17, 19,
            17, 18, 19,

            20, 21, 23,
            21, 22, 23
    };

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };


    GLuint VBO, lightVAO, floorVAO, VAO, EBO, floorVBO;
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &lightVAO);
    glGenVertexArrays(1, &floorVAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &floorVBO);

    //Cube VAO
    {
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // TexCoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0); // Unbind VAO
    }

    //Floor VAO
    {
        glBindVertexArray(floorVAO);

        glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
        // TexCoord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0); // Unbind VAO
    }


    // lightVAO
    {
        glBindVertexArray(lightVAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
//        // Color attribute
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
//        glEnableVertexAttribArray(1);
//        // TexCoord attribute
//        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
//        glEnableVertexAttribArray(2);

        glBindVertexArray(0); // Unbind VAO
    }

    // ====================
    // Texture 1
    // ====================
    GLuint texture1 = load_texture("container.jpg");
    GLuint floorTex = load_texture("floor.jpg");

    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    GLuint depthMap = generate_buffer_texture();
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_CULL_FACE);


    Light light;
    light.position = glm::vec3(1.0f, 2.0f, -3.0f);
    light.color = glm::vec3(1.0f);

    light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    light.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    Material material;
    material.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
    material.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
    material.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    material.shininess = 64.0f;

    GLProgram* sceneProgram = new GLProgram(shader);
    sceneProgram->Use();
    glUniform1i(sceneProgram->GetUniformLocation("texture1"), 0);
    glUniform1i(sceneProgram->GetUniformLocation("depthTexture"), 1);

    GLProgram* lightProgram = new GLProgram(lightShader);
    GLProgram* depthProgram = new GLProgram(depthShader);
    GLProgram* debugProgram = new GLProgram(debugShader);

    debugProgram->Use();
    glUniform1i(debugProgram->GetUniformLocation("depthTexture"), 0);



    glm::mat4 p = glm::perspective(glm::radians(90.0f), (float)1/(float)1, 1.0f, 2.0f);
    std::cout << p[0][0] << std::endl;
    std::cout << p[1][1] << std::endl;
    std::cout << p[2][2] << std::endl;
    std::cout << p[3][2] << std::endl;
    std::cout << p[2][3] << std::endl;

    glm::vec4 v1 = glm::vec4(1.0, 1.0, -1.0, 1.0);
    glm::vec4 v2 = glm::vec4(2.0, 2.0, -2.0, 1.0);

    v1 = p * v1;
    v2 = p * v2;

    std::cout << v1[0] << " " << v1[1] << " " << v1[2] << " " << v1[3] << " " << std::endl;
    std::cout << v2[0] << " " << v2[1] << " " << v2[2] << " " << v2[3] << " " << std::endl;


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        double curFrame = glfwGetTime();
        deltaTime = curFrame - lastFrame;
        lastFrame = curFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        input_process(window);

        glm::mat4 view;
        glm::mat4 projection;

//        glm::mat4 lightView = glm::lookAt(light.position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//        glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 1.0f, 20.0f);

        glm::mat4 lightView = glm::lookAt(light.position, glm::vec3(light.position.x, 0.0f, light.position.z), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 lightProjection = glm::perspective(glm::radians(150.0f), (float)SHADOW_WIDTH/(float)SHADOW_HEIGHT, 0.1f, 100.0f);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

//        view = glm::lookAt(light.position, glm::vec3(light.position.x, 0, light.position.z), glm::vec3(0.0f, 0.1f, 0.0f));
//        projection = glm::perspective(glm::radians(89.0f), (float)SHADOW_WIDTH/SHADOW_HEIGHT, 0.1f, 100.0f);

        depthProgram->Use();

        depthProgram->SetViewMatrix(lightView);
        depthProgram->SetProjection(lightProjection);
        renderCube(depthProgram, VAO, cubePositions);
        renderFloor(depthProgram, floorVAO);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, WIDTH, HEIGHT);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)WIDTH/HEIGHT, 0.1f, 100.0f);

        sceneProgram->Use();

        sceneProgram->SetViewMatrix(view);
        sceneProgram->SetProjection(projection);

        sceneProgram->SetMaterial(material);
        sceneProgram->SetLight(light);
        sceneProgram->SetViewPosition(camera.Position);

        glUniformMatrix4fv(sceneProgram->GetUniformLocation("lightMatrix"), 1, GL_FALSE, glm::value_ptr(lightProjection * lightView));

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        renderCube(sceneProgram, VAO, cubePositions);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTex);
        renderFloor(sceneProgram, floorVAO);

        lightProgram->Use();

        lightProgram->SetViewMatrix(view);
        lightProgram->SetProjection(projection);

        glUniform3f(lightProgram->GetUniformLocation("lightColor"), light.color.x, light.color.y, light.color.z);

        glBindVertexArray(lightVAO);
        int lightCnt = 1;
        for (int i = 0; i < lightCnt; ++i) {
            glm::mat4 model;
            model = glm::translate(model, light.position);
            model = glm::scale(model, glm::vec3(0.5));
            lightProgram->SetModelMatrix(model);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        debugProgram->Use();
        glUniform1f(debugProgram->GetUniformLocation("nearPlane"), 0.1f);
        glUniform1f(debugProgram->GetUniformLocation("farPlane"), 100.0f);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        RenderQuad();

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
        Keys[key] = true;
    else if (action == GLFW_RELEASE)
        Keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
//    mouse->ProcessMove((float)xpos, (float)ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((GLfloat)yoffset);
}

void input_process(GLFWwindow* window)
{
    mouse->Update();

    if(Keys[GLFW_KEY_ESCAPE])
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(Keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(Keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(Keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(Keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (mouse->keys[GLFW_MOUSE_BUTTON_LEFT]){
        camera.ProcessMouseMovement(mouse->xOffset, mouse->yOffset);
    }

    if (mouse->keys[GLFW_MOUSE_BUTTON_RIGHT]){
        camera.ProcessMouseTranslate(mouse->xOffset, mouse->yOffset);
    }
}

void click_callback(GLFWwindow* window, int button, int action, int mods)
{
    mouse->ProcessClick(button, action);
}

GLuint load_texture(const char* path){
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // Load, create texture and generate mipmaps
    int width, height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    return texture;
}

GLuint generate_buffer_texture(){
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
    // Set our texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // Load, create texture and generate mipmaps
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
    return texture;
}

void renderCube(GLProgram *program, GLuint cubeVAO, glm::vec3* cubePositions){
    glBindVertexArray(cubeVAO);
    int cubeCnt = 1;
    for (int i = 0; i < cubeCnt; ++i) {

        glm::mat4 model;
        model = glm::translate(model, cubePositions[i]);
        model = glm::rotate(model, glm::radians(i * 20.0f), glm::vec3(1.0f, 0.3f, 0.5f));
        program->SetModelMatrix(model);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    }
    glBindVertexArray(0);
}

void renderFloor(GLProgram *program, GLuint floorVAO){
    glBindVertexArray(floorVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}



GLuint quadVAO = 0;
GLuint quadVBO;
void RenderQuad()
{
    if (quadVAO == 0)
    {
        GLfloat quadVertices[] = {
                // Positions        // Texture Coords
                -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
                1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
                1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}