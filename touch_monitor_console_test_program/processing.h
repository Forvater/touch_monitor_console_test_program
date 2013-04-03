#ifndef PROCESSING_H
#define PROCESSING_H

#define LINES 4
#define TRANSISTORS_IN_LINE 80

extern unsigned char processing_structure[LINES][TRANSISTORS_IN_LINE];

//extern const unsigned char kLines = 4;
//extern const unsigned char kTransistorsInLine = 80;

//extern unsigned char processing_structure[kLines][kTransistorsInLine];

void free_processing_structure();
void find_shadow_centers();
void processing_transistors();

#endif
