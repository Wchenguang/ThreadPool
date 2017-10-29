#include <list>
#include <exception>
#include <mutex>

#define LOCKALL std::lock_guard<std::mutex> fguard(frontOperateMutex); std::lock_guard<std::mutex> bguard(backOperateMutex)
#define LOCKGRONT std::lock_guard<std::mutex> fguard(frontOperateMutex)
#define LOCKBACK std::lock_guard<std::mutex> bguard(backOperateMutex)

struct EXP_EmptyList : std::exception{
public:
    const char *what() const throw(){
        return "empty list\n";
    }
};

//typedef int T;
template <typename T>
class TS_List{
private:
    std::list<T> mList;
    std::mutex frontOperateMutex;
    std::mutex backOperateMutex;



public:
    TS_List() : mList(std::list<T>()){

    }

    //前后都要上锁
    TS_List(const TS_List &otherList){
        //按照相同顺序获取锁
        LOCKALL;
        mList = otherList.mList;
    }

    TS_List &operator =(const TS_List &) = delete;

    bool empty(){
        LOCKALL;
        return mList.empty();
    }

    size_t size(){
        LOCKALL;
        return mList.size();
    }

    //需要为前面上锁
    void push_front(const T &element){
        LOCKGRONT;
        mList.push_front(element);
    }
    void push_front(T &&element){
        LOCKGRONT;
        mList.push_front(element);
    }
    void pop_front(T &target){
        LOCKGRONT;
        if(mList.empty())
            throw EXP_EmptyList();
        target = mList.front();
        mList.pop_front();
    }
    std::shared_ptr<T> pop_front(){
        LOCKGRONT;
        if(mList.empty())
            throw EXP_EmptyList();
        std::shared_ptr<T> ptr = std::make_shared(mList.front());
        mList.pop_front();
        return ptr;
    }

    //需要为后面上锁
    void push_back(const T &element){
        LOCKBACK;
        mList.push_back(element);
    }
    void push_back(T &&element){
        LOCKBACK;
        mList.push_back(element);
    }
    void pop_back(T &target){
        LOCKBACK;
        if(mList.empty())
            throw EXP_EmptyList();
        target = mList.back();
        mList.pop_back();
    }
    std::shared_ptr<T> pop_back(){
        LOCKBACK;
        if(mList.empty())
            throw EXP_EmptyList();
        std::shared_ptr<T> ptr = std::make_shared(mList.back());
        mList.pop_back();
        return ptr;
    }

    void remove(const T &element){
        LOCKALL;
        mList.remove(element);
    }
};