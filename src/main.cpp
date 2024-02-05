#include <stdio.h>
#include <stdlib.h>
#include <uv.h>

#define DEFAULT_PORT 12345

void on_read(uv_udp_t *req, ssize_t nread, const uv_buf_t *buf,
             const struct sockaddr *addr, unsigned flags) {
  if (nread < 0) {
    fprintf(stderr, "Read error %s\n", uv_err_name(nread));
    uv_close((uv_handle_t *)req, NULL);
    free(buf->base);
    return;
  }

  char sender[17] = {0};
  uv_ip4_name((const struct sockaddr_in *)addr, sender, 16);
  fprintf(stderr, "Received data from %s\n", sender);

  // echo back the data
  uv_udp_send_t *res = (uv_udp_send_t *)malloc(sizeof(uv_udp_send_t));
  uv_udp_send(res, req, buf, 1, addr, NULL);

  // free the read buffer
  free(buf->base);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
  buf->base = (char *)malloc(suggested_size);
  buf->len = suggested_size;
}

int main() {
  uv_loop_t *loop = uv_default_loop();

  uv_udp_t recv_socket;
  uv_udp_init(loop, &recv_socket);

  struct sockaddr_in recv_addr;
  uv_ip4_addr("0.0.0.0", DEFAULT_PORT, &recv_addr);

  uv_udp_bind(&recv_socket, (const struct sockaddr *)&recv_addr,
              UV_UDP_REUSEADDR);
  uv_udp_recv_start(&recv_socket, alloc_buffer, on_read);

  printf("Listening on UDP port %d\n", DEFAULT_PORT);
  return uv_run(loop, UV_RUN_DEFAULT);
}
