#include <string.h>
#include <malloc.h>
#include <errno.h>
#include "QGame.hpp"




void QGame::Init (void) {
	
}



void QGame::DeInit (void) {
	
}



void QGame::SetQuestions (char*** q) {
	Questions = q;
}



err_t QGame::NewCharacter (QGame::character_t* c) {
	if (Characters+1 >= CharactersAlloc) {
		if (reallocarray (&Character, sizeof(QGame::character_t), (CharactersAlloc += 10)) == NULL)
			return 1;
	}
	
	Character [++ Characters] = c;
	return 0;
}



err_t QGame::LoadCSV (FILE* f) {
	
}



err_t QGame::SaveCSV (FILE* f) {
	
}



char* QGame::GetQuestion (void) {
	
}



void QGame::SubmitAns (QGame::qid_t q, QGame::answer_t a) {
	
}



QGame::character_t* QGame::GuessWho (void) {
	
}



char* lcase (char* str) {
	for (size_t i = 0; str [i]; i++)
		if (str [i] >= 'A' && str [i] <= 'Z') str [i] -= 'A' - 'a';
	return str;
}

void QGame::TrainModel (QGame::character_t* correct) {
	lcase (correct -> name);
	
	// See if we can find the character as preexisting
	for (QGame::cid_t c = 0; c != Characters; c++)
		if (!strcmp (Character[c].name, correct -> name)) goto has;
	
	// does not have
	
	
	has:
	
}
