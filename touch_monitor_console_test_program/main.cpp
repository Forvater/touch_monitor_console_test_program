#include "com_port_settings.h"
#include "processing.h"
#include "read_settings_from_file.h"

int main() {
  read_settings_from_file();
  free_processing_structure();
  open_com_port();
  configure_com_port();
  start_reading_thread();
  getchar();
  stop_reading_thread = true;
  Sleep(100); // wait until reading thread stops
  return 0;
}
