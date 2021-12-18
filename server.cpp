#include <string.h>
#include "common.hpp"
#include "QGame.hpp"

const char* Questions [] = {
//#include "questions.h"
"test",
"hi",
""
};


QGame game;
int qno = 0;
char* query = NULL;

void mkhtml (void) {
	printf ("<a href=\"?%s\">Different question</a><br />", query);
	printf ("<p>%s</p>", game.GetQuestion ());
}

// ?Q=S
// where Q is question number and S is the string of answers
int main (void) {
	query = getenv ("QUERY_STRING");
	puts ("\n");
	
	game.Init ();
	game.SetQuestions (Questions);
	
	{ // parse arguments
		char* str = nullptr;
		
		if (sscanf (query, "%u=%m[uyn]", &qno, &str) == EOF)
			exit (1);
		
		if (strlen (str) >= LEN (Questions) - 1) exit (1);
		
		for (size_t i = 0;; i ++) {
			switch (str [i]) {
				case '\0': goto done;
				case 'u': game.UserAnswer [i] = QGame::U; break;
				case 'y': game.UserAnswer [i] = QGame::T; break;
				case 'n': game.UserAnswer [i] = QGame::F; break;
				default: exit (1);
			}
		}
		
		
		done:
		free (str);
	}
	mkhtml ();
}
