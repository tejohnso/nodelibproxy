#include "call-handler.h"
#include "node-v8-interface.h"
#include "async-proxy-fetch.h"
#include "callback.h"

void processNodeJSCall(const v8::FunctionCallbackInfo<v8::Value>& args) {
  if (!validateArgs(args)) return;

  thread_baton* baton = initiateThreadBaton(args);
  queueWork(baton, asyncProxyFetch, callBack);
}

bool validateArgs(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  if (args.Length() != 2) {
    isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return false;
  }

  if (!args[0]->IsString() || !args[1]->IsFunction()) {
    isolate->ThrowException(v8::Exception::TypeError(
          v8::String::NewFromUtf8(isolate, "Wrong argument types")));
    return false;
  }

  return true;
}

thread_baton* initiateThreadBaton(const v8::FunctionCallbackInfo<v8::Value>& args) {
  thread_baton *baton = new thread_baton();

  baton->work_t.data = baton;

  v8::String::Utf8Value urlArg(args[0]);
  std::string str(*urlArg, urlArg.length());
  baton->targetUrl = str;

  v8::Isolate* isolate = args.GetIsolate();
  v8::Local<v8::Function> cb = v8::Local<v8::Function>::Cast(args[1]);
  baton->callBack.Reset(isolate, cb);

  return baton;
}

void queueWork(thread_baton* baton, uv_work_cb workFn, uv_after_work_cb cb) {
  uv_queue_work(uv_default_loop(), &baton->work_t, workFn, cb);
}
