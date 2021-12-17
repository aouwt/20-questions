#define LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define MAXNAMELEN 20

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

extern const qid QuestionLen;

typedef struct {
	char name [MAXNAMELEN+2];
	ans answer [QuestionLen+1];
} tdat;

typedef struct {
	tdat info;
	float chance;
} character;

extern character *Characters; extern cid CharactersLen;
extern ans UserAnswer [];
extern const char* Questions [];
extern cid TargetCharacter;


extern ans GetCharAns (char);
extern qid GetNextQuestion (void);
extern void TrainModel (qid, ans);
extern void Init (void);
extern void DeInit (void);
extern void TrainCharacter (cid);
extern void NewCharacter (char*);
extern uchar LoadTD (void);
extern uchar SaveTD (void);
