#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar_t;
typedef uchar_t card_t;

struct player {
	card_t hand [53];
	uchar_t sz;
};

card_t Discard[53] = 0; uchar_t Discard_size = 0;
struct player *Player;


void init_game (uchar_t players) {
	Player = (struct player*) malloc (sizeof (struct player) * players);
}
