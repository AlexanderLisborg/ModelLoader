
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
import Config;
import Graphics;

#include <DebugMacros.hpp>
#ifndef SRCPATH
#define SRCPATH "null"
#endif

void terminateRoutine();

Debug::Logger *logger;
Config::Instance *confInstance;

void Callback(int i, bool j){std::cout<<i << " : " << j <<  std::endl;}

int main()
{
    const char* message = "Application startup";
    const char logDirRelPath[] = "log";
    logger = new Debug::Logger(logDirRelPath,std::size(logDirRelPath));
    logger->Write(Debug::INFO,MACRO_STAT,message, strlen(message)+1);
    
    Common::Obvc<bool> obvc;
    const char confDirRelPath[] = "conf";
    confInstance = new Config::Instance(confDirRelPath,std::size(confDirRelPath)); // Throws uncaught errors on fail. (as intended)
    //confInstance->SetConf(0,10);
    //confInstance->SetConf(1,false);
    obvc.RegisterCallback(0,Callback);
    obvc.Notify(0,true);
    
    Graphics::Window window;
    try{
        window.init();
    } catch(std::runtime_error& err){
        logger->Write(Debug::ERROR,MACRO_STAT,err.what(),strlen(err.what())+1);
    }
    terminateRoutine();
}

void terminateRoutine(){
    try{delete(confInstance);}
    catch(std::exception &err){
        const char msg[] = "Failed executing destructor of config instance.";
        logger->Write(Debug::WARNING, MACRO_STAT, msg, std::size(msg));
    }
    try{delete(logger);}
    catch(std::exception &err){std::printf("Error: Failed termination of logger. Logs were not generated correctly.");}
        glfwTerminate();
}
