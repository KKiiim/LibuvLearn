#include <stdio.h>
#include <uv.h>

class EventLoop {
  EventLoop();

  uv_loop_t* loop;
  struct sockaddr_in addr;
  uv_tcp_t server;
};