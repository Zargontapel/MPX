#include "mpx_cmd.h"
#include "mpx_r1.h"
#include "mpx_supt.h"
#include "mystdlib.h"
#include <string.h>
#include <stdio.h>

/** The function Prompt y n prompts the user to answer a Yes or No question. */
int mpxprompt_yn(void) {
	char yn = mpxprompt_anykey();
	if( yn == 'Y' || yn == 'y' ) {
		return 1; /* true */
	} else {
		return 0; /* false */
	}
}

/** The function Prompt Any key Prompts the user to press the return key. */
char mpxprompt_anykey(void) {
	/* user must press enter. */
	int buflen = 3;
	char buf[5];
	buf[0] = ' ';
	sys_req(READ, TERMINAL, buf, &buflen);
	return buf[0];
}

/** The function Prompt int reads the in the input from the user. */
int mpxprompt_int(void) {
	char input[MAX_LINE];
	mpx_readline(input, MAX_LINE);	
	return atoi(input);
}

/** Readline function reads in a line from the Terminal. */
void mpx_readline ( char *buffer /**[inout] Points to the sting being read.  */, int buflen /** [in] Defines the maximum characters read. */ ) {
	int local_buflen = buflen;
	sys_req(READ, TERMINAL, buffer, &local_buflen);

	/* remove newline from end of string. */
	if( buffer[strlen(buffer)-1] == '\n' || buffer[strlen(buffer)-1] == '\r' ) {
		buffer[strlen(buffer)-1] = '\0';
	} /* FIXME: strlen() is unsafe; should use strnlen(). */
}

/** Clear, blanks the screen */
int mpx_cls (void) {
	/* fixme: add error catching */
	int err = sys_req(CLEAR, TERMINAL, NULL, 0);
	
	if ( err != OK ) return err;
	
	return OK;
}

