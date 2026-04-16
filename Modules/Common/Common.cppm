#include <cstddef>
#include <iterator>
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
        // TODO: Implement operators according to https://stackoverflow.com/questions/37031805/preparation-for-stditerator-being-deprecated/38103394
        class Iterator{
            // Iterator traits
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = ptrdiff_t;
            using pointer = T*;
            using reference = T&;
        };
        Obvc(){
            obvs = new std::map<int, std::pair<T,std::vector<void (*) (int,T)>>>;
            
        }
        ~Obvc(){
            delete(obvs);
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
            // If key dne, create it and init it with value
            catch(std::out_of_range& err){
                std::vector<void (*) (int, T)> empt {};
                std::pair<T,std::vector<void (*) (int, T)>> in {value,empt};
                obvs->insert({i,in});
            }
        }
        bool Exists(int i){
            return obvs->contains(i);
        }
        T Poll(int i){
            try{
                return obvs->at(i).first;
            } catch(std::out_of_range& err){
                throw err;
            }
        }
        
};

class Obbc{
    private:
    int *values;
    int *exists;
    std::vector<void (*) ()> *callbacks; 
    public:
    Obbc(int nrOfButtons){
        values = new int[(nrOfButtons / sizeof(int) ) + 1];
        exists = new int[(nrOfButtons / sizeof(int) ) + 1];
        for(int i = 0 ; i < ((nrOfButtons/sizeof(int))+1) ; i++){
            values[i]=0;
            exists[i]=0;
        }
    }
    ~Obbc(){
        delete(values);
        delete(exists);
    }
    void RegisterCallback(int i, void (*callback) ()){
        callbacks[i].push_back(callback);
    }
    void RemoveCallback(int i, void (*callback) ()){
        std::erase(callbacks[i],callback);
    }
    void Notify(int i){
        if(!Exists(i)){throw std::out_of_range("dne");}
        std::vector<void (*) ()> v = callbacks[i];
        std::for_each(v.cbegin(), v.cend(),[](void (*callback) ())->void{callback();});
        *(values + (i / sizeof(int))) ^= (1 << (i%sizeof(int)));
        *(exists + (i / sizeof(int))) &= (1 << (i%sizeof(int)));
    }
    void AddValue(int i){
        *(exists + (i / sizeof(int))) |= (1 << (i % sizeof(int)));
    }
    void RemoveValue(int i){
        callbacks[i].clear();
        *(exists + (i/sizeof(int))) &= ( (-1) ^ (1 << (i % sizeof(int)) ) );
    }
    inline bool Exists(int i){
        return ((*(exists + (i/sizeof(int))) >> (i%sizeof(int)) )& 1);
    }
    
    bool Poll(int i){
        if(!Exists(i)){throw std::out_of_range("dne");}
        // Don't question the laws of the universe
        return (((*(values + (i/sizeof(int)))) >> (i%sizeof(int)) )&1);
    }
};
template <class T> class Obv{
    private:
        T val;
        std::vector<void (*) (T)> callbacks;
    public:
        Obv(T init){
            val = init;
            callbacks = new std::vector<void (*) (T)>();
        }
        ~Obv(){
            delete(callbacks);
        }
        void RegisterCallback(void (*callback) (T)){
            callbacks.push_back(callback); 
        }
        void RemoveCallback(void (*callback) (T)){
            std::erase(callbacks,callback);
        }
        void Notify(T val){
            this->val = val;
            std::for_each(callbacks.cbegin(),callbacks.cend(),[&](void (*callback) (T))->void {callback(val);});
        }
        T Poll(){
            return val;
        }
};
}
