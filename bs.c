#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uchar_t;
typedef char card_t;

typedef struct _deck {
	card_t card [54];
	uchar_t size = 0;
} deck_t;

typedef struct player {
	deck_t hand;
} player_t;

deck_t Discard;
player_t *Player;


void newdeck (deck_t* deck) {
}

void shuffledeck (deck_t* deck) {
	
}

void init_game (uchar_t players) {
	Player = (struct player*) malloc (sizeof (struct player) * players);
}
