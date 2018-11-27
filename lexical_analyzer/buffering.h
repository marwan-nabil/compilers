#include <unistd.h>			/* for system calls */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFFER_SIZE 4096	/* 4 kilobytes, a disk block for efficient access */

extern char buffer_one[BUFFER_SIZE];	/* using dual buffering on disk blocks */
extern char buffer_two[BUFFER_SIZE];

extern char *lexeme_begin;			/* always at the beginning of the next lexeme to match */
extern char *lexeme_forward;		/* scans forward to try to match the longes lexeme possible */
extern int the_open_file;			/* the open file descriptor */

extern void refill_buffer(char *buffer);
extern void open_file(const char *filename);