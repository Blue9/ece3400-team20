#ifndef intersect_globals_h
#define intersect_globals_h

#define x_mask 0xe000
#define x_shift 13
#define y_mask 0x1c00
#define y_shift 10
#define iamhere_mask 0x0200
#define iamhere_shift 9
#define w_mask 0x0100
#define w_shift 8
#define n_mask 0x0080
#define n_shift 7
#define e_mask 0x0040
#define e_shift 6
#define s_mask 0x0020
#define s_shift 5
#define robot_mask 0x0010
#define robot_shift 4
#define shape_mask 0x000c
#define shape_shift 2
#define color_mask 0x0002
#define color_shift 1

#define get_x(a) (a&x_mask) >> x_shift
#define get_y(a) (a&y_mask) >> y_shift
#define get_iamhere(a) (a&iamhere_mask) >> iamhere_shift
#define get_w(a) (a&w_mask) >> w_shift
#define get_n(a) (a&n_mask) >> n_shift
#define get_e(a) (a&e_mask) >> e_shift
#define get_s(a) (a&s_mask) >> s_shift
#define get_robot(a) (a&robot_mask) >> robot_shift
#define get_shape(a) (a&shape_mask) >> shape_shift
#define get_color(a) (a&color_mask) >> color_shift

#define red 0b0
#define blue 0b1

#define no_treasure 0x0
//#define square 0x1
#define triangle 0x2
#define circle 0x3
#define square_square 0x1

#define x_max 10
#define y_max 10

#endif
