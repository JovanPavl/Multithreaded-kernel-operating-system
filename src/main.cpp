#include "thread.h"
#include <dos.h>
#include "PCB.h"
#include "loop.h"
#include "semaphor.h"
#include "IVTE.h"
int ret;
int userMain (int argc, char* argv[]){
	
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

class UserThread : public Thread{
	private: int argc;
			char** argv;
	public: UserThread(int a, char* ar[]):Thread(){
				argc = a;
				argv = ar;
		   }
		   void run();
};
void UserThread :: run(){
	ret = userMain(argc,argv);
}
unsigned oldTimerSEG,oldTimerOFF;
int main (int argc, char* argv[]){
		asm{										//difoltni deo, nema sta
			cli
			push es
			push ax

			mov ax,0
			mov es,ax

			mov ax, word ptr es:0022h
			mov word ptr oldTimerSEG, ax

			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFF, ax

			mov word ptr es:0022h, seg clock
			mov word ptr es:0020h, offset clock

			mov ax, oldTimerSEG
			mov word ptr es:0182h, ax

			mov ax, oldTimerOFF
			mov word ptr es:0180h, ax
			pop ax
			pop es
            sti
		}
	lock
	PCB :: running = new PCB();
	slices = 2;
	Thread* u = new UserThread(argc,argv);
	Thread* l = new Loop();
	unlock
	u->start();
	l->start();

	u->waitToComplete();
	//while(1);
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
	return ret;
}
