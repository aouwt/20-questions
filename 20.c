#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define QUESTIONS (LEN(Questions) - 1)
#define NAMELEN 20

#define TDFILE ".td"

#define TD_TRUE 2
#define TD_FALSE 1
#define TD_UNKNOWN 0

#define IDK_CHANCE /* 1 / */ 2

typedef unsigned char uchar;
typedef unsigned int uint;
typedef uchar ans;
typedef uint qid;
typedef uint cid;

static const char* Questions[] = {
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
	"Do you routinely exercise?",
	"Have you ever had a pet?",
	"Do you regularly use Rust?",
	"Do you often try to program in esoteric programming languages?",
	"Are you a part of the GEORGE webring?",
	"Do you have more than 10 GitHub repositories?",
	"Tabs (Y) or spaces (N)?",
	"Is programming your primary hobby?",
	"Do people call you by your nickname/real name more often than your username/online name?",
	"`if(...)` (Y) or `if (...)` (N)?",
	"Do you actively participate in the Esolangs community? (ie. develop and utilize esolangs)",
	"Do you enjoy linguistics? (eg. conlangs)",
	"Do you use Windows primarily?",
	"Would you consider yourself a \"computer whiz\"?",
	//"`while(...)\n{` (Y) or `while(...) {` (N)?",
	"Are you content with your current living situation?",
	"Do you primarily use Python? (ie. use it more often than other languages)",
	"Are you a \"long-standing\" member of Esolangs?",
	"Do you have a (meaningful) website?",
	"Are you currently in high school?",
	"Have you ever participated in code guessing?",
	"Have you ever had a LyricLy or gollark name derivative?",
	"Are you from Europe?",
	"Have you ever developed for an embedded system?",
	"Are you multilingual?",
	"Is English your primary language?",
	""
};

typedef struct {
	char name [NAMELEN+2];
	ans q[QUESTIONS+1];
} tdat;

typedef struct {
	tdat info;
	float chance;
} character;




static character *Characters; static cid CharactersLen = 0;
static tdat *TrainingDat; static cid TrainingDatLen = 0;
static ans CurAns [QUESTIONS+1];
static cid Target = 0;






static float chance (character *C) { // calculates the chance of a character being the right guess
	qid e = 0;
	
	for (qid i = 0; i != QUESTIONS; i++) {
	
		if ((CurAns [i] == TD_UNKNOWN) || (C -> info.q [i] == TD_UNKNOWN)) continue;
		
		if (CurAns [i] == C -> info.q [i])
			e++;
		else
			e = ((int)e - (int)(QUESTIONS/2) < 0) ? 0 : e - (QUESTIONS/2);
		
	}

	//printf ("\t%i\n", (int)e);
	return (C -> chance = (float)e / (float)QUESTIONS);
}



