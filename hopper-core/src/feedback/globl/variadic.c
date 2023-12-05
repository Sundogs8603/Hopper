// Some hooks that can't implemented in Rust
#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>

extern void __hopper_open_hook(u_int32_t, const char*, int);

// int open(const char *pathname, int flags);
// int open(const char *pathname, int flags, mode_t mode);
int __hopper_open(const char* pathname, int flags, ...) {
  va_list ap;
  va_start(ap, flags);
  // mot_t mode = va_arg (args, int);
  int fd = open(pathname, flags, ap);
  va_end(ap);
  u_int32_t id = (uintptr_t)__builtin_return_address(0) % 0xFFFFFFFF;
  __hopper_open_hook(id, pathname, flags);
  return fd;
}

int __hopper_open64(const char* pathname, int flags, ...) {
  va_list ap;
  va_start(ap, flags);
  // mot_t mode = va_arg (args, int);
  int fd = open64(pathname, flags, ap);
  va_end(ap);
  u_int32_t id = (uintptr_t)__builtin_return_address(0) % 0xFFFFFFFFF;
  __hopper_open_hook(id, pathname, flags);
  return fd;
}

void* __hopper_open_fn = __hopper_open;
void* __hopper_open64_fn = __hopper_open64;