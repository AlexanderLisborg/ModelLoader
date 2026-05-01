#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <GL/gl.h>
#include <iostream>
#include <string.h>

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
    obvc.Notify(10,true);
    obvc.Notify(100,false);
    obvc.Notify(5,true);
    std::cout << "get start \n";
    for(Common::Obvc<bool>::Iterator iter = obvc.Begin(); (iter != obvc.End()) ; ++iter){
        std::cout << iter.Get().first<< "\n";
    }
    Common::Obvc<bool>::Iterator it = obvc.Begin();
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
