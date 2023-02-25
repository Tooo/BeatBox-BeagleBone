"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();
$(document).ready(function() {
	sendCommandViaUDP("mode");
	sendCommandViaUDP("volume");
	sendCommandViaUDP("tempo");

	window.setInterval(function() {requestDeviceUpTime()}, 1000);

	$('#btnModeNone').click(function(){
		sendCommandViaUDP("mode 0");
	});
	$('#btnModeRock1').click(function(){
		sendCommandViaUDP("mode 1");
	});
	$('#btnModeRock2').click(function(){
		sendCommandViaUDP("mode 2");
	});
	$('#btnVolumeDown').click(function(){
		sendCommandViaUDP("volumedown");
	});
	$('#btnVolumeUp').click(function(){
		sendCommandViaUDP("volumeup");
	});
	$('#btnTempoDown').click(function(){
		sendCommandViaUDP("tempodown");
	});
	$('#btnTempoUp').click(function(){
		sendCommandViaUDP("tempoup");
	});
	$('#btnPlayHiHat').click(function(){
		sendCommandViaUDP("sound 1");
	});
	$('#btnPlaySnare').click(function(){
		sendCommandViaUDP("sound 2");
	});
	$('#btnPlayBase').click(function(){
		sendCommandViaUDP("sound 0");
	});
	$('#btnStop').click(function(){
		sendCommandViaUDP("stop");
	});
	
	socket.on('commandReply', function(result) {
		const strArray = result.split(" ");
		switch(strArray[0]) {
			case "mode":
				var modes = ["None", "Rock #1", "Rock #2"];
				var mode = Number(strArray[1]);
				$('#modeid').text(modes[mode]);
				break;
			case "volume":
				$('#volumeid').val(strArray[1]);
				break;
			case "tempo":
				$('#tempoid').val(strArray[1]);
				break;
			case "uptime":
				updateDeviceUpTime(strArray[1]);
				break;
		}
	});
	
});

function sendCommandViaUDP(message) {
	socket.emit('daUdpCommand', message);
};

function requestDeviceUpTime() {
	socket.emit('daUdpCommand', "uptime");
};

function updateDeviceUpTime(uptime) {
	const uptimeNum = Number(uptime);
	const hours = Math.floor(uptimeNum / 60 / 60);
	const minutes = Math.floor((uptimeNum - (60*60*hours)) / 60);
	const seconds = Math.floor(uptimeNum - (60*60*hours) - (60*minutes));
	$('#status').text("Device up for: " + hours + ":" + minutes + ":" + seconds + "(H:M:S)");
}