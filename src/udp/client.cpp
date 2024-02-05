#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <uv.h>

#include <cstdint>
#include <string>
#include <thread>

// void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
// {
//     printf("alloc buffer\n");
//   buf->base = malloc(suggested_size);
//   buf->len = suggested_size;
// }

void echo_write(uv_udp_send_t *req, int status) {
  if (status < 0) {
    fprintf(stderr, "Write error %s\n", uv_strerror(status));
  }
  printf("free echo write\n");
  free(req);
}

// void after_read(uv_udp_t *handle, ssize_t nread, const uv_buf_t *buf,
//                 const struct sockaddr *addr, unsigned flags) {
//   if (nread > 0) {
//     printf("Received: %.*s", (int)nread, buf->base);
//   }
//   printf("free after read\n");
//   free(buf->base);
// }

int main() {
  // std::thread client1([] {});
  // std::thread client2([] {});
  // client1.join();
  // client2.join();
  // uv_async_t asyncEvent;

  uv_udp_t client;
  uv_loop_t *loop = uv_default_loop();
  struct sockaddr_in server_addr;
  uv_ip4_addr("127.0.0.1", 7000, &server_addr);

  uv_udp_init(loop, &client);
  std::string message = "Hello Server";
  for (uint16_t count = 0U; count < 100; count++) {
    uv_buf_t write_buf = uv_buf_init(message.data(), sizeof(message));

    uv_udp_send_t *req =
        static_cast<uv_udp_send_t *>(malloc(sizeof(uv_udp_send_t)));
    uv_udp_send(req, &client, &write_buf, 1,
                (const struct sockaddr *)&server_addr, echo_write);
    // printf("req addr 0x%x\n", req);
    printf("Client sending message\n");
    sleep(1);  // simulate a big message send and then send_cb
    int r = uv_run(loop, UV_RUN_DEFAULT);
    if (r) {
      fprintf(stderr, "uv_run failed: %s\n", uv_strerror(r));
    }
  }

  return uv_run(loop, UV_RUN_DEFAULT);
}