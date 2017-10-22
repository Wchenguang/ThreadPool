#include <iostream>
#include <fstream>

#include "WThreadPool.h"

using namespace std;

class job : public WJob{
private:
    int a;
    fstream file;
public:
    job(int aa){
        a = aa;
    }
    void *run(){
        char ch[5] = {'0'};
        ch[0] = 'a' + a;
        file.open(ch, std::fstream::out);
        for(int i = 0; i < 1000; ++i){
            file.write("good\n", 5);
        }
        file.close();
        ch[1] = ' ';
        cerr<<ch;
    }
};

int main(){

    WThreadPool pool;

    pool.start();

    for(int i = 0; i < 26; ++i){
        pool.pushJob(new job(i));
    }

    pthread_join(pool.getDispatchThread(), NULL);

    while (!pool.tryTerminate());

    return 0;
}