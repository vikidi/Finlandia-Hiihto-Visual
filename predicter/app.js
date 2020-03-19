const express = require('express');
const fs = require('fs');
const hbs = require('express-handlebars');
const path = require('path');

const app = express();

app.set('view engine', 'hbs');

app.engine(
    'hbs',
    hbs({
        extname: 'hbs',
        defaultLayout: 'default',
        layoutsDir: path.join(__dirname, '/views/layouts/'),
        partialsDir: path.join(__dirname, '/views/partials/')
    })
);

app.set('views', path.join(__dirname, '/views'));

const staticFileOptions = {
    dotfiles: 'ignore',
    etag: true,
    fallthrough: true,
    lastModified: true,
    maxAge: app.get('env') === 'development' ? '1m' : '2d'
};

const publicDir = path.resolve(`${__dirname}/`, 'public');
app.use(express.static(publicDir, staticFileOptions));

app.get('/', function(req, res) {
    res.render('home');
});

// Reroute to home page
app.all('*', function(req, res) {
    res.redirect('/');
});

module.exports = app;
