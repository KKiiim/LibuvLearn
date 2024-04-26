# LibuvLearn
  
## Memory management of event  
uv_close after uv_timer_stop  
uv_close after uv_udp_recv_stop
```C
void uv_close(uv_handle_t *handle, uv_close_cb close_cb)
```
**uv_close is not a Synchronized method.**  

Once calling close_cb, the event corresponds to the handle is close.  
It is only at this time that the event management object can be deleted.  
  
## Watch out for errors  
Neglecting to assert some of the return values can be extremely confusing for subsequent debugging.  
Especially the following:  
### Udp bind  
```C
int uv_udp_bind(uv_udp_t *handle, const struct sockaddr *addr, unsigned int flags)
```
This function will not throw exception or terminate when occur error that this socket has been occupied.  
Hidden errors may cause confusion when debug.  
So, the return code of uv_udp_bind is a type of [[nodiscard]].  
    int32_t error = uv_udp_bind(...);
    if (error != 0) ...Log << uv_strerror(error) << uv_err_name(error);

### Send callback  
```C
int uv_udp_send(uv_udp_send_t *req, uv_udp_t *handle, const uv_buf_t bufs[],  
                unsigned int nbufs, const struct sockaddr *addr, uv_udp_send_cb send_cb);  
uv_udp_send_cb send_cb (uv_udp_send_t *req, int status)
```
in send_cb, status should also be taken seriously.  
Assert it Like uv_strerror(status) | uv_err_name(status)  