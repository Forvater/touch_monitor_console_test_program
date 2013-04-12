#include <Windows.h>

void mouse_lmb_click(long x, long y) {
  MOUSEINPUT mouse_input_lmb_pressed;
  INPUT m_input_lmb_pressed;

  MOUSEINPUT mouse_input_lmb_released;
  INPUT m_input_lmb_released;

  mouse_input_lmb_pressed.dx = x;
  mouse_input_lmb_pressed.dy = y;
  mouse_input_lmb_pressed.mouseData = 0;
  mouse_input_lmb_pressed.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
  mouse_input_lmb_pressed.time = 0;
  m_input_lmb_pressed.type = INPUT_MOUSE;
  m_input_lmb_pressed.mi = mouse_input_lmb_pressed;

  mouse_input_lmb_released.dx = x;
  mouse_input_lmb_released.dy = y;
  mouse_input_lmb_released.mouseData = 0;
  mouse_input_lmb_released.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
  mouse_input_lmb_released.time = 0;
  m_input_lmb_released.type = INPUT_MOUSE;
  m_input_lmb_released.mi = mouse_input_lmb_released;

  SendInput(1, &m_input_lmb_pressed, sizeof(m_input_lmb_pressed));
  SendInput(1, &m_input_lmb_released, sizeof(m_input_lmb_released));
}

void mouse_rmb_click(long x, long y) {
  MOUSEINPUT mouse_input_rmb_pressed;
  INPUT m_input_rmb_pressed;

  MOUSEINPUT mouse_input_rmb_released;
  INPUT m_input_rmb_released;

  mouse_input_rmb_pressed.dx = x;
  mouse_input_rmb_pressed.dy = y;
  mouse_input_rmb_pressed.mouseData = 0;
  mouse_input_rmb_pressed.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTDOWN;
  mouse_input_rmb_pressed.time = 0;
  m_input_rmb_pressed.type = INPUT_MOUSE;
  m_input_rmb_pressed.mi = mouse_input_rmb_pressed;

  mouse_input_rmb_released.dx = x;
  mouse_input_rmb_released.dy = y;
  mouse_input_rmb_released.mouseData = 0;
  mouse_input_rmb_released.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_RIGHTUP;
  mouse_input_rmb_released.time = 0;
  m_input_rmb_released.type = INPUT_MOUSE;
  m_input_rmb_released.mi = mouse_input_rmb_released;

  SendInput(1, &m_input_rmb_pressed, sizeof(m_input_rmb_pressed));
  SendInput(1, &m_input_rmb_released, sizeof(m_input_rmb_released));
}

void mouse_lmb_down(long x, long y) {
  MOUSEINPUT mouse_input_lmb_pressed;
  INPUT m_input_lmb_pressed;

  MOUSEINPUT mouse_input_lmb_released;
  INPUT m_input_lmb_released;

  mouse_input_lmb_pressed.dx = x;
  mouse_input_lmb_pressed.dy = y;
  mouse_input_lmb_pressed.mouseData = 0;
  mouse_input_lmb_pressed.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
  mouse_input_lmb_pressed.time = 0;
  m_input_lmb_pressed.type = INPUT_MOUSE;
  m_input_lmb_pressed.mi = mouse_input_lmb_pressed;

  SendInput(1, &m_input_lmb_pressed, sizeof(m_input_lmb_pressed));
}

void mouse_lmb_up(long x, long y) {
  MOUSEINPUT mouse_input_lmb_released;
  INPUT m_input_lmb_released;

  mouse_input_lmb_released.dx = x;
  mouse_input_lmb_released.dy = y;
  mouse_input_lmb_released.mouseData = 0;
  mouse_input_lmb_released.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;
  mouse_input_lmb_released.time = 0;
  m_input_lmb_released.type = INPUT_MOUSE;
  m_input_lmb_released.mi = mouse_input_lmb_released;

  SendInput(1, &m_input_lmb_released, sizeof(m_input_lmb_released));
}
