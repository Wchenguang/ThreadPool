#include <pthread.h>
#include <iostream>

using namespace std;

/*atomic_int total(0);

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *func(void *args){
    for(int i = 0; i < 1000; ++i){
        cout<<*((int*)args)<<" "<<i<<endl;
        total += i;
    }
}

int main(){

    pthread_t th1;
    pthread_t th2;

    int id = 1;
    if(pthread_create(&th1, NULL, &func, &id)){
        throw;
    }

    int id2 = 2;
    if(pthread_create(&th2, NULL, &func, &id2)){
        throw;
    }
    pthread_join(th1, NULL);
    pthread_join(th2, NULL);

    cout<<total<<endl;

    return 0;
}*/

