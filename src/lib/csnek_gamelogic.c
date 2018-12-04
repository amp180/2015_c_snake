#include "csnek_gamelogic.h"

const char* SCORE_STR = "Score: %i";
const char* GAMEOVER_STR = "Game Over. \n Press Enter to Restart.";
const wchar_t* TUTORIAL_WSTR = L"Controls:\nWASD or ▲▼►◄\nESC to reset\nCTRL-C to quit.";
const char* TITLE_STR = "S.N.E.K.";

const wchar_t* SNAKE_HEAD_WSTR = L"O"; // L"\u1f600"
const wchar_t* SNAKE_BODY_WSTR = L"0";
const wchar_t* SNAKE_TAIL_WSTR = L"I"; // L"\u1f389" 


struct scoreboard scoreboard_create(WINDOW* parent_win, struct pair pos, struct pair size)
{
	struct scoreboard s;
	s.score = 0;
	s.size = size;
	s.window = derwin(parent_win, size.y, size.x, pos.y, pos.x);
	return s;
}

void scoreboard_update(struct scoreboard* s, uint64_t score_diff)
{
	s->score += score_diff;
	scoreboard_draw(s);
}

void scoreboard_draw(struct scoreboard* s)
{
	werase(s->window);
	wattron(s->window, COLOR_PAIR(2));
	box(s->window, 0, 0);

	wattron(s->window, COLOR_PAIR(3));
	mvwprintw(s->window, 1, 8,  TITLE_STR);
	mvwprintw(s->window, 21, 8, SCORE_STR, s->score);

	WINDOW* tutorial = derwin(s->window, s->size.y / 4, s->size.x - 8, s->size.y / 3, 6);
	mvwaddwstr(tutorial, 0, 0, TUTORIAL_WSTR);
	touchwin(tutorial);

	touchwin(s->window);
	wrefresh(s->window);
}

struct snakegame
snakegame_create(WINDOW* parent_window, struct scoreboard* scoreboard, struct pair size, struct pair pos)
{
	struct snakegame s;
	s.window = derwin(parent_window, size.y, size.x, pos.y, pos.x);
	s.snake_length = 4;
	s.scoreboard = scoreboard;
	s.snake_direction = (struct pair){ .x = 0, .y = 1 };
	s.fruit = (struct pair){ .x = 4, .y = 4 };
	s.size = size;
	s.snake = ALLOC_N(struct pair, 4);
	s.snake = memmove(s.snake, (struct pair[4]){ {.x = 2, .y = 7 }, { .x = 2, .y = 6 }, { .x = 2, .y = 5 }, { .x = 2, .y = 4 } }, sizeof(struct pair) * 4);
	return s;
}

void snakegame_gameover(struct snakegame *s)
{
	int sizeY = s->size.y / 2;
	int sizeX = s->size.x / 2;

	WINDOW* popup = derwin(s->window, sizeY, sizeX, sizeY / 2, sizeX / 2);
	wattron(s->window, COLOR_PAIR(2));
	mvwprintw(popup, sizeY / 4 + 3, sizeX / 4 + 2, GAMEOVER_STR);
	wrefresh(popup);

	//get char then clean up
	nocbreak(); getch(); halfdelay(1); delwin(popup);
	snakegame_reset(s);
}

void snakegame_reset(struct snakegame *s)
{
	werase(s->window);
	scoreboard_update(s->scoreboard, -(s->scoreboard->score));
	s->snake_direction = (struct pair){ .y=0, .x=1 };
	s->snake_length = 4;

	free(s->snake);
	s->snake = ALLOC_N(struct pair, 4);
	s->snake = memmove(s->snake, (struct pair[4]){ {.x = 2, .y = 7 }, { .x = 2, .y = 6 }, { .x = 2, .y = 5 }, { .x = 2, .y = 4 } }, sizeof(struct pair) * 4);

	snakegame_draw(s);
};

void snakegame_draw(struct snakegame *s)
{
	werase(s->window);

	wattron(s->window, COLOR_PAIR(2));
	box(s->window, 0, 0);

	struct pair head = s->snake[0];
	struct pair tail = s->snake[s->snake_length - 1];
	struct pair fruit = s->fruit;

	wattron(s->window, COLOR_PAIR(3));
	mvwprintw(s->window, fruit.y, fruit.x, "@");

	wattron(s->window, COLOR_PAIR(4));

	for (int i = 1; i < s->snake_length - 1; i++)
	{//fill in segments
		struct pair seg = s->snake[i];
		mvwaddwstr(s->window, seg.y, seg.x, SNAKE_BODY_WSTR);
	};

	mvwaddwstr(s->window, tail.y, tail.x, SNAKE_TAIL_WSTR);
	mvwaddwstr(s->window, head.y, head.x, SNAKE_HEAD_WSTR);
	wmove(s->window, head.y, head.x);

	touchwin(s->window);
	wrefresh(s->window);

};

void snakegame_step(struct snakegame* s, struct pair snake_direction)
{//if snake goes out of bounds
	struct pair next = (pair_add(s->snake[0], snake_direction));

	if (!pair_inside_box(next, (struct pair){ 0, 0 }, (struct pair){ .y=(s->size.y -1), .x=(s->size.x -1) }))
	{
		snakegame_gameover(s);
		return;
	}

	if (pair_equal(next, s->fruit))
	{//Handle fruit eating.;

		s->snake = realloc(s->snake, (++(s->snake_length)) * sizeof(struct pair));
		for (int i = s->snake_length - 1; i >= 1; i--)
			s->snake[i] = s->snake[i - 1];
		s->snake[0] = s->fruit;
		s->fruit = (struct pair){ .x=(rand() % (s->size.x - 4)) + 3, .y=(rand() % (s->size.y - 4)) + 3 };

		scoreboard_update(s->scoreboard, 1);
		s->snake_direction = snake_direction;//save last  direction
		return;

	}
	else if (!(pair_equal(pair_add(s->snake_direction, snake_direction), (struct pair){ 0, 0 })))//forbid going backwards
	{
		//if snake collides with self without going backwards..
		for (int j = 1; j < s->snake_length; j++)
		{
			if (pair_equal(s->snake[j], next))
			{
				snakegame_gameover(s);
				return;
			}
		};
		//move normally 

		for (int i = s->snake_length - 1; i >= 1; i--)
			s->snake[i] = s->snake[i - 1];
		s->snake[0] = next;

		s->snake_direction = snake_direction;//save last direction
		return;
	};
}

