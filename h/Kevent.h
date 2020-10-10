#ifndef Kevent_h
#define Kevent_h
#include "event.h"
#include "PCB.h"
class KernelEv{
public: static KernelEv* t[256];
		PCB* creator,*blocked;					//the PCB* that should be blocked too
		KernelEv(IVTNo id);
		~KernelEv();
		void signal();
		void wait();
		int val;
		IVTNo ulaz;
};

#endif
