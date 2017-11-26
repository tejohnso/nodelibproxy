#ifndef NODE_V8_INTERFACE
#define NODE_V8_INTERFACE

#include <node.h>
#include <v8.h>
#include <uv.h>
#include "thread-baton.h"

namespace nvi {
  void setProxies(uv_work_t*, char**);

  char** getProxies(uv_work_t*);

  char* getTargetUrl(uv_work_t*);

  void callbackWith(uv_work_t*, char**);

  void processNodeJSCall(const v8::FunctionCallbackInfo<v8::Value>&);
}

#endif
