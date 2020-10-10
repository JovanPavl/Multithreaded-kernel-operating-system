/*
 * PCB.cpp
 *
 *  Created on: Apr 7, 2020
 *      Author: OS1
 */
#include "PCB.h"
extern void tick();
volatile int t = -1;						
volatile Time ts;
volatile int lockin = 0;
volatile int soft = 0;
volatile int async = 0, inrutine = 0;
volatile Time slices = 0;
volatile int tsp,tss,tbp,sizesleepy = 0;
volatile PCB* tmppcb;

int* PCB :: activesignals = new int[16];
ID PCB::idinc = 0;
PCB** PCB :: allpcbs = new PCB*[200];		
int PCB :: sizepcbs = 200;					
PCB* PCB :: running = 0;			
int PCB :: donepcbs = 0;
PCB* PCB :: killme = 0;
PCB* PCB :: idle = 0;
List<PCB> PCB :: sleepy;
void PCB :: solveSignals(){
	softlock
	if(killme != 0 && killme != PCB :: running){							
		releasewaiters(killme);
		killme->signal(2);												
		if(killme->fatherpcb != 0)
			killme->fatherpcb->signal(1);	
		delete killme->stack;
		killme->stack = 0;
		killme = 0;
	}
	int i = running->signalstohandle.size();
	while(i--){
		int tmp = running->signalstohandle.front();
		running->signalstohandle.pop();
		if(activesignals[tmp] == 1 || running->myblockedsignals[tmp] == 1){
			running->signalstohandle.push(tmp);									// we will solve this later
		}
		else{
			if(tmp == 0 && running->destroy == 1){								//now we have to kill this pcb
				killme = PCB :: running;				//we will kill this later
				lockit
				unlockit
				softunlock
				return;									
			}
			else{
				int ss = running->handlers[tmp].size();
				while(ss--){
					SignalHandler s = running->handlers[tmp].front();
					s();												//call function for signal
					running->handlers[tmp].pop_front();
					running->handlers[tmp].push_back(s);
				}
			}
		}
	}
	softunlock
}
void PCB :: releasewaiters(PCB* x){
	lockit
	if(x == 0)				//PCB is already killed
		return;
	while(x->waitme.size() > 0){
		PCB* tmp;
		tmp = x->waitme.front();
		push(tmp);
		x->waitme.pop_front();
	}
	x->state = DONE;
	unlockit	
}
void PCB :: wraper(){
	running->par->run();
	releasewaiters(running);
	running->signal(2);												//signal that it's done
	if(running->fatherpcb != 0)
		running->fatherpcb->signal(1);							//signal to father that I'm done 
	solveSignals();
	dispatch();
}
void PCB :: push(PCB* x){
			x->state = READY;
			Scheduler :: put(x);
		}
		
