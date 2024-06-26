#include <stdio.h>
#include <uv.h>

int64_t counter = 0;

void wait_for_a_while(uv_idle_t* handle) {
  counter++;

  if (counter >= 10e7) uv_idle_stop(handle);
}

int main() {
  uv_idle_t idler;
  uv_idle_t idler1;
  uv_idle_t idler2;
  uv_idle_t idler3;

  uv_idle_init(uv_default_loop(), &idler);
  uv_idle_init(uv_default_loop(), &idler1);
  uv_idle_init(uv_default_loop(), &idler2);
  uv_idle_init(uv_default_loop(), &idler3);
  uv_idle_start(&idler, wait_for_a_while);
  uv_idle_start(&idler1, wait_for_a_while);
  uv_idle_start(&idler2, wait_for_a_while);
  uv_idle_start(&idler3, wait_for_a_while);

  printf("Idling...\n");
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  uv_loop_close(uv_default_loop());
  return 0;
}
