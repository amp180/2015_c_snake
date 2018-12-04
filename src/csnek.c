//snek.c - implements snake game with curses ui.
#include "locale.h"
#include "csnek_gamelogic.h"
#define ALLOC(type) ( (type*) malloc(sizeof(type)) )
#define ALLOC_N(type, n) ( (type*) calloc(n, sizeof(type) ) )


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "en_IE.UTF-8");
	WINDOW * win = initscr();
	start_color();
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);

	attron(COLOR_PAIR(1));
	wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
	wrefresh(win);

	const struct pair score_win_position = { 1, 1 };
	const struct pair score_win_size = { .y=(LINES - 2), .x=(COLS / 3) };

	const struct pair game_win_size = { .y=(LINES - 2), .x=((2 * COLS / 3) - 2) };
	const struct pair game_win_position = { .y=1, .x=(COLS / 3 + 1) };

	struct scoreboard scoreboard = scoreboard_create(stdscr, score_win_position, score_win_size);
	struct snakegame snakegame = snakegame_create(stdscr, &scoreboard, game_win_size, game_win_position);

	noecho();
	keypad(stdscr, TRUE);
	halfdelay(1);

	while (true)
	{
		scoreboard_draw(&scoreboard);
		snakegame_draw(&snakegame);
		int key = getch();

		if (key == 'a' || key == KEY_LEFT)
			snakegame_step(&snakegame, (struct pair){ 0, -1 });
		else if (key == 'd' || key == KEY_RIGHT)
			snakegame_step(&snakegame, (struct pair){ 0, 1 });
		else if (key == 'w' || key == KEY_UP)
			snakegame_step(&snakegame, (struct pair){ -1, 0 });
		else if (key == 's' || key == KEY_DOWN)
			snakegame_step(&snakegame, (struct pair){ 1, 0 });
		else
			snakegame_step(&snakegame, snakegame.snake_direction);
		if (key == (char)(3)) { //handle ctrl-c on windows
			break;
		}
		if (key == (char)(27))
			snakegame_reset(&snakegame);
	}
	endwin();
	return 0;
};