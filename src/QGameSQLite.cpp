#include <string.h>
#include <stdlib.h>
#include "QGameSQLite.hpp"


size_t qalloc = 0;


// callbacks
static int loadq (void* game, int cols, char** field, char** col) {

	if (cols < 2)
		return 1;
	
	if (strcmp (col [0], "id"))
		return 2;
	
	if (strcmp (col [1], "text"))
		return 3;

	if (QGame* (game) -> NewQuestion (field [1]))
		return 4;
	
	return 0;
}

static int loadchr (void* game, int cols, char** field, char** col) {
	QGame::character_t c;
	
	if (cols < 2)
		return 1;

	if (strcmp (col [0], "name"))
		return 2;
	
	if (strcmp (col [1], "answers"))
		return 3;
	
	strcpy (c.name, field [0]);

	for (size_t i = 0; i != QGame* (game) -> Questions; i ++) {
		switch (field [1] [i]) {
			case 'y': c.answer [i] = QGame::T; break;
			case 'n': c.answer [i] = QGame::F; break;
			case 'u': c.answer [i] = QGame::U; break;
			default: return 4;
		}
	}

	if (QGame* (game) -> NewCharacter (&c))
		return 5;

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


err_t QGameSQLite::SaveTD (QGame* game, sqlite3* db) {
	if (sqlite3_exec (db,
			"BEGIN TRANSACTION;"
			"DROP TABLE people;"
		NULL, NULL, NULL));
			return 1;

	for (QGame::cid_t c = 0; c != QGame* (game) -> Characters; c ++) {

		char ans [QGame* (game) -> Questions + 2];
		for (QGame::qid_t q = 0; q != QGame* (game) -> Questions; q ++) {
			switch (QGame* (game) -> Character [c].answer [q]) {
				case QGame::T: ans [q] = 't'; break;
				case QGame::F: ans [q] = 'f'; break;
				case QGame::U: ans [q] = 'u'; break;
			}
		}
		ans [QGame* (game) -> Questions + 1] = '\0';

		sqlite3_stmt* s;

		if (sqlite3_prepare_v2 (db, "INSERT INTO people VALUES (?, ?)", -1, &s, NULL))
			return 2;

		if (sqlite3_bind_text (s, 1, QGame* (game) -> Character[c].name, -1, SQLITE_STATIC))
			return 3;

		if (sqlite3_bind_text (s, 2, ans, -1, SQLITE_STATIC))
			return 4;

		if (sqlite3_step (s))
			return 5;

		if (sqlite3_finalize (s))
			return 6;

		/*if (snprintf (sql, LEN (sql), "INSERT INTO people VALUES (\"%s\", \"%s\");", QGame* (game) -> Character [c].name, ans) == EOF)
			return 2;

		if (sqlite3_exec (db, sql, NULL, NULL, NULL))
			return 3;*/

	}

	if (sqlite3_exec (db, "COMMIT;", NULL, NULL, NULL))
		return 6;

	return 0;
}




err_t QGameSQLite::Load (QGame* game, sqlite3* db) {
	if (QGameSQLite::LoadQs (game, db)) return 1;
	if (QGameSQLite::LoadTD (game, db)) return 2;
	return 0;
}
