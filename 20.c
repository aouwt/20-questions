#include <stdio.h>
#include <stdlib.h>

#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define QUESTIONS LEN(Questions)

#define TD_TRUE 1
#define TD_FALSE 2
#define TD_UNKNOWN 0

#define IDK_CHANCE /* 1 / */ 100

typedef unsigned char uchar;
typedef unsigned int uint;
typedef uchar ans;
typedef uint qid;
typedef uint cid;

const char* Questions[] = {
	"Are you transgender?",
	"Are you gay/lesbian?",
	"Are you an adult?",
	"Do you know C++ well?",
	"Are you currently in a relationship?",
	"Are you a part of the staff?"
};

typedef struct _trainingdata = {
	const char *name;
	ans q[QUESTIONS];
} tdat;

#define t TD_TRUE
#define f TD_FALSE
#define idk TD_UNKNOWN
	tdat InitialTrainingData[] = {
		{"lyricly", {t,t,f,f,t}},
		{"olivia", {idk,t,t,idk,idk,t}},
		{"palaiologos", {t,t,idk,t,f,t}},
		{"gollark", {f,idk,idk,idk,idk,f}},
		{"kaylynn", {idk,t,idk,idk,idk,f}},
		{"sinthorion", {f,t,t,idk,f,f}},
		{"sofia", {idk,idk,idk,idk,idk,t}},
		{"kit", {t,t,f,t,f,f}},
		{"olive", {t,t,idk,idk,idk,f}},
		{"iso", {idk,idk,idk,idk,f,f}}
	};
#undef t
#undef f
#undef idk

typedef struct _character {
	tdat *info;
	float chance;
} character;


character *Characters;
tdat *TrainingDat;
ans CurAns [QUESTIONS];
cid TrainingDatLen = 0;

float chance (character *C) {
	float chance = 0;
	cid e = 0;
	
	for (qid i = 0; i != QUESTIONS; i++) {
		if ((C -> info -> q [i] == TD_UNKNOWN) || (CurAns [i] == TD_UNKNOWN))
			e--;
		else
			chance += (C -> info -> q [i] == CurAns [i]);
		e++;
	}

	if (e < (QUESTIONS / 2)) e = QUESTIONS / 2;
	return (chance /= e);
}

cid highestchance (void) {
	float curch = 0;
	cid h;
	
	for (cid c = 0; c != TrainingDatLen; c++)
		if (chance(Characters[c]) > curch) {
			curch = Characrers[c].chance;
			h = c;
		}
	
	return h;
}

void train (qid qu, ans an) {
	cid hc = highestchance ();
	
	if (TrainingDat[hc].q[qu] == TD_UNKNOWN)
		TrainingDat[hc].q[qu] == an;
	
	/*for (cid c = 0; c != TrainingDatLen; c++) {
		
		if (TrainingDat[c].q[qu] == TD_UNKNOWN) {
			if (c == hc) {
				TrainingDat[c].q[qu] = an;
				return;
			}
		}
	
	}*/
}

void main () {
	
}
