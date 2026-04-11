
module;
#include <sstream>
#include <string>
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
    virtual void Update(LogLevel level, const Stat stat, const char *const message, unsigned int length) = 0;
    virtual ~ILoggerObserver(){}
};
// Pure subject interface
class ILoggerSubject{
public:
    virtual void RegisterObserver(ILoggerObserver* observer) = 0;
    virtual void UnRegisterObserver(ILoggerObserver* observer) = 0;
    virtual ~ILoggerSubject(){}
    virtual void Write(LogLevel level, const Stat stat, const char *const message, unsigned int length) = 0;
};


class FileLoggerObserver : public ILoggerObserver{
private:
    char *logDirRelPath;
    const char *logFileName = "Log.txt";
    const unsigned int bufferSize = 4106; 
    char *buffer;
    int bufferIndex = 0;
    bool directoryInitialized = 0;

public:
    FileLoggerObserver(const char* logDirRelPath, int lenIncl, ILoggerSubject* subject){
        this->logDirRelPath = new char[lenIncl];
        strcpy(this->logDirRelPath, logDirRelPath);
        buffer= new char[bufferSize];
        for(int i = 0; i < bufferSize ; i++){*(buffer+i) = '\0';} 
        if(!std::filesystem::exists(logDirRelPath)){ 
            try{
                if(!std::filesystem::create_directory(logDirRelPath)) {
                    const char msg[] = "Error creating log directory.";
                    subject->Write(ERROR, MACRO_STAT, msg, std::size(msg));
                } else {
                    directoryInitialized = true;
                    const char msg[] = "Log directory created.";
                    subject->Write(INFO,MACRO_STAT,msg, std::size(msg));
                }
            } 
            catch(std::filesystem::filesystem_error &err){
                std::stringstream ss; 
                ss << "std::filesystem::create_directory filesystem_error. Failed on path \"" << err.path1()<< "\". OS error code " <<err.code().value()<<". Error message \""<<err.what()<<"\"";
                subject->Write(ERROR,MACRO_STAT,ss.str().c_str(),ss.str().size()+1);} 
            catch(std::bad_alloc &err){
                const char msg[] = "std::filesystem::create_directory bad alloc.";
                subject->Write(ERROR, MACRO_STAT, msg,std::size(msg)); 
            }       
        }
        std::string msg1 = ("FileLoggerObserver initialized with buffer size: " + std::to_string((bufferSize * sizeof(char))));
        subject->Write(INFO,MACRO_STAT,msg1.c_str(),msg1.length());
        std::string msg2 = ("Large buffer size.");
        if(bufferSize>4106){subject->Write(WARNING,MACRO_STAT,msg2.c_str(),msg2.length());}
    }
    ~FileLoggerObserver(){
        WriteBufferToFile();
        delete(logDirRelPath);
        delete(buffer);
    }
// METHODS
private:    
    void WriteBufferToFile(){
        if(!directoryInitialized) return; // Do nothing if directory was not initialized...
        std::ofstream fileStream;
        std::stringstream ss;
        ss << logDirRelPath << "/" << logFileName;
        std::cout<<std::endl<< logDirRelPath<<std::endl<<logFileName<<std::endl;
        fileStream.open(ss.str());
        if(!fileStream.is_open()){throw new std::runtime_error("Failed to open log file.");}
        
        for(int j = 0 ; j < bufferIndex ; j++){
            std::cout<< *(buffer+j);
        }
        for(int i = 0 ; i < bufferIndex ; i++){
            if(!fileStream<<*(buffer+i)){throw new std::runtime_error("Failed to write to fileStream.");}
        }
        fileStream.close();
        if(fileStream.failbit)
            throw new std::runtime_error("Failed to close fileStream, data not written to disk properly.");
        bufferIndex=0;
    }
    inline void WriteToBuffer(const char* chars, char lenIncl){
        while(lenIncl>1){
            *(buffer+(bufferIndex++))= *(chars++);
            lenIncl--;
        }
    }
    inline void WriteToBuffer(const char c){
        *(buffer + (bufferIndex++)) = c; 
    }
    // Helper function used in ChangeDir(). Reallocates path and copies arg.
    inline void ReallocLogDirRelPath(const char* logDirRelPath, int lenIncl){
        directoryInitialized = true;
        delete(this->logDirRelPath); 
        this->logDirRelPath = new char[lenIncl];
        strcpy(this->logDirRelPath,logDirRelPath);
    }
    
public:
    // Try to change dir, if it dne, attempt to create it. If creation fails, keep old path and return false. Return true on successfull change.
    bool ChangeDir(const char *logDirRelPath, int lenIncl, ILoggerSubject* subject){
        if(!std::filesystem::exists(logDirRelPath)){ 
            try{
                if(!std::filesystem::create_directory(logDirRelPath)) {
                    const char msg[] = "Error creating log directory.";
                    subject->Write(ERROR,MACRO_STAT,msg,std::size(msg));
                    return false;
                } else {
                    ReallocLogDirRelPath(logDirRelPath, lenIncl); 
                    std::stringstream ss ; 
                    ss<<"Log directory created at \""<< logDirRelPath <<"\"";
                    subject->Write(INFO,MACRO_STAT,ss.str().c_str(), ss.str().length()+1);
                    return true;
                }
            } 
            catch(std::filesystem::filesystem_error &err){
                std::stringstream ss; 
                ss << "std::filesystem::create_directory filesystem_error. Failed on path \"" << err.path1()<< "\". OS error code " <<err.code().value()<<". Error message \""<<err.what()<<"\"";
                subject->Write(ERROR,MACRO_STAT,ss.str().c_str(),ss.str().size()+1); 
                return false;
            }
            catch(std::bad_alloc &err){
                const char msg[] = "std::filesystem::create_directory bad alloc.";
                subject->Write(ERROR, MACRO_STAT, msg,std::size(msg)); 
                return false;
            } 
        } else {
            ReallocLogDirRelPath(logDirRelPath, lenIncl);
            return true;
        }

    }
    void Update(LogLevel level, const Stat stat, const char *const message, unsigned int length) override{ 
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
        WriteToBuffer(levelChars,lenInclude);
        unsigned int messageIndex = 0; 
        while(*(message+(messageIndex)) != '\0'){
            *(buffer+bufferIndex) = *(message+messageIndex);
            messageIndex++;
            bufferIndex++;
            if(bufferIndex >= bufferSize-1)
                WriteBufferToFile(); // Resets bufferIndex
        }
        WriteToBuffer('{');
        for(int i = 0 ; i < stat.srcLen-1 ; i++){*(buffer+(bufferIndex++))= *(stat.src+i); }
        WriteToBuffer(" ; ", 4);
        for(int i = 0 ; i < stat.funLen-1 ; i++){*(buffer+(bufferIndex++))= *(stat.fun+i); }
        WriteToBuffer(" ; ", 4);
        for(int i = 0 ; i < stat.lineLen-1 ; i++){*(buffer+(bufferIndex++))= *(stat.line+i); }
        WriteToBuffer('}');
        WriteToBuffer('\n');
        
        *(buffer+bufferIndex++) = '\n';
    }
};

class Logger : public ILoggerSubject {
private:
    std::vector<ILoggerObserver*> observers;
    public:
    Logger(){}
    Logger(const char* logDirRelPath, int lenIncl){
        ILoggerObserver* a = new FileLoggerObserver(logDirRelPath,lenIncl,this);
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
    void Write(LogLevel level, const Stat stat, const char *const message, unsigned int length) override {
        std::cout << "Length : " << observers.size()<< std::endl;
        for( ILoggerObserver* observer : observers) {
            observer->Update(level, stat, message, length);
        }
    } 
    
    void RegisterObserver(ILoggerObserver *observer) override {
        observers.push_back(observer);
    }
    
    void UnRegisterObserver(ILoggerObserver* observer) override {
        observers.erase(std::find(observers.begin(),observers.end(),observer));
    }
};
} // END Namespace
