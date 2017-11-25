const proxyCheck = require("./build/Release/proxycheck");
const checkProxyFor = require("util").promisify(proxyCheck.checkProxyFor);

checkProxyFor("http://www.google.com")
.then(console.log);

checkProxyFor("http://localhost")
.then(console.log);

checkProxyFor()
.catch(err=>err.message.includes("number of arguments") ? "" : err);
