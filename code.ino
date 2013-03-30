//-*- coding: utf-8 -*-

#include <stdio.h>
#include <SoftwareSerial.h>
#include <PololuQik.h>
#include "ode.h"
#include "motion.h"

inline double degree_rad(double deg){
  /* return the angle in radians */
  return deg/180*M_PI;
}

// Intern-odometri structure
odotype odo;
smtype mission;
motiontype mot;

/* mission state. Each mission state combine the basic motion types in order to
   do complex movement  */
enum {ms_init,ms_fwd,ms_turn,ms_end};
const char* mission_type[] = {
  "ms_init","ms_fwd","ms_turn","ms_end"};

// left encoder
#define left_encPinA  2
#define left_encPinB  4

// right encoder
#define right_encPinA  3
#define right_encPinB  5

// Motor controller
PololuQik2s12v10 qik(8, 9, 10); // pin8 -> TX, pin9 -> RX, pin10 -> rst

void doLeftEncoder();
void doRightEncoder();
int fwd(double dist, double speed,int time);
int turn(double angle, double speed,int time);
void sync_arduino(motiontype *p);
void sm_update(smtype *p);

void setup(){

  // left encoder setup
  pinMode(left_encPinA, INPUT);
  digitalWrite(left_encPinA, HIGH);       // turn on pullup resistor
  pinMode(left_encPinB, INPUT);
  digitalWrite(left_encPinB, HIGH);       // turn on pullup resistor
  attachInterrupt(0, doLeftEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2

  // right encoder setup
  pinMode(right_encPinA, INPUT);
  digitalWrite(right_encPinA, HIGH);       // turn on pullup resistor
  pinMode(right_encPinB, INPUT);
  digitalWrite(right_encPinB, HIGH);       // turn on pullup resistor
  attachInterrupt(1, doRightEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2

  Serial.begin (9600);
  Serial.println("Start");

  // init motor
  qik.init();
  delay(100);

  //qik.setM0Speed(-10);
  qik.setSpeeds(-50,50);
  
  // init odometri
  odo.w = WHEEL_SEPARATION;
  odo.cr = c_r;
  odo.cl = c_l;
  mot.w = odo.w;
  /* Lidt forkert! */
  odo.left_enc = 0;
  odo.right_enc = 0;
  reset_odo(&odo);

  double speed,dist,angle;
  int n = 4;
  int running = 1;
  while(running){

    update_odo(&odo);
    /* Transfer position */
    mot.left_pos=odo.left_pos;
    mot.right_pos=odo.right_pos;
    
    /* set mission type */
    sm_update(&mission);
    switch (mission.state) {
    case ms_init:
      mission.state = ms_fwd;
      dist = 3;
      speed = 20;
      angle = degree_rad(90);
      break;
    case ms_fwd:
      speed = 20;
      if (fwd(dist,speed,mission.time)) {
        speed = 40;
        mission.state=ms_turn; //ms_end;
        } break;
    case ms_turn:
      /* make a square */
      if (turn(angle,speed,mission.time)){
        n=n-1;
        if (n==0) 
          mission.state=ms_end;
        else
          mission.state=ms_fwd;
      } break;
    case ms_end:
      mot.cmd=mot_stop;
      running=0;
    }
    /* start movement */
    update_motcon(&mot);

    /* synchronize arduino */
    sync_arduino(&mot);
    //    Serial.print(", left_pos:");
    //    Serial.print(odo.left_pos);

    Serial.print("theta:");
    Serial.print(odo.theta);
    Serial.print(", x-pos:");
    Serial.print(odo.x);
    Serial.print(", encoder: ");
    Serial.print(odo.left_enc);
    double dist_remain=(mot.dist+mot.startpos) - (mot.right_pos+mot.left_pos)/2;
    Serial.print(", dist-remain: ");
    Serial.print(dist_remain);
    Serial.print(" vs: ");
    Serial.print(mot.right_pos - mot.startpos);
    Serial.print(" hs: ");    
    /* Serial.print(fabs(mot.angle) ); */
    /* Serial.print( mot.w/2); */
    Serial.println(fabs(mot.angle) * mot.w/2);
    delay(100);

  }
}

void loop(){

}

void doLeftEncoder() {
  /* If pinA and pinB are both high or both low, it is spinning
   * forward. If they're different, it's going backward.
   *
   * For more information on speeding up this process, see
   * [Reference/PortManipulation], specifically the PIND register.
   */
  if (digitalRead(left_encPinA) == digitalRead(left_encPinB)) {
    odo.left_enc++;
  } else {
    odo.left_enc--;
  }
}

void doRightEncoder() {
  /* If pinA and pinB are both high or both low, it is spinning
   * forward. If they're different, it's going backward.
   *
   * For more information on speeding up this process, see
   * [Reference/PortManipulation], specifically the PIND register.
   */
  if (digitalRead(right_encPinA) == digitalRead(right_encPinB)) {
    odo.right_enc++;
  } else {
    odo.right_enc--;
  }
}


int fwd(double dist, double speed,int time){
  if (time==0){
    mot.cmd=mot_move;
    mot.speedcmd=speed;
    mot.dist=dist;
    return 0;
  }
  else
    // note that mot.finished is set in update_motcon
    return mot.finished;
}

int turn(double angle, double speed,int time){
  if (time==0 ){
    mot.cmd=mot_turn;
    mot.speedcmd=speed;
    mot.angle=angle;
    return 0;
  }
  else
    return mot.finished;
}

void sync_arduino(motiontype *p){
  /* update the arduino */

  //qik.setM0Speed(-10);
  qik.setSpeeds(-p->motorspeed_l,p->motorspeed_r);   
}

void sm_update(smtype *p){
  if (p->state!=p->oldstate){
    /* Print the change in mission_type. */
    /* printf ("Mission state changed from %s to: %s\n" */
    /*         ,mission_type[p->oldstate],mission_type[p->state]); */
    Serial.print("Mission state changed from ");
    Serial.print(mission_type[p->oldstate]);
    Serial.print(" to ");
    Serial.println(mission_type[p->state]);
    p->time=0;
    p->oldstate=p->state;
  }
  else {
    p->time++;
  }
}
