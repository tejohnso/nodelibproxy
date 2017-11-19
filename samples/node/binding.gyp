{
  "targets": [
    {
      "target_name": "node",
      "sources": [ "node.cpp" ],
      "link_settings": {
        "ldflags": ["-L/usr/lib", "-lproxy"]
      }
    }
  ]
}
