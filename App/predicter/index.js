const http = require('http');
const app = require('./app');


const serverListenPort = normalizePort(process.env.PORT || '3000');
app.set('port', serverListenPort);
const server = http.createServer(app);
server.listen(serverListenPort, () => {
    console.log("App running in port 3000");
});

function normalizePort(val) {
    const port = parseInt(val, 10);

    if (isNaN(port)) {
        // named pipe
        return val;
    }

    if (port >= 0) {
        // port number
        return port;
    }

    return false;
}