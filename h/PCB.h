#ifndef PCB_H_
#define PCB_H_
#include "thread.h"
#include "SCHEDULE.H"
#include "stdlib.h"
#include "LISTA.h"
#include "listar.h"
#include "Queue.h"
#include <iostream.h>
#include <dos.h>
#include <stdio.h>
#define softlock soft = 1;
#define softunlock soft = 0;
#define lock asm cli
#define unlock asm sti
#define lockit lockin = 1;
#define unlockit 	lockin = 0;
extern volatile int t;								
extern volatile int lockin;												
extern volatile int async, inrutine;
extern volatile Time slices;
extern volatile int tsp,tss,tbp,sizesleepy;
extern volatile Time ts;
extern volatile int soft;
class Thread;
typedef void interrupt (*pInterrupt)(...);




class PCB{
private:int infinity;			
		StackSize sz;
		ListFunctions handlers[16];				//all handlers here 
		int myblockedsignals[16];
		Queue signalstohandle;													//ne zaboravi da napravis destruktor od Queue
		PCB* fatherpcb;																			
public: static int* activesignals;
		static PCB* idle;
		int destroy;
		static PCB* killme;
		Time ts;
		int suspendedtime;						
		int signaled;
		enum phase { NOTSTARTED, READY, WAITING, RUNNING, DONE};
		~PCB();
		Thread* par;
		static ID idinc;
		ID id;
		unsigned sp;
		unsigned ss;
		unsigned bp;
		unsigned* stack;
		static List<PCB> sleepy;			
		List<PCB> waitme;			
		static PCB* running;
		static PCB** allpcbs;		
		static int sizepcbs;		
		static int schedulersize;	
		static int donepcbs;		
		phase state;
		void start();
		static ID getRunningId(){			
			return running->id;
		}
		static void wraper();
		static PCB * getThreadById(ID ide){
			return allpcbs[ide];
		}
		ID getId(){
			return id;
		}
		static void releasewaiters(PCB* x);
		static void dispatch();
		friend void interrupt clock();
		static void push(PCB* x);
		static void solveSignals();
		void waitToComplete();
		static void pop();					
		PCB(StackSize size = defaultStackSize, Time timeSlice = defaultTimeSlice, Thread* cur = 0);
		void signal(SignalId signal);
		void registerHandler(SignalId signal, SignalHandler handler);
		void unregisterAllHandlers(SignalId id);
		void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
		void blockSignal(SignalId signal);
		static void blockSignalGlobally(SignalId signal);
		void unblockSignal(SignalId signal);
		static void unblockSignalGlobally(SignalId signal);
}; 
#endif /* PCB_H_ */
