const express = require('express');
const path = require('path');
const app = new express();

var options = {
    root: path.join(__dirname, 'public'),
    dotfiles: 'deny',
    headers: {
      'x-timestamp': Date.now(),
      'x-sent': true
    }
};

const staticFileOptions = {
    dotfiles: 'ignore',
    etag: true,
    fallthrough: true,
    lastModified: true,
    maxAge: app.get('env') === 'development' ? '1m' : '2d'
};

const publicDir = path.resolve(`${__dirname}/`, 'public/html');
app.use(express.static(publicDir, staticFileOptions));

app.get('/', function(request, response){
    response.sendFile('./html/index.html', options);
});

module.exports = app;