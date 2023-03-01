"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();
var errorNodeTimer;
var nodeFailCount = 0;
var cFailCount = 0;
var failTolerance = 5;
$(document).ready(function() {
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
		if (errorNodeTimer) {
			clearTimeout(errorNodeTimer);
			errorNodeTimer = null;
		}
		$('#error-box').hide();
		nodeFailCount = 0;
		cFailCount = 0;

		const strArray = result.split(" ");
		switch(strArray[0]) {
			case "mode":
				updateMode(strArray[1]);
				break;
			case "volume":
				updateVolume(strArray[1]);
				break;
			case "tempo":
				updateTempo(strArray[1]);
				break;
			case "info":
				updateMode(strArray[2]);
				updateVolume(strArray[4]);
				updateTempo(strArray[6]);
				updateDeviceUpTime(strArray[8]);
				break;
		}
	});

	socket.on('daError', function(result) {
		if (errorNodeTimer) {
			clearTimeout(errorNodeTimer);
			errorNodeTimer = null;
		}
		cFailCount++;
		if (cFailCount >= failTolerance) {
			$('#error-box').show();
			$('#error-text').text("SERVER ERROR: No response from beatbox application. Is it running?");
			console.log("SERVER ERROR: No response from beatbox application. Is it running?");
		}
	});
	
});

function sendCommandViaUDP(message) {
	socket.emit('daUdpCommand', message);
	errorNodeTimer = setTimeout(function() {
		nodeFailCount++;
		if (nodeFailCount >= failTolerance) {
			$('#error-box').show();
			$('#error-text').text("SERVER ERROR: No response from Nodejs. Is it running?");
			console.log("SERVER ERROR: No response from Nodejs. Is it running?");
		}
	}, 2000);
};

function requestDeviceUpTime() {
	sendCommandViaUDP("info");
};

function updateDeviceUpTime(uptime) {
	const uptimeNum = Number(uptime);
	const hours = Math.floor(uptimeNum / 60 / 60);
	const minutes = Math.floor((uptimeNum - (60*60*hours)) / 60);
	const seconds = Math.floor(uptimeNum - (60*60*hours) - (60*minutes));
	$('#status').text("Device up for: " + hours + ":" + minutes + ":" + seconds + "(H:M:S)");
}

function updateMode(mode) {
	var modes = ["None", "Rock #1", "Rock #2"];
	var num = Number(mode);
	$('#modeid').text(modes[num]);
}

function updateVolume(volume) {
	$('#volumeid').val(volume);
}

function updateTempo(tempo) {
	$('#tempoid').val(tempo);
}