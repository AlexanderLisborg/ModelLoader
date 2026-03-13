module;

#include <string>
#include <tuple>
#include <vector>
#include<map>

export module Obvc;
export namespace Obvc{

template <class T> class Obvc{
    public:
    private:
        // Key : {ObservableValue,Callbacks} <- Trigger callbacks whenever value is updated. Retrieve stored value when polled. 
        std::map<int, std::tuple<T, std::vector<void (*) (int,T)>>> obvs;
    public:
        void RegisterCallback(int i, void (*callback) (int)){
            // Add to observers
            std::get<1>(obvs[i]);
        }
        void RemoveCallback(int i, void (*callback) (int)){
            obvs[i];
        }
        void Notify(int i);
    private:

};

}
