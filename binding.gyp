{
  "targets": [
    {
      "target_name": "proxycheck",
      "sources": [
        "bindings/node/src/callback.cpp",
        "bindings/node/src/async-proxy-fetch.cpp",
        "bindings/node/src/node-addon.cpp",
        "bindings/node/src/node-v8-interface.cpp",
        "bindings/node/src/call-handler.cpp",
      ],
      "link_settings": {
        "libraries": ["-lproxy"]
      }
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
        {
          "files": [
            "<(PRODUCT_DIR)/<(module_name).node",
            "<(PRODUCT_DIR)/../../build-libproxy/install/lib/libproxy.so.1.0.0",
            "<(PRODUCT_DIR)/../../build-libproxy/install/lib/libproxy.so.1",
            "<(PRODUCT_DIR)/../../build-libproxy/install/lib/libproxy.so",
            "<(PRODUCT_DIR)/../../build-libproxy/install/modules/config_gnome3.so",
            "<(PRODUCT_DIR)/../../build-libproxy/install/modules/config_kde.so",
            "<(PRODUCT_DIR)/../../build-libproxy/install/libexec/pxgsettings"
          ],
          "destination": "<(module_path)"
        }
      ]
    }
  ]
}
