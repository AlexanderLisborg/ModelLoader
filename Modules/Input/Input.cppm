module;

#include <bits/stl_pair.h>

export module Input;

import Common;

export namespace Input{

class Instance{
    private:
        // Left bottom is (0,0)
        Common::Obv<unsigned int> cursorPosY; 
        Common::Obv<unsigned int> cursorPosX;
        Common::Obbc binaryActions {32};
    public:
};
// 32 button actions fits neatly in an int and is way more than you will ever need
enum binaryActions{
    UP,DOWN,LEFT,RIGHT,
    INTERACT1,INTERACT2,
    SNEAK,SPRINT,
    INVENTORY,
    MENU1,MENU2,
    SCROLL_UP,SCROLL_DOWN,SCROLL_LEFT,SCROLL_RIGHT,
    A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16
    
};

}
