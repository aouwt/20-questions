#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdio.h>
#include "QGame.hpp"


#define RANDOM(type) ({ \
		type n; \
		fread (&n, sizeof (type), 1, urand); \
		n; \
	})


void QGame::Init (void) {
	free (Character);
	Character = new character_t [CharactersAlloc = 10];
	Target = &Character [0];
	urand = fopen ("/dev/urandom", "r");
}



void QGame::DeInit (void) {
	free (Character);
	CharactersAlloc = 0;
	fclose (urand);
}



void QGame::SetQuestions (const char* q[]) {
	Question = q;
	for (Questions = 0; q [Questions] [0] != '\0'; Questions ++);
	Questions --;
}



void QGame::CopyCharacter (character_t* c, cid_t slot) {
	for (qid_t q = 0; q != Questions; q ++)
		Character[slot].answer [q] = c -> answer [q];
	
	for (size_t i = 0; i != QGAME_NAMELEN + 1; i ++)
		Character[slot].name [i] = c -> name [i];
}



err_t QGame::NewCharacter (character_t* c) {
	if (Characters+1 >= CharactersAlloc)
		if (reallocarray (&Character, sizeof(character_t), (CharactersAlloc += 10)) == NULL)
			return 1;
	
	CopyCharacter (c, ++ Characters);
	
	return 0;
}



err_t QGame::LoadCSV (FILE* f) {

	for (cid_t ent = 0;; ent ++) {
		
		char fmt [20];
		if (snprintf (fmt, LEN (fmt), "%%%u[^,],%%%u[ynu]\n", (unsigned int) (QGAME_NAMELEN + 1), (unsigned int) (Questions + 1)) == EOF)
			return -1;
		
		
		char ans [Questions + 2];
		character_t chr;
		
		if (fscanf (f, fmt, chr.name, ans) == EOF)
			break;
		
		
		for (qid_t q = 0; q != Questions; q ++) {
			switch (ans [q]) {
				case '\0':
					for (; q != Questions; q ++) // fill rest with U
						chr.answer [q] = U;
					goto eos;
					
				case 'y': chr.answer [q] = T; break;
				case 'n': chr.answer [q] = F; break;
				case 'u': default:
					chr.answer [q] = U; break;
			}
		}
		
		eos:
		
		if (NewCharacter (&chr)) return -2;
		
	}
	
	return 0;
}



err_t QGame::SaveCSV (FILE* f) {
	for (cid_t ent = 0; ent != Characters; ent ++) {
		char ans [Questions + 2];
		
		for (qid_t q = 0; q != Questions + 1; q ++) {
			switch (Character[ent].answer [q]) {
				case T: ans [q] = 'y'; break;
				case F: ans [q] = 'n'; break;
				case U: ans [q] = 'u'; break;
				default: return -1;
			}
		}
		
		ans [Questions + 1] = '\0';
		
		if (fprintf (f, "%s,%s\n", Character[ent].name, ans) < 0) return 1;
	}
	
	return 0;
}


const char* QGame::GetQuestion (void) {
	qid_t q;
	return GetQuestion (&q);
}


const char* QGame::GetQuestion (qid_t *id) {
	for (;;) {
		*id = RANDOM (qid_t) % Questions;
		
		if (UserAnswer [*id] != U) continue; // dont ask duplicates
		
		if ((RANDOM (u16) % QGAME_RANDQCHANCE) == 0) {
			if (Target -> answer [*id] == U) return Question [*id];
		} else
			return Question [*id];
	}
}



void QGame::SubmitAns (qid_t q, answer_t a) {
	
}



QGame::character_t* QGame::GuessWho (void) {
	
}



char* lcase (char* str) {
	for (size_t i = 0; str [i]; i ++)
		if (str [i] >= 'A' && str [i] <= 'Z')
			str [i] -= 'A' - 'a';
	return str;
}

err_t QGame::TrainModel (character_t* correct) {
	lcase (correct -> name);
	
	// See if we can find the character as preexisting
	cid_t c = 0;
	for (; c != Characters; c ++)
		if (!strcmp (Character[c].name, correct -> name)) goto has;
	
	// does not have
	if (NewCharacter (correct)) return 1;
	
	has:
	CopyCharacter (correct, c);
	for (qid_t q = 0; q != Questions; q ++) {
		if (correct -> answer [q] != U) {
			
			if (Character[c].answer [q] == U) // if we didnt know it before, set it now
				Character[c].answer [q] = correct -> answer [q];
			else
			if (Character[c].answer [q] != correct -> answer [q]) // if they conflict then set to unknown
				Character[c].answer [q] = U;
		
		}
	}
	
	return 0;
}

