#ifndef loop_h_
#define loop_h_
#include "thread.h"
class Loop : public Thread{
public: int x;
		void run();
		Loop(): Thread() {}
};
#endif