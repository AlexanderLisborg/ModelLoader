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
    // TODO: fix this method so it follows the format below...
    // "name : value"
    // where value is one of {true,false,number,string}
    // (Make a DFA out of it and then implement accordingly)
    void ReadFile(const char *confDirRelPath, const char *confFileName){
        std::stringstream ss;
        ss << confDirRelPath << "/" << confFileName;
        std::ifstream file(ss.str());
        char *tempbuffer;

        // 0: pre ':'
        // 1 ... 10: after ':' NUMBER
        // 11: error long config value
        // 12: FINAL NUMBER
        // 13: t 14: r 15: u 16: e FINAL
        // 17: f 18: a 19: l 20: s 21: e FINAL
        // 22...121: after ':' CHAR
        // 122: error long config value
        // 123: FINAL CHAR

        int number = 0;
        int state;
        while(state != 102){
            switch(state){
                char c;
                case 0:
                    file.get(c);
                    if(c==':'){
                        state = 1;
                    }
                    break;
                case 1 ... 10:
                    file.get(c);
                    if(c=='\n')
                        // tempbuffer -> interpret
                        state = 12;
                    else if(c=='0'){number = (number * 10);     state++;}
                    else if(c=='1'){number = (number * 10) + 1; state++;}
                    else if(c=='2'){number = (number * 10) + 2; state++;}
                    else if(c=='3'){number = (number * 10) + 3; state++;}
                    else if(c=='4'){number = (number * 10) + 4; state++;}
                    else if(c=='5'){number = (number * 10) + 5; state++;}
                    else if(c=='6'){number = (number * 10) + 6; state++;}
                    else if(c=='7'){number = (number * 10) + 7; state++;}
                    else if(c=='8'){number = (number * 10) + 8; state++;}
                    else if(c=='9'){number = (number * 10) + 9; state++;}

                    else if(state == 1 && c=='t'){state = 13;}
                    else if(state == 1 && c=='f'){state = 17;}
                    else{state += 23;}
                    
                    break;
                case 11:
                    throw std::runtime_error("Config integer value larger than integer max. (2^32 = 2 147 483 647)");
                    break;
                case 12:
                    // number -> observable ints
                    break;
                case 13:
                    file.get(c);
                    if(c=='r'){state++;}
                    else{state=25;}
                    break;
                case 14:
                    file.get(c);
                    if(c=='u'){state++;}
                    else{state=26;}
                    break;
                case 15:                
                    file.get(c);        
                    if(c=='e'){state++;}
                    else{state=27;}     
                    break;
                case 16:
                    file.get(c);
                    if(c=='\n'){
                        // true -> observable bools
                    }
                    else{state=28;}
                    break;
                case 17:
                    file.get(c);
                    if(c=='a'){state++;}
                    else{state=25;}
                    break;
                case 18:
                    file.get(c);
                    if(c=='l'){state++;}
                    else{state=26;}
                    break;
                case 19:
                    file.get(c);
                    if(c=='s'){state++;}
                    else{state=27;}
                    break;
                case 20:
                    file.get(c);
                    if(c=='e'){state++;}
                    else{state=28;}
                    break;
                case 21:
                    file.get(c);
                    if(c=='\n'){
                        // false -> observable bools
                    }
                case 22 ... 121:
                    file.get(c);
                    if(c=='\n'){state=123;}
                    else{state++;}
                case 122:
                    throw std::runtime_error("Config value too long. max length: 100 ascii characters.");
                case 123:
                    // stringValue -> observable strings 

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
