
module;
#include <stdlib.h>
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <DebugMacros.hpp>

export module Debug;
export namespace Debug{

// Static debug info. Specifies static info about the source of debug messages.
// Use macros __FILE__, __func__, __LINE__ when initializing the struct.

struct Stat{
    const char *const src;
    const unsigned char srcLen; // Max src len = 256 characters... (May be problematic with unreasonably long subfolder chains. Should be fine with reasonable folder structure)
    const char *const fun;
    const unsigned char funLen; // Max fun len = 256 characters... (should be no prob)
    const char *const line;
    const unsigned char lineLen; // Max line len = 256 characters... (should be no prob)
};

enum LogLevel{
    INFO,
    WARNING,
    ERROR
};

// Pure observer interface
class ILoggerObserver{
public:
    virtual void update(LogLevel level, const Stat stat, const char *const message, unsigned int length) = 0;
    virtual ~ILoggerObserver(){}
};
// Pure subject interface
class ILoggerSubject{
public:
    virtual void registerObserver(ILoggerObserver* observer) = 0;
    virtual void unRegisterObserver(ILoggerObserver* observer) = 0;
    virtual ~ILoggerSubject(){}
    virtual void write(LogLevel level, const Stat stat, const char *const message, unsigned int length) = 0;
};


class FileLoggerObserver : public ILoggerObserver{
private:
    const char *logDirRelPath = "log";
    const char *logFileName = "Log.txt";
    const unsigned int bufferSize = 4106; 
    char *buffer;
    int bufferIndex = 0;

public:
    FileLoggerObserver(const char* logDirRelPath, ILoggerSubject* subject){
        buffer= new char[bufferSize];
        for(int i = 0; i < bufferSize ; i++){*(buffer+i) = '\0';} 
        if(!std::filesystem::exists(logDirRelPath)){
            std::filesystem::create_directory(logDirRelPath);
            char msg0[] = "Log directory created...";
            subject->write(INFO,MACRO_STAT,msg0, std::size(msg0));
        }
        std::string msg1 = (std::string(__func__) + "FileLoggerObserver initialized with buffer size: " + std::to_string((bufferSize * sizeof(char))));
        subject->write(INFO,MACRO_STAT,msg1.c_str(),msg1.length());
        std::string msg2 = ("Large buffer size.");
        if(bufferSize>4106){subject->write(WARNING,MACRO_STAT,msg2.c_str(),msg2.length());}
    }
    ~FileLoggerObserver(){
        writeBufferToFile();
        delete(buffer);
    }
// METHODS
private:    
    void writeBufferToFile(){
        std::cout<<"Writing to file..."<<std::endl;
        std::ofstream fileStream;
        std::stringstream ss;
        ss << logDirRelPath << "/" << logFileName;
        std::cout<<std::endl<< logDirRelPath<<std::endl<<logFileName<<std::endl;
        fileStream.open(ss.str());
        for(int j = 0 ; j < bufferIndex ; j++){
            std::cout<< *(buffer+j);
        }
        for(int i = 0 ; i < bufferIndex ; i++){
            fileStream<<*(buffer+i);
        }
        fileStream.close();
        bufferIndex=0;
    }
    inline void writeToBuffer(const char* chars, char lenInclude){
        while(lenInclude>1){
            *(buffer+(bufferIndex++))= *(chars++);
            lenInclude--;
        }
    }
    inline void writeToBuffer(const char c){
        *(buffer + (bufferIndex++)) = c; 
    }
    
public:
    void update(LogLevel level, const Stat stat, const char *const message, unsigned int length) override{ 
        // Find end of string char
        if( (*(message + length-1)!='\0'))
            throw std::out_of_range("Incorrect message length.");
        const char* levelChars;
        char lenInclude;
        switch(level){
            case INFO: levelChars = "INFO: "; lenInclude = 7;
            break;
            case WARNING: levelChars = "WARNING: "; lenInclude = 10;
            break;
            case ERROR: levelChars = "ERROR: "; lenInclude = 8;
            break;
            default: throw std::invalid_argument("Invalid LogLevel arg.");
        }
        writeToBuffer(levelChars,lenInclude);
        unsigned int messageIndex = 0; 
        while(*(message+(messageIndex)) != '\0'){
            *(buffer+bufferIndex) = *(message+messageIndex);
            messageIndex++;
            bufferIndex++;
            if(bufferIndex >= bufferSize-1)
                writeBufferToFile(); // Resets bufferIndex
        }
        writeToBuffer('{');
        for(int i = 0 ; i < stat.srcLen-1 ; i++){*(buffer+(bufferIndex++))= *(stat.src+i); }
        writeToBuffer(" ; ", 4);
        for(int i = 0 ; i < stat.funLen-1 ; i++){*(buffer+(bufferIndex++))= *(stat.fun+i); }
        writeToBuffer(" ; ", 4);
        for(int i = 0 ; i < stat.lineLen-1 ; i++){*(buffer+(bufferIndex++))= *(stat.line+i); }
        writeToBuffer('}');
        writeToBuffer('\n');
        
        *(buffer+bufferIndex++) = '\n';
    }
};

class Logger : public ILoggerSubject {
private:
    std::vector<ILoggerObserver*> observers;
    public:
    Logger(){}
    Logger(const char* logDirRelPath){
        ILoggerObserver* a = new FileLoggerObserver(logDirRelPath,this);
        observers.push_back(a);
    }
    ~Logger(){
        // Delete all observers in vector
        while(!observers.empty()){ 
            ILoggerObserver* temp = observers.back();
            observers.pop_back();
            delete(temp);
        }
    }

// Interface implementations
public: 
    void write(LogLevel level, const Stat stat, const char *const message, unsigned int length) override {
        std::cout << "Length : " << observers.size()<< std::endl;
        for( ILoggerObserver* observer : observers) {
            observer->update(level, stat, message, length);
        }
    } 
    
    void registerObserver(ILoggerObserver *observer) override {
        observers.push_back(observer);
    }
    
    void unRegisterObserver(ILoggerObserver* observer) override {
        observers.erase(std::find(observers.begin(),observers.end(),observer));
    }
};
} // END Namespace
