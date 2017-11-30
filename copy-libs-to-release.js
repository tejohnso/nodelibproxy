var gyp = require("node-pre-gyp");
var glob = require("glob");
var path = require("path");
var dirname = path.dirname;
var basename = path.basename;
var join = path.join;
var resolve = path.resolve;
var targetDir = dirname(gyp.find(resolve(join(__dirname,'./package.json'))));
var copyFile = require("fs").copyFile;

glob("./build-libproxy/install/**/*.so*", function(err, strArr) {
  for (var i = 0; i < strArr.length; i += 1) {
    var source = strArr[i];
    var targetFile = join(targetDir, basename(strArr[i]));
    console.log("Copying " + source + " to " + targetFile);

    copyFile(source, targetFile, function(err) {
      if (err && err.message.indexOf("already exists") === -1) {
        console.error(err);
      }
    });
  }
});
