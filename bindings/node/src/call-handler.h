#ifndef PROXY_CALL_HANDLER
#define PROXY_CALL_HANDLER

#include "node-v8-interface.h"

bool validateArgs(const v8::FunctionCallbackInfo<v8::Value>& args);

thread_baton* initiateThreadBaton(const v8::FunctionCallbackInfo<v8::Value>& args);

void queueWork(thread_baton* baton, uv_work_cb workFn, uv_after_work_cb cb);

void processNodeJSCall(const v8::FunctionCallbackInfo<v8::Value>& args);

#endif
