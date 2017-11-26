#include "node-v8-interface.h"

void callBack(uv_work_t* work_t, int status) {
  if (status == UV_ECANCELED) {
    return nvi::callbackWith(work_t, NULL);
  }

  nvi::callbackWith(work_t, nvi::getProxies(work_t));
}
