#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "shader.h"
#include "stb_image.h"
#include "cube_data.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

float win_width = 800.0f;
float win_height = 600.0f;

void window_size_changed_cb(GLFWwindow *win, int width, int height) {
    std::cout << "GLFW window size changed: "<< width
        << "x" << height << std::endl;
    win_width = width; win_height = height;
    glViewport(0, 0, width, height);
}

void processKeyInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

GLFWwindow *configGlfwWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a GLFW window */
    GLFWwindow *win = glfwCreateWindow(win_width, win_height,
            "opengl-demo", NULL, NULL);
    if (win == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); exit(-1);
    }
    glfwMakeContextCurrent(win);

    glfwSetFramebufferSizeCallback(win, window_size_changed_cb);

    return win;
}

void configCubeVAO(unsigned int *ptrVAO) {
    glGenVertexArrays(1, ptrVAO);
    glBindVertexArray(*ptrVAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    int cube_data_len = sizeof(vertices_cube);
    glBufferData(GL_ARRAY_BUFFER, cube_data_len, vertices_cube, GL_STATIC_DRAW);

    /* position & texture coords attribetes */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)12);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);   // Unbind VAO
}

void configLightVAO(unsigned int *ptrVAO) {
    glGenVertexArrays(1, ptrVAO);
    glBindVertexArray(*ptrVAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    int light_data_len = sizeof(vertices_cube);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, light_data_len, vertices_cube, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void generateTexture(int texUnitID, const char *resPath) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    glActiveTexture(GL_TEXTURE0 + texUnitID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    /* set texture wrapping/filtering options */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nchannels;

    unsigned char *data =
        stbi_load(resPath, &width, &height, &nchannels, 0);
    if (!data) {
        std::cout << "Failed to load texture: " << resPath << std::endl;
        glfwTerminate(); exit(-1);
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
            nchannels == 4 ? GL_RGBA : GL_RGB,
            GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void configCubeModelMatrix(Shader &shader) {
    static glm::vec3 rot_axis;
    static bool rot_axis_set = false;

    if (!rot_axis_set) { // init only once
        srand(1000 * glfwGetTime());
        float vecx = (rand()%5 - 2) / 2.0f;
        float vecy = (rand()%5 - 2) / 2.0f;
        float vecz = (rand()%5 - 2) / 2.0f;
        std::cout << "rotating axis: " << vecx<< ", "
            << vecy << ", " << vecz << std::endl;
        rot_axis = glm::vec3(vecx, vecy, vecz);
        rot_axis_set = (vecx != 0 || vecy != 0 || vecz != 0);
    }
    float rot_degrees = glfwGetTime() * 60.0f;

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(-0.8f, 0.0f, -3.0f));
    model = glm::rotate(model, glm::radians(rot_degrees), rot_axis);
    shader.setMat4("model", model);
}

void configLightModelMatrix(Shader &shader) {
    static glm::vec3 rot_axis;
    static bool rot_axis_set = false;

    if (!rot_axis_set) { // init only once
        srand(1000 * glfwGetTime());
        float vecx = (rand()%5 - 2) / 2.0f;
        float vecy = (rand()%5 - 2) / 2.0f;
        float vecz = (rand()%5 - 2) / 2.0f;
        std::cout << "rotating axis: " << vecx<< ", "
            << vecy << ", " << vecz << std::endl;
        rot_axis = glm::vec3(vecx, vecy, vecz);
        rot_axis_set = (vecx != 0 || vecy != 0 || vecz != 0);
    }
    float rot_degrees = glfwGetTime() * 150.0f;

    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(1.5f, 0.2f, -2.5f));
    model = glm::rotate(model, glm::radians(rot_degrees), rot_axis);
    model = glm::scale(model, glm::vec3(0.2f));
    shader.setMat4("model", model);
}

void configViewMatrix(Shader &shader) {
    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 camera_dst = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view = glm::lookAt(camera_pos, camera_dst, camera_up);
    shader.setMat4("view", view);
}

void configProjectionMatrix(Shader &shader) {
    glm::mat4 proj(1.0f);
    float win_ratio = win_width / win_height;
    proj = glm::perspective(glm::radians(45.0f), win_ratio, 0.1f, 100.0f);
    //float hmax = win_height / 400; float wmax = hmax * win_ratio;
    //proj = glm::ortho(-wmax, wmax, -hmax, hmax, 0.1f, 100.0f);
    shader.setMat4("proj", proj);
}

void drawCubeObject(unsigned int cubeVAO, Shader &shader) {
    shader.use();
    shader.setInt("ourTexture", 0);
    shader.setVec3("lightColor", glm::vec3(1.0f, 0.6f, 0.8f));

    configCubeModelMatrix(shader);
    configViewMatrix(shader);
    configProjectionMatrix(shader);

    glBindVertexArray(cubeVAO);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void drawLightObject(unsigned int lightVAO, Shader &shader) {
    shader.use();
    shader.setVec3("lightColor", glm::vec3(1.0f, 0.2f, 0.6f));

    configLightModelMatrix(shader);
    configViewMatrix(shader);
    configProjectionMatrix(shader);

    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

int main() {
    GLFWwindow *window = configGlfwWindow();

    /* Loading OpenGL functions */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate(); exit(-1);
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, win_width, win_height);

    unsigned int cubeVAO, lightVAO;

    configCubeVAO(&cubeVAO);
    configLightVAO(&lightVAO);

    Shader cubeShader("src/cube_02.vs", "src/cube_02.fs");
    Shader lightShader("src/light_01.vs", "src/light_01.fs");

    generateTexture(0, "res/moting.jpg");

    while (!glfwWindowShouldClose(window)) {

        processKeyInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawCubeObject(cubeVAO, cubeShader);
        drawLightObject(lightVAO, lightShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
