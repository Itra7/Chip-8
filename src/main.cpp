#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <unistd.h>
#include "shader.h"
#include "Chip8.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void printScreen(Chip8& chip8);
void processInput(GLFWwindow* window);

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
Chip8 chip8;


int main(int argc,char *argv[]){

    if(argc != 2){
      std::cout << "Chip8::main: Wrong number of arguments" << std::endl;
      return 0;
    }
    try{
        chip8.LoadRom(std::string(argv[1]));
    }catch(std::invalid_argument& e){
         std::cout << e.what() << std::endl;
         return 1;
    }   

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,600, "Chip-8",NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }   
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  

    Shader shader("shaders/vertexshader.vs", "shaders/fragmentshader.fs");

    float vertices[]={
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER,0);

    glBindVertexArray(0);
    glm::mat4 projection = glm::ortho(0, 800, 600, 0, -100, 1000);


    while(!glfwWindowShouldClose(window)){

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        printScreen(chip8);
        processInput(window);
        // draw our first triangle
        shader.use();
        glm::mat4 view = glm::mat4(1.0f);
        shader.setMat4("view", view);
        // TODO: calculate formula for area of each rectangular and scale it
        shader.setMat4("projection", projection);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        if(chip8.drawflag){
        for(uint8_t col = 0; col < 32; col++){
            for(uint8_t row = 0; row < 64; row++){
                if(chip8.video[(col*64)+row] == 0x1){ // A LOT OF PROBLEMS  IMPLEMENT bool draw;
                    glm::mat4 model = glm::mat4(1.0f);
                    float scale_x, scale_y;
                    scale_x = -0.99 + (0.0310 * row);
                    scale_y = 0.99 - (0.0320 * col * 2);
                    //std::cout << "scale_x: " << scale_x << " scale_y: " << scale_y << std::endl;
                    model = glm::translate(model, glm::vec3(scale_x,scale_y, 0.0f));
                    model = glm::scale(model, glm::vec3(0.03325f,0.0483f,0.0f));
                    shader.setMat4("model", model); 
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
                }
            }
        }
        }

        
        // for(uint8_t col = 0; col < 32; col++){
        //     for(uint8_t row = 0; row < 64; row++){
        //         std::cout << static_cast<int>(chip8.video[(col*64)+row]);

        //     }
        //     std::cout << std::endl;
        // }
        //std::cout << "--------------" << std::endl;
        //  sleep(1);
        glfwSwapBuffers(window);
        glfwPollEvents();

        chip8.Cycle();
        
    }

    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}  

void printScreen(Chip8& chip8){
    for(int i = 0; i < 64*32; i++){
        if(chip8.video[i] == 0x1){
            
        }
    }

}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS){
        chip8.setKey(0);
    }else{
       chip8.unsetKey(0);
    } 
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        chip8.setKey(1);
    }else{
       chip8.unsetKey(1);
    }
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
        chip8.setKey(2);
    }else{
       chip8.unsetKey(2);
    }
    if(glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
        chip8.setKey(3);
    }else{
       chip8.unsetKey(3);
    }
    if(glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS){
        chip8.setKey(4);
    }else{
       chip8.unsetKey(4);
    }
    if(glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS){
        chip8.setKey(5);
    }else{
       chip8.unsetKey(5);
    }
    if(glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS){
        chip8.setKey(6);
    }else{
       chip8.unsetKey(6);
    }
    if(glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS){
        chip8.setKey(7);
    }else{
       chip8.unsetKey(7);
    }
    if(glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS){
        chip8.setKey(8);
    }else{
       chip8.unsetKey(8);
    }
    if(glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS){
        chip8.setKey(9);
    }else{
       chip8.unsetKey(9);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        chip8.setKey(10);
    }else{
       chip8.unsetKey(10);
    }
    if(glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS){
        chip8.setKey(11);
    }else{
       chip8.unsetKey(11);
    }
    if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
        chip8.setKey(12);
    }else{
       chip8.unsetKey(12);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        chip8.setKey(13);
    }else{
       chip8.unsetKey(13);
    }
    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        chip8.setKey(14);
    }else{
       chip8.unsetKey(14);
    }
    if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        chip8.setKey(15);
    }else{
       chip8.unsetKey(15);
    }
}