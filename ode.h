#ifndef ode_h
#define ode_h

// #define M_PI 3.14159265
/* Physical parameters */
#define WHEEL_DIAMETER   0.0641164  /* m */
#define WHEEL_SEPARATION 2    /* m */
#define c_l      0.001   /* 1.001409 */
#define c_r      0.001   /* 0.998591 */
#define DELTA_M (M_PI * WHEEL_DIAMETER / 2000)
#define ROBOT_LENGTH 0.2 /*   */

typedef struct{ //input signals
  unsigned int left_enc,right_enc; // encoder ticks
  // parameters
  double w; // wheel separation
  double cr,cl;   // meters per encoder tick
  /* internal position from odometri */
  double x, y, theta;
  //output signals
  double right_pos,left_pos;
  // internal variables
  unsigned int left_enc_old, right_enc_old;
} odotype;

void update_odo(odotype *p);
void reset_odo(odotype * p);

// NB!
// Don't initialize variables in headers. Put declaration in header(eg use extern)
// and initialization in one of the c files.

// Intern-odometri structure
extern odotype odo;

#endif
