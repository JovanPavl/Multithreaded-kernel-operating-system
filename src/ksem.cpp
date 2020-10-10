#include "ksem.h"
int max(int a,int b){
	if(a > b) return a;
	return b;
}
int KernelSem :: val() const{
//	updatelist();																	//NOVOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
	return value;
}
void KernelSem :: updatelist(){				
//	lockit
	int sz = waitsem.size();
	while(sz--){
		PCB* tmp = waitsem.front();
		waitsem.pop_front();
		if(tmp->state == PCB :: WAITING)
			waitsem.push_back(tmp);
		else{
			value++;						
		}			
	}
//	unlockit
}
KernelSem :: KernelSem(int init){
	value = init;
}

int KernelSem :: wait(Time maxTimeToWait){
	lockit
	updatelist();																	
	PCB :: running->signaled = 0;
	if(--value < 0){												
		if(PCB :: killme == PCB :: running){				
			value++;
			unlockit
			PCB :: dispatch();								//we will never get context back
		}
		if(maxTimeToWait == 0){
			block(maxTimeToWait);
			unlockit
			PCB :: dispatch();
			return 1;			
		}
		PCB :: sleepy.push_back(PCB :: running);
		block(maxTimeToWait);
		unlockit
		PCB :: dispatch();
		if(PCB :: running->signaled)				//otpustena signalom
			return 1;
		return 0;
	}
	unlockit
	return 1;
}
int KernelSem :: signal (int n){
	lockit
	updatelist();																//NOVOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
	if(n < 0){
		unlockit
		return n;
	}
	if(n == 0)
		n = 1;
	int use = value;
	value += n;
	if(use < 0){
		deblock(n);	//sve deblokiraj
		unlockit
		return n;
	}		
	deblock(max(0,-use));				//ne pokusavaj vise od 0 niti da zablokiras
	unlockit
	return -use;
}

void KernelSem :: block(Time maxTimeToWait){
	PCB :: running->state = PCB :: WAITING;
	PCB :: running->suspendedtime = maxTimeToWait;	
	waitsem.push_back(PCB :: running);
}

void KernelSem :: deblock(int x){
	while (x){
		PCB* tmp = waitsem.front();
		waitsem.pop_front();
		if(tmp->state == PCB :: WAITING){												//znaci stvarno ceka, nije pokrenuta
			tmp->signaled = 1;
			PCB :: push(tmp);
			x--;
	/*		if(tmp->suspendedtime){									//verovatno nepotrebno ali reko da mi se ne prepuni globalna lista
				while(PCB :: sleepy.front()->id != tmp->id){
					PCB* use = PCB :: sleepy.front();
					PCB :: sleepy.pop_front();
					PCB :: sleepy.push_back(use);
				}
				PCB :: sleepy.pop_front();
			}*/
		}
	}
//	unlockit
}