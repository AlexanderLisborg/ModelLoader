module;

#include <vector>
#include <cmath>
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
    const char *confDirRelPath;
    const char *confFileName;
    const unsigned int bufferSize = 4106;
    int bufferIndex = 0; 
    Common::Obvc<int> *integers;
    Common::Obbc *booleans;
    Common::Obvc<std::string> *strings;

    public:
    FileHandler(Instance i, const char *confDirRelPath, const char *confFileName){
        integers = new Common::Obvc<int>();
        booleans = new Common::Obbc(128); // Max 128 boolean configs can be loaded at once.
        strings = new Common::Obvc<std::string>();
        
    }
    ~FileHandler(){
        WriteFile();
        delete(integers);
        delete(booleans);
        delete(strings);
    }
    void WriteFile(const char *confDirRelPath, const char *confFileName){
        int state;
        std::stringstream ss;
        ss << confDirRelPath << "/" << confFileName;
        std::string path = ss.str(); 
        std::ifstream fileIn(path);
        ss.clear();
        int line = 0;
        char c;
        std::vector<std::string> comments;
        // 0: pre ':'
        // 1: post ':'
        for(fileIn.get(c);!std::ios_base::iostate::_S_eofbit;fileIn.get(c)){
            switch(state){
                case 0:
                    if(c==':'){state=1;comments.push_back(ss.str());ss.clear();}
                    else if(c=='\n'){comments.push_back(ss.str());ss.clear();}
                    else{ss<<c;}
                    break;
                case 1:
                    if(c=='\n'){state=0;}
                    break;
            }
        }
        fileIn.close();
        std::ofstream fileOut(path);
        
        for(int i = 0 ; i < comments.size() ; i++){
            ss.clear();
            ss<<comments[i];
            if(booleans->Exists(i)){
                if(booleans->Poll(i))
                    ss<<"true";
                else
                    ss<<"false";
            }
            else if(integers->Exists(i)){ 
                ss << integers->Poll(i);
            }
            else if(strings->Exists(i)){
                ss << strings->Poll(i);
            }
            ss<<"\n";
            const char* lineOut = ss.str().c_str();
            fileOut.write(lineOut,ss.str().length());
        }
    } 
    void ReadFile(const char *confDirRelPath, const char *confFileName){
        std::stringstream ss;
        ss << confDirRelPath << "/" << confFileName;
        std::ifstream file(ss.str());
        ss.clear();
        int line = 0;
        int number = 0;
        int state = 0;
        // 0: pre ':'
        // 1 ... 10: after ':' NUMBER
        // 11: error long config value
        // 12: t 13: r 14: u 15: e FINAL
        // 16: f 17: a 18: l 19: s 20: e FINAL
        // 21...120: after ':' CHAR
        // 121: error long config value
        // 122: END
        while(state != 122){
            switch(state){
                char c;
                if(state != 11 || state != 121){file.get(c);}
                if(std::ios_base::iostate::_S_failbit){throw std::runtime_error("Error reading config file.");}
                case 0:
                    if(c==':'){state = 1;}
                    break;
                case 1 ... 10:     
                    if     (c=='0'){number = (number * 10);     state++;}
                    else if(c=='1'){number = (number * 10) + 1; state++;}
                    else if(c=='2'){number = (number * 10) + 2; state++;}
                    else if(c=='3'){number = (number * 10) + 3; state++;}
                    else if(c=='4'){number = (number * 10) + 4; state++;}
                    else if(c=='5'){number = (number * 10) + 5; state++;}
                    else if(c=='6'){number = (number * 10) + 6; state++;}
                    else if(c=='7'){number = (number * 10) + 7; state++;}
                    else if(c=='8'){number = (number * 10) + 8; state++;}
                    else if(c=='9'){number = (number * 10) + 9; state++;}

                    else if(state == 1 && c=='t'){state = 12;}
                    else if(state == 1 && c=='f'){state = 16;}
                    
                    else if(c=='\n'){integers->Notify(line++,number);number=0; state = 0;}
                    else if(std::ios_base::iostate::_S_eofbit){integers->Notify(line, number);state=122;}
                    
                    else{
                        for(int i = (state-1) ; i > 0 ; i--){
                            int a = number/(std::pow(10,i));
                            number -= a;
                            switch(a){
                                case 0:ss<<'0';break;case 1:ss<<'1';break;case 2:ss<<'2';break;case 3:ss<<'3';break;case 4:ss<<'4';break;case 5:ss<<'5';break;case 6:ss<<'6';break;case 7:ss<<'7';break;case 8:ss<<'8';break;case 9:ss<<'9';break;
                            }
                        }
                        file.get(c);
                        ss<<c;
                        state += 21;
                    }
                    break;
                case 11:
                    throw std::runtime_error("Config integer value larger than integer max. (2^32 = 2 147 483 647)");
                    break;
                case 12:
                    if(c=='r'){state++;}
                    else{ss<<'t';if(c=='\n'){strings->Notify(line,ss.str());ss.clear();state=0;}else if(std::ios_base::iostate::_S_eofbit){strings->Notify(line, ss.str());state=122;} else state=22;}
                    break;
                case 13:
                    if(c=='u'){state++;}
                    else{ss<<'t'<<'r';if(c=='\n'){strings->Notify(line,ss.str());ss.clear();state=0;}else if(std::ios_base::iostate::_S_eofbit){strings->Notify(line, ss.str());state=122;} else state=23;}
                    break;
                case 14:                
                    if(c=='e'){state++;}
                    else{ss<<'t'<<'r'<<'u';if(c=='\n'){strings->Notify(line,ss.str());ss.clear();state=0;}else if(std::ios_base::iostate::_S_eofbit){strings->Notify(line, ss.str());state=122;} else state=24;}     
                    break;
                case 15:
                    if(c=='\n'){booleans->Notify(line++);state = 0;}
                    if(std::ios_base::iostate::_S_eofbit){booleans->Notify(line);state=122;}
                    else{ss<<'t'<<'r'<<'u'<<'e';if(std::ios_base::iostate::_S_eofbit){strings->Notify(line, ss.str());state=122;} else state=25;}
                    break;
                case 16:
                    if(c=='a'){state++;}
                    else{ss<<'f';if(c=='\n'){strings->Notify(line,ss.str());ss.clear();state=0;}else if(std::ios_base::iostate::_S_eofbit){strings->Notify(line, ss.str());state=122;} else state=22;}
                    break;
                case 17:
                    if(c=='l'){state++;}
                    else{ss<<'f'<<'a'; if(c=='\n'){strings->Notify(line,ss.str());ss.clear();state=0;}else if(std::ios_base::iostate::_S_eofbit){strings->Notify(line, ss.str());state=122;} else state=23;}
                    break;
                case 18:
                    if(c=='s'){state++;}
                    else{ss<<'f'<<'a'<<'l';if(c=='\n'){strings->Notify(line,ss.str());ss.clear();state=0;}else if(std::ios_base::iostate::_S_eofbit){strings->Notify(line, ss.str());state=122;} else state=24;}
                    break;
                case 19:
                    if(c=='e'){state++;}
                    else{ss<<'f'<<'a'<<'l'<<'s';if(c=='\n'){strings->Notify(line,ss.str());ss.clear();state=0;}else if(std::ios_base::iostate::_S_eofbit){strings->Notify(line, ss.str());state=122;} else state=25;}
                    break;
                case 20:
                    if(c=='\n'){state = 0;}
                    else if(std::ios_base::iostate::_S_eofbit){state=122;}
                    else{ss<<'f'<<'a'<<'l'<<'s'<<'e';if(c=='\n'){strings->Notify(line,ss.str());ss.clear();state=0;}else if(std::ios_base::iostate::_S_eofbit){strings->Notify(line, ss.str());state=122;} else state=26;}
                case 21 ... 120:
                    if(c=='\n'){strings->Notify(line, ss.str());ss.clear();state=0;}
                    else if(std::ios_base::iostate::_S_eofbit){state=122;}
                    else{state++;}
                case 121:
                    throw std::runtime_error("Config value too long. max length: 100 ascii characters.");
            }
        }
    }
    void WriteFile(){
    
    }
        
};

}
