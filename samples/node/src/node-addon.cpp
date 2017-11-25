#include <node.h>
#include <v8.h>
#include <uv.h>
#include "thread-baton.h"
#include "async-proxy-fetch.h"
#include "callback.h"

void ExportMethod(const v8::FunctionCallbackInfo<v8::Value>& args) {
  v8::Isolate* isolate = args.GetIsolate();

  if (args.Length() != 2) {
    isolate->ThrowException(v8::Exception::TypeError(
      v8::String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  if (!args[0]->IsString() || !args[1]->IsFunction()) {
    isolate->ThrowException(v8::Exception::TypeError(
      v8::String::NewFromUtf8(isolate, "Wrong argument types")));
    return;
  }

  v8::String::Utf8Value urlArg(args[0]);
  std::string str(*urlArg, urlArg.length());
  v8::Local<v8::Function> cb = v8::Local<v8::Function>::Cast(args[1]);

  thread_baton *baton = new thread_baton();
  baton->work_t.data = baton;
  baton->targetUrl = str;
  baton->callBack.Reset(isolate, cb);

  uv_queue_work(uv_default_loop(), &baton->work_t, asyncProxyFetch, callback);
}

void Init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "checkProxyFor", ExportMethod);
}

NODE_MODULE(node, Init)
