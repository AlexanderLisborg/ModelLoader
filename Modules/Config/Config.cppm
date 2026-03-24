module;

#include <stdexcept>
#include <ios>
#include <fstream>
#include <sstream>

export module Config;
import Common;

export namespace Config{

// Singleton ? Mayb not since it has a definite place it will be initialized
class Instance{

};

class FileHandler {
    private:
    const char* confDirRelPath;
    const char* confFileName;
    const unsigned int bufferSize = 4106;
    char *buffer;
    int bufferIndex = 0; 
    Common::Obvc<int> *integers;
    Common::Obbc *booleans;
    Common::Obvc<std::string> *strings;


    public:
    FileHandler(Instance i, const char *confDirRelPath, const char *confFileName){
    buffer = new char[bufferSize];
    for(int i = 0 ; i < bufferSize ; i++){
        *(buffer + i)='\0';
    }
    
    }
    ~FileHandler(){delete(buffer);}
    
    void ReadNextLine(){
            }

    void ReadFile(const char *confDirRelPath, const char *confFileName){
        std::stringstream ss;
        ss << confDirRelPath << "/" << confFileName;
        std::ifstream file(ss.str());
        char *tempbuffer;
        // states:
        // 0 : Before ':'
        // 1 ... 100 : After ':'
        // 101 : Error value too long
        // 102 : Done
        char state;
        while(state != 102){
            switch(state){
                char c;
                case 0:
                    file.get(c);
                    if(c==':'){
                        state = 1;
                    }
                    break;
                case 1 ... 100:
                    file.get(c);
                    if(c=='\n')
                        // tempbuffer -> interpret
                        state = 0;
                    else {
                        *(tempbuffer++) = c;
                        state++;
                    }
                    break;
                case 101:
                    throw std::runtime_error("Config value larger than 100 characters, format: \"option : value\" where value is under 100 (8bit ascii) characters");
            }
            if( std::ios_base::iostate::_S_failbit){
                    throw std::runtime_error("Failed to read config file.");
            } else if (std::ios_base::iostate::_S_eofbit){
                 state=2;
            }
        }
    }
    void WriteFile(){
    
    }
        
};

}
