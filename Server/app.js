
/**
 * Module dependencies.
 */






var app     = require('express')(),
    //port    = parseInt(process.env.PORT, 10) || 3000,
    server = require('http').createServer(app),
    io = require('socket.io').listen(server);

var express = require("express");

var request = require("request");


app.use(express.static(__dirname, 'public'));

server.listen(3000);

app.get("/", function(req, res) {
  res.sendfile(__dirname + '/index.html');
});


  var string = '';

var myData;





var util = require('util');
var SerialPort = require('serialport').SerialPort;
var xbee_api = require('./lib/xbee-api.js');



var $ = require('jquery').create();
























var C = xbee_api.constants;


var xbeeAPI = new xbee_api.XBeeAPI({
  api_mode: 2
});








var frameId = xbeeAPI.nextFrameId();





var connectArd = function(){



var serialport = new SerialPort("/dev/tty.usbserial-AM01ON5J", {
    baudrate: 9600,
    parser: xbeeAPI.rawParser()
  });



	serialport.on("open", function () {

    var getConferenceStatus = function(){
      console.log("firing!");

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
          }

          var package = JSON.stringify(defaultString);

          var length = package.length;



          for (i=0; i < length; i++){
            string += package[i];
          }

          string = string.replace(/{/g,"<<");
          string = string.replace(/}/g,">>");




          frame_obj0.data = package;



          serialport.write(xbeeAPI.buildFrame(frame_obj0));



      });


      }, 5000);
  }

  getConferenceStatus();


		io.sockets.on('connection', function (socket) {



		socket.emit('test', { hello: 'world' });

		socket.on('green', function (data) {
		  console.log(data);

      //myData = data;
      packageData(data);

      serialport.write(xbeeAPI.buildFrame(frame_obj0));


		  console.log(frame_obj0);

		  console.log("xbee write!");
		});




		//console.log(colorMsg);
		// serialport.write(xbeeAPI.buildFrame(frame_obj));

		  // console.log(data.r);
		  // console.log(data.g);
		  // console.log(data.b);

		  // var dataToSend =


		  // frame_obj0.data = dataToSend;


		  // console.log(dataToSend);

		  // serialport.write(xbeeAPI.buildFrame(frame_obj0));
		});


		socket.on('blue', function (data) {
			console.log("blue!");

		  	 fx[f] = $.parseJSON(clipboard2);
		      //console.log(fx[f]);
		      redraw(serialport);

		  	//serialport.write(xbeeAPI.buildFrame(frame_obj4));

		  //console.log("xbee write!");
		});


		});




		serialport.on('close', function(){
		console.log('ARDUINO PORT CLOSED');
		reconnectArd();
		});


		serialport.on('error', function (err) {
		console.error("error", err);
		reconnectArd();
		});



}


connectArd();








// check for connection errors or drops and reconnect
var reconnectArd = function () {
  console.log('INITIATING RECONNECT');
  setTimeout(function(){
	console.log('RECONNECTING TO ARDUINO');
	connectArd();

  var url = "http://officeapi.lespeabody.net/rooms/folsom_prison.json";

  $.ajax({
       type: 'GET',
        url: url,
        async: false,
        jsonpCallback: 'jsonCallback',
        contentType: "application/json",
        dataType: 'jsonp',
        success: function(json) {

            console.log(json);
        },
        error: function(e) {
           console.log(e.message);
        }
    });


  }, 2000);
};


var test = "test";

var data = {
  "name": "test"
};






var frame_obj0 = {
    // // type: 0x10,
    // // id: 0x01, // optional, nextFrameId() is called per default
    // // destination64: "0013A20040AC14B4",
    // // destination16: "562E", // optional, "fffe" is default
    // // broadcastRadius: 0x00, // optional, 0x00 is default
    // // options: 0x00, // optional, 0x00 is default
    // // data: "0x34" // Can either be string or byte array.

   //    type: 0x17,
   //    id: 0x01, // optional, nextFrameId() is called per default
   //    destination64: "0013A20040AC16FB",
   //    destination16: "ddf2", // optional, "fffe" is default
   //    // remoteCommandOptions: 0x02, // optional, 0x02 is default
   //    command: "ND",
   //    commandParameter: [] // Can either be string or byte array.


    type: 0x10,
    id: 0x02, // optional, nextFrameId() is called per default
    destination64: "0013A20040AC16FB",
    destination16: "fffe", // optional, "fffe" is default
    broadcastRadius: 0x00, // optional, 0x00 is default
    options: 0x00, // optional, 0x00 is default
    "data": string
    //[0x01, 0x3C, 0x00, 0x3E, 0x3C, 0x00, 0x3E, 0x3C, 0xFF, 0x3E]  // Can either be string or byte array.
}


var packageData = function(myData){


  var defaultString = {
    "name":"myData",
    "booked":true
  }

  var package = JSON.stringify(defaultString);

  var length = package.length;



  for (i=0; i < length; i++){
    string += package[i];
  }

  string = string.replace(/{/g,"<<");
  string = string.replace(/}/g,">>");




  frame_obj0.data = package;

}







