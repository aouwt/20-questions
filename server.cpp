#include <string.h>
#include "common.hpp"
#include "QGame.hpp"

const char* Questions [] = {
#include "questions.h"
,"" };


FILE* csv;
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
		
		resp [q] = 'y'; printf ("<a href=\"?%s%s\">Yes</a>\n", nextq, resp);
		resp [q] = 'n'; printf ("<a href=\"?%s%s\">No</a>\n", nextq, resp);
	}
}


// ?Q=S
// where Q is question number and S is the string of answers
int main (void) {
	csv = fopen ("/usr/share/cgi-data/QGame/tdat.csv", "rw");
	if (csv == NULL) return 1;
	
	query = getenv ("QUERY_STRING");
	if (query == NULL) return 4;
	puts ("\n");
	
	game.Init ();
	game.SetQuestions (Questions);

	if (game.LoadCSV (csv)) return -1;
	
	{ // parse arguments
		
		if (sscanf (query, "%u=%m[uyn]", &qno, &ans) == EOF)
			return 1;
		
		ans_len = strlen (ans);
		if (ans_len >= LEN (Questions) - 1) return 2;
		
		for (; ans_len != LEN (Questions) - 2; ans_len ++) ans [ans_len] = 'u'; // append with unknowns
		
		for (size_t i = 0; ans [i]; i ++) {
			switch (ans [i]) {
				case 'u': game.UserAnswer [i] = QGame::U; break;
				case 'y': game.UserAnswer [i] = QGame::T; break;
				case 'n': game.UserAnswer [i] = QGame::F; break;
				default: return 3;
			}
		}
	}
	mkhtml ();
	
	fclose (csv);
	free (ans);
}
