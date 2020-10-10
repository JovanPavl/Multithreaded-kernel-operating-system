#ifndef IVTE_h
#define IVTE_h
#include <dos.h>
#include "Kevent.h"

#define PREPAREENTRY(u, callme)\
void interrupt newroutine##u(...)\
{\
	IVTEntry::t[u]->signal();\
	if(callme == 1) IVTEntry::t[u]->callold();\
}\
IVTEntry Entry##u = IVTEntry(u, newroutine##u);


class IVTEntry{
public: static IVTEntry* t[256];
		pInterrupt oldroutine;
		IVTNo entery;
		IVTEntry(IVTNo e, pInterrupt p1);
		void  signal();
		void callold();
		void skloni();
		~IVTEntry();
};
#endif
