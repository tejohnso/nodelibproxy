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

void Method(const FunctionCallbackInfo<Value>& args)
{
  Isolate* isolate = args.GetIsolate();

  const char *url = "http://www.wired.com";
  std::cout << "Checking proxy url for " << url << std::endl;

  pxProxyFactory* proxyFactory = px_proxy_factory_new();
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
}

void Init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "node", Method);
}

NODE_MODULE(node, Init)
