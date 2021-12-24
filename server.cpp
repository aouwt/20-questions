#include <string.h>
#include "src/common.hpp"
#include "src/QGame.hpp"
#include "src/QGameSQLite.hpp"

sqlite3 *db;
QGame game;
int qno = 0;
char* query = NULL;
char* ans = NULL; size_t ans_len = 0;
char* correct = NULL;

void mkhtml (void) {
	if (qno == 21) {
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
	
	if (sqlite3_open ("/usr/share/cgi-data/QGame/tdat.db", &db))
		return -1;
	
	query = getenv ("QUERY_STRING");
	if (query == NULL) return 4;
	puts ("\n");
	
	game.Init ();

	if (QGameSQLite::Load (&game, db)) return -1;
	
	
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
	for (; ans_len < game.Questions;) ans [ans_len ++] = 'u'; // append with unknowns
	
	ans [ans_len] = '\0';
	
	for (size_t i = 0;/* ans [i] != '\0'*/; i ++) {
		switch (ans [i]) {
			case '\0': goto done;
			case 'u': game.UserAnswer [i] = QGame::U; break;
			case 'y': game.UserAnswer [i] = QGame::T; break;
			case 'n': game.UserAnswer [i] = QGame::F; break;
			default: putchar (ans [i]); return 3;
		}
	}
	
	done:
	
	if (qno > 21) return 4;
	
	
	if (correct == NULL) {
		mkhtml ();
		
	} else {
		puts ("Answer submitted.");
		QGame::character_t c;
		
		// copy
		strcpy (c.name, correct);
		for (QGame::qid_t q = 0; q != game.Questions; q ++)
			c.answer [q] = game.UserAnswer [q];
		
		game.TrainModel (&c);
		return (QGameSQLite::SaveTD (&game, db));// return -2;
	}
	
	game.DeInit ();
	free (ans);
	sqlite3_close (db);
	if (correct != nullptr) free (correct);
}
