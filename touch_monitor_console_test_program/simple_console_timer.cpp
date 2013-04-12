#include <Windows.h>
#include "simple_console_timer.h"

unsigned long __stdcall ThreadProc(void* p_param);

void simple_timer(unsigned int time_to_elapse,
                  bool (*conditional_proc)(),
                  void (*callback_proc)()) {
  unsigned long tread_id = 0;
  HANDLE thread_handle = NULL;
  timer_params* params = new timer_params; // non-global for use of multiple timers
  params->time_to_elapse = time_to_elapse;
  params->conditional_proc = conditional_proc;
  params->callback_proc = callback_proc;
  thread_handle = CreateThread(NULL, 0, ThreadProc, (void*) params, 0, &tread_id);
}

unsigned long __stdcall ThreadProc(void* p_param) {
  MSG Msg;
  timer_params* pars = (timer_params*) p_param;
  unsigned int TimerId = SetTimer(NULL, 0, pars->time_to_elapse, NULL/*&TimerProc*/);
  while (GetMessage(&Msg, NULL, 0, 0)) {
    if (Msg.message == WM_TIMER) {
      if (Msg.wParam == TimerId) {
        if (pars->conditional_proc()) {
          pars->callback_proc();
          KillTimer(NULL, TimerId);
          delete pars;
          return 0;
        } else {
          KillTimer(NULL, TimerId);
          delete pars;
          return 0;
        }
      }
    }
//    DispatchMessage(&Msg); // calls TimerProc
  }
  delete pars;
  return 0;
}
