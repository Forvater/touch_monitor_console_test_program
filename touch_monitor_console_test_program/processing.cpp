#include <string.h>
#include "processing.h"
#include <stdio.h>
#include <Windows.h>
#include "intersection.h"

unsigned char processing_structure[LINES][TRANSISTORS_IN_LINE];
unsigned char shadow_centers[LINES];

void find_line_shadow_center(int line_num);
void coordinates();
void compute_coordinates_0_1();
void compute_coordinates_1_2();
void compute_coordinates_2_3();
void compute_coordinates_0_3();
void coordinates_scale_print_and_set_cursor(double x, double y);

void free_shadow_centers_structure() {
  for(int i = 0; i < 4; i++) {
    shadow_centers[i] = 0;
  }
}

void free_processing_structure() {
//   for(int i = 0; i < LINES; i++) {
//     for(int j = 0; j < TRANSISTORS_IN_LINE; j++) {
//       processing_structure[i][j] = j;
//     }
//   }
  memset(processing_structure, 0, (LINES * TRANSISTORS_IN_LINE));
}

void processing_transistors() {
  find_shadow_centers();
}

void find_shadow_centers() {
  free_shadow_centers_structure();
  for(int i = 0; i < 4; i++) {
    find_line_shadow_center(i);
  }
  coordinates();
//   printf("\r\n");
//   printf("\r\n");
//   for(int i = 0; i < 4; i++) {
//    printf("%d\r\n",shadow_centers[i]);
//   }
//   printf("\r\n");
//   printf("\r\n");
}

void find_line_shadow_center(int line_num) {

  unsigned int center = 0;
  unsigned char count = 0;

  for (int i = 0; i < 80; i++) {
    if (processing_structure[line_num][i] == 1) {
      center = center + i;
      count++;
    }
  }
  if (count > 0) {
    center = (center/count);
  }
  shadow_centers[line_num] = center;
}

void coordinates() {
  if ((shadow_centers[0] != 0) && (shadow_centers[1] != 0)) {
    compute_coordinates_0_1();
  }
  if ((shadow_centers[1] != 0) && (shadow_centers[2] != 0)) {
    compute_coordinates_1_2();
  }
  if ((shadow_centers[2] != 0) && (shadow_centers[3] != 0)) {
    compute_coordinates_2_3();
  }
  if ((shadow_centers[3] != 0) && (shadow_centers[0] != 0)) {
    compute_coordinates_0_3();
  }
}

void compute_coordinates_0_1() {
  double x_transmitter_1 = 550.0;
  double y_transmitter_1 = (333.54 / 80.0) * 77.5;
  double x_transmitter_2 = 550.0;
  double y_transmitter_2 = (333.54 / 80.0) * 2.5;

  double x_point1 = 0.0;
  double y_point1 = (333.54 / 80.0) * (80 - shadow_centers[0]);

  double x_point2 = 0.0;
  double y_point2 = (333.54 / 80.0) * (80 - shadow_centers[1]);

  double x = 0.0;
  double y = 0.0;
  find_intersection(x_point1,
      y_point1,
      x_transmitter_1,
      y_transmitter_1,
      x_point2,
      y_point2,
      x_transmitter_2,
      y_transmitter_2,
      &x,
      &y);
  coordinates_scale_print_and_set_cursor(x, y);
}

void compute_coordinates_1_2() {
  double x_transmitter_1 = 550.0;
  double y_transmitter_1 = (333.54 / 80.0) * 2.5;
  double x_transmitter_2 = 0.0;
  double y_transmitter_2 = (333.54 / 80.0) * 2.5;

  double x_point1 = 0.0;
  double y_point1 = (333.54 / 80.0) * (80 - shadow_centers[1]);
  double x_point2 = 550.0;
  double y_point2 = (333.54 / 80.0) * shadow_centers[2];

  double x = 0.0;
  double y = 0.0;
  find_intersection(x_point1,
      y_point1,
      x_transmitter_1,
      y_transmitter_1,
      x_point2,
      y_point2,
      x_transmitter_2,
      y_transmitter_2,
      &x,
      &y);
  coordinates_scale_print_and_set_cursor(x, y);
}

void compute_coordinates_2_3() {
  double x_transmitter_1 = 0.0;
  double y_transmitter_1 = (333.54 / 80.0) * 2.5;
  double x_transmitter_2 = 0.0;
  double y_transmitter_2 = (333.54 / 80.0) * 77.5;

  double x_point1 = 550.0;
  double y_point1 = (333.54 / 80.0) * shadow_centers[2];
  double x_point2 = 550.0;
  double y_point2 = (333.54 / 80.0) * shadow_centers[3];

  double x = 0.0;
  double y = 0.0;
  find_intersection(x_point1,
      y_point1,
      x_transmitter_1,
      y_transmitter_1,
      x_point2,
      y_point2,
      x_transmitter_2,
      y_transmitter_2,
      &x,
      &y);
  coordinates_scale_print_and_set_cursor(x, y);
}

void compute_coordinates_0_3() {
  double x_transmitter_1 = 550.0;
  double y_transmitter_1 = (333.54 / 80.0) * 77.5;
  double x_transmitter_2 = 0.0;
  double y_transmitter_2 = (333.54 / 80.0) * 77.5;

  double x_point1 = 0.0;
  double y_point1 = (333.54 / 80.0) * (80 - shadow_centers[0]);
  double x_point2 = 550.0;
  double y_point2 = (333.54 / 80.0) * shadow_centers[3];

  double x = 0.0;
  double y = 0.0;
  find_intersection(x_point1,
                    y_point1,
                    x_transmitter_1,
                    y_transmitter_1,
                    x_point2,
                    y_point2,
                    x_transmitter_2,
                    y_transmitter_2,
                    &x,
                    &y);
  coordinates_scale_print_and_set_cursor(x, y);
}

void coordinates_scale_print_and_set_cursor(double x, double y) {
  x = x * (1920.0/550.0);
  y = y * (1080.0/333.54);
  printf("X:  %4.2f\r\n", x);
  printf("Y:  %4.2f\r\n", y);
  SetCursorPos(x,y);
}

// void const_test(const int* input1, int* output1) {
//   *input1 = 0x1CBA; //error 
//   *output1 = 1;
// }





