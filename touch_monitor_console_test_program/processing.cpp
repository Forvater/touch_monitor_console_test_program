#include <Windows.h>
#include "processing.h"
#include "intersection.h"
#include "simple_filter.h"
#include "screen_coordinates_processing.h"

unsigned char processing_structure[LINES][TRANSISTORS_IN_LINE];
unsigned char shadow_centers[LINES];

double real_x = 0.0;
double real_y = 0.0;
double real_x_prev = 0.0; // x, computed from previous packet
double real_y_prev = 0.0; // y, computed from previous packet

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
  } else if ((shadow_centers[1] != 0) && (shadow_centers[2] != 0)) {
    compute_coordinates_1_2();
  } else if ((shadow_centers[2] != 0) && (shadow_centers[3] != 0)) {
    compute_coordinates_2_3();
  } else if ((shadow_centers[3] != 0) && (shadow_centers[0] != 0)) {
    compute_coordinates_0_3();
  } else {
    real_x = 0.0;
    real_y = 0.0;
  }

  coordinates_scale_print_and_set_cursor(real_x, real_y);

  real_x_prev = real_x; // for filtering
  real_y_prev = real_y; // for filtering

}

void compute_coordinates_0_1() {
  double x_transmitter_1 = 0.0;//550.0;
  double y_transmitter_1 = (333.54 / 80.0) * 2.5;//(333.54 / 80.0) * 77.5;
  double x_transmitter_2 = 0.0;//550.0;
  double y_transmitter_2 = (333.54 / 80.0) * 77.5;//(333.54 / 80.0) * 2.5;

  double x_point1 = 550.0;//0.0;
  double y_point1 = (333.54 / 80.0) * (/*80 - */shadow_centers[0]);

  double x_point2 = 550.0;//0.0;
  double y_point2 = (333.54 / 80.0) * (/*80 - */shadow_centers[1]);

  find_intersection(x_point1,
                    y_point1,
                    x_transmitter_1,
                    y_transmitter_1,
                    x_point2,
                    y_point2,
                    x_transmitter_2,
                    y_transmitter_2,
                    &real_x,
                    &real_y);

  real_x = simple_filter(real_x, real_x_prev, 0.3, 0.7);
  real_y = simple_filter(real_y, real_y_prev, 0.3, 0.7);
}

void compute_coordinates_1_2() {
  double x_transmitter_1 = 0.0;//550.0;
  double y_transmitter_1 = (333.54 / 80.0) * 77.5;//(333.54 / 80.0) * 2.5;
  double x_transmitter_2 = 550.0;//0.0;
  double y_transmitter_2 = (333.54 / 80.0) * 77.5;//(333.54 / 80.0) * 2.5;

  double x_point1 = 550.0;//0.0;
  double y_point1 = (333.54 / 80.0) * (/*80 - */shadow_centers[1]);
  double x_point2 = 0.0;//550.0;
  double y_point2 = (333.54 / 80.0) * (80 - shadow_centers[2]);


  find_intersection(x_point1,
                    y_point1,
                    x_transmitter_1,
                    y_transmitter_1,
                    x_point2,
                    y_point2,
                    x_transmitter_2,
                    y_transmitter_2,
                    &real_x,
                    &real_y);

  real_x = simple_filter(real_x, real_x_prev, 0.3, 0.7);
  real_y = simple_filter(real_y, real_y_prev, 0.3, 0.7);

}

void compute_coordinates_2_3() {

  double x_transmitter_1 = 550.0;//0.0;
  double y_transmitter_1 = (333.54 / 80.0) * 77.5;//(333.54 / 80.0) * 2.5;
  double x_transmitter_2 = 550.0;//0.0;
  double y_transmitter_2 = (333.54 / 80.0) * 2.5;//(333.54 / 80.0) * 77.5;

  double x_point1 = 0.0;//550.0;
  double y_point1 = (333.54 / 80.0) * (80 - shadow_centers[2]);
  double x_point2 = 0.0;//550.0;
  double y_point2 = (333.54 / 80.0) * (80 - shadow_centers[3]);


  find_intersection(x_point1,
                    y_point1,
                    x_transmitter_1,
                    y_transmitter_1,
                    x_point2,
                    y_point2,
                    x_transmitter_2,
                    y_transmitter_2,
                    &real_x,
                    &real_y);

  real_x = simple_filter(real_x, real_x_prev, 0.3, 0.7);
  real_y = simple_filter(real_y, real_y_prev, 0.3, 0.7);
}

void compute_coordinates_0_3() {
  double x_transmitter_1 = 0.0;//550.0;
  double y_transmitter_1 = (333.54 / 80.0) * 2.5;//(333.54 / 80.0) * 77.5;
  double x_transmitter_2 = 550.0;//0.0;
  double y_transmitter_2 = (333.54 / 80.0) * 2.5;//(333.54 / 80.0) * 77.5;

  double x_point1 = 550.0;//0.0;
  double y_point1 = (333.54 / 80.0) * (/*80 - */shadow_centers[0]);
  double x_point2 = 0.0;//550.0;
  double y_point2 = (333.54 / 80.0) * (80 - shadow_centers[3]);

  find_intersection(x_point1,
                    y_point1,
                    x_transmitter_1,
                    y_transmitter_1,
                    x_point2,
                    y_point2,
                    x_transmitter_2,
                    y_transmitter_2,
                    &real_x,
                    &real_y);

  real_x = simple_filter(real_x, real_x_prev, 0.3, 0.7);
  real_y = simple_filter(real_y, real_y_prev, 0.3, 0.7);

}

void coordinates_scale_print_and_set_cursor(double x, double y) {
  if ((x != 0.0) && (y != 0.0)) {
    x = x * (1920.0/550.0);
    y = y * (1080.0/333.54);
  }
  print_coordinates_and_set_cursor(x, y);
}

// void const_test(const int* input1, int* output1) {
//   *input1 = 0x1CBA; //error 
//   *output1 = 1;
// }
