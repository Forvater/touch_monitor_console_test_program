double simple_filter(double x, double x_prev, double coeff1, double coeff2) {
  if (x_prev == 0.0) x_prev = x;
  return (x * coeff1) + (x_prev * coeff2);
}