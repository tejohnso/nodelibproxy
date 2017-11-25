#include <iostream>
#include <proxy.h>
#include <node.h>
#include <v8.h>
#include <uv.h>

using v8::Function;
using v8::Persistent;
using v8::FunctionCallbackInfo;
using v8::Exception;
using v8::Isolate;
using v8::Local;
using v8::Array;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Handle;

struct func_baton {
  uv_work_t shared_ptr;
  std::string targetUrl;
  char **proxies;
  Persistent<Function> callBack;
};

void proxyFunc(uv_work_t *shared_ptr) {
  func_baton *baton = static_cast<func_baton*>(shared_ptr->data);

  pxProxyFactory* proxyFactory = px_proxy_factory_new();
  baton->proxies = px_proxy_factory_get_proxies(proxyFactory, &baton->targetUrl[0u]);

  px_proxy_factory_free(proxyFactory);
}

void after(uv_work_t *shared_ptr, int status) {
  Isolate *isolate = Isolate::GetCurrent();
  v8::HandleScope handleScope(isolate);

  func_baton *baton = static_cast<func_baton*>(shared_ptr->data);
  std::string targetUrl = baton->targetUrl;

  char **proxies = baton->proxies;
  Local<Array> result_list = Array::New(isolate);
  for (unsigned int i = 0; proxies[i]; i++) {
    Local<String> proxy = String::NewFromUtf8(isolate, &proxies[i][0u]);
    result_list->Set(i, proxy);
    free(proxies[i]);
  }
  free(proxies);
  Handle<Value> argv[] = { Null(isolate) , result_list };
  Local<Function>::New(isolate, baton->callBack)->Call(isolate->GetCurrentContext()->Global(), 2, argv);
  baton->callBack.Reset();
  delete shared_ptr->data;
}

void Method(const FunctionCallbackInfo<Value>& args)
{
  Isolate* isolate = args.GetIsolate();

  if (args.Length() != 2) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  if (!args[0]->IsString() || !args[1]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(
      String::NewFromUtf8(isolate, "Wrong argument types")));
    return;
  }

  v8::String::Utf8Value v8String(args[0]);
  std::string str(*v8String, v8String.length());
  Local<Function> cb = Local<Function>::Cast(args[1]);

  func_baton *fb = new func_baton();
  fb->shared_ptr.data = fb;
  fb->targetUrl = str;
  fb->callBack.Reset(isolate, cb);

  uv_queue_work(uv_default_loop(), &fb->shared_ptr, proxyFunc, after);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "checkProxyFor", Method);
}

NODE_MODULE(node, Init)
