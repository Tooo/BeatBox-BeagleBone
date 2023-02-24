"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();
$(document).ready(function() {

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
		var newDiv = $('<code></code>')
			.text(result)
			.wrapInner("<div></div>");
		$('#messages').append(newDiv);
		$('#messages').scrollTop($('#messages').prop('scrollHeight'));
	});
	
});

function sendCommandViaUDP(message) {
	socket.emit('daUdpCommand', message);
};