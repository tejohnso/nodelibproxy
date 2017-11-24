#include <iostream>
#include <proxy.h>
#include <node.h>
#include <v8.h>
#include <uv.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void proxyFunc(uv_work_t *shared_ptr) {
  pxProxyFactory* proxyFactory = px_proxy_factory_new();
  char* url = (char*) shared_ptr->data;

  std::cout << "passed in " << url << std::endl;

  char** proxies = px_proxy_factory_get_proxies(proxyFactory, url);

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
  char* url = (char*) shared_ptr->data;
  std::cout << "after..." << url << std::endl;
  free(shared_ptr);
}

void Method(const FunctionCallbackInfo<Value>& args)
{
  Isolate* isolate = args.GetIsolate();

  char *url = "http://www.wired.com";

  std::cout << "Checking proxy url for " << url << std::endl;

  uv_work_t *shared_ptr = new uv_work_t();
  shared_ptr->data = (void*) url;
  uv_queue_work(uv_default_loop(), shared_ptr, proxyFunc, after);
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "checkProxyFor", Method);
}

NODE_MODULE(node, Init)
