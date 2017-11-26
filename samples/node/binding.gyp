{
  "targets": [
    {
      "target_name": "proxycheck",
      "sources": [
        "src/callback.cpp",
        "src/async-proxy-fetch.cpp",
        "src/node-addon.cpp",
        "src/node-v8-interface.cpp",
      ],
      "link_settings": {
        "libraries": ["-L/home/tyler/development/libproxy/samples/node", "-lproxy"]
      }
    }
  ]
}
