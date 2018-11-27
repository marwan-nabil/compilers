#include "buffering.h"

static char buffer_one[BUFFER_SIZE];	/* using dual buffering on disk blocks */
static char buffer_two[BUFFER_SIZE];

static char *lexeme_begin;			/* always at the beginning of the next lexeme to match */
static char *lexeme_forward;		/* scans forward to try to match the longes lexeme possible */
static int the_open_file;			/* the open file descriptor */

/* refills a buffer from a file */
void refill_buffer(char *buffer)
{
	if ((result = read(the_open_file, buffer, BUFFER_SIZE)) > 0){
		return;
	}
	else if (result == 0){
		*buffer = EOF;
		buffer[BUFFER_SIZE-1] = EOF;
	}
	else {
		printf("ERROR: LEXER: refill_buffer: failed to read the file specified.\n");
		exit(1);
	}
}

/* opens a file and fills the two buffers and sets pointers*/
void open_file(const char *filename)
{
	if ((the_open_file = open(filename, O_RDONLY)) < 0){
		printf("ERROR: LEXER: open_file(): cannot open the file specified.\n");
		exit(1);
	}
}
