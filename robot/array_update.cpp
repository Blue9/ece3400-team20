/***********************************
 * Created by Alec on 10/21/2018.  *
 ***********************************
 */

/* Element format:
 * ---------------
 * | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 *  x  x  x  x  x  \__ts__/ tc  wv   w  nv   n  ev   e  sv   s
 */

#include "array_update.h"
#include "intersect_globals.h"

#define array_wv_mask 0x80
#define array_w_mask 0x40
#define array_nv_mask 0x20
#define array_n_mask 0x10
#define array_ev_mask 0x8
#define array_e_mask 0x4
#define array_sv_mask 0x2
#define array_s_mask 0x1
#define array_tcolor_mask 0x100
#define array_tshape_mask 0x600

#define array_s_shift 0
#define array_sv_shift 1
#define array_e_shift 2
#define array_ev_shift 3
#define array_n_shift 4
#define array_nv_shift 5
#define array_w_shift 6
#define array_wv_shift 7
#define array_tcolor_shift 8
#define array_tshape_shift 9

#define x_max 7
#define y_max 7

short intersect_array[x_max + 1][y_max + 1];

void set_array_w(short &a) {
  a = a | array_w_mask | array_wv_mask;
}

void clear_array_w(short &a) {
  a = a & ~array_w_mask | array_wv_mask;
}

void set_array_n(short &a) {
  a = a | array_n_mask | array_nv_mask;
}

void clear_array_n(short &a) {
  a = a & ~array_n_mask | array_nv_mask;
}

void set_array_e(short &a) {
  a = a | array_e_mask | array_ev_mask;
}

void clear_array_e(short &a) {
  a = a & ~array_e_mask | array_ev_mask;
}

void set_array_s(short &a) {
  a = a | array_s_mask | array_sv_mask;
}

void clear_array_s(short &a) {
  a = a & ~array_s_mask | array_sv_mask;
}

void set_array_treasure(short &a, short shape, short color) {
  switch (shape) {
    case square:
    case triangle:
    case circle:
      a = a & ~array_tshape_mask & ~array_tcolor_mask | (shape << array_tshape_shift) | (color << array_tcolor_shift);
      break;
    default:
      a = a & ~array_tshape_mask & ~array_tcolor_mask;
      break;
  }
}

void array_update(short x, short y, bool west, bool north, bool east, bool south, short tshape, short tcolor) {
  short intersection = 0;

  if (west) {
    set_array_w(intersection);
    if (x) {
      set_array_e(intersect_array[x-1][y]);
    }
  } else {
    clear_array_w(intersection);
    if (x) {
      clear_array_e(intersect_array[x-1][y]);
    }
  }

  if (north) {
    set_array_n(intersection);
    if (y) {
      set_array_s(intersect_array[x][y-1]);
    }
  } else {
    clear_array_n(intersection);
    if (y) {
      clear_array_s(intersect_array[x][y-1]);
    }
  }

  if (east) {
    set_array_e(intersection);
    if (x < x_max) {
      set_array_w(intersect_array[x+1][y]);
    }
  } else {
    clear_array_e(intersection);
    if (x < x_max) {
      clear_array_w(intersect_array[x+1][y]);
    }
  }

  if (south) {
    set_array_s(intersection);
    if (y < y_max) {
      set_array_n(intersect_array[x][y+1]);
    }
  } else {
    clear_array_s(intersection);
    if (y < y_max) {
      clear_array_n(intersect_array[x][y+1]);
    }
  }
  set_array_treasure(intersection, tshape, tcolor);
  intersect_array[x][y] = intersection;
}
