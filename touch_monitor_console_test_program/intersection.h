#ifndef INTERSECTION_H
#define INTERSECTION_H

//finds intersection of two lines defined by two points
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
                       double* y);

#endif
