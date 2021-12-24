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
	Character = (character_t*) malloc ((CharactersAlloc = 10) * sizeof (character_t));
	Characters = 0;
	Question = (char**) malloc ((QuestionsAlloc = 20) * sizeof (char*));
	Questions = 0;
	
	Target = &Character [0];
	urand = fopen ("/dev/urandom", "r");
}



void QGame::DeInit (void) {
	free (Character);
	Characters = 0;
	CharactersAlloc = 0;
	
	for (qid_t q = 0; q != Questions; q ++) free (Question [q]);
	free (Question);
	Questions = 0;
	QuestionsAlloc = 0;
	
	fclose (urand);
}



/*void QGame::SetQuestions (const char* q[]) {
	Question = q;
	for (Questions = 0; q [Questions] [0] != '\0'; Questions ++)
		;
}*/



err_t QGame::NewQuestion (char str []) {
	if ((size_t) Questions + 1 >= QuestionsAlloc) {
		if ((Question = (char**) reallocarray (Question, sizeof (Question [0]), QuestionsAlloc += 10)) == NULL) {
			Questions = 0;
			QuestionsAlloc = 0;
			return 1;
		}
	}
	
	Question [Questions] = (char*) malloc (sizeof (char) * (strlen (str) + 1));
	strcpy (Question [Questions], str);
	
	Questions ++;
	
	return 0;
}



void QGame::CopyCharacter (character_t* c, cid_t slot) {
	for (qid_t q = 0; q != Questions; q ++)
		Character[slot].answer [q] = c -> answer [q];
	
	for (size_t i = 0; i != QGAME_NAMELEN + 1; i ++)
		Character[slot].name [i] = c -> name [i];
}



err_t QGame::NewCharacter (character_t* c) {
	if ((size_t) Characters + 1 >= CharactersAlloc) {
		if ((Character = (character_t*) reallocarray (Character, sizeof (Character [0]), CharactersAlloc += 10)) == NULL) {
			Characters = 0;
			CharactersAlloc = 0;
			return 1;
		}
	}
	
	CopyCharacter (c, Characters ++);
	
	return 0;
}



const char* QGame::GetQuestion (void) {
	qid_t q;
	return GetQuestion (&q);
}


const char* QGame::GetQuestion (qid_t *id) {
	for (qid_t q = 0; q != Questions; q ++)
		if (UserAnswer [q] == U) goto ok;
	
	return NULL;
	
	ok:
	for (;;) {
		*id = RANDOM (qid_t) % Questions;
		
		if (UserAnswer [*id] != U) continue; // dont ask duplicates
		
		if ((RANDOM (u16) % QGAME_RANDQCHANCE) == 0) {
			if (Target -> answer [*id] == U) return Question [*id];
		} else
			return Question [*id];
	}
}



float QGame::CalculateChance (character_t* c) {
	qid_t num = 0; qid_t denom = 0;
	
	for (qid_t q = 0; q != Questions; q ++) {
		
		if (UserAnswer [q] == U || c -> answer [q] == U) continue;
		
		denom ++;
		
		if (UserAnswer [q] == c -> answer [q])
			num ++;
		else // if conflicting answers
			num = ((int) num - (int) (Questions / 2) < 0) ? 0 : num - (Questions / 2); // remove chance by half of questions, and make sure it doesnt go negative
		
	}
	
	
	return (float) num / (float) denom;
}



void QGame::SubmitAns (qid_t q, answer_t a) {
	UserAnswer [q] = a;
	GetHighest ();
}



QGame::character_t* QGame::GuessWho (void) {
	GetHighest ();
	return Target;
}



void QGame::GetHighest (void) {
	float curhighest = 0;
	for (cid_t c = 0; c != Characters; c ++) { // get highest chance character
		float chance = CalculateChance (&Character [c]);
		
		if (chance > curhighest) {
			TargetCharacter = c;
			Target = &Character [c];
			curhighest = chance;
		}
	}
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
	return NewCharacter (correct);
	
	has:
	strcpy (Character[c].name, correct -> name);
	
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

