#include "thread.h"
void Thread :: start(){
	myPCB->start();
}
void Thread :: waitToComplete(){
	myPCB->waitToComplete();
}
Thread :: ~Thread(){
	delete myPCB;						
}
ID Thread :: getId(){
	return this->myPCB->getId();
}
ID Thread :: getRunningId(){
	return PCB :: running->id;
}
Thread* Thread :: getThreadById(ID ide){
	return PCB :: getThreadById(ide)->par;
}
Thread :: Thread (StackSize stackSize, Time timeSlice ){
	myPCB = new PCB(stackSize,timeSlice,this);
}
void dispatch(){
	async = 1;
	asm int 0x8
}
void Thread :: signal(SignalId signal){
	myPCB->signal(signal);
}
void Thread :: registerHandler(SignalId signal, SignalHandler handler){
	myPCB->registerHandler(signal,handler);
}
void Thread :: unregisterAllHandlers(SignalId id){
	myPCB->unregisterAllHandlers(id);
}
void Thread :: swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	myPCB->swap(id,hand1,hand2);
}
void Thread :: blockSignal(SignalId signal){
	myPCB->blockSignal(signal);
}
void Thread :: blockSignalGlobally(SignalId signal){
	PCB :: blockSignalGlobally(signal);
}
void Thread :: unblockSignal(SignalId signal){
	myPCB->unblockSignal(signal);
}
void Thread :: unblockSignalGlobally(SignalId signal){
	PCB :: unblockSignalGlobally(signal);
}