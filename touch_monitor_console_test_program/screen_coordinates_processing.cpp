#include <stdio.h>
#include <Windows.h>
#include "mouse_functions.h"
#include "simple_console_timer.h"

const double hold_area_x = 30.0;
const double hold_area_y = 30.0;
double screen_x = 0.0;
double screen_y = 0.0;
double screen_x_prev = 0.0;
double screen_y_prev = 0.0;
double screen_x_ent_zone = 0.0; // x coor of entering zone point
double screen_y_ent_zone = 0.0; // y coor of entering zone point
double screen_x_lv_zone = 0.0; // x coor of leaving zone point
double screen_y_lv_zone = 0.0; // y coor of leaving zone point
unsigned long enter_zone_time = 0; // milliseconds from machine start
unsigned long leave_zone_time = 0; // milliseconds from machine start
unsigned long inside_zone_time = 0;


void enter_zone_event_handler();
void leave_zone_event_handler();

void print_coordinates_and_set_cursor(double x, double y) {
  if ((x != 0.0) && (y != 0.0)) {
//    printf("X:  %4.2f\r\n", x);
//    printf("Y:  %4.2f\r\n", y);
    SetCursorPos(x, y);
  }
  screen_x = x;
  screen_y = y;
  if ((screen_x != 0.0) && (screen_y != 0.0)) {
    if ((screen_x_prev == 0.0) && (screen_y_prev == 0.0)) {
      screen_x_ent_zone = screen_x;
      screen_y_ent_zone = screen_y;
      // enter zone event handler
      enter_zone_event_handler();
    }
  }
  if ((screen_x_prev != 0.0) && (screen_y_prev != 0.0)) {
    if ((screen_x == 0.0) && (screen_y == 0.0)) {
      screen_x_lv_zone = screen_x_prev;
      screen_y_lv_zone = screen_y_prev;
      // leave zone event handler
      leave_zone_event_handler();
    }
  }
  screen_x_prev = screen_x;
  screen_y_prev = screen_y;
}

bool condit() {
  if (((screen_x_ent_zone - screen_x) < hold_area_x) && ((screen_y_ent_zone - screen_y) < hold_area_y)) {
    return true;
  } else {
    return false;
  }
}

void callback_proc() {
//  mouse_lmb_up((long) screen_x_ent_zone, (long) screen_x_ent_zone);
  mouse_rmb_click((long) screen_x_ent_zone, (long) screen_x_ent_zone);
}


void enter_zone_event_handler() {
  enter_zone_time = GetTickCount();
  mouse_lmb_down(screen_x_ent_zone,screen_y_ent_zone);
  simple_timer(1000, &condit, &callback_proc);
  // SetTimer
  printf("ent X:  %4.2f\r\n", screen_x_ent_zone);
  printf("ent Y:  %4.2f\r\n", screen_y_ent_zone);
}

void leave_zone_event_handler() {
  leave_zone_time = GetTickCount();
  inside_zone_time = leave_zone_time - enter_zone_time;
  mouse_lmb_up(screen_x_lv_zone, screen_y_lv_zone);
//  if(inside_zone_time >= 1000) {
//    mouse_rmb_click(screen_x_lv_zone,screen_y_lv_zone);
//    mouse_lmb_click(screen_x_lv_zone,screen_y_lv_zone);
//  }
  printf("lv X:  %4.2f\r\n", screen_x_lv_zone);
  printf("lv Y:  %4.2f\r\n", screen_y_lv_zone);
  printf("time inside zone:  %d\r\n", inside_zone_time);
}
