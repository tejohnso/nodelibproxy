#include <node.h>
#include <v8.h>
#include <uv.h>
#include "thread-baton.h"
#include "node-v8-interface.h"
#include "async-proxy-fetch.h"
#include "callback.h"

namespace nvi {
  void setProxies(uv_work_t* work_t, char** proxies) {
    thread_baton *baton = static_cast<thread_baton*>(work_t->data);
    baton->proxies = proxies;
  }

  char** getProxies(uv_work_t* work_t) {
    thread_baton *baton = static_cast<thread_baton*>(work_t->data);
    return baton->proxies;
  }

  char* getTargetUrl(uv_work_t* work_t) {
    thread_baton *baton = static_cast<thread_baton*>(work_t->data);

    return &baton->targetUrl[0u];
  }

  void callbackWith(uv_work_t* work_t, char** proxies) {
    const unsigned int argc = 2;
    thread_baton *baton = static_cast<thread_baton*>(work_t->data);
    v8::Isolate *isolate = v8::Isolate::GetCurrent();
    v8::HandleScope handleScope(isolate);

    v8::Local<v8::Array> nodeArray = v8::Array::New(isolate);
    for (unsigned int i = 0; proxies[i]; i++) {
      v8::Local<v8::String> proxy = v8::String::NewFromUtf8(isolate, &proxies[i][0u]);
      nodeArray->Set(i, proxy);
      free(proxies[i]);
    }
    free(proxies);

    v8::Handle<v8::Value> argv[] = {Null(isolate) , nodeArray};

    auto cb = v8::Local<v8::Function>::New(isolate, baton->callBack);
    cb->Call(isolate->GetCurrentContext()->Global(), argc, argv);
    baton->callBack.Reset();
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

  void processNodeJSCall(const v8::FunctionCallbackInfo<v8::Value>& args) {
    if (!validateArgs(args)) return;

    thread_baton* baton = initiateThreadBaton(args);
    queueWork(baton, asyncProxyFetch, callBack);
  }
}
