#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
//#include <readline.h>
//#include <history.h>

#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define QUESTIONS (LEN(Questions) - 1)

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
	"Are you a part of the staff?",
	"Are you currently in school?",
	"Do you have your own Discord server? (doesn't include \"test servers\")",
	"Do you have a GitHub account?",
	"Do you live in the U.S.A.?",
	"Do you routinely exercize?",
	"Have you ever had a pet?",
	"Do you regularly use Rust?",
	"Do you often try to program in esoteric programming languages?",
	"Are you a part of the GEORGE webring?",
	"Do you have more than 10 GitHub repositories?",
	"Tabs (Y) or spaces (N)?",
	"Is programming your primary hobby?",
	"Do people call you by your nickname/real name more often than your username/online name?",
	"`func(...);` (Y) or `func (...);` (N)?",
	"Do you actively participate in the Esolangs community? (ie. develop and utilize esolangs)",
	"Do you enjoy linguistics? (eg. conlangs)",
	"Do you use Windows primarily?",
	"Would you consider yourself a \"computer whiz\"?",
	//"`while(...)\n{` (Y) or `while(...) {` (N)?",
	"Are you content with your current living situation?",
	"Do you primarily use Python? (ie. use it more often than other languages)",
	"Are you a \"long-standing\" member of Esolangs?",
	"Do you have a (meaningful) website?"
};

typedef struct _trainingdata {
	char name [20];
	ans q[QUESTIONS+1];
} tdat;

typedef struct _character {
	tdat info;
	float chance;
} character;


character *Characters;
tdat *TrainingDat;
ans CurAns [QUESTIONS+1];
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

ans parseans (char a) {
	switch (a) {
		case 'Y': case 'y': case 'T': case 't':
			return TD_TRUE;
		case 'N': case 'n': case 'F': case 'f':
			return TD_FALSE;
		default:
			return TD_UNKNOWN;
	}
}

cid highestchance (void) {
	float curch = 0;
	cid h;
	
	for (cid c = 0; c != TrainingDatLen; c++) {
		if (chance(&Characters[c]) > curch) {
			curch = Characters[c].chance;
			h = c;
		}
	}
	
	return h;
}

void train (qid qu, ans an) {
	cid hc = highestchance ();
	
	if (TrainingDat[hc].q[qu] == TD_UNKNOWN)
		TrainingDat[hc].q[qu] = an;
	
	CurAns [qu] = an;
	
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
	
	{
		FILE* f = fopen (path, "r");
		if (f == NULL) return 1;
		cid ent;
		
		for (ent = 0;; ent++) {

			if (ent >= TrainingDatLen)
				TrainingDat = (tdat*) realloc (TrainingDat, ((TrainingDatLen *= 1.5) + 1) * sizeof (tdat));

			char answers [QUESTIONS + 2];
			if (fscanf (f, "%[^,]s,%[^,]s", &TrainingDat[ent].name, &answers)) break;

			for (qid i = 0; i != QUESTIONS; i++)
				TrainingDat[ent].q[i] = parseans (answers [i]);

		}
		TrainingDatLen = ent;
		
		fclose (f);
	}
	
	
	TrainingDat = (tdat*) realloc (TrainingDat, (TrainingDatLen + 1) * sizeof (tdat));
	Characters = (character*) malloc ((TrainingDatLen + 2) * sizeof (character));
	
	for (cid ent = 0; ent != TrainingDatLen; ent++)
		Characters[ent].info = TrainingDat[ent];
		
	return 0;
}

uchar savechars (const char* path) {
	FILE* f = fopen (path, "w");
	if (f == NULL) return 1;
	
	for (cid ent = 0; ent != TrainingDatLen+1; ent++) {
		char answers [QUESTIONS + 2];
		
		for (qid qu = 0; qu != QUESTIONS; qu++)
			answers[qu] = parseans (Characters[ent].info.q[qu]);
		answers [QUESTIONS + 1] = '\0';
		
		fprintf (f, "%s,%s\n", Characters[ent].info.name, answers);
	}
	
	fclose (f);
	return 0;
}

qid getquestion (void) {
	// TODO: better alg
	
	return rand() % (QUESTIONS+1);
}

void init (void) {
	free (TrainingDat); free (Characters);
	TrainingDat = (tdat*) malloc (((TrainingDatLen = 100) + 1) * sizeof (tdat));
}

void main () {
	init ();
	if (loadchars ("training.csv")) {
		puts ("ERROR: Cannot find `training.csv`!\n");
		puts ("Please create it and put the data table in it\n");
		puts ("See the source code for information on how to create it.\n");
		exit (1);
	}
	
	for (uchar i = 0; i != 19; i++) {
		qid qu = getquestion ();
		ans an;
		char reply;
		
		
		//do { printf ("%s (Y/N)\t", Questions[qu]);
		//} while (scanf ("%[YNynTFtf?]c", &reply));
		printf ("%s (Y/N)\t", Questions[qu]);
		an = parseans (getchar ());
		while (getchar () != '\n');
		
		train (qu, an);
	}
	
	printf ("Are you %s? ", Characters[highestchance ()].info.name);
	
	if (parseans (getchar ()) == TD_TRUE) {
		puts ("Yay!\n");
	} else {
		puts ("Aww...\n");
	}
	
	savechars ("training.csv");
}
