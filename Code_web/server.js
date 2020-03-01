console.log('Server-side code running');

//---------------------------------------------------------------- Globals Servidor web
const express = require('express');
const app = express();
const port = 8080;

// tmp
var router = express.Router();
let estat_botonet = 0;

//---------------------------------------------------------------- Globals Port Serial
var portName = "/dev/ttyUSB0";//process.argv[2];
var comando = "on\n";//process.argv[3];
var SerialPort = require("serialport");



//----------------------------------------------------------------<Server>
// serve files from the public directory
app.use(express.static('public'));

// start the express web server listening on 8080
app.listen(port, () => {
  console.log('listening on 8080');
});

// serve the homepage
app.get('/', (req, res) => {
  res.sendFile(__dirname + '/index.html');
});
//----------------------------------------------------------------</Server>



//----------------------------------------------------------------<USB serial>
var serialport = new SerialPort(portName, {
  baudRate: 9600,
  // Look for return and newline at the end of each data packet
  parser: SerialPort.parsers.readline("\n")
});
//----------------------------------------------------------------</USB serial>



//----------------------------------------------------------------<Temporal>
app.get("/temp", function (request, response) {

  var id = request.query.id; // pillem el nom que busquem
  console.log("ID: " + id);

  if (id == 333) {
    if (estat_botonet == 1) {
      serialport.write("off");
      estat_botonet = 0;
    }
    else {
      serialport.write("on");
      estat_botonet = 1;
    }

  }
});
//----------------------------------------------------------------<Temporal>