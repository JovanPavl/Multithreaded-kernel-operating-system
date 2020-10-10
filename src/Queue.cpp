#include "Queue.h"
void Queue :: push(int x){
			if(last == 0){
				fir = last = new Elem(x);
				sz = 1;
			}
			else{
				Elem* cur = last->next = new Elem(x);
				cur->prev = last;
				last = cur;
				sz++;
			}	
}
void Queue :: pop(){
			if(fir == 0)
				return;
			if(fir == last){
				delete fir;
				fir = last = 0;
				sz--;
				return ;
			}
			Elem* cur = fir;
			cur = fir->next;
			fir = cur;
			delete fir->prev;
			sz--;
		}
void Queue :: deletelist(){
			Elem* cur = fir;
			while(cur != 0){
				Elem* tmp = cur;
				cur = cur->next;
				delete tmp;
			}
			fir = last = 0;
			sz = 0;
		}
Queue :: ~Queue(){
	deletelist();
}