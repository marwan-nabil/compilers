/* This is a simple recursive descent parser for the following grammar:
	S -> T END | T + E END
	E -> T | T + E
	T -> int | int * T | (E)
*/

#include <stdio.h>
#include <ctype.h>
typedef enum {INTEGER, PLUS, OPEN, CLOSE, TIMES, END} TOKEN;
typedef _Bool bool;

bool S(void);
bool T(void);
bool E(void);

/* token stream of expression : int+(int*int) */
TOKEN valid_str[] = { INTEGER, PLUS, INTEGER, PLUS, INTEGER, TIMES, INTEGER, END};
/* token stream of invalid expression : (() */
TOKEN invalid_str[] = { INTEGER, OPEN, OPEN, CLOSE, END};
/* token stream pointer */
TOKEN *next;

bool term(TOKEN t) { return *next++ == t; }
bool E1() { return T(); }
bool E2() { return T() && term(PLUS) && E(); }
bool E()
{ 
	TOKEN *save = next;
	return E1() || 		/* if E1() failed, undo pointer increments and try E2() */
	 (next = save, E2());
}
bool T1() { return term(INTEGER); }
bool T2() { return term(INTEGER) && term(TIMES) && T(); }
bool T3() { return term(OPEN) && E() && term(CLOSE); }
bool T()
{
	TOKEN *save = next;
	return T1() ||
		(next = save, T2()) ||
		(next = save, T3());
}
bool S1(){ return T()&&term(END); }
bool S2(){ return T()&&term(PLUS)&&E()&&term(END); }
bool S()
{
	TOKEN *save = next;
	return S1() ||
		(next = save, S2());
}

int main()
{
	printf("\ntrying a valid expression.");
	next = valid_str;
	if (S()){
		printf("\nparse succeeded.");
	}
	else
	{
		printf("\nparse failed\n");
	}

	printf("\ntrying an invalid expression.");
	next = invalid_str;
	if (S()){
		printf("\nparse succeeded.");
	}
	else {
		printf("\nparse failed\n");
	}
}

