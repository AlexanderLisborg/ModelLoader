
#include <exception>
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <GL/gl.h>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include "string.h"

import Debug;
import Common;
import Graphics;

#include <DebugMacros.hpp>
#ifndef SRCPATH
#define SRCPATH "null"
#endif

void terminateRoutine();

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
    
    Graphics::Window window;
    try{
        window.init();
    } catch(std::runtime_error& err){
        logger->write(Debug::ERROR,MACRO_STAT,err.what(),strlen(err.what())-1);
    }
}

void terminateRoutine(){
    try{delete(logger);}
    catch(std::exception){std::printf("Error: Failed termination of logger. Logs were not generated correctly.");}
    glfwTerminate();
}
