#include <curses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "csnek_pair.h"
#define ALLOC(type) ( (type*) malloc(sizeof(type)) )
#define ALLOC_N(type, n) ( (type*) calloc(n, sizeof(type) ) )

#ifndef GAMELOGIC_H
#define GAMELOGIC_H

extern const char* SCORE_STR;
extern const char* GAMEOVER_STR ;
extern const wchar_t* TUTORIAL_WSTR;
extern const char* TITLE_STR;

extern const wchar_t* SNAKE_HEAD_WSTR;
extern const wchar_t* SNAKE_BODY_WSTR;
extern const wchar_t* SNAKE_TAIL_WSTR;

struct scoreboard
{
	WINDOW* window;
	uint64_t score;
	struct pair size;
};

struct scoreboard scoreboard_create(WINDOW* parent_win, struct pair pos, struct pair size);
void scoreboard_update(struct scoreboard* s, uint64_t score_diff);
void scoreboard_draw(struct scoreboard* s);

struct snakegame
{
	WINDOW* window;
	struct pair size;
	struct pair* snake;
	struct pair snake_direction;
	int snake_length;
	struct pair fruit;
	struct scoreboard* scoreboard;
};

struct snakegame snakegame_create(WINDOW* parent_window, struct scoreboard* scoreboard, struct pair size, struct pair pos);
void snakegame_gameover(struct snakegame *s);
void snakegame_reset(struct snakegame *s);
void snakegame_draw(struct snakegame *s);
void snakegame_step(struct snakegame* s, struct pair snake_direction);

#endif