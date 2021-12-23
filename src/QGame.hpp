#ifndef QGAME_HPP
#define QGAME_HPP
	#include "common.hpp"

	#define QGAME_NAMELEN 20
	#define QGAME_ANSWERLEN 64
	#define QGAME_RANDQCHANCE 2

	class QGame {
		public:

			typedef u16 qid_t;
			typedef u16 cid_t;

			enum answer_t {
				T, F, U
			};

			typedef struct {
				char name [QGAME_NAMELEN + 2];
				answer_t answer [QGAME_ANSWERLEN + 1];
			} character_t;

			character_t *Character; cid_t Characters = 0;
			char** Question = nullptr; qid_t Questions = 0;
			answer_t UserAnswer [QGAME_ANSWERLEN + 1];
			
			void Init (void);
			void DeInit (void);
			
			void SetQuestions (const char* q[]);
			err_t NewQuestion (char str []);
			
			err_t NewCharacter (character_t* c);
			
			const char* GetQuestion (void);
			const char* GetQuestion (qid_t* id);
			void SubmitAns (qid_t q, answer_t ans);
			
			character_t* GuessWho (void);
			err_t TrainModel (character_t* correct);
			
		
		private:
			size_t CharactersAlloc = 0; size_t QuestionsAlloc = 0;
			
			character_t* Target = nullptr; cid_t TargetCharacter = 0;
			
			FILE* urand = nullptr;
			
			void CopyCharacter (character_t* c, cid_t slot);
			float CalculateChance (character_t* c);
			void GetHighest (void);
			unsigned int random (void);
	};
#endif
