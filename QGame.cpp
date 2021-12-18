#include <string.h>
#include <malloc.h>
#include <errno.h>
#include "QGame.hpp"




void QGame::Init (void) {
	free (Character);
	Character = new QGame::character_t [CharactersAlloc = 10];
}



void QGame::DeInit (void) {
	free (Character);
	CharactersAlloc = 0;
}



void QGame::SetQuestions (const char* q[]) {
	Question = q;
}



void QGame::CopyCharacter (QGame::character_t* c, cid_t slot) {
	for (QGame::qid_t q = 0; q != Questions; q ++)
		Character[slot].answer [q] = c -> answer [q];
	
	for (size_t i = 0; i != NAMELEN+1; i ++)
		Character[slot].name [i] = c -> name [i];
}



err_t QGame::NewCharacter (QGame::character_t* c) {
	if (Characters+1 >= CharactersAlloc)
		if (reallocarray (&Character, sizeof(QGame::character_t), (CharactersAlloc += 10)) == NULL)
			return 1;
	
	CopyCharacter (c, ++ Characters);
	
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
	for (size_t i = 0; str [i]; i ++)
		if (str [i] >= 'A' && str [i] <= 'Z')
			str [i] -= 'A' - 'a';
	return str;
}

void QGame::TrainModel (QGame::character_t* correct) {
	lcase (correct -> name);
	
	// See if we can find the character as preexisting
	QGame::cid_t c = 0;
	for (; c != Characters; c ++)
		if (!strcmp (Character[c].name, correct -> name)) goto has;
	
	// does not have
	CERR ("NewCharacter", NewCharacter (correct));
	
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
}
