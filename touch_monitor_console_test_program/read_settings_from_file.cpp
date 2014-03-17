#include <Windows.h>
#include <atlstr.h>

int screen_width = 0;
int screen_height = 0;
CString com_port_name = _T("");

static const CString settings_file_name = _T("settings.txt");

static HANDLE settings_file_handle = NULL;

int pick_out_value(unsigned int start_index, const char* str) {
  char tempstr[200];
  strcpy(tempstr, &(str[start_index]));
  return atoi(tempstr);
}

CString pick_out_string_value(unsigned int start_index, const char* str) {
  char tempstr[200];
  strcpy(tempstr, &(str[start_index]));
  CString temp_cstring(tempstr);
  return temp_cstring;
}



// void compare_strings_and_pick_out_value(char* strings_array, const int number_of_strings_in_array) {
//   const char* screen_width_string = {"screen_width = "};
//   const char* screen_height_string = {"screen_height = "};
//   const char* com_port_string = {"com_port = "};
// 
//   for (int i = 0; i < number_of_strings_in_array; i++) {
//     if (0 == strncmp(screen_width_string, &(strings_array[i]), strlen(screen_width_string))) {
//       screen_width = pick_out_value(strlen(screen_width_string), &(strings_array[i]));
//     }
//     if (0 == strncmp(screen_height_string, &(strings_array[i]), strlen(screen_height_string))) {
//       screen_height = pick_out_value(strlen(screen_height_string), &(strings_array[i]));
//     }
//     if (0 == strncmp(com_port_string, &(strings_array[i]), strlen(com_port_string))) {
//       com_port_name = pick_out_string_value(strlen(com_port_string), &(strings_array[i]));
//     }
//   }
// }

// convert raw byte buffer to array of strings
void parse_buffer(unsigned char* buffer, int buffer_size) {
  const int max_strings_count = 10;
  const int max_strings_length = 200;

  char strings_array[max_strings_count][max_strings_length];

  int current_string_number = 0;
  int current_string_begin_index = 0;

  unsigned char CR_symbol = 0x0d;
  unsigned char LF_symbol = 0x0a;

  for (int i = 0; i < buffer_size; i++) {
    if ((buffer[i] == CR_symbol) && (buffer[i+1] == LF_symbol)) { //CRLF - end of the string in text file
      memcpy(strings_array[current_string_number], &(buffer[current_string_begin_index]), (i - current_string_begin_index));
      strings_array[current_string_number][(i - current_string_begin_index)] = 0;
      current_string_begin_index = i+2;
      current_string_number++;
    }
  }


//  compare_strings_and_pick_out_value(&(strings_array[0][0]), current_string_number);

  const char* screen_width_string = {"screen_width = "};
  const char* screen_height_string = {"screen_height = "};
  const char* com_port_string = {"com_port = "};

  for (int i = 0; i < current_string_number; i++) {
    if (0 == strncmp(screen_width_string, strings_array[i], strlen(screen_width_string))) {
      screen_width = pick_out_value(strlen(screen_width_string), strings_array[i]);
    }
    if (0 == strncmp(screen_height_string, strings_array[i], strlen(screen_height_string))) {
      screen_height = pick_out_value(strlen(screen_height_string), strings_array[i]);
    }
    if (0 == strncmp(com_port_string, strings_array[i], strlen(com_port_string))) {
      com_port_name = pick_out_string_value(strlen(com_port_string), strings_array[i]);
    }
  }
}


void read_file(HANDLE file, DWORD file_size) {
  unsigned char* buffer = new unsigned char[file_size];
  DWORD number_of_bytes_read = 0;
  ReadFile(file, buffer, file_size, &number_of_bytes_read, NULL);
  parse_buffer(buffer, file_size);
  delete[] buffer;
}

void read_settings_from_file() {
  settings_file_handle = CreateFile(settings_file_name,
                                    GENERIC_READ,
                                    0,      //  must be opened with exclusive-access
                                    NULL,   //  default security attributes
                                    OPEN_EXISTING, //  must use OPEN_EXISTING
                                    FILE_ATTRIBUTE_NORMAL,      //  not overlapped I/O
                                    NULL); //  hTemplate must be NULL for comm devices
  if (settings_file_handle == INVALID_HANDLE_VALUE) {
    if (ERROR_FILE_NOT_FOUND == GetLastError()) {
      screen_width = 1024;
      screen_height = 768;
      com_port_name =_T("COM3");
    }
  } else {
     DWORD settings_file_size = GetFileSize(settings_file_handle, NULL);
     read_file(settings_file_handle, settings_file_size);
  }
  CloseHandle(settings_file_handle);
}
