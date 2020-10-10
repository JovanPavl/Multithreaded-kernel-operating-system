#include "listar.h"
SignalHandler ListFunctions :: front(){
	return fir->val;
}
int ListFunctions :: size(){
	return sz;
}
void ListFunctions :: push_back(SignalHandler u){
			if(last == 0){
				fir = last = new Elem(u);
				sz = 1;
			}
			else{
				Elem* cur = last->next = new Elem(u);
				cur->prev = last;
				last = cur;
				sz++;
			}	
}
void ListFunctions :: pop_front(){
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
void ListFunctions :: deletelist(){
			Elem* cur = fir;
			while(cur != 0){
				Elem* tmp = cur;
				cur = cur->next;
				delete tmp;
			}
			fir = last = 0;
			sz = 0;
		}
ListFunctions :: ~ListFunctions(){
			deletelist();
	}
ListFunctions :: Elem* ListFunctions :: search(SignalHandler f){
	Elem* cur = fir;
	while(cur != 0){
		if(cur->val == f)
			return cur;
		cur = cur->next;
	}
	return 0;
}

void ListFunctions :: swap(SignalHandler f, SignalHandler s){
	Elem* ff = search(f);
	Elem* ss = search(s);
	if(ff == 0 || ss == 0)
		return ;
	SignalHandler x = ff->val;
	ff->val = ss->val;
	ss->val = x;
}