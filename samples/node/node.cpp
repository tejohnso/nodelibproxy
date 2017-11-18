#include <iostream>
#include <proxy.h>

int main()
{
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

  return 0;
}
