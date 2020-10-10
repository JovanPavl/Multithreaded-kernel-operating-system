#ifndef Queue_h
#define Queue_h
class Queue{
public: struct Elem{
			int x;
			Elem* next, *prev;
			Elem(int X, Elem* n = 0, Elem* p = 0){
				next = n, prev = p;
				x = X;
			}
		};
private: Elem* fir,*last;
		int sz;

public: Queue(){
			fir = last = 0;
			sz = 0;
		}

		int size(){
			return sz;
		}
		int front(){
			return fir->x;
		}
		void pop();
		void push(int x);
		void deletelist();
		~Queue();
};
#endif