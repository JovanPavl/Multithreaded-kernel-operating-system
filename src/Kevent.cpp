#include "Kevent.h"
#include "IVTE.h"
KernelEv* KernelEv :: t[256];
KernelEv :: KernelEv(IVTNo id){
		this->ulaz = id;
		creator = PCB :: running;
		t[id] = this;
		blocked = 0;
		val = 1;
	}
void KernelEv :: wait(){
	lockit
	if(creator != PCB :: running){
		unlockit
		return;
	}
	if(val == 1){
		val = 0;
		unlockit
		return;
	}
	if(PCB :: running != PCB :: killme){
		blocked = PCB :: running;
		PCB :: running->state = PCB :: WAITING;
	}
	unlockit
	PCB :: dispatch();
}
void KernelEv :: signal(){
	lockit
	if(blocked == 0){
		val = 1;
	}
	else{
		PCB :: push(blocked);
		blocked = 0;
	}
	unlockit
}
KernelEv :: ~KernelEv(){
/*	printf("Brise KernelEv %d\n",ulaz);*/
	IVTEntry :: t[ulaz]->skloni();
}