var frame_obj1 = {
		// type: 0x10,
		// id: 0x01, // optional, nextFrameId() is called per default
		// destination64: "0013A20040AC14B4",
		// destination16: "562E", // optional, "fffe" is default
		// broadcastRadius: 0x00, // optional, 0x00 is default
		// options: 0x00, // optional, 0x00 is default
		// data: "0x34" // Can either be string or byte array.



		type: 0x10,
		id: 0x02, // optional, nextFrameId() is called per default
		destination64: "0013A20040AC16FB",
		destination16: "fffe", // optional, "fffe" is default
		broadcastRadius: 0x00, // optional, 0x00 is default
		options: 0x00, // optional, 0x00 is default
		data: [ 0x01 ]  // Can either be string or byte array.
	}

 var frame_obj2 = {
	// type: 0x10,
	// id: 0x01, // optional, nextFrameId() is called per default
	// destination64: "0013A20040AC14B4",
	// destination16: "562E", // optional, "fffe" is default
	// broadcastRadius: 0x00, // optional, 0x00 is default
	// options: 0x00, // optional, 0x00 is default
	// data: "0x34" // Can either be string or byte array.



	type: 0x10,
	id: 0x02, // optional, nextFrameId() is called per default
	destination64: "0013A20040AC16FB",
	destination16: "fffe", // optional, "fffe" is default
	broadcastRadius: 0x00, // optional, 0x00 is default
	options: 0x00, // optional, 0x00 is default
	data: "Cloudy"  // Can either be string or byte array.
}


var frame_obj3 = {
  // type: 0x10,
  // id: 0x01, // optional, nextFrameId() is called per default
  // destination64: "0013A20040AC14B4",
  // destination16: "562E", // optional, "fffe" is default
  // broadcastRadius: 0x00, // optional, 0x00 is default
  // options: 0x00, // optional, 0x00 is default
  // data: "0x34" // Can either be string or byte array.



  type: 0x10,
  id: 0x03, // optional, nextFrameId() is called per default
  destination64: "0013A20040AC16FB",
  destination16: "fffe", // optional, "fffe" is default
  broadcastRadius: 0x00, // optional, 0x00 is default
  options: 0x00, // optional, 0x00 is default
  data: '' // Can either be string or byte array.
}







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



xbeeAPI.on("frame_object", function(frame) {



  if (frame.type === 149){
  	console.log(frame);
    console.log("rejoin!");
  }


 if (frame.data !== undefined){



	var length = (frame.data).length;

	  var string = '';

	  for (i=0; i < length; i++){
	    string += (String.fromCharCode(frame.data[i]));
	  }

	  string = string.replace(/<</g,"{");
	  string = string.replace(/>>/g,"}");


	  var obj = JSON.parse(string);



	  var room = obj["Room Name"];

	  var status = obj["Booked"];



	  request({
        uri: "http://officeapi.lespeabody.net/rooms/" + room + "/",
        method: "PUT",
        timeout: 10000,
        followRedirect: true,
        maxRedirects: 10,
        form: {
        	"utf8": "✓",
        	"_method": "patch",
        	"authenticity_token": "0mMI7mhgT2rueSlK82VFqYicXHrGZlAOL+pnekMDkpI=",
        	"room[booked]": status
        }
      }, function(error, response, body) {

      		console.log(body);


          // var obj = JSON.parse(body);


          // var defaultString = {
          //   "name":"Room Status",
          //   "booked": obj.booked
          // }

          // var package = JSON.stringify(defaultString);

          // var length = package.length;



          // for (i=0; i < length; i++){
          //   string += package[i];
          // }

          // string = string.replace(/{/g,"<<");
          // string = string.replace(/}/g,">>");




          // frame_obj0.data = package;



          // serialport.write(xbeeAPI.buildFrame(frame_obj0));



      });



 }

 //  if ((util.inspect(frame.data))){
	//   //if ((util.inspect(frame.data[0])) == 49){
	//   var data = (util.inspect(frame.data));
	//   //console.log(data.length);

	//   // for (var i=0; i<data.length; i++){
	//   //   if (data[i] === '['){
	//   //     write = true;
	//   //   } else if (data[i] === ']'){
	//   //     write = false;
	//   //   }

	//   //   if (write === true){
	//   //     console.log(data[i]);
	//   //   }
	//   // }
	//   //console.log(data[1]);
	// //}
 //  }

});



var triggerFunction = function(serialport){





	var fireThis = function(serialport){



		// var url = '10.10.14.136:3000/api/v1/rooms/folsom';

		// $.ajax({
		//    type: 'GET',
		//     url: url,
		//     async: false,
		//     jsonpCallback: 'jsonCallback',
		//     contentType: "application/json",
		//     dataType: 'jsonp',
		//     success: function(json) {
		//        //console.dir(json.sites);
		//        console.log(json);
		//     },
		//     error: function(e) {
		//        console.log(e.message);
		//     }
		// });


		// var req = http.get('10.10.14.136:3000/api/v1/rooms/folsom', function(res) {
		//   console.log("Got response: " + res.statusCode);
		// }).on('error', function(e) {
		//   console.log("Got error: " + e.message);
		// });


		// // write data to request body
		// req.write('data\n');
		// req.write('data\n');
		// req.end();



	};
};

