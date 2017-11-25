{
  "targets": [
    {
      "target_name": "proxycheck",
      "sources": [ "callback.cpp", "async-proxy-fetch.cpp", "node-v8-interface.cpp"],
      "link_settings": {
        "libraries": ["-L/home/tyler/development/libproxy/samples/node", "-lproxy"]
      }
    }
  ]
}
