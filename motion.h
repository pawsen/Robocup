#ifndef MOTION_H
#define MOTION_H


typedef struct{//input
  int cmd;
  int curcmd;
  double speedcmd;
  double dist;
  double angle;
  double left_pos,right_pos;
  // parameters
  double w;
  //output
  double motorspeed_l,motorspeed_r;
  int finished;
  // internal variables
  double startpos;
}motiontype;

typedef struct{
  int state,oldstate;
  int time;
}smtype;

void update_motcon(motiontype *p);
void stop_motor(motiontype *p);

/* motion type. */
enum {mot_stop=1,mot_move,mot_turn};

#endif
