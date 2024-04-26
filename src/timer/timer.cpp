#include <uv.h>

#include <iostream>

void timerCB(uv_timer_t *handle) {
  auto value = (long long)handle->data;
  std::cout << value << std::endl;
  if (value >= 126) {
    uv_timer_stop(handle);
    return;
  }
  handle->data = (void *)++value;
}

int main() {
  uv_loop_t *loop = uv_default_loop();

  uv_timer_t timer;
  uv_timer_init(loop, &timer);

  timer.data = (void *)123;
  uv_timer_start(&timer, timerCB, 1000, 4000);

  uv_run(loop, UV_RUN_DEFAULT);

  return 0;
}