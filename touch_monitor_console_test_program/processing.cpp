#include <Windows.h>
#include "processing.h"
#include "intersection.h"
#include "simple_filter.h"
#include "screen_coordinates_processing.h"

//constants
const double kLength = 549.0;
const double kTransistorsLineLength = 333.54;
const int kTransistorsAmountInLine = 80;
const double filter_coef1 = 0.3;
const double filter_coef2 = 0.7;


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
void FindIntersectionAndFilterCoordinates(const double x_point1,
                                          const double y_point1,
                                          const double x_transmitter_1,
                                          const double y_transmitter_1,
                                          const double x_point2,
                                          const double y_point2,
                                          const double x_transmitter_2,
                                          const double y_transmitter_2);
// monitor like coordinates(y axis goes down, x axis goes right)
bool isPointInRectangle(double x_point,
                        double y_point,
                        double x_upper_left_corner_rectangle,
                        double y_upper_left_corner_rectangle,
                        double x_lower_right_corner_rectangle,
                        double y_lower_right_corner_rectangle);

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

  if(isPointInRectangle(real_x, real_y, 35.0, 29.0, 514.0, 298.0)) {
    double scale_x = 0.0;
    double scale_y = 0.0;
    if ((real_x != 0.0) && (real_y != 0.0)) {
      scale_x = real_x - 35.0;
      scale_y = real_y - 29.0;
    }
    coordinates_scale_print_and_set_cursor(scale_x, scale_y);
//    coordinates_scale_print_and_set_cursor(real_x, real_y);
  }
//  coordinates_scale_print_and_set_cursor(real_x, real_y);

  real_x_prev = real_x; // for filtering
  real_y_prev = real_y; // for filtering

}

void compute_coordinates_0_1() {
  double x_transmitter_1 = 0.0;
  double y_transmitter_1 = (kTransistorsLineLength / kTransistorsAmountInLine) * 2.5;
  double x_transmitter_2 = 0.0;
  double y_transmitter_2 = (kTransistorsLineLength / kTransistorsAmountInLine) * 77.5;

  double x_point1 = kLength;
  double y_point1 = (kTransistorsLineLength / kTransistorsAmountInLine) * (shadow_centers[0]);

  double x_point2 = kLength;
  double y_point2 = (kTransistorsLineLength / kTransistorsAmountInLine) * (shadow_centers[1]);

  FindIntersectionAndFilterCoordinates(x_point1,
                                       y_point1,
                                       x_transmitter_1,
                                       y_transmitter_1,
                                       x_point2,
                                       y_point2,
                                       x_transmitter_2,
                                       y_transmitter_2);
}

void compute_coordinates_1_2() {
  double x_transmitter_1 = 0.0;
  double y_transmitter_1 = (kTransistorsLineLength / kTransistorsAmountInLine) * 77.5;
  double x_transmitter_2 = kLength;
  double y_transmitter_2 = (kTransistorsLineLength / kTransistorsAmountInLine) * 77.5;

  double x_point1 = kLength;
  double y_point1 = (kTransistorsLineLength / kTransistorsAmountInLine) * (shadow_centers[1]);
  double x_point2 = 0.0;
  double y_point2 = (kTransistorsLineLength / kTransistorsAmountInLine) * (kTransistorsAmountInLine - shadow_centers[2]);

  FindIntersectionAndFilterCoordinates(x_point1,
                                       y_point1,
                                       x_transmitter_1,
                                       y_transmitter_1,
                                       x_point2,
                                       y_point2,
                                       x_transmitter_2,
                                       y_transmitter_2);
}

void compute_coordinates_2_3() {

  double x_transmitter_1 = kLength;
  double y_transmitter_1 = (kTransistorsLineLength / kTransistorsAmountInLine) * 77.5;
  double x_transmitter_2 = kLength;
  double y_transmitter_2 = (kTransistorsLineLength / kTransistorsAmountInLine) * 2.5;

  double x_point1 = 0.0;
  double y_point1 = (kTransistorsLineLength / kTransistorsAmountInLine) * (kTransistorsAmountInLine - shadow_centers[2]);
  double x_point2 = 0.0;
  double y_point2 = (kTransistorsLineLength / kTransistorsAmountInLine) * (kTransistorsAmountInLine - shadow_centers[3]);

  FindIntersectionAndFilterCoordinates(x_point1,
                                       y_point1,
                                       x_transmitter_1,
                                       y_transmitter_1,
                                       x_point2,
                                       y_point2,
                                       x_transmitter_2,
                                       y_transmitter_2);
}

void compute_coordinates_0_3() {
  double x_transmitter_1 = 0.0;
  double y_transmitter_1 = (kTransistorsLineLength / kTransistorsAmountInLine) * 2.5;
  double x_transmitter_2 = kLength;
  double y_transmitter_2 = (kTransistorsLineLength / kTransistorsAmountInLine) * 2.5;

  double x_point1 = kLength;
  double y_point1 = (kTransistorsLineLength / kTransistorsAmountInLine) * (shadow_centers[0]);
  double x_point2 = 0.0;
  double y_point2 = (kTransistorsLineLength / kTransistorsAmountInLine) * (kTransistorsAmountInLine - shadow_centers[3]);

  FindIntersectionAndFilterCoordinates(x_point1,
                                       y_point1,
                                       x_transmitter_1,
                                       y_transmitter_1,
                                       x_point2,
                                       y_point2,
                                       x_transmitter_2,
                                       y_transmitter_2);
}

void coordinates_scale_print_and_set_cursor(double x, double y) {
  if ((x != 0.0) && (y != 0.0)) {
    x = x * (1920.0/479.0 /*kLength*/);
    y = y * (1080.0/269.0 /*kTransistorsLineLength*/);
  }
  print_coordinates_and_set_cursor(x, y);
}

void FindIntersectionAndFilterCoordinates(const double x_point1,
                                          const double y_point1,
                                          const double x_transmitter_1,
                                          const double y_transmitter_1,
                                          const double x_point2,
                                          const double y_point2,
                                          const double x_transmitter_2,
                                          const double y_transmitter_2) {
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
  real_x = simple_filter(real_x, real_x_prev, filter_coef1, filter_coef2);
  real_y = simple_filter(real_y, real_y_prev, filter_coef1, filter_coef2);
}

// monitor like coordinates(y axis goes down, x axis goes right)
bool isPointInRectangle(double x_point,
                        double y_point,
                        double x_upper_left_corner_rectangle,
                        double y_upper_left_corner_rectangle,
                        double x_lower_right_corner_rectangle,
                        double y_lower_right_corner_rectangle) {
  if ((x_point == 0.0) && (y_point == 0.0)) {
    return true;
  }
  if ((x_point > x_upper_left_corner_rectangle) && (x_point < x_lower_right_corner_rectangle)) {
    if ((y_point > y_upper_left_corner_rectangle) && (y_point < y_lower_right_corner_rectangle)) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}
// void const_test(const int* input1, int* output1) {
//   *input1 = 0x1CBA; //error 
//   *output1 = 1;
// }
