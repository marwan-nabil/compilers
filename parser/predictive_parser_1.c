/*
This is a predictive parser for the LL(1) grammar:
	E -> TX
	T -> (E) | INT Y
	X -> +E | epsilon
	Y -> *T | epsilon
which is a modified version of the grammar:
	E -> T+E | T
	T -> INT | INT*T | (E)
*/
#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 100
#define TB_SIZE 10

typedef enum { INTEGER, OPEN, CLOSE, MUL, PLUS, MINUS, INPUT_END, E, T, Y, X, TB_END} Symbol;
typedef enum { TERMINAL, NONTERMINAL} Type;
typedef struct
{
	Symbol 	s;
	Type 	t;	
} StackElement;

StackElement stack[STACK_SIZE];
unsigned int stack_pointer = 0;
StackElement table_buffer[TB_SIZE];

StackElement pop(void)
{
	if(stack_pointer <= 0)
	{
		printf("ERROR: stack underflow.\n");
		exit(1);
	}
	return stack[--stack_pointer];
}

void push(StackElement e)
{
	if(stack_pointer >= STACK_SIZE)
	{
		printf("ERROR: stack overflow.\n");
		exit(1);
	}
	stack[stack_pointer++] = e;
}

// returns the string of symbols from the parsing table entry in table_buffer
void parse_table_lookup(Symbol leftmost_nonterminal, Symbol next_token)
{
	switch(leftmost_nonterminal)
	{
		// table rows
		case E:
		{
			// table columns
			switch(next_token)
			{
				case INTEGER:
				{
					table_buffer[0] = (StackElement) {T, NONTERMINAL};
					table_buffer[1] = (StackElement) {X, NONTERMINAL};
					table_buffer[2] = (StackElement) {TB_END, TERMINAL};
				} break;
				case OPEN:
				{
					table_buffer[0] = (StackElement) {T, NONTERMINAL};
					table_buffer[1] = (StackElement) {X, NONTERMINAL};
					table_buffer[2] = (StackElement) {TB_END, TERMINAL};
				} break;
				default:
				{
					printf("ERROR: table lookup: parse error.\n");
					exit(1);
				} break;
			}
		} break;
		case X:
		{
			// table columns
			switch(next_token)
			{
				case PLUS:
				{
					table_buffer[0] = (StackElement) {PLUS, TERMINAL};
					table_buffer[1] = (StackElement) {E, NONTERMINAL};
					table_buffer[2] = (StackElement) {TB_END, TERMINAL};
				} break;
				case CLOSE:
				{
					table_buffer[0] = (StackElement) {TB_END, TERMINAL};
				} break;
				case INPUT_END:
				{
					table_buffer[0] = (StackElement) {TB_END, TERMINAL};
				} break;
				default:
				{
					printf("ERROR: table lookup: parse error.\n");
					exit(1);
				} break;
			}
		} break;
		case T:
		{
			// table columns
			switch(next_token)
			{
				case INTEGER:
				{
					table_buffer[0] = (StackElement) {INTEGER, TERMINAL};
					table_buffer[1] = (StackElement) {Y, NONTERMINAL};
					table_buffer[2] = (StackElement) {TB_END, TERMINAL};
				} break;
				case OPEN:
				{
					table_buffer[0] = (StackElement) {OPEN, TERMINAL};
					table_buffer[1] = (StackElement) {E, NONTERMINAL};
					table_buffer[2] = (StackElement) {CLOSE, TERMINAL};
					table_buffer[3] = (StackElement) {TB_END, TERMINAL};
				} break;
				default:
				{
					printf("ERROR: table lookup: parse error.\n");
					exit(1);
				} break;
			}
		} break;
		case Y:
		{
			// table columns
			switch(next_token)
			{
				case MUL:
				{
					table_buffer[0] = (StackElement) {MUL, TERMINAL};
					table_buffer[1] = (StackElement) {T, NONTERMINAL};
					table_buffer[2] = (StackElement) {TB_END, TERMINAL};
				} break;
				case PLUS:
				{
					table_buffer[0] = (StackElement) {TB_END, TERMINAL};
				} break;
				case CLOSE:
				{
					table_buffer[0] = (StackElement) {TB_END, TERMINAL};
				} break;
				case INPUT_END:
				{
					table_buffer[0] = (StackElement) {TB_END, TERMINAL};
				} break;
				default:
				{
					printf("ERROR: table lookup: parse error.\n");
					exit(1);
				} break;
			}
		} break;
	}
}

int main()
{
	// this is the string to parse
	// int * int + (int + int * int) $
	Symbol string[] = {INTEGER, MUL, INTEGER, PLUS, OPEN, INTEGER, PLUS, INTEGER, MUL, INTEGER, CLOSE, INPUT_END}; 
	Symbol *next_token = string;
	// initialize stack with start symbol E
	push((StackElement) {INPUT_END, TERMINAL});
	push((StackElement) {E, NONTERMINAL});
	// parser loop
	do 
	{
		StackElement leftmost = pop();
		
		if(leftmost.t == TERMINAL)
		{
			if(leftmost.s == *next_token++)
				;
			else
			{
				printf("ERROR: parser loop: parse error.\n");
				exit(1);
			}
		}
		else	// if it's non terminal
		{
			parse_table_lookup(leftmost.s, *next_token);
			// push from the table buffer to stack
			int i = 0;
			for(; table_buffer[i].s != TB_END; i++) // get to buffer end and push backwards
				;
			// i is now pointing to the buffer end symbol
			i--;
			for(; i >= 0; i--)
			{
				push(table_buffer[i]);
			}
		}
	} while(stack[stack_pointer-1].s != (Symbol) INPUT_END);  // until reaching the end of stack successfully
	printf("Parse succeeded, your string belongs to my language.\n");
}