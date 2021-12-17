#include <stdio.h>
#include <stdlib.h>
#include "common.h"



static ans getans (void) {
	ans ret;

	#ifdef release
loop:
		if ((ret = GetCharAns (getchar ())) == TD_UNKNOWN) goto loop;

	#else
		ret = GetCharAns (getchar ());
	#endif

	while (getchar () != '\n');
	return ret;
}


static char* lcase (char* str) {
	for (uint i = 0; str [i]; i++)
		if (str [i] >= 'A' && str [i] <= 'Z') str [i] -= 'A' - 'a';
	return str;
}


static void init_td (void) { // creates initial CSV file
	// NOTE: .td is now hosed on github
	const char initialcsv [] = " ,?";
	
	FILE* f = fopen (TDFILE, "w");
	if (f == NULL) return;
	
	fprintf (f, "%s", initialcsv);
	
	fclose (f);
}


int main () {
begin:
	Init ();
	if (LoadTD ()) {
		printf ("Warning: Training data not found, should I create it? (Y/N)\t");
		if (getans () == TD_TRUE) {
			init_td ();
			goto begin;
		} else {
			puts ("Aborting");
			exit (1);
		}
	}
	
	for (uchar i = 0; i != 20; i++) {
		qid qu = GetNextQuestion ();
		printf ("%u\t%s (Y/N)\t", i+1, Questions [qu]);
		
		TrainModel (qu, getans ());
	}
	
	printf ("Are you %s? ", Characters[TargetCharacter].info.name);
	
	if (getans () == TD_TRUE) {
		puts ("Yay!");
		TrainCharacter (TargetCharacter);
		
	} else {
		char name [MAXNAMELEN+2];
		char fmt [20]; snprintf (fmt, LEN(fmt), "%%%u[^\n]", (uint)(MAXNAMELEN+1));

		do printf ("Aww...\nWho are you, then? ");
		while (scanf (fmt, name) == EOF);
		
		NewCharacter (lcase(name));
	}
	
	SaveTD ();
	
	DeInit ();
}
