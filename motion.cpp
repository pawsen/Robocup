#include "motion.h"
#include "ode.h"
#include <math.h>
#include <SoftwareSerial.h>


void update_motcon(motiontype *p){

  double dV =0 ;
  double dist_remain;
  /* keep start_theta between calls (local persistent) */
  static double start_theta;

  /* This is only run when state is changed.
     Eg. turn, run, etc. Used for setting one-time variables */
  if (p->cmd !=0){
    p->finished=0;
    switch (p->cmd){
    case mot_stop:
      p->curcmd=mot_stop;
      break;
    case mot_move:
      p->startpos=(p->left_pos+p->right_pos)/2;
      start_theta = odo.theta;
      p->curcmd=mot_move;
      break;
    case mot_turn:
      if (p->angle > 0)
        p->startpos=p->right_pos;
      else
        p->startpos=p->left_pos;
      p->curcmd=mot_turn;
      break;
    }
    p->cmd=0;
  }

  /* do the actual movement */
  switch (p->curcmd){
  case mot_stop:
    stop_motor(p);
    break;
  case mot_move:
    dist_remain=(p->dist+p->startpos) - (p->right_pos+p->left_pos)/2;
    if (dist_remain <= 0){
      stop_motor(p);
    }else{
      p->motorspeed_l=p->speedcmd;
      p->motorspeed_r=p->speedcmd;
    }
    break;
  case mot_turn:
    if (p->angle>0){
      /* turn over wheel */
      //p->motorspeed_l=0;
      /* turn over center of robot */
      if (p->right_pos-p->startpos < fabs(p->angle)*p->w/2){
        p->motorspeed_r=p->speedcmd/2;
        p->motorspeed_l=-(p->speedcmd)/2;
      }
      else
        stop_motor(p);
    }
    else {
      if (p->left_pos-p->startpos < fabs(p->angle)*p->w/2){
        p->motorspeed_l=p->speedcmd/2;
        p->motorspeed_r=-(p->speedcmd)/2;
      }
      else
        stop_motor(p);
    }
    break;
  }
}


void stop_motor(motiontype *p){
  p->motorspeed_r=0;
  p->motorspeed_l=0;
  p->finished=1;
}


/* void sm_update(smtype *p){ */
/*   if (p->state!=p->oldstate){ */
/*     /\* Print the change in mission_type. *\/ */
/*     /\* printf ("Mission state changed from %s to: %s\n" *\/ */
/*     /\*         ,mission_type[p->oldstate],mission_type[p->state]); *\/ */
/*     p->time=0; */
/*     p->oldstate=p->state; */
/*   } */
/*   else { */
/*     p->time++; */
/*   } */
/* } */
