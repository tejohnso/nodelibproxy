#include <node.h>
#include <v8.h>
#include <uv.h>
#include "thread-baton.h"
#include "node-v8-interface.h"

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
}
