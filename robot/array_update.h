#ifndef array_update_h
#define array_update_h

#include "intersect_globals.h"

extern short intersect_array[x_max + 1][y_max + 1];

/**
 * Update the maze information array
 * @param x The x coordinate of the intersection where 0 is the westernmost intersection
 * @param y The y coordinate of the intersection where 0 is the northernmost intersection
 * @param west True if there is a wall to the west of the intersection & false otherwise
 * @param north True if there is a wall to the north of the intersection & false otherwise
 * @param east True if there is a wall to the east of the intersection & false otherwise
 * @param south True if there is a wall to the south of the intersection & false otherwise
 * @param tshape Shape of the treasure if present - definitions of shapes in intersect_globals.h
 * @param tcolor Color of the treasure if present - definitions of colors in intersect_globals.h
*/
void array_update (short x, short y, bool west, bool north, bool east, bool south, short tshape, short tcolor);

#endif
