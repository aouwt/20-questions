#ifndef QGAMECSV_HPP
#define QGAMECSV_HPP
	#include "QGame.hpp"
	
	namespace QGameCSV {
		err_t LoadTD (QGame* g, FILE* f);
		err_t SaveTD (QGame* g, FILE* f);
		err_t LoadQs (QGame* g, FILE* f);
	}
#endif
