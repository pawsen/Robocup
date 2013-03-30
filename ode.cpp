#include "ode.h"
#include <math.h>
#include <stdio.h>

void reset_odo(odotype * p)
{
  p->right_pos = p->left_pos = 0.0;
  p->right_enc_old = p->right_enc;
  p->left_enc_old = p->left_enc;
}

void update_odo(odotype *p)
{
  /* get position from encoder ticks */

  int delta;
  double dUr, dUl, dU, dTheta;

  delta = p->right_enc - p->right_enc_old;
  /* if (delta > 0x8000) delta -= 0x10000; */
  /* else if (delta < -0x8000) delta += 0x10000; */
  p->right_enc_old = p->right_enc;

  // incremental displacement of right wheel
  dUr = delta * p->cr;
  p->right_pos += delta * p->cr;
  // p->cr is c_m in notes. Eg. only dependent on physical parameters
  // delta is N_L/R in notes.

  delta = -(p->left_enc - p->left_enc_old);
  /* if (delta > 0x8000) delta -= 0x10000; */
  /* else if (delta < -0x8000) delta += 0x10000; */
  p->left_enc_old = p->left_enc;
  // incremental displacement of left wheel
  dUl = delta * p->cl;
  //print("delta: %f \n");
  p->left_pos += delta * p->cl;

  //displacement of the center point of the robot
  dU = (dUr + dUl)/2;
  // change in orientation
  dTheta = (dUr - dUl)/p->w;
  // new pos. NB. calc theta before linear pos.

  p->theta = p->theta+dTheta;
  p->x = p->x+dU*cos(p->theta);
  p->y = p->y+dU*sin(p->theta);
}
