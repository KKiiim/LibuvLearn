#include <stdlib.h>
#include <uv.h>

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
  printf("alloc buffer\n");
  buf->base = static_cast<char *>(malloc(suggested_size));
  buf->len = suggested_size;
}

void echo_write(uv_udp_send_t *req, int status) {
  printf("free echo write\n");
  if (status) {
    fprintf(stderr, "Write error %s\n", uv_strerror(status));
  }
  free(req);
}

void echo_read(uv_udp_t *handle, ssize_t nread, const uv_buf_t *buf,
               const struct sockaddr *addr, unsigned flags) {
  printf("Server received %d bytes\n", (int)nread);
  if (nread > 0) {
    printf("%s\n", buf->base);
    uv_udp_send_t *req =
        static_cast<uv_udp_send_t *>(malloc(sizeof(uv_udp_send_t)));
    uv_buf_t send_buf = uv_buf_init(buf->base, nread);
    uv_udp_send(req, handle, &send_buf, 1, addr, echo_write);
  }

  if (buf->base) {
    free(buf->base);
  }
}

int main() {
  uv_udp_t server;
  uv_loop_t *loop = uv_default_loop();
  struct sockaddr_in bind_addr;

  uv_udp_init(loop, &server);

  uv_ip4_addr("0.0.0.0", 7000, &bind_addr);
  uv_udp_bind(&server, (const struct sockaddr *)&bind_addr, 0);

  int r = uv_udp_recv_start(&server, alloc_buffer, echo_read);
  if (r) {
    fprintf(stderr, "Failed to start recv %s\n", uv_strerror(r));
    return 1;
  }
  printf("Server listening on port %d\n", 7000);

  return uv_run(loop, UV_RUN_DEFAULT);
}