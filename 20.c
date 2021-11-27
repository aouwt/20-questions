#include <malloc.h>
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
	"Are you gay/lesbian/bi/etc.?",
	"Are you an adult?",
	"Do you know C++ well?",
	"Are you currently in a relationship?",
	"Are you a part of the staff?"
};

typedef struct _trainingdata {
	char name [20];
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
		{"iso", {idk,idk,idk,idk,f,f}},
		{"coral", {f,t,t,idk,idk,f}}
	};
#undef t
#undef f
#undef idk

typedef struct _character {
	tdat info;
	float chance;
} character;


character *Characters;
tdat *TrainingDat;
ans CurAns [QUESTIONS];
cid TrainingDatLen = 0;

float chance (character *C) {
	qid e = 0;
	
	for (qid i = 0; i != QUESTIONS; i++) {
		if ((CurAns [i] == TD_UNKNOWN) || (C -> info.q [i] == TD_UNKNOWN)) continue;
		if (CurAns [i] == C -> info.q [i])
			e++;
		else {
			e = 0;
			break;
		}
	}

	return (C -> chance = (e / QUESTIONS));
}

cid highestchance (void) {
	float curch = 0;
	cid h;
	
	for (cid c = 0; c != TrainingDatLen; c++)
		if (chance(&Characters[c]) > curch) {
			curch = Characters[c].chance;
			h = c;
		}
	
	return h;
}

void train (qid qu, ans an) {
	cid hc = highestchance ();
	
	if (TrainingDat[hc].q[qu] == TD_UNKNOWN)
		TrainingDat[hc].q[qu] = an;
	
	/*for (cid c = 0; c != TrainingDatLen; c++) {
		
		if (TrainingDat[c].q[qu] == TD_UNKNOWN) {
			if (c == hc) {
				TrainingDat[c].q[qu] = an;
				return;
			}
		}
	
	}*/
}

uchar loadchars (const char* path) {
	FILE* f = fopen (path, "r");
	if (f == NULL) return 1;
	for (cid ent = 0;; ent++) {

		if (ent >= TrainingDatLen)
			TrainingDat = (tdat*) reallocarray (&TrainingDat, TrainingDatLen *= 1.5, sizeof (tdat));

		char answers [QUESTIONS + 1];
		if (fscanf (f, "%[^,]s,%[^,]s", &TrainingDat[ent].name, &answers)) break;

		for (qid i = 0; i != QUESTIONS; i++) {
			switch (answers [i]) {
				case 't': case 'T':
					TrainingDat[ent].q[i] = TD_TRUE; break;
				case 'f': case 'F':
					TrainingDat[ent].q[i] = TD_FALSE; break;
				default:
					TrainingDat[ent].q[i] = TD_UNKNOWN; break;
			}
		}

	}
}

void main () {
	
}
