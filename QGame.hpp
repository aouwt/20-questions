#ifndef QGAME_HPP
#define QGAME_HPP
	#include "common.hpp"

	#define NAMELEN 20
	#define ANSWERLEN 64

	class QGame {
		public:

			typedef u16 qid_t;
			typedef u16 cid_t;

			enum answer_t {
				T, F, U
			};

			typedef struct {
				char name [NAMELEN + 2];
				answer_t answer [ANSWERLEN + 1];
			} character_t;

			character_t *Character; cid_t Characters = 0;
			answer_t UserAnswer [ANSWERLEN + 1];
			
			void Init (void);
			void DeInit (void);
			void SetQuestions (char*** q);
			
			err_t LoadCSV (FILE* f);
			err_t SaveCSV (FILE* f);
			
			char* GetQuestion (void);
			void SubmitAns (qid_t q, answer_t ans);
			
			character_t* GuessWho (void);
			void TrainModel (character_t* correct);
			
		
		private:
			cid_t CharactersAlloc = 0;
			character_t* Target = NULL; cid_t TargetCharacter = 0;
			char*** Question = NULL; qid_t Questions = 0;
			
			err_t NewCharacter (character_t* c);
			void CopyCharacter (character_t* c, cid_t slot);
	};
#endif
