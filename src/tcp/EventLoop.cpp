#include "EventLoop.hpp"

#include <uv.h>

#include <stdexcept>

#define PORT 1234
#define DEFAULT_BACKLOG 511

void connectionCallBack(uv_stream_t* server, int status) {
  // callback for new tcp connection
  // do nothing here
}

EventLoop::EventLoop() {
  loop = uv_default_loop();

  uv_ip4_addr("0.0.0.0", PORT, &addr);  // init addr
  uv_tcp_init(loop, &server);           // init server
  auto rt_bind = uv_tcp_bind(&server, (const struct sockaddr*)&addr,
                             0);  // bind server and addr
  if (rt_bind != 0) {
    throw std::runtime_error("uv_tcp_bind failed");
  }
  uv_listen((uv_stream_t*)&server, DEFAULT_BACKLOG,
            connectionCallBack);  // start server
}