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

app.get('/data', function(req, res) {
    var distance = req.query.distance;

    const { readdirSync } = require('fs');

    const getDirectories = source =>
    readdirSync(source, { withFileTypes: true })
        .filter(dirent => dirent.isDirectory())
        .map(dirent => dirent.name);

    const directories = getDirectories('public/data/FinlandiaData/');

    // Sort for smallest to be first so that first object gets right weather
    directories.sort();

    var json = [];
    for(var i = 0; i < directories.length; i++) {
        var dir = directories[i];
        var data = fs.readFileSync('public/data/FinlandiaData/' + dir + '/' + distance + '/Data.txt', 'utf8');

        var rows = data.split('\r\n');
        
        for(var j = 0; j < Math.min(rows.length, 100); j++) {
            var element = rows[j];

            if (!element || element === '') {
                continue;
            }

            var columns = element.split(';');

            var rowData = {
                place: columns[3],
                name: columns[7]
            };

            if (columns[0] === '2019' || i === 10) {
                rowData.rain = 0.0;
                rowData.snow = 44;
                rowData.temp = -0.4;
            } else if (columns[0] === '2018' || i === 9) {
                rowData.rain = 0.0;
                rowData.snow = 35;
                rowData.temp = -13.6;
            } else if (columns[0] === '2017' || i === 8) {
                rowData.rain = 1.9;
                rowData.snow = 12;
                rowData.temp = -0.6;
            } else if (columns[0] === '2016' || i === 7) {
                rowData.rain = 0.4;
                rowData.snow = 28;
                rowData.temp = -7.4;
            } else if (columns[0] === '2015' || i === 6) {
                rowData.rain = 2.7;
                rowData.snow = 26;
                rowData.temp = 1.2;
            } else if (columns[0] === '2014' || i === 5) {
                rowData.rain = 1.4;
                rowData.snow = 10;
                rowData.temp = 1.1;
            } else if (columns[0] === '2013' || i === 4) {
                rowData.rain = 0.0;
                rowData.snow = 55;
                rowData.temp = -2.2;
            } else if (columns[0] === '2012' || i === 3) {
                rowData.rain = 1.9;
                rowData.snow = 52;
                rowData.temp = -4.1;
            } else if (columns[0] === '2011' || i === 2) {
                rowData.rain = 0.0;
                rowData.snow = 63;
                rowData.temp = -5.2;
            } else if (columns[0] === '2010' || i === 1) {
                rowData.rain = 1.5;
                rowData.snow = 65;
                rowData.temp = -0.6;
            } else if (columns[0] === '2009' || i === 0) {
                rowData.rain = 0.0;
                rowData.snow = 21;
                rowData.temp = -5.7;
            }

            json.push(rowData);
        }
    }

    res.send(JSON.stringify(json));
});

app.get('/', function(req, res) {
    res.render('home');
});

// Reroute to home page
app.all('*', function(req, res) {
    res.redirect('/');
});

module.exports = app;
