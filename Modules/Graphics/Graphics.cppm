module;

#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <GL/gl.h>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <sstream>
#include <map>
#include "string.h"


export module Graphics;
export namespace Graphics{

class Window{
    public:
    Window(){

    }
    void init(){
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
            int code = glfwGetError(NULL);
            std::stringstream ss;
            if (code != GLFW_NO_ERROR)
                ss << "Failed to create GLFW window. GLFW Error code: " << code;
            glfwTerminate();
            throw std::runtime_error(ss.str());
        }
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw std::runtime_error("Failed to init GLAD");
        }

        glViewport(0, 0, 800, 600);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        
        while(!glfwWindowShouldClose(window))
        {
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        terminateRoutine();
    }
    
    void terminateRoutine(){
        // delete(inputCallbackManager);
        glfwTerminate();
    }

    static void glfwErrorCallbackFun(int code, const char* description)
    {
        std::stringstream ss;
        ss<< "GLFW Error code ["<<code<<"] : ";
        if (description)
            ss<<description;
        throw std::runtime_error(ss.str());
    } 
    
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
    void initInput(){
        
    }
};

class InputCallbackManager{
    private:
        std::map<int, void (*) ()> *buttonCallbacks;
        void (*mousePosCallback) (unsigned int);
        
    public:
        InputCallbackManager(){
            buttonCallbacks = new std::map<int,void (*) ()>;

        }
        ~InputCallbackManager(){
            delete(buttonCallbacks);
        }
        // Get keybinds from a config file.
        void initKeyboard(){}
        // Next input does not trigger any action, instead it rebinds that input to the action. (Safety checks should be performed to prevent soft locks)
        void rebindActionToNextInput(int i){}


};

}
