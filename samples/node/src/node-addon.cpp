#include "node-v8-interface.h"
#include <node.h>

void Init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "checkProxyFor", nvi::processNodeJSCall);
}

NODE_MODULE(proxycheck, Init);
