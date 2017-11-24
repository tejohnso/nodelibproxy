const proxyCheck = require("./build/Release/proxycheck");
proxyCheck.checkProxyFor("http://www.google.com", (err, proxy)=>{
  if (err) {return console.log(err);}
  console.log(proxy);
});
