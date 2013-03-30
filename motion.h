#ifndef motion_h
#define motion_h


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
void sm_update(smtype *p);

#endif
