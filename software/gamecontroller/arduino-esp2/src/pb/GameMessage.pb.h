/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.2 */

#ifndef PB_SRC_PB_GAMEMESSAGE_PB_H_INCLUDED
#define PB_SRC_PB_GAMEMESSAGE_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _Vec2 {
    float x;
    float y;
} Vec2;

typedef struct _BallState {
    bool has_position;
    Vec2 position;
} BallState;

typedef struct _BoardState {
    bool has_orientation;
    Vec2 orientation;
} BoardState;

typedef struct _JoystickState {
    bool has_orientation;
    Vec2 orientation;
} JoystickState;


/* Initializer values for message structs */
#define Vec2_init_default                        {0, 0}
#define JoystickState_init_default               {false, Vec2_init_default}
#define BallState_init_default                   {false, Vec2_init_default}
#define BoardState_init_default                  {false, Vec2_init_default}
#define Vec2_init_zero                           {0, 0}
#define JoystickState_init_zero                  {false, Vec2_init_zero}
#define BallState_init_zero                      {false, Vec2_init_zero}
#define BoardState_init_zero                     {false, Vec2_init_zero}

/* Field tags (for use in manual encoding/decoding) */
#define Vec2_x_tag                               1
#define Vec2_y_tag                               2
#define BallState_position_tag                   1
#define BoardState_orientation_tag               1
#define JoystickState_orientation_tag            1

/* Struct field encoding specification for nanopb */
#define Vec2_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, FLOAT,    x,                 1) \
X(a, STATIC,   SINGULAR, FLOAT,    y,                 2)
#define Vec2_CALLBACK NULL
#define Vec2_DEFAULT NULL

#define JoystickState_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, MESSAGE,  orientation,       1)
#define JoystickState_CALLBACK NULL
#define JoystickState_DEFAULT NULL
#define JoystickState_orientation_MSGTYPE Vec2

#define BallState_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, MESSAGE,  position,          1)
#define BallState_CALLBACK NULL
#define BallState_DEFAULT NULL
#define BallState_position_MSGTYPE Vec2

#define BoardState_FIELDLIST(X, a) \
X(a, STATIC,   OPTIONAL, MESSAGE,  orientation,       1)
#define BoardState_CALLBACK NULL
#define BoardState_DEFAULT NULL
#define BoardState_orientation_MSGTYPE Vec2

extern const pb_msgdesc_t Vec2_msg;
extern const pb_msgdesc_t JoystickState_msg;
extern const pb_msgdesc_t BallState_msg;
extern const pb_msgdesc_t BoardState_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define Vec2_fields &Vec2_msg
#define JoystickState_fields &JoystickState_msg
#define BallState_fields &BallState_msg
#define BoardState_fields &BoardState_msg

/* Maximum encoded size of messages (where known) */
#define Vec2_size                                10
#define JoystickState_size                       12
#define BallState_size                           12
#define BoardState_size                          12

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif