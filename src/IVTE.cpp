#include "IVTE.h"
IVTEntry* IVTEntry :: t[256];
IVTEntry :: IVTEntry (IVTNo e, pInterrupt p1){
	lockit
		entery = e;
		t[e] = this;
		lock
		oldroutine = getvect(e);
		setvect(entery,p1);
		unlock
	unlockit
}

void IVTEntry :: callold(){
	oldroutine();
}

void IVTEntry :: signal(){
		if(KernelEv :: t[entery] != 0)
			KernelEv :: t[entery]->signal();
}

void IVTEntry :: skloni(){
	lock
	setvect(entery,oldroutine);
	unlock
}

IVTEntry :: ~IVTEntry (){
	lock
		setvect(entery,oldroutine);
	unlock
}
