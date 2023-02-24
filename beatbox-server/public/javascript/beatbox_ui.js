"use strict";
// Client-side interactions with the browser.

// Make connection to server when web page is fully loaded.
var socket = io.connect();
$(document).ready(function() {

	$('#modeNone').click(function(){
		sendCommandViaUDP("mode 0");
	});
	$('#modeRock1').click(function(){
		sendCommandViaUDP("mode 1");
	});
	$('#modeRock2').click(function(){
		sendCommandViaUDP("mode 2");
	});
	$('#volumeDown').click(function(){
		sendCommandViaUDP("volumedown");
	});
	$('#volumeUp').click(function(){
		sendCommandViaUDP("volumeup");
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