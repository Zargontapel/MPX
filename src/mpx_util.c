#include "mpx_cmd.h"
#include "mpx_util.h"
#include "mpx_supt.h"
#include "mystdlib.h"
#include <string.h>
#include <stdio.h>

#define LINES_PER_PAGE 23
static int lines_printed;
static int pages_printed;
static int header_lines;
static char *page_header;


/** The pager function permits displaying output screen-full at a time.

The line to output MUST NOT end with a \n (newline) character.
*/
void mpx_pager(char *line_to_print) {

	if ( lines_printed == 0 ) {
		mpx_cls();
		printf("%s", page_header);
	}
	
	printf("%s\n", line_to_print);

	if ( (lines_printed != 0) && (lines_printed % (LINES_PER_PAGE-header_lines) == 0)) {
		lines_printed = 0;
		printf("<<Press enter for MORE>>"); mpxprompt_anykey();
	} else {
		lines_printed++;
	}
}

/** The pager initialization function must be used before the pager function.

If no per-page header is required, pass NULL for that parameter.

All lines in the header, including the last one, MUST end with a \n (newline) character.
*/
void mpx_pager_init(char *header) {
	char *cur_pos	= header;

	page_header	= header;
	lines_printed	= 0;
	pages_printed	= 0;
	header_lines	= 0;

	if (header != NULL) {
		while (*cur_pos != '\0') {
			if (*cur_pos == '\n') {
				header_lines++;
			}
			cur_pos++;
		}
	}
}

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

/**
*Decodes the errors thrown by various functions in the MPX suport files.
*/
void errorDecode(int err/**[in] The error value to decode.*/){
	switch( err ){
		case ERR_SUP_INVDEV:
			printf("Invalid device ID");
			break;
		case ERR_SUP_INVOPC:
			printf("Invalid operation Code");
			break;
		case ERR_SUP_INVPOS:
			printf("Invalid character postition");
			break;
		case ERR_SUP_RDFAIL:
			printf("Read Failed"); // could be sysrec or sys get entry
			break;
		case ERR_SUP_WRFAIL:
			printf("Write Failed");
			break;
		// ERR_SUP_INVMOD Exists in documentation but is not present in support code?
		case ERR_SUP_INVMEM:
			printf("Invalid memory block pointer");
			break;
		case ERR_SUP_FRFAIL:
			printf("Memory Freeing Op Failed");
			break;
		case ERR_SUP_INVDAT:
			printf("Invalid Date");
			break;
		case ERR_SUP_DATNCH:
			printf("Date not properly changed");
			break;
		case ERR_SUP_INVDIR:
			printf("Invalid name or no such directory");
			break;
		case ERR_SUP_DIROPN:
			printf("Error Opening Directory");
			break;
		case ERR_SUP_DIRNOP:
			printf("No directory is open");
			break;
		case ERR_SUP_NOENTR:
			printf("No more entries found");
			break;
		case ERR_SUP_NAMLNG:
			printf("The name was too long for the buffer");
			break;
		case ERR_SUP_DIRCLS:
			printf("Error closing the directory");
			break;
		default:
			printf("Unknown Error Code: %d /n",err);
			break;
	}
}
