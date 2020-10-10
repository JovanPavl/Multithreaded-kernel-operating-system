#include "loop.h"
void Loop :: run(){
	while(PCB :: getThreadById(0)->state == PCB :: WAITING){				//first time it's called and there is no job we're done
	
	}
}