#include <iostream>
#include <proxy.h>
#include <node.h>
#include <v8.h>
#include <uv.h>

using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Exception;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

struct func_baton {
  uv_work_t shared_ptr;
  std::string targetUrl;
  Local<Function> callBack;
};

void proxyFunc(uv_work_t *shared_ptr) {
  pxProxyFactory* proxyFactory = px_proxy_factory_new();

  func_baton *baton = (func_baton*) shared_ptr->data;
  std::string targetUrl = baton->targetUrl;
  std::cout << "passed in " << targetUrl << std::endl;

  char** proxies = px_proxy_factory_get_proxies(proxyFactory, &targetUrl[0u]);

  int i = 0;
  char* proxy = proxies[i];
  if (!proxy) {
    std::cout << "No proxies" << std::endl;
  }

  while (proxy) {
    std::cout << proxy << std::endl;
    i++;
    proxy = proxies[i];
  }

  std::cout << "freeing proxies" << std::endl;
  for (i = 0; proxies[i]; i++) {
    free(proxies[i]);
  }
  std::cout << "freeing proxy" << std::endl;
  free(proxies);
  std::cout << "freeing factory" << std::endl;
  px_proxy_factory_free(proxyFactory);
  std::cout << "proxyFunc complete" << std::endl;
}

void after(uv_work_t *shared_ptr, int status) {
  std::cout << "need to call callback" << std::endl;
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

  char *url = "http://www.wired.com";

  std::cout << "Checking proxy url for " << url << std::endl;

  v8::String::Utf8Value v8String(args[0]);
  std::string str(*v8String, v8String.length());
  Local<Function> cb = Local<Function>::Cast(args[1]);

  func_baton *fb = new func_baton();
  fb->shared_ptr.data = (void*) fb;
  fb->targetUrl = str;
  fb->callBack = cb;

  uv_queue_work(uv_default_loop(), &fb->shared_ptr, proxyFunc, after);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "checkProxyFor", Method);
}

NODE_MODULE(node, Init)
