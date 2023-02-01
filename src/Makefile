SHELL = /bin/bash

# définition des commandes utilisées
CC = g++
RM = rm -f

# déclaration des options du compilateur
CFLAGS = -std=c++11 -Wall -O2

# définition des fichiers et dossiers
PROGNAME = r0
COMMANDS = compile_commands.json
VERSION = 0.1
SOURCES = r0.cpp
HEADERS = mysok.h

all: $(PROGNAME)

$(PROGNAME): $(SOURCES) $(HEADERS)
	$(CC) $(SOURCES) $(HEADERS) $(CFLAGS) -o $(PROGNAME)

clean:
	@$(RM) -r $(PROGNAME) $(COMMANDS)
