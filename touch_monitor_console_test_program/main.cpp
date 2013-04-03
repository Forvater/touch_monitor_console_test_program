#include "com_port_settings.h"
#include "processing.h"

int main() {
  free_processing_structure();
  open_com_port();
  configure_com_port();
  start_reading_thread();
  getchar();
  stop_reading_thread = true;
  Sleep(100); // wait until reading thread stops
  return 0;
}
