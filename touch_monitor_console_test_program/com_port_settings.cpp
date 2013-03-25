#include <windows.h>
#include <atlstr.h>
#include <iostream>

const unsigned char kPacketSize = 42;

const unsigned char kBufferSize = (kPacketSize * 2) - 1;

const unsigned long event_mask_to_set = EV_RXCHAR;
const unsigned long kInputBufferSize = 2000;
const unsigned long kOutputBufferSize = 2000;
const CString com_port_name = _T("COM3");
const long kRecvBufferSize = 2000;
HANDLE com_port_handle;
COMSTAT com_status;
unsigned long event_mask = 0;
unsigned long errors = 0;
unsigned char recv_buffer[kRecvBufferSize];
unsigned long number_of_bytes_read = 0;

unsigned long recv_buf_pos = 0;

HANDLE read_tread_handle_ = NULL;

bool stop_reading_thread = false;

DCB dcb;

unsigned char transistor_state_buffer[80];

unsigned long __stdcall ReadFromComPortInThread(void* p_param);
void find_packet_in_buffer(unsigned char* buff, unsigned long size);
void process_packet(unsigned char* buff, unsigned char size);

void configure_com_port() {
  //  Initialize the DCB structure.
  SecureZeroMemory(&dcb, sizeof(DCB));
  dcb.DCBlength = sizeof(DCB);
  dcb.BaudRate = CBR_38400;     //  baud rate
  dcb.ByteSize = 8;             //  data size, xmit and rcv
  dcb.Parity   = NOPARITY;      //  parity bit
  dcb.StopBits = ONESTOPBIT;    //  stop bit

  SetCommState(com_port_handle, &dcb);
  // Set mask on EV_RXCHAR event
  SetCommMask(com_port_handle, event_mask_to_set);
  // Set size of the receive and transmission queue
  SetupComm(com_port_handle,kInputBufferSize,kOutputBufferSize);
  // Purge the receive buffer
  PurgeComm(com_port_handle, PURGE_RXCLEAR);
}

void open_com_port() {
  com_port_handle = CreateFile(com_port_name,
                               GENERIC_READ | GENERIC_WRITE,
                               0,      //  must be opened with exclusive-access
                               NULL,   //  default security attributes
                               OPEN_EXISTING, //  must use OPEN_EXISTING
                               FILE_ATTRIBUTE_NORMAL,      //  not overlapped I/O
                               NULL); //  hTemplate must be NULL for comm devices
}

void start_reading_thread() {
  read_tread_handle_ = CreateThread(NULL, 0, ReadFromComPortInThread, NULL, 0,
                                    NULL);
}

void receive_bytes_and_put_them_in_buffer() {
  ReadFile(com_port_handle, &(recv_buffer[recv_buf_pos]), com_status.cbInQue, &number_of_bytes_read, NULL);
  recv_buf_pos += com_status.cbInQue;
  if(recv_buf_pos >= kBufferSize) {
    find_packet_in_buffer(recv_buffer, recv_buf_pos);
    SecureZeroMemory(recv_buffer, kRecvBufferSize);
    recv_buf_pos = 0;
  }
}

unsigned long __stdcall ReadFromComPortInThread(void* p_param) {
  SecureZeroMemory(recv_buffer, kRecvBufferSize);
  SecureZeroMemory(transistor_state_buffer, 80);
  recv_buf_pos = 0;
  while (!stop_reading_thread) {
    WaitCommEvent(com_port_handle, &event_mask, NULL);
    if (event_mask == EV_RXCHAR) {
      ClearCommError(com_port_handle, &errors, &com_status);
        if (errors == 0) {
          if (com_status.cbInQue) {
            receive_bytes_and_put_them_in_buffer();
          } else {
            PurgeComm(com_port_handle, PURGE_RXCLEAR);
          }
        } else {
          PurgeComm(com_port_handle, PURGE_RXCLEAR);
        }
    } else {
      PurgeComm(com_port_handle, PURGE_RXCLEAR);
    }
  }
  return 0;
}

void find_packet_in_buffer(unsigned char* buff, unsigned long size) {
  for (int i = 0; i < size; i++) {
    if ((buff[i] == 0x55) && (buff[i+1] == 0x54)) {
      if (i+42 <= size) {
        process_packet(&(buff[i+2]),40);
      }
    }
  }
}

void proccess_byte(unsigned char byte_, int number) {
  for (int i = 0; i < 8; i++) {
    if ((byte_ >> i) & 1) {
      transistor_state_buffer[(number * 8) + i] = 1;
    } else {
      transistor_state_buffer[(number * 8) + i] = 0;
    }
  }
}

void print_transistors_state() {
  for (int i = 0; i < 80; i++) {
    printf("%d",transistor_state_buffer[i]);
  }
  printf("\r\n");
}

void proccess_line(unsigned char* buff, unsigned char size) {
  for(int i = 0; i < size; i++) {
    proccess_byte(buff[i], i);
  }
  print_transistors_state();
  SecureZeroMemory(transistor_state_buffer, 80);
}

void process_packet(unsigned char* buff, unsigned char size) {
  printf("\r\n");// Needed to show transistor lines properly
  printf("\r\n");//
  printf("\r\n");//
  printf("\r\n");//
  printf("\r\n");//
  printf("\r\n");//
  printf("\r\n");//
  printf("\r\n");//
  for(int i = 0; i < 4; i++) {
    proccess_line(&(buff[i*10]), 10);
  }
  printf("\r\n");// Needed to show transistor lines properly
  printf("\r\n");//
  printf("\r\n");//
  printf("\r\n");//
  printf("\r\n");//
  printf("\r\n");//
  printf("\r\n");//
  printf("\r\n");//
}
