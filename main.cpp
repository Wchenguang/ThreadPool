#include "WThreadPool.h"
#include <fstream>

using namespace std;

//测试job写文件
class job : public WJob{
private:
    int a;
    fstream file;
public:
    job(int aa){
        a = aa;
    }
    job() : job(-1) {};
    void run(){
        string str= to_string(a);
        cerr<<str;
        file.open(str.c_str(), std::fstream::out);
        for(int i = 0; i < 77870; ++i){
            file.write("good\n", 5);
        }
        file.close();
    }
};


int main(){

    job *jobs = new job[26];

    WThreadPool pool(17);

    pool.start();

    for(int i = 0; i < 26; ++i){
        pool.pushJob(&jobs[i]);
    }

    //任意输入字符结束
    pool.getCharToTerminate();

    delete [] jobs;
}

