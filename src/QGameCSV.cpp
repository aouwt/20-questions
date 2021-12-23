#include <stdio.h>
#include "QGame.hpp"
#include "QGameCSV.hpp"

err_t QGameCSV::Load (QGame* g, FILE* f) {

	for (QGame::cid_t ent = 0;; ent ++) {
		
		char fmt [20];
		if (snprintf (fmt, LEN (fmt), "%%%u[^,],%%%u[ynu]\n", (unsigned int) (QGAME_NAMELEN + 2), (unsigned int) (g -> Questions + 1)) == EOF)
			return -1;
		
		
		char ans [g -> Questions + 2];
		QGame::character_t chr;
		
		if (fscanf (f, fmt, chr.name, ans) == EOF)
			break;
		
		
		for (QGame::qid_t q = 0; q != g -> Questions; q ++) {
			switch (ans [q]) {
				case '\0':
					for (q --; q <= g -> Questions; q ++) // fill rest with U
						chr.answer [q] = QGame::U;
					goto eos;
					
				case 'y': chr.answer [q] = QGame::T; break;
				case 'n': chr.answer [q] = QGame::F; break;
				case 'u': default:
					chr.answer [q] = QGame::U; break;
			}
		}
		
		eos:
		
		if (g -> NewCharacter (&chr)) return -2;
		
	}
	
	return 0;
}



err_t QGameCSV::Save (QGame* g, FILE* f) {
	for (QGame::cid_t ent = 0; ent != g -> Characters; ent ++) {
		char ans [g -> Questions + 2];
		
		for (qid_t q = 0; q != g -> Questions; q ++) {
			switch (g -> Character [ent].answer [q]) {
				case QGame::T: ans [q] = 'y'; break;
				case QGame::F: ans [q] = 'n'; break;
				case QGame::U: ans [q] = 'u'; break;
				default: return -1;
			}
		}
		
		ans [g -> Questions + 1] = '\0';
		
		if (fprintf (f, "%s,%s\n", g -> Character [ent].name, ans) < 0) return 1;
	}
	
	return 0;
}
