//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <stb_image.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "skybox.h"
//
//int main()
//{
//    //init
//    glfwInit();
//    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "skybox", NULL, NULL);
//
//    glfwMakeContextCurrent(window);
//    glfwSetCursorPosCallback(window, CursorPosCallback);
//    //glfwSetScrollCallback(window, ScrollCallback);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    // load glad 
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    glEnable(GL_DEPTH_TEST);
//
//    // create a shader, parameters should be changed in the constructor
//    Shader skyboxShader;
//    skyboxShader.setShader("assets/skybox/shaders/vertexShader.vs", "assets/skybox/shaders/fragmentShader.fs");
//
//    //set vertex data
//    float Vertices[] = {
//
//        -1.0, -1.0, -1.0,
//        1.0, -1.0, -1.0,
//        1.0, -1.0, 1.0,
//        -1.0, -1.0, -1.0,
//        1.0, -1.0, 1.0,
//        -1.0, -1.0, 1.0,
//
//        -1.0, -1.0, -1.0,
//        -1.0, 1.0, -1.0,
//        1.0, 1.0, -1.0,
//        -1.0, -1.0, -1.0,
//        1.0, 1.0, -1.0,
//        1.0, -1.0, -1.0,
//
//        -1.0, -1.0, -1.0,
//        -1.0, -1.0, 1.0,
//        -1.0, 1.0, 1.0,
//        -1.0, -1.0, -1.0,
//        -1.0, 1.0, 1.0,
//        -1.0, 1.0, -1.0,
//
//        1.0, -1.0, -1.0,
//        1.0, 1.0, -1.0,
//        1.0, -1.0, 1.0,
//        1.0, 1.0, -1.0,
//        1.0, 1.0, 1.0,
//        1.0, -1.0, 1.0,
//
//        -1.0, -1.0, 1.0,
//        1.0, -1.0, 1.0,
//        -1.0, 1.0, 1.0,
//        1.0, -1.0, 1.0,
//        1.0, 1.0, 1.0,
//        -1.0, 1.0, 1.0,
//
//        -1.0, 1.0, 1.0,
//        1.0, 1.0, 1.0,
//        1.0, 1.0, -1.0,
//        -1.0, 1.0, 1.0,
//        1.0, 1.0, -1.0,
//        -1.0, 1.0, -1.0
//
//    };
//
//    //set images
//    std::string TextureNumber;
//    //TextureNumber = "assets/skybox/textures/picture/bak0";
//    TextureNumber = "assets/skybox/textures/picture/bak0";
//    std::vector<std::string> images
//    {
//        TextureNumber + "/right.jpg",
//        TextureNumber + "/left.jpg",
//        TextureNumber + "/top.jpg",
//        TextureNumber + "/down.jpg",
//        TextureNumber + "/front.jpg",
//        TextureNumber + "/back.jpg"
//    };
//    unsigned int cubemapTexture = LoadTextures(images);
//
//    unsigned int VAO;
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    unsigned int VBO;
//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//    // run time
//    while (!glfwWindowShouldClose(window))
//    {
//        //input
//        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
//            glfwSetWindowShouldClose(window, true);
//        }
//
//        // render
//        glClearColor(1.0, 0.0, 0.0, 1.0);
//        glClear(GL_COLOR_BUFFER_BIT);
//        glClear(GL_DEPTH_BUFFER_BIT);
//
//        glDisableVertexAttribArray(0);
//        glBindVertexArray(0);
//
//        // draw scene
//        glm::mat4 view;
//        glm::mat4 projection;
//        float fovy = camera.Zoom;
//        
//        view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
//        projection = glm::perspective(glm::radians(fovy), (float)(WIDTH / HEIGHT), 0.1f, 1000.0f);
//
//        // draw skybox 
//        glDepthFunc(GL_LEQUAL);
//        skyboxShader.use();
//
//        glBindVertexArray(VAO);
//        glEnableVertexAttribArray(0);
//
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
//
//        skyboxShader.setMat4("view", view);
//        skyboxShader.setMat4("projection", projection);
//
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//
//    glfwTerminate();
//    return 0;
//}