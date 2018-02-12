//SQL_GET_DELIVER_INFO
#include <mqueue.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstring>
#include <thread>
#include <mutex>

using namespace std;
const char *MSGQOBJ_NAME = "/Test_ClientServer";
mutex mqMtx;
const int MAX_THREAD = 10;
struct mq_attr attribute;
mqd_t msgq_id ;
void handler(int i)
{
    char msg[1024];
    string c(msg);
	while ("exit" != c)
    {
        
        memset(msg,0,1024);
       auto size = mq_receive(msgq_id, msg, 1024, nullptr);
        c = msg;
        {
            lock_guard<mutex> g(mqMtx);
            cout<<"thread "<<i<<" message="<<msg<<", size="<<size<<endl;
                    
        }    
    }
    {
        lock_guard<mutex> g(mqMtx);
         cout<<"exit thread "<<i<<endl;
    }
   
}

int main(){
    attribute.mq_maxmsg = 2;
	attribute.mq_msgsize = 5;
	msgq_id = mq_open(MSGQOBJ_NAME, O_RDWR | O_CREAT, S_IRWXU, &attribute);
	cout<<"id="<<msgq_id<<endl;
	if (msgq_id == (mqd_t)-1) {
	    perror("In mq_open()");
	    return 1;
	}

    thread threads[MAX_THREAD];
    for (auto j = 0; j<MAX_THREAD; j++)
    {
            threads[j] = thread(handler, j);  
    }

    for (auto j = 0; j<MAX_THREAD; j++)
    {
            threads[j].join();  
    }
    mq_close(msgq_id);
}
