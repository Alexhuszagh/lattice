//  :copyright: (c) 2015-2017 The Regents of the University of California.
//  :license: MIT, see LICENSE.md for more details.
/**
 *  \addtogroup Example
 *  \brief Simple GET request with basic auth.
 */

var http = require('http'),
    httpProxy = require('http-proxy');

/** \brief Initialize proxy server.
 */
var proxy = httpProxy.createProxyServer();


/** \brief Create proxy server that forwards request with latency.
 */
http.createServer(function (req, res) {
  setTimeout(function () {
    proxy.web(req, res, {
      target: 'http://localhost:9008'
    });
  }, 250);
}).listen(8008);


/** \brief Create the target server.
 */
http.createServer(function (req, res) {
  res.write('Request successfully proxied to: ' + req.url + '\n' + JSON.stringify(req.headers, true, 2));
  res.end();
}).listen(9008);
