#include <mqueue.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;
const char *MSGQOBJ_NAME = "/Test_ClientServer";
struct mq_attr attribute;
mutex mqMtx;
const int MAX_THREAD = 10;
mqd_t msgq_id ;
void handler(int i)
{
	string c;
	while ("exit" != c)
	{
		{
			lock_guard<mutex> g(mqMtx);
			cin>>c;
			std::cin.ignore(1);
			cout<<"thread "<<i<<" send "<<endl;
		}
		
		/* sending the message      --  mq_send() */
		int ret = mq_send(msgq_id, c.c_str(), c.size(), 0);

	}
	{
		lock_guard<mutex> g(mqMtx);
		cout<<"exit thread "<<i<<endl;
	}
}
int main()
{
	attribute.mq_maxmsg = 1024;
	attribute.mq_msgsize = 1024;

	/* queue has already been created, this opens it*/
	msgq_id = mq_open(MSGQOBJ_NAME, O_RDWR);
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

	/* closing the queue        -- mq_close() */
	mq_close(msgq_id);
}

