#include <string.h>
#include <stdlib.h>
#include "QGameSQLite.hpp"


size_t qalloc = 0;


// callbacks
static int loadq (void* game, int cols, char** field, char** col) {
	if (strcmp (col [0], "id"))
		return 1;
	
	if (strcmp (col [1], "text"))
		return 2;

	return ((QGame*) game) -> NewQuestion (field [1]);
}

static int loadchr (void* game, int cols, char** field, char** col) {
	QGame::character_t c;

	if (strcmp (col [0], "name"))
		return 1;
	
	if (strcmp (col [1], "answers"))
		return 2;
	
	strcpy (c.name, field [0]);

	for (size_t i = 0; i != ((QGame*) game) -> Questions; i ++) {
		switch (field [1] [i]) {
			case 't': c.answer [i] = QGame::T; break;
			case 'f': c.answer [i] = QGame::F; break;
			case 'u': c.answer [i] = QGame::U; break;
			default: return 3;
		}
	}

	((QGame*) game) -> NewCharacter (&c);
	return 0;
}


err_t QGameSQLite::LoadTD (QGame* game, sqlite3* db) {
	if (sqlite3_exec (db, "SELECT * FROM people;", loadchr, (void*) game, NULL))
		return 1;
	
	return 0;
}

err_t QGameSQLite::LoadQs (QGame* game, sqlite3* db) {	
	if (sqlite3_exec (db, "SELECT * FROM questions;", loadq, (void*) game, NULL))
		return 1;
	
	return 0;
}


err_t QGameSQLite::Load (QGame* game, sqlite3* db) {
	if (QGameSQLite::LoadQs (game, db)) return 1;
	if (QGameSQLite::LoadTD (game, db)) return 2;
	return 0;
}
