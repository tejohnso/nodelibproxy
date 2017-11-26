#include <proxy.h>
#include "node-v8-interface.h"

void asyncProxyFetch(uv_work_t* work_t) {
  pxProxyFactory* pf = px_proxy_factory_new();
  char* url = nvi::getTargetUrl(work_t);

  nvi::setProxies(work_t, px_proxy_factory_get_proxies(pf, url));
  px_proxy_factory_free(pf);
}
