#include "stdbool.h"

struct pair
{
	int y, x;
};

struct pair pair_add(struct pair a, struct pair b);
bool pair_equal(struct pair a, struct pair b);
bool pair_inside_box(struct pair a, struct pair topleft, struct pair bottomright);
