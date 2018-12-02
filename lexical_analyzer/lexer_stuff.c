
void stuff_to_put_somewhere()
{
	/* stuff ... */

	/* input buffering stuff */
	switch(*lexeme_forward++)
	{
		case EOF: 
		{
			if (lexeme_forward == &buffer_one[BUFFER_SIZE - 1]){
				refill_buffer(buffer_two, the_open_file);
				lexeme_forward = buffer_two;
			}
			else if (lexeme_forward == &buffer_two[BUFFER_SIZE - 1]){
				refill_buffer(buffer_one, the_open_file);
				lexeme_forward = buffer_one;
			}
			else {
				terminate_lexer();
			}
		}
		/* TODO: cases for any other kind of character */
	}
	/* stuff ... */
}