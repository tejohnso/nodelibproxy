#include <proxy.h>
#include "thread-baton.h"

void asyncProxyFetch(uv_work_t *work_t) {
  thread_baton *baton = static_cast<thread_baton*>(work_t->data);

  pxProxyFactory* proxyFactory = px_proxy_factory_new();
  baton->proxies = px_proxy_factory_get_proxies(proxyFactory, &baton->targetUrl[0u]);

  px_proxy_factory_free(proxyFactory);
}
