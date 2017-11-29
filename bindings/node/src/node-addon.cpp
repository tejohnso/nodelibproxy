#include "call-handler.h"
#include <node.h>

void Init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "checkProxyFor", processNodeJSCall);
}

NODE_MODULE(proxycheck, Init);
