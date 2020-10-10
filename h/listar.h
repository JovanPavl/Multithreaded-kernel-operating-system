#ifndef listar_h
#define listar_h
typedef void (*SignalHandler)();
class ListFunctions {
public:	struct Elem{
		SignalHandler val;
		Elem* next, *prev;
		Elem(SignalHandler v, Elem* n = 0, Elem* p = 0){
			val = v;
			next = n, prev = p;
		}
	};
private: Elem* fir,*last;
		int sz;
public: ListFunctions(){
			fir = last = 0;
			sz = 0;
		}
		int size();
		void push_back(SignalHandler u);
		void pop_front();
		SignalHandler front();
		void deletelist();
		void swap(SignalHandler f, SignalHandler s);
		Elem* search (SignalHandler f);
		~ListFunctions();
		
};
#endif