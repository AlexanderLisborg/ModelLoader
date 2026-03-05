#include <cmath>

#define MACRO_STAT Debug::Stat(__FILE__,std::size(__FILE__),__func__,std::size(__func__),std::to_string(__LINE__).c_str(),(int)((ceil(log10(__LINE__))+1)*sizeof(char)))
