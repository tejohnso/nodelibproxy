#include <uv.h>
#include <v8.h>

#ifndef THREAD_BATON
#define THREAD_BATON

// http://docs.libuv.org/en/v1.x/guide/utilities.html#baton
struct thread_baton {
  uv_work_t work_t;
  std::string targetUrl;
  char **proxies;
  v8::Persistent<v8::Function> callBack;
};

#endif
