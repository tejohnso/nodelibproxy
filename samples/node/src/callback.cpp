#include <uv.h>
#include <v8.h>
#include "thread-baton.h"

void callback(uv_work_t *work_t, int status) {
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  v8::HandleScope handleScope(isolate);

  thread_baton *baton = static_cast<thread_baton*>(work_t->data);
  std::string targetUrl = baton->targetUrl;

  char **proxies = baton->proxies;
  v8::Local<v8::Array> result_list = v8::Array::New(isolate);
  for (unsigned int i = 0; proxies[i]; i++) {
    v8::Local<v8::String> proxy = v8::String::NewFromUtf8(isolate, &proxies[i][0u]);
    result_list->Set(i, proxy);
    free(proxies[i]);
  }
  free(proxies);
  v8::Handle<v8::Value> argv[] = { Null(isolate) , result_list };
  v8::Local<v8::Function>::New(isolate, baton->callBack)->Call(isolate->GetCurrentContext()->Global(), 2, argv);
  baton->callBack.Reset();
}