void PCB :: pop(){					
			lockit
			PCB :: running = Scheduler :: get();
			if(running == 0)
				running = idle;
			PCB :: running->state = RUNNING;
			slices = running->ts;
			unlockit
}
void PCB :: start(){
	if(this->state != NOTSTARTED){
		return ;
	}
	push(this);
}
PCB :: 	PCB(StackSize size, Time timeSlice, Thread* cur ){		
	destroy = 1;
	this->fatherpcb = PCB :: running;
	this->par = cur;
	int i = 0,it;
	for(i = 0; i < 16; i++)
		 myblockedsignals[i] = 0;							//nema blokiranih
	lockit
	 if(this->fatherpcb){										//take all handlers
		 for(i = 0; i < 16; i++){
			it = fatherpcb->handlers[i].size();
			while(it--){
				SignalHandler x = fatherpcb->handlers[i].front();
				fatherpcb->handlers[i].pop_front();
				fatherpcb->handlers[i].push_back(x);
				handlers[i].push_back(x);
			}
		 }
	 }
	unlockit
	sz = size;
	suspendedtime = 0;												//nije suspendovana uopste ???
	id = idinc++;
	if(id == 2)
		PCB :: idle = PCB :: running;
	if(id >= sizepcbs){
		PCB** tmp = (PCB**)realloc(allpcbs,2*id*sizeof(PCB*));
		if(tmp == 0){
			cout << "Nema dovoljno memorije " << endl;
		}
		allpcbs = tmp;
		sizepcbs = 2*id;
		allpcbs[id] = this;
	}
	else{
		allpcbs[id] = this;
	}
	ts = timeSlice;
	if(ts == 0)
		infinity = 1;
	else
		infinity = 0;
	state = NOTSTARTED;
	stack = new unsigned[size];
#ifndef BCC_BLOCK_IGNORE
	stack[size - 1] = FP_SEG(cur);
	stack[size - 2]= FP_OFF(cur);
	stack[size - 5] = 0x200;
	stack[size - 6] = FP_SEG(PCB :: wraper);
	stack[size - 7] = FP_OFF(PCB :: wraper);
	sp = FP_OFF(stack + size - 16);
	ss = FP_SEG(stack + size - 16);
	bp = sp;
#endif
}
void interrupt clock(){							//DON'T forget to put tic() when testing ! ! 
	if(!async)
		asm int 60h
	if(!async && !lockin){				
    	if(slices)
			slices--;
	//	tick();
		sizesleepy = PCB :: sleepy.size();
		while(sizesleepy--){
			if((PCB :: sleepy.front())->state == PCB :: WAITING)						//znaci signal ga nije vec skinuo
				if((PCB :: sleepy.front())->suspendedtime-- == 0){					//vrv treba postdekrement
					PCB :: push(PCB :: sleepy.front());
				}
				else{																
					(PCB :: sleepy).push_back(PCB :: sleepy.front());
				}
			PCB :: sleepy.pop_front();
		}
	}
	asm{
		mov tsp, sp
		mov tss, ss
		mov tbp, bp
	}
	PCB :: running->sp = tsp;
	PCB :: running->ss = tss;
	PCB :: running->bp = tbp;
	if((async == 1 || (slices == 0 && PCB :: running->infinity == 0)) && lockin == 0 && soft == 0){
			asm{
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
			PCB :: running->sp = tsp;
			PCB :: running->ss = tss;
			PCB :: running->bp = tbp;
			if(PCB::running->state != PCB :: WAITING && PCB::running->state != PCB :: DONE && PCB :: killme != PCB :: running && PCB :: running != PCB :: idle){	
				PCB :: push(PCB :: running);
			}
			PCB :: pop();
			tsp = PCB :: running->sp;
			tss = PCB :: running->ss;
			tbp = PCB :: running->bp;
			asm{
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
			async = 0;	
	}
	if(!soft && !lockin)								
		PCB :: solveSignals();
}
void PCB :: waitToComplete(){
	lockit
	if(state != DONE && state != NOTSTARTED){
		running->state = WAITING;
		waitme.push_back(running);
		unlockit
		dispatch();
	}
	unlockit
}
PCB :: ~PCB(){
	waitToComplete();
	delete stack;
}
void PCB :: dispatch(){
	async = 1;
	asm int 0x8
}
void PCB :: signal(SignalId signal){
	lockit
		signalstohandle.push(signal);
	unlockit
}
void PCB :: registerHandler(SignalId signal, SignalHandler handler){
	lockit
		handlers[signal].push_back(handler);
	unlockit
}
void PCB :: unregisterAllHandlers(SignalId id){
	lockit
		if(id == 0)
			destroy = 0;
		handlers[id].deletelist();
	unlockit
}
void PCB :: swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	lockit
		handlers[id].swap(hand1,hand2);
	unlockit
}
void PCB :: blockSignal(SignalId signal){
	lockit
		myblockedsignals[signal] = 1;
	unlockit
}
void PCB :: blockSignalGlobally(SignalId signal){
	lockit
		PCB :: activesignals[signal] = 1;
	unlockit
}
void PCB :: unblockSignal(SignalId signal){
	lockit
		myblockedsignals[signal] = 0;
	unlockit
}
void PCB :: unblockSignalGlobally(SignalId signal){
	lockit
		PCB :: activesignals[signal] = 0;
	unlockit
}
