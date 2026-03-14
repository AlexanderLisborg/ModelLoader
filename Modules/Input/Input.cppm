module;

#include <bits/stl_pair.h>

export module Input;

import Common;

export namespace Input{
// 32 button actions fits neatly in a 32 bit int
enum BinaryAction{
    UP,DOWN,LEFT,RIGHT,
    INTERACT1,INTERACT2,
    SNEAK,SPRINT,
    INVENTORY,
    MENU1,MENU2,
    SCROLL_UP,SCROLL_DOWN,SCROLL_LEFT,SCROLL_RIGHT,
    A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15,A16 
};


class Instance{
    private:
        // Left bottom is (0,0) first 16 bits = X pos, next 16 bits = Y pos
        // NOTE: int must be at least 32 bits, running on an architecture with 16 bit ints prob wont work
        Common::Obv<unsigned int> cursorPos; 
        Common::Obbc binaryActions {32};
    public:
        void RegisterCursorCallback(void (*callback) (unsigned int)){
            cursorPos.RegisterCallback(callback);
        }
       
        void RegisterBinaryActionCallback(BinaryAction a, void (*callback) ()){
            binaryActions.RegisterCallback(a, callback);
        }
        void RemoveBinaryActionCallback(BinaryAction a, void (*callback) ()){
            binaryActions.RemoveCallback(a,callback);
        }
        void RemoveCursorCallback(void (*callback) (unsigned int)){
            cursorPos.RemoveCallback(callback);
        }
        void NotifyBinaryAction(BinaryAction a){
            binaryActions.Notify(a);
        }
        void NotifyCursorPos(unsigned int pos){
            cursorPos.Notify(pos);
        }
};
}
