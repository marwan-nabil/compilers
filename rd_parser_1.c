/* This is a simple recursive descent parser for the following grammar:
	E -> T | T + E
	T -> int | int * T | (E)
	
	the grammar has a flaw, in that it could accept malformed expressions, 
	because it would be short circuited by correct sub-expressions at the beginning 
	of the bad expression, it is lazy, for example :
	int (()
	would be accepted, because int is a valid E, the parser was short circuited here.
	the solution to this is a proper expression ending terminal introduced into the grammar.
*/

#include <stdio.h>
#include <ctype.h>
typedef enum {INTEGER, PLUS, OPEN, CLOSE, TIMES} TOKEN;
typedef _Bool bool;

bool T(void);
bool E(void);

/* token stream of expression : int+((int+int)*int) */
TOKEN str[] = { INTEGER, PLUS, OPEN, OPEN, INTEGER, PLUS, INTEGER, CLOSE, TIMES, INTEGER, CLOSE};
/* token stream of invalid expression : (() */
TOKEN invalid_str[] = { OPEN, OPEN, CLOSE};
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

int main()
{
	printf("\ntrying a valid expression.");
	next = str;
	if (E() == 1){
		printf("\nparse succeeded.");
	}

	printf("\ntrying an invalid expression.");
	next = invalid_str;
	if (E() == 1){
		printf("\nparse succeeded.");
	}
	else {
		printf("\nparse failed\n");
	}
}

