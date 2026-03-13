
#include <exception>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <GL/gl.h>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include "string.h"

import Debug;
import Common;

#include <DebugMacros.hpp>
#ifndef SRCPATH
#define SRCPATH "null"
#endif

void terminateRoutine();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void glfwErrorCallbackFun(int code, const char* description);

Debug::Logger *logger;

void Callback(int i, bool j){std::cout<<i << " : " << j <<  std::endl;}

int main()
{

    const char* message = "hello";
    const char* logDirRelPath = "log";
    logger = new Debug::Logger(logDirRelPath);
    logger->write(Debug::INFO,MACRO_STAT,message, 6);
    
    Common::Obvc<bool> obvc;
    obvc.RegisterCallback(0,Callback);
    obvc.Notify(0,true);
    

    glfwSetErrorCallback(glfwErrorCallbackFun);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "ModelLoader", NULL, NULL);
    
    std::cout << SRCPATH << std::endl;

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        int code = glfwGetError(NULL);
 
        if (code != GLFW_NO_ERROR)
            std::cout << "Error code: " << code << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    terminateRoutine();

    return 0;
}

void terminateRoutine(){
    try{delete(logger);}
    catch(std::exception){std::printf("Error: Failed termination of logger. Logs were not generated correctly.");}
    glfwTerminate();
}

void glfwErrorCallbackFun(int code, const char* description)
{
    std::string msg = "GLFW Error code [";
    msg.append(std::to_string(code));
    msg.append("] : ");
    if (description)
        msg.append(description);
    logger->write(Debug::ERROR,MACRO_STAT,msg.c_str(),msg.size()+1);
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
