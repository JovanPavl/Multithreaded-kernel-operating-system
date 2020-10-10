#include "event.h"
#include "Kevent.h"
Event :: Event(IVTNo ivtNo){
	myImpl = new KernelEv(ivtNo);
}

Event :: ~Event(){
	delete myImpl;
}

void Event :: signal(){
	myImpl->signal();
}

void Event :: wait(){
	myImpl->wait();
}
