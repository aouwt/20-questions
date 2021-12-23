#include "QGameSQLite.hpp"

err_t QGameSQL::LoadTD (QGame* game, sqlite3* db) {
	
	if (sqlite3_exec (db, "SELECT * FROM people;", loadchr, NULL, NULL))
		return 1;
}
