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
char* query = nullptr;
char* ans = nullptr; size_t ans_len = 0;

void mkhtml (void) {
	QGame::qid_t q;
	printf ("<a href=\"?%s\">Different question</a><br />", query);
	printf ("<p>%s</p>", game.GetQuestion (&q));
	
	{
		char nextq [5]; sprintf (nextq, "%i=", qno + 1);
		
		char resp [ans_len + 2];
		strcpy (resp, ans);
		
		resp [q] = 'y'; printf ("<a href=\"?%s%s\">Yes</a>", nextq, resp);
		resp [q] = 'n'; printf ("<a href=\"?%s%s\">No</a>", nextq, resp);
	}
}


// ?Q=S
// where Q is question number and S is the string of answers
int main (void) {
	query = getenv ("QUERY_STRING");
	puts ("\n");
	
	game.Init ();
	game.SetQuestions (Questions);
	
	{ // parse arguments
		
		if (sscanf (query, "%u=%m[uyn]", &qno, &ans) == EOF)
			exit (1);
		
		if ((ans_len = strlen (ans)) >= LEN (Questions) - 1) exit (1);
		
		for (size_t i = 0;; i ++) {
			switch (ans [i]) {
				case '\0': goto done;
				case 'u': game.UserAnswer [i] = QGame::U; break;
				case 'y': game.UserAnswer [i] = QGame::T; break;
				case 'n': game.UserAnswer [i] = QGame::F; break;
				default: exit (1);
			}
		}
		
		
		done:;
	}
	mkhtml ();
	
	free (ans);
}
