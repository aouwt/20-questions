#include <string.h>
#include <stdlib.h>
#include "QGameSQLite.hpp"

err_t QGameSQL::LoadTD (QGame* game, sqlite3* db) {

	static int loadchr (void* unused, int cols, char** feild, char** col) {
		QGame::character_t ch;

		if (!strcmp (col [0], "name"))
			return 1;
		strcpy (ch.name, feild [0]);

		for (int c = 1; c != cols, c ++) {

			QGame::qid_t q = atoi (col [c]);

			if (q > game -> Questions)
				return 2;

			switch (feild [c] [0]) {
				case 't': ch.answer [q] = QGame::T; break;
				case 'f': ch.answer [q] = QGame::F; break;
				case 'u': ch.answer [q] = QGame::U; break;
				default: return 3;
			}

		}

		game -> NewCharacter (&ch);
	}

	if (sqlite3_exec (db, "SELECT * FROM people;", loadchr, NULL, NULL))
		return 1;
}
