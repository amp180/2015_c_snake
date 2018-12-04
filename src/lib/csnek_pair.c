#include "stdbool.h"
#include "csnek_pair.h"

struct pair pair_add(struct pair a, struct pair b)
{
	return (struct pair){ .x = (a.x + b.x), .y = (a.y + b.y) };
}

bool pair_equal(struct pair a, struct pair b)
{
	return ((a.x == b.x) && (a.y == b.y));
}

bool pair_inside_box(struct pair a, struct pair topleft, struct pair bottomright)
{
	return ((a.x > topleft.x) && (a.y > topleft.y) && (a.x < bottomright.x) && (a.y < bottomright.y));
}
