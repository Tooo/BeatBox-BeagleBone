# Makefile for building embedded application.
PUBDIR = $(HOME)/cmpt433/public/myApps

all: wav # node
	cd beatbox-code && make all

clean:
	cd beatbox-code && make clean

# Copy the sound files, and the nodeJS server to the public directory.
wav:
	mkdir -p $(PUBDIR)/beatbox-wav-files/ 
	cp -R beatbox-wav-files/* $(PUBDIR)/beatbox-wav-files/ 
# node:
# 	mkdir -p $(PUBDIR)/beatbox-server-copy/ 
# 	cp -R beatbox-server/* $(PUBDIR)/beatbox-server-copy/ 
# 	cd $(PUBDIR)/beatbox-server-copy/ && npm install