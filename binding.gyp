{
  "targets": [
    {
      "target_name": "nodelibproxy",
      "sources": [
        "bindings/node/src/callback.cpp",
        "bindings/node/src/async-proxy-fetch.cpp",
        "bindings/node/src/node-addon.cpp",
        "bindings/node/src/node-v8-interface.cpp",
        "bindings/node/src/call-handler.cpp",
      ],
      "link_settings": {
        "libraries": ["-lproxy"]
      },
      "include_dirs": [
        "./libproxy"
      ]
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [
            "<(PRODUCT_DIR)/<(module_name).node"
          ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}
