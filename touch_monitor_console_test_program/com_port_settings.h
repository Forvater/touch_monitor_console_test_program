#ifndef COM_PORT_SETTINGS_H
#define COM_PORT_SETTINGS_H

#include <windows.h>
#include <atlstr.h>

extern const unsigned long event_mask_to_set;
extern const unsigned long kInputBufferSize;
extern const unsigned long kOutputBufferSize;
extern const CString com_port_name;
extern const long kRecvBufferSize;
extern HANDLE com_port_handle;
extern COMSTAT com_status;
extern unsigned long event_mask;
extern unsigned long errors;
extern unsigned char* recv_buffer;
extern unsigned long number_of_bytes_read;
extern bool stop_reading_thread;

extern DCB dcb;

void configure_com_port();
void open_com_port();
void start_reading_thread();

#endif