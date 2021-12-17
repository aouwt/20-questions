#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "questions.h"


const qid QuestionLen = LEN (Questions) - 1;

static tdat *TrainingData; static cid TrainingDataLen = 0;
character *Characters; cid CharactersLen = 0;
ans UserAnswer [QuestionLen+1];
cid TargetCharacter = 0;


static float chance (character *C) { // calculates the chance of a character being the right guess
	qid e = 0;
	
	for (qid i = 0; i != QuestionLen; i++) {
	
		if ((UserAnswer [i] == TD_UNKNOWN) || (C -> info.answer [i] == TD_UNKNOWN)) continue;
		
		if (UserAnswer [i] == C -> info.answer [i])
			e++;
		else
			e = ((int)e - (int)(QuestionLen/2) < 0) ? 0 : e - (QuestionLen/2);
		
	}

	//printf ("\t%i\n", (int)e);
	return (C -> chance = (float)e / (float)QuestionLen);
}


ans GetCharAns (char a) { // string to ans
	switch (a) {
		case 'Y': case 'y': case 'T': case 't':
			return TD_TRUE;
		case 'N': case 'n': case 'F': case 'f':
			return TD_FALSE;
		default:
			return TD_UNKNOWN;
	}
}


static cid highestchance (void) { // finds most likely match
	float curch = 0;
	cid h = 0;
	
	for (cid c = 0; c != CharactersLen; c++) {
		if (chance(&Characters[c]) > curch) {
			curch = Characters[c].chance;
			h = c;
		}
	}
	return h;
}



void TrainModel (qid qu, ans an) { // "trains" the "ai" -- just fills in any unknown things
	// TODO: make better
	TargetCharacter = highestchance ();
/*	
	if (Characters[Target].info.q[qu] == TD_UNKNOWN)
		Characters[Target].info.q[qu] = an;
*/
	UserAnswer [qu] = an;
}


qid GetNextQuestion (void) { // Chooses a question
	qid q;
	
	for (;;) {
		while (UserAnswer [(q = rand() % (QuestionLen))] != TD_UNKNOWN);
		if ((rand () % IDK_CHANCE) == 0) {
			if (Characters[TargetCharacter].info.answer[q] == TD_UNKNOWN) return q;
		} else return q;
	}
}



void Init (void) { // mallocs and stuff
	free (TrainingData); free (Characters);
	TrainingData = (tdat*) malloc (((TrainingDataLen = 100) + 1) * sizeof (tdat));
	
	for (qid i = 0; i != QuestionLen; i++)
		UserAnswer [i] = TD_UNKNOWN;

	srand (time (0));
}



void DeInit (void) { // frees
	free (TrainingData); free (Characters);
	TrainingDataLen = 0; CharactersLen = 0;
}




void TrainCharacter (cid c) {
	for (qid q = 0; q != QuestionLen; q++) {
		if (UserAnswer [q] != TD_UNKNOWN) {
		
			if (TrainingData[c].answer[q] == TD_UNKNOWN) // if unknown then set to known
				Characters[c].info.answer[q] = UserAnswer [q];
			else
			if (TrainingData[c].answer[q] != UserAnswer [q])
				Characters[c].info.answer[q] = TD_UNKNOWN; // if they conflict then set it to unknown
		
		}
	}
}

void NewCharacter (char* name) {
	for (cid c = 0; c != CharactersLen; c++) {
		if (!strcmp (Characters[c].info.name, name)) {
			TrainCharacter (c);
			return;
		}
	}
	
	// if it cant find it then add it
	for (uchar i = 0; i != MAXNAMELEN+1; i++)
		Characters[CharactersLen].info.name[i] = name [i];
		
	for (qid i = 0; i != QuestionLen; i++)
		Characters[CharactersLen].info.answer[i] = UserAnswer [i];
	
	CharactersLen ++; // we already have it reserved in `loadchars()`
}


uchar LoadTD (void) { // loads the CSV file into TrainingDat and Characters
	
	{
		char answers [QuestionLen + 2];
		FILE* f = fopen (TDFILE, "r");
		if (f == NULL) return 1;
		cid ent;
		
		for (ent = 0;; ent++) {

			{ // build format string
				char fmt [20];
				snprintf (fmt, LEN(fmt), "%%%u[^,],%%%u[^\n]\n", (uint)(MAXNAMELEN+1), (uint)QuestionLen+1);

				if (fscanf (f, fmt, TrainingData[ent].name, answers) == EOF) goto done;
			}

			if (ent >= TrainingDataLen)
				TrainingData = (tdat*) realloc (TrainingData, ((TrainingDataLen *= 1.5) + 1) * sizeof (tdat));

			for (qid i = 0; i != QuestionLen; i++)
				TrainingData[ent].answer[i] = GetCharAns (answers [i]);

		}
done:
		TrainingDataLen = CharactersLen = ent;
		
		fclose (f);
	}
	
	
	TrainingData = (tdat*) realloc (TrainingData, (TrainingDataLen + 1) * sizeof (tdat));
	Characters = (character*) malloc ((CharactersLen + 2) * sizeof (character));
	
	// copy it to Characters
	for (cid ent = 0; ent != TrainingDataLen; ent++) {
	
		for (uchar i = 0; i != MAXNAMELEN; i++)
			Characters[ent].info.name[i] = TrainingData[ent].name[i];
			
		for (qid i = 0; i != QuestionLen; i++)
			Characters[ent].info.answer[i] = TrainingData[ent].answer[i];
		
		Characters[ent].chance = 0;
	}
	return 0;
}



uchar SaveTD (void) { // stores Characters to CSV file -- this is partially broken
	FILE* f = fopen (TDFILE, "w");
	if (f == NULL) return 1;
	
	for (cid ent = 0; ent != CharactersLen; ent++) {
		char answers [QuestionLen + 2];
		
		for (qid q = 0; q != QuestionLen+1; q++) {
			switch (Characters[ent].info.answer[q]) {
				case TD_TRUE: answers[q] = 'T'; break;
				case TD_FALSE: answers[q] = 'F'; break;
				case TD_UNKNOWN: answers[q] = '?'; break;
			}
		}
		
		answers [QuestionLen + 1] = '\0';
		
		fprintf (f, "%s,%s\n", Characters[ent].info.name, answers);
	}
	
	fclose (f);
	return 0;
}
