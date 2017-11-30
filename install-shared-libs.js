var copyFile = require("fs").copyFile;
var readdir = require("fs").readdir;
var noOverwrite = require("fs").constants.COPYFILE_EXCL;
var binary = require('node-pre-gyp');
var path = require('path');
var join = require("path").join;
var binding_path = binary.find(path.resolve(join(__dirname,'./package.json')));
var binding_dir = path.dirname(binding_path);

if (!require("os").platform() === "linux") {return;}

console.log("copying shared libraries");
readdir(binding_dir, function(err, files) {
  if (err) {return console.error(err)}

  for(var i = 0;i < files.length; i += 1) {
    var source = join(binding_dir, files[i]);
    var target = join("/usr/lib", files[i]);

    copyFile(source, target, noOverwrite, function(err) {
      if (err && err.message.indexOf("already exists") === -1) {
        console.error(err);
      }
    });
  }
});
