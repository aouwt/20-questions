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
char* correct = nullptr;

void mkhtml (void) {
	if (qno == 19) {
		QGame::character_t* guess = game.GuessWho ();
		
		printf ("<p>Are you <i>%s</i>?</p>\n", guess -> name);
		printf ("<form action=\"\">\n");
		printf ("  <label for=who>I am</label>\n");
		printf ("  <input id=who name=\"%s\" type=text list=people value=\"%s\">\n", ans, guess -> name);
		printf ("  <datalist id=people>\n");
		for (QGame::cid_t c = 0; c != game.Characters; c ++)
			printf ("    <option value=\"%s\">\n", game.Character [c].name);
		printf ("  </datalist>\n");
		printf ("  <button type=submit>Submit</button>\n");
		printf ("</form>\n");
		
		
	} else {
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
}



int main (void) {
	csv = fopen ("/usr/share/cgi-data/QGame/tdat.csv", "w+");
	if (csv == NULL) return 1;
	
	query = getenv ("QUERY_STRING");
	if (query == NULL) return 4;
	puts ("\n");
	
	game.Init ();
	game.SetQuestions (Questions);

	if (game.LoadCSV (csv)) return -1;
	
	
	switch (query [0]) {
		case 'y':
		case 'n':
		case 'u':
			// end-game parse arguments
			// build fmt string
			char fmt [20];
			
			// ANS=CHAR
			snprintf (fmt, LEN (fmt), "%%m[uyn]=%%%ums", QGAME_NAMELEN);
			if (sscanf (query, fmt, &ans, &correct) == EOF)
				return 1;
			
			break;
			
			
		default: // midgame parse arguments
			// QNO=ANS
			if (sscanf (query, "%i=%m[uyn]", &qno, &ans) == EOF)
				return 1;
			
			break;
	}
	
	ans_len = strlen (ans);
	if (ans_len > game.Questions) return 2;
	for (; ans_len != game.Questions; ans_len ++) ans [ans_len] = 'u'; // append with unknowns
	
	for (size_t i = 0; ans [i]; i ++) {
		switch (ans [i]) {
			case 'u': game.UserAnswer [i] = QGame::U; break;
			case 'y': game.UserAnswer [i] = QGame::T; break;
			case 'n': game.UserAnswer [i] = QGame::F; break;
			default: return 3;
		}
	}
	
	
	if (qno > 20) return 4;
	
	
	if (correct == nullptr) {
		mkhtml ();
		
	} else {
		puts ("Answer submitted.");
		QGame::character_t c;
		
		// copy
		strcpy (c.name, correct);
		for (QGame::qid_t q = 0; q != game.Questions; q ++)
			c.answer [q] = game.UserAnswer [q];
		
		game.TrainModel (&c);
		if (game.SaveCSV (csv)) return -2;
	}
	
	game.DeInit ();
	fclose (csv);
	free (ans);
	if (correct != nullptr) free (correct);
}
