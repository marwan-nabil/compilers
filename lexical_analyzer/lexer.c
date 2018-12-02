#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define MAX_GETCH_BUFFER 4
#define BUFFER_SIZE 4096	/* 4 kilobytes, a disk block for efficient access */

typedef enum 
{
	TKN_RELOP, TKN_ID, TKN_FOR, TKN_IF, TKN_ELSE /* ... and so on for all token names */
} TokenName;

typedef struct 
{
	TokenName token_name;
	void *attribute_value;
} Token;

typedef struct 
{
	char *type;
	char *value;
} Symbol;

Symbol *symbol_table;

char getch_buffer[MAX_GETCH_BUFFER];
unsigned int getch_buffer_ptr = 0;

char buffer_one[BUFFER_SIZE];	/* using dual buffering on disk blocks */
char buffer_two[BUFFER_SIZE];

char *lexeme_begin;			/* always at the beginning of the next lexeme to match */
char *lexeme_forward;		/* scans forward to try to match the longes lexeme possible */
int the_open_file;			/* the open file descriptor */

/* refills a buffer from a file */
void refill_buffer(char *buffer)
{
	if ((result = read(the_open_file, buffer, BUFFER_SIZE)) > 0)
	{
		return;
	}
	else if (result == 0)
	{
		*buffer = EOF;
		buffer[BUFFER_SIZE-1] = EOF;
	}
	else
	{
		printf("ERROR: LEXER: refill_buffer: failed to read the file specified.\n");
		exit(1);
	}
}

/* opens a file and fills the two buffers and sets pointers */
void open_file(const char *filename)
{
	if ((the_open_file = open(filename, O_RDONLY)) < 0)
	{
		printf("ERROR: LEXER: open_file(): cannot open the file specified.\n");
		exit(1);
	}
}

void init_symbol_table()
{
	symbol_table = (Symbol *) malloc(20 * sizeof(Symbol));
}

// TODO: input character buffering
char get_next_char()
{
	
}

void unget_last_char()
{

}

/* an FSM to match and discard the white space regular expression
   this is translated directly from a state transition 
   diagram describing the machine: 

start--> (1)------->(2)-------->((3))*
			   ws   |^   other
					||
					--
					ws

 */
void skip_white_space()
{
	unsigned int state = 1;
	char c;
	for(;;)
	{
		switch(state)
		{
			case 1:
			{
				c = get_next_char();
				if(c == ' ' || c == '\t' || c == '\n')
				{	
					state = 2;
				}
				else
				{
					unget_last_char();
					return;
				}
			} break;

			case 2:
			{
				c = get_next_char();
				if(c == ' ' || c == '\t' || c == '\n')
				{	
					state = 2;
				}
				else
				{
					state = 3;
				}
			} break;

			case 3:
			{
				unget_last_char();
				return;
			} break;

			default:
			{
				printf("ERROR: LEXER: Internal fuckup that is very unlikely to happen.\n");
				exit(1);
			}
		}	
	}
}