static ans parseans (char a) { // string to ans
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



static void train (qid qu, ans an) { // "trains" the "ai" -- just fills in any unknown things
	// TODO: make better
	Target = highestchance ();
/*	
	if (Characters[Target].info.q[qu] == TD_UNKNOWN)
		Characters[Target].info.q[qu] = an;
*/
	CurAns [qu] = an;
}



static uchar loadchars (void) { // loads the CSV file into TrainingDat and Characters
	
	{
		char answers [QUESTIONS + 2];
		FILE* f = fopen (TDFILE, "r");
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
		TrainingDatLen = CharactersLen = ent;
		
		fclose (f);
	}
	
	
	TrainingDat = (tdat*) realloc (TrainingDat, (TrainingDatLen + 1) * sizeof (tdat));
	Characters = (character*) malloc ((CharactersLen + 2) * sizeof (character));
	
	// copy it to Characters
	for (cid ent = 0; ent != TrainingDatLen; ent++) {
	
		for (uchar i = 0; i != NAMELEN; i++)
			Characters[ent].info.name[i] = TrainingDat[ent].name[i];
			
		for (qid i = 0; i != QUESTIONS; i++)
			Characters[ent].info.q[i] = TrainingDat[ent].q[i];
		
		Characters[ent].chance = 0;
	}
	return 0;
}



static uchar savechars (void) { // stores Characters to CSV file -- this is partially broken
	FILE* f = fopen (TDFILE, "w");
	if (f == NULL) return 1;
	
	for (cid ent = 0; ent != CharactersLen; ent++) {
		char answers [QUESTIONS + 2];
		
		for (qid qu = 0; qu != QUESTIONS+1; qu++) {
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

static qid getquestion (void) { // Chooses a question
	qid q;
	
	for (;;) {
		while (CurAns[(q = rand() % (QUESTIONS))] != TD_UNKNOWN);
		if ((rand () % IDK_CHANCE) == 0) {
			if (Characters[Target].info.q[q] == TD_UNKNOWN) return q;
		} else return q;
	}
}



static void init (void) { // mallocs and stuff
	free (TrainingDat); free (Characters);
	TrainingDat = (tdat*) malloc (((TrainingDatLen = 100) + 1) * sizeof (tdat));
	
	for (qid i = 0; i != QUESTIONS; i++)
		CurAns [i] = TD_UNKNOWN;

	srand (time (0));
}



static void deinit (void) { // frees
	free (TrainingDat); free (Characters);
	TrainingDatLen = 0; CharactersLen = 0;
}



static void init_td (void) { // creates initial CSV file
	// NOTE: .td is now hosed on github
	const char initialcsv[] = " ,?";
	
	FILE* f = fopen (TDFILE, "w");
	if (f == NULL) return;
	
	fprintf (f, "%s", initialcsv);
	
	fclose (f);
}



static void trainchar (cid c) {
	for (qid q = 0; q != QUESTIONS; q++) {
		if (CurAns [q] != TD_UNKNOWN) {
		
			if (TrainingDat[c].q[q] == TD_UNKNOWN) // if unknown then set to known
				Characters[c].info.q[q] = CurAns [q];
			else
			if (TrainingDat[c].q[q] != CurAns [q])
				Characters[c].info.q[q] = TD_UNKNOWN; // if they conflict then set it to unknown
		
		}
	}
}

static void insertchar (char* name) {
	for (cid c = 0; c != CharactersLen; c++) {
		if (!strcmp (Characters[c].info.name, name)) {
			trainchar (c);
			return;
		}
	}
	
	// if it cant find it then add it
	for (uchar i = 0; i != NAMELEN+1; i++)
		Characters[CharactersLen].info.name[i] = name [i];
		
	for (qid i = 0; i != QUESTIONS; i++)
		Characters[CharactersLen].info.q[i] = CurAns [i];
	
	CharactersLen ++; // we already have it reserved in `loadchars()`
}


static ans getans (void) {
	ans ret;

	#ifdef release
loop:
		if ((ret = parseans (getchar ())) == TD_UNKNOWN) goto loop;

	#else
		ret = parseans (getchar ());
	#endif

	while (getchar () != '\n');
	return ret;
}

char* lcase (char* str) {
	for (uint i = 0; str [i]; i++)
		if (str [i] >= 'A' && str [i] <= 'Z') str [i] -= 'A' - 'a';
	return str;
}


int main () {
begin:
	init ();
	if (loadchars ()) {
		printf ("Warning: Training data not found, should I create it? (Y/N)\t");
		if (getans () == TD_TRUE) {
			init_td ();
			goto begin;
		} else {
			puts ("Aborting");
			exit (1);
		}
	}
	
	for (uchar i = 0; i != 20; i++) {
		qid qu = getquestion ();
		printf ("%u\t%s (Y/N)\t", i+1, Questions[qu]);
		
		train (qu, getans ());
	}
	
	printf ("Are you %s? ", Characters[Target].info.name);
	
	if (getans () == TD_TRUE) {
		puts ("Yay!");
		trainchar (Target);
		
	} else {
		char name [NAMELEN+2];
		char fmt [20]; snprintf (fmt, LEN(fmt), "%%%u[^\n]", (uint)(NAMELEN+1));

		do printf ("Aww...\nWho are you, then? ");
		while (scanf (fmt, name) == EOF);
		
		insertchar (lcase(name));
	}
	
	savechars ();
	
	deinit ();
}
