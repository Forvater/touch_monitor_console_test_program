static void find_line_factors(/*input*/
                              const double x1,
                              const double y1,
                              const double x2,
                              const double y2,
                              /*output*/
                              double* k,
                              double* b) {
  *k = (y2 - y1) / (x2 - x1);
  *b = y1 - ((*k) * x1);
}

void find_intersection(/*input*/
                       const double first_line_x1,
                       const double first_line_y1,
                       const double first_line_x2,
                       const double first_line_y2,
                       const double second_line_x1,
                       const double second_line_y1,
                       const double second_line_x2,
                       const double second_line_y2,
                       /*output*/
                       double* x,
                       double* y) {
  double k1 = 0;// should be static ?
  double b1 = 0;// should be static ?
  double k2 = 0;// should be static ?
  double b2 = 0;// should be static ?
  find_line_factors(first_line_x1,
                    first_line_y1,
                    first_line_x2,
                    first_line_y2,
                    &k1,
                    &b1);
  find_line_factors(second_line_x1,
                    second_line_y1,
                    second_line_x2,
                    second_line_y2,
                    &k2,
                    &b2);
  *x = ((b1 - b2) / (k2 - k1));
  *y = (k2 * (*x)) + b2;
}
