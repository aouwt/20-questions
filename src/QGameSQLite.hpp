#ifndef QGAMESQLITE_HPP
#define QGAMESQLITE_HPP
	#include <sqlite3.h>
	#include "QGame.hpp"
	
	namespace QGameSQLite {
		err_t LoadTD (QGame* game, sqlite3* db);
		err_t SaveTD (QGame* game, sqlite3* db);
		err_t LoadQs (QGame* game, sqlite3* db);
		err_t Load (QGame* game, sqlite3* db);
	}
#endif
