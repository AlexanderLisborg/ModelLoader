module;

#include <stdexcept>
#include<vector>
#include<map>
#include <algorithm>

export module Common;

export namespace Common{

template <class T> class Obvc{
    private:
        // Key : {ObservableValue,Callbacks} <- Trigger callbacks whenever value is updated. Retrieve stored value when polled. 
        std::map<int, std::pair<T,std::vector<void (*) (int, T)>>>* obvs;

    public:
        Obvc(){
            obvs = new std::map<int, std::pair<T,std::vector<void (*) (int,T)>>>;
            
        }
        // Add callback to observable value i. Creates key if not present in map. 
        // WARNING: CAN ADD DUPLICATES TO LIST OF CALLBACKS. 
        void RegisterCallback(int i, void (*callback) (int, T)){
            (*obvs)[i].second.push_back(callback);    
        }
        // Remove registered callback method. Throws out_of_range exception when i is not found
        // amongst observable values. If callback is not present, does nothing. Removes duplicates.
        void RemoveCallback(int i, void (*callback) (int)){
            try{
                std::erase(obvs->at(i).second,callback);
            }
            catch(const std::out_of_range& err){
                throw err;
            }
        }

        void Notify(int i, T value){
            try{
                std::vector<void (*) (int, T)> v = obvs->at(i).second;
                std::for_each(v.cbegin(),v.cend(),[&](void (*callback) (int,T))->void{callback(i,value);});
            }
            catch(std::out_of_range& err){
                throw err;
            }
        }
        T Poll(int i){
            try{
                return obvs->at(i).first;
            } catch(std::out_of_range& err){
                throw err;
            }
        }
};
}
