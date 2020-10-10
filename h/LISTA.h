/*
 * LISTA.h
 *
 *  Created on: Apr 8, 2020
 *      Author: OS1
 */

#ifndef LISTA_H_
#define LISTA_H_
#include <iostream.h>
template <class T>
class List{
public:	struct Elem{
		T* val;
		Elem* next, *prev;
		Elem(T* v, Elem* n = 0, Elem* p = 0){
			val = v;
			next = n, prev = p;
		}
/*		~Elem(){
			delete val;
		}*/
	};
private: Elem* fir,*last;
		int sz;
public: List(){
			fir = last = 0;
			sz = 0;
		}
		int size(){
			return sz;
		}
		void push_back(T* u){
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
		void pop_front();
		void deletelement(T* u);
		T* front(){
			return fir->val;
		}
		T* getelement(int x);
		~List();
};
template<class T> 
void List<T> :: pop_front(){
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
template<class T>
T*  List <T> :: getelement(int x){
			int i = 0;
			Elem* cur;
			for(; i < x; i++)
				cur = cur->next;
			return cur->val;
		}

template <class T>
List<T> :: ~List(){
			Elem* cur = fir;
			while(cur != 0){
				Elem* tmp = cur;
				cur = cur->next;
				delete tmp;
			}
		}
		
template <class T>
void  List <T> :: deletelement(T* u){
	Elem* cur = fir;
	while(cur != 0 && cur->val != u){
		cur = cur->next;
	}
	if(cur == 0)
		return;
	if(cur == fir){
		if(fir == last){
			delete cur;
			fir = last = 0;
			return;
		}
		if(cur == last){
			last = last->prev;
			last->next = 0;
			delete cur;
			return;
		}
		Elem* tmp1 = cur->prev;
		Elem* tmp2 = cur->next;
		tmp1->next = tmp2;
		tmp2->prev = tmp1;
		delete cur;
	}
}	
#endif /* LISTA_H_ */
