#include <stdio.h>
#include <stdlib.h>
#include <uchar.h>
#include <wchar.h>
#include <locale.h>

#define unicodecard(c) (0x1F0A0 + (c))
typedef unsigned char uchar_t;
typedef char card_t;

typedef struct _deck_t {
	card_t card [54];
	uchar_t size;
} deck_t;

typedef struct _player_t {
	deck_t hand;
} player_t;

deck_t Discard [53];
player_t *Player;

void debug_printdeck (deck_t* deck) {
	for (uchar_t i = 1; i != deck -> size; i++)
		wprintf (L"%lc ", unicodecard(deck -> card [i]));
}

void newdeck (deck_t* deck) {
	uchar_t card = 0;
	for (uchar_t ct = 1; ct != 14; ct++) {
		deck -> card [card++] = ct;
		deck -> card [card++] = ct;
		deck -> card [card++] = ct;
		deck -> card [card++] = ct;
	}
	deck -> size = 52;
}

void shuffledeck (deck_t* deck) {
	for (uchar_t i = 0; i != deck -> size; i++) {
	
		uchar_t r = rand () % 53;
		
		//swap them
		card_t c = deck -> card [i];
		deck -> card [i] = deck -> card [r];
		deck -> card [r] = c;
		
	}
}

void initgame (uchar_t players) {
	deck_t deck [53];
	newdeck (deck);
	shuffledeck (deck);
	
	// distribute deck
	
	
	Player = (player_t*) malloc (sizeof (player_t) * players);
}

int main (void) {
	setlocale (LC_ALL, "C.UTF-8");
	
	deck_t deck [53];
	newdeck (deck); shuffledeck (deck);
	
	debug_printdeck (deck);
	return 0;
}
