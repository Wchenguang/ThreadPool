#include <iostream>

#include "WThread.h"

using namespace std;

class jb : public WJob{
private:
    int a;
public:
    jb(){
        a = 1;
    }
    void *run(){
        std::cout<<"hello "<<a<<endl;
    }
};

int main(){

    WThread thread;

    thread.start();

    for(int i = 0; i < 100; ++i){
        cout<<i<<endl;
    }

    thread.setJob(new jb());


    pthread_join(thread.getMyThread(), NULL);

    return 0;
}