typedef uint_least8_t u8;	typedef int_least8_t s8;
typedef uint_least16_t u16;	typedef int_least16_t s16;
typedef uint_least32_t u32;	typedef int_least32_t s32;

class Game {
	public:

		typedef u8 answer_t;
		typedef u16 qid_t;
		typedef u16 cid_t;

		enum answer_t {
			T, F, U
		};

		typedef struct {
			char name [NAMELEN + 2];
			answer_t answer [ANSWERLEN + 1];
		} character_t;

		character_t *Characters; cid_t CharactersLen = 0;
		

}
