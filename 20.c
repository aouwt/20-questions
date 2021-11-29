#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
//#include <readline.h>
//#include <history.h>
//#include <math.h>
//#pragma GCC optimize ("-Os")

#define _STR(x) #x
#define STR(x) _STR(x)

#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define QUESTIONS (LEN(Questions) - 1)
#define NAMELEN 20

#define print(s) printf("%s", s)

#define TD_TRUE 2
#define TD_FALSE 1
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

typedef struct {
	char name [NAMELEN+2];
	ans q[QUESTIONS+1];
} tdat;

typedef struct {
	tdat info;
	float chance;
} character;


character *Characters; cid CharactersLen = 0;
tdat *TrainingDat; cid TrainingDatLen = 0;
ans CurAns [QUESTIONS+1];

float chance (character *C) {
	qid e = 0;
	
	for (qid i = 0; i != QUESTIONS; i++) {
		if ((CurAns [i] == TD_UNKNOWN) || (C -> info.q [i] == TD_UNKNOWN)) continue;
		if (CurAns [i] == C -> info.q [i])
			e++;
		else {
			e = (e - (QUESTIONS/2) < 0) ? 0 : e - (QUESTIONS/2);
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
	cid h = 0;
	
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
		char answers [QUESTIONS + 2];
		FILE* f = fopen (path, "r");
		if (f == NULL) return 1;
		cid ent;
		
		for (ent = 0;; ent++) {

			{ // build format string
				char fmt [20];
				snprintf (fmt, LEN(fmt), "%%%u[^,],%%%u[^\n]\n", (uint)(NAMELEN+1), (uint)QUESTIONS+1);

				if (fscanf (f, fmt, TrainingDat[ent].name, answers) == EOF) goto done;
			}

			if (ent >= TrainingDatLen)
				TrainingDat = (tdat*) realloc (TrainingDat, ((TrainingDatLen *= 1.5) + 1) * sizeof (tdat));

			for (qid i = 0; i != QUESTIONS; i++)
				TrainingDat[ent].q[i] = parseans (answers [i]);

		}
		done:
		TrainingDatLen = ent;
		
		fclose (f);
	}
	
	
	TrainingDat = (tdat*) realloc (TrainingDat, (TrainingDatLen + 1) * sizeof (tdat));
	Characters = (character*) malloc (((CharactersLen = TrainingDatLen+1) + 1) * sizeof (character));
	
	// copy it to Characters
	for (cid ent = 0; ent != TrainingDatLen; ent++) {
	
		for (uchar i = 0; i != LEN(TrainingDat[0].name)-1; i++)
			Characters[ent].info.name[i] = TrainingDat[ent].name[i];
			
		for (qid i = 0; i != LEN(TrainingDat[0].q)-1; i++)
			Characters[ent].info.q[i] = TrainingDat[ent].q[i];
		
		Characters[ent].chance = 0;
	}
	return 0;
}

uchar savechars (const char* path) {
	FILE* f = fopen (path, "w");
	if (f == NULL) return 1;
	
	for (cid ent = 0; ent != CharactersLen; ent++) {
		char answers [QUESTIONS + 2];
		
		for (qid qu = 0; qu != QUESTIONS; qu++) {
			switch (Characters[ent].info.q[qu]) {
				case TD_TRUE: answers[qu] = 'T'; break;
				case TD_FALSE: answers[qu] = 'F'; break;
				case TD_UNKNOWN: answers[qu] = '?'; break;
			}
		}
		answers [QUESTIONS + 1] = '\0';
		
		fprintf (f, "%s,%s\n", Characters[ent].info.name, answers);
	}
	
	fclose (f);
	return 0;
}

qid getquestion (void) {
	// TODO: better alg
	qid q;
	while (CurAns[(q = rand() % (QUESTIONS+1))] != TD_UNKNOWN);
	return q;
}

void init (void) {
	free (TrainingDat); free (Characters);
	TrainingDat = (tdat*) malloc (((TrainingDatLen = 100) + 1) * sizeof (tdat));
	
	for (qid i = 0; i != QUESTIONS; i++)
		CurAns [i] = TD_UNKNOWN;
}

void main () {
	puts ("Initializing...");
	init ();
	puts ("Loading `training.csv`...");
	if (loadchars ("training.csv")) {
		puts ("ERROR: Cannot find `training.csv`!");
		puts ("Please create it and put the data table in it");
		puts ("See the source code for information on how to create it.");
		exit (1);
	}
	
	for (uchar i = 0; i != 19; i++) {
		qid qu = getquestion ();
		printf ("%s (Y/N)\t", Questions[qu]);
		
		ans an = parseans (getchar ());
		while (getchar () != '\n');
		
		train (qu, an);
	}
	
	printf ("Are you %s? ", Characters[highestchance ()].info.name);
	
	if (parseans (getchar ()) == TD_TRUE) {
		puts ("Yay!");
	} else {
	
		char name [NAMELEN+2];
		char fmt [20]; snprintf (fmt, LEN(fmt), "%%%u[^\n]", (uint)(NAMELEN+1));

		do printf ("Aww...\nWho are you, then? ");
		while (scanf (fmt, name) == EOF);

		puts ("Entering into database...");

		for (uchar i = 0; i != NAMELEN+1; i++)
			Characters[TrainingDatLen].info.name[i] = name[i];
			
		for (qid i = 0; i != QUESTIONS; i++)
			Characters[TrainingDatLen].info.q[i] = CurAns [i];
	}
	
	savechars ("training.csv");
}
