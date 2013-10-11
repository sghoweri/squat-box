




/** Pull in and setup modules / libraries required **/



var app     = require('express')(),
    server = require('http').createServer(app), // Create a new Express server called app
    express = require("express"),

    io = require('socket.io').listen(server), // Socket.io for async data with the web page being served

    request = require("request"), //Request - used for AJAX data with external Ruby server with face conference room data

    util = require('util'), //Used with the XBee Library to inspect, debug data packets

    SerialPort = require('serialport').SerialPort, //SerialPort - required for interfacing with the USB-connected XBee (in an XBee Explorer USB Adapter)
    xbee_api = require('./lib/xbee-api.js'), //The XBee NodeJS library used with SerialPort

    $ = require('jquery').create(); //jQuery





//Setup the express server to deliver static assets (static HTML page)
app.use(express.static(__dirname, 'public'));


// Configure the server to use port 3000 - we should probably change this to port 80 on the actual server being used
server.listen(3000);


// Setup the root of the server to point to the index.html page
app.get("/", function(req, res) {
  res.sendfile(__dirname + '/index.html');
});


// Utility function for the Express server for handling path requests
function handler (req, res) {
  fs.readFile(__dirname + '/index.html',
  function (err, data) {
    if (err) {
      res.writeHead(500);
      return res.end('Error loading index.html');
    }

    res.writeHead(200);
    res.write(fs.readFileSync(__dirname + path, 'utf8'));
    res.end(data);
  });
}












var string = '';
var myData;





// Configure the XBee API library to use API Mode 2. This essentially just sets which type of delimiter is used when parsing out the data stream. API Mode 2 is required with the Arduino XBee library.
var xbeeAPI = new xbee_api.XBeeAPI({
  api_mode: 2
});


//Part of the original XBee library code I referenced to write this. I believe this frameId variable is used to reference whatever the current FrameId is set to (if one is actually set). The FrameId is the unique identifier that can optionally be referenced and used in acknowledging a message you are sending / receiving was successfully received / sent.
var frameId = xbeeAPI.nextFrameId();



// Configure the SerialPort library to point to the local port being used with the XBee Explorer (the XBee connected via USB to the local machine running node)
var serialport = new SerialPort("/dev/tty.usbserial-AM01ON5J", {

    baudrate: 9600, //Eventually I would like to set this to a higher baudrate so we can handle higher data rates. We'll need to update each XBee's firmware to handle the higher baudrate

    parser: xbeeAPI.rawParser() //Configures the SerialPort library's data parser to instead pass through the built-in parser the XBee library provides, allowing for each frame to get handled automatically (for checksums, etc.)
});












var frame_obj0 = {
    type: 0x10,
    id: 0x02, // optional, nextFrameId() is called per default
    destination64: "0013A20040AC16FB",
    destination16: "fffe", // optional, "fffe" is default
    broadcastRadius: 0x00, // optional, 0x00 is default
    options: 0x00, // optional, 0x00 is default
    "data": string
}







//var C = xbee_api.constants;









// check for connection errors or drops and reconnect
var reconnectArd = function () {
  console.log('INITIATING RECONNECT');

  setTimeout(function(){
    console.log('RECONNECTING TO ARDUINO');

    connectArd();

  }, 2000);

};














var connectArd = function(){


    // If the serialport specified is available, startup Socket.io logic for the browser component
    serialport.on("open", function () {


        //When getConferenceStatus status fires (commented out below for now), it makes a GET request to the fake conference room API service we have setup, sending out the response data to the XBee network every 5 seconds. With the real server setup, this should only fire off whenever there has been a change in a room's status - either from the Arduino-end of things or from the latest data we've received

        var getConferenceStatus = function(){



            setInterval(function(){

                request({
                    uri: "http://officeapi.lespeabody.net/rooms/folsom_prison.json",
                    method: "GET",
                    timeout: 10000,
                    followRedirect: true,
                    maxRedirects: 10
                    }, function(error, response, body) {


                        var obj = JSON.parse(body);


                        var defaultString = {
                            "name":"Room Status",
                            "booked": obj.booked
                        };


                        var package = JSON.stringify(defaultString);

                        var length = package.length;

                        for (var i = 0; i < length; i++){
                            string += package[i];
                        }

                        //Replace any left or right curly brackets with double less-than or greater-than symbols instead. This is a fix an issue I discovered where the right curly bracket was being used as a reserved character in XBee API Mode 2.
                        string = string.replace(/{/g,"<<");
                        string = string.replace(/}/g,">>");


                        frame_obj0.data = string;


                        serialport.write(xbeeAPI.buildFrame(frame_obj0));

                });

            }, 5000);
        };



        //getConferenceStatus();


        io.on('connection', function (socket) {



            socket.emit('test', { hello: 'world' });


            socket.on('green', function (data) {

                console.log(data);

                packageData(data);


                serialport.write(xbeeAPI.buildFrame(frame_obj0));

            });

        });
    });



    // Automatically try to reconnect to the
    serialport.on('close', function(){
        console.log('ARDUINO PORT CLOSED');
        reconnectArd();
    });


    serialport.on('error', function (err) {
        console.error("error", err);
        reconnectArd();
    });

};


connectArd();



























var packageData = function(myData){


  var defaultString = {
    "name":myData,
    "booked":true
  }

  var package = JSON.stringify(defaultString);

  var length = package.length;



  for (i=0; i < length; i++){
    string += package[i];
  }

  string = string.replace(/{/g,"<<").replace(/}/g,">>");


  frame_obj0.data = string;

}






















xbeeAPI.on("frame_object", function(frame) {


    // Frame Type 149 occurs when an XBee rejoins the network (when the firmware is configured to emit the rejoin data packet)
    if (frame.type === 149){
        console.log(frame);
        console.log("rejoin!");
    }


    // If the frame packet being received contains data, log it here. Originally I had logic here for handling requests to book a room from the Arduino. This will be added back in with the refactoring underway.
    if (frame.data !== undefined){


        //console.log(frame);


        //This commented out logic handles the string to JSON conversion that needs to occur when receiving data packets from the Arduino / XBee. The left and right curly brackets that would typically be in the serialized JSON string have to be temporarily converted to double less than / greater than characters in order to get around the right curly bracket being a reserved delimiter character in the XBee API Mode 2 specification. This conversion needs to happen on both sides - when the JSON data is getting created and prepped to send and when it is getting interpreted on either end (on the Arduino and the Node Server)

        var length = (frame.data).length;

        var string = '';

        for (i=0; i < length; i++){
            string += (String.fromCharCode(frame.data[i]));
        }

        string = string.replace(/<</g,"{");
        string = string.replace(/>>/g,"}");


        var obj = JSON.parse(string);


        console.log(obj);


    }
});





