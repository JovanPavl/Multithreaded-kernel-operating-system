#ifndef _ksem_h
#define _ksem_h
#include "semaphor.h"
#include "LISTA.h"
#include "PCB.h"
class Semaphore;
class KernelSem{
private: Semaphore* par;					
public: List<PCB> waitsem;					
		int value;
		int val() const;
		 KernelSem (int init=1);
		virtual ~KernelSem (){							
	
		}
		virtual int wait (Time maxTimeToWait);
		virtual int signal(int n = 0);
private: void block(Time tos);	
		 void deblock(int x);
		 void updatelist();
};
#endif