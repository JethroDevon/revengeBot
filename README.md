# revengeBot

testing my SFML socket multiplexer with a cli controlled IRC bot, this will also help with my final uni project as I want to include a prototype 
message handler to handle messages between the user, the network connection and the main application itself, which in this case will be a bot but in my final uni project could be anything.

After this a similar structure will be used to make a framework to support a similar program, my aim is to be able to 'plop' a main application in and simply hook it up to the message handler.

In order  for cli to work smoothly program wil use GNUs readline library



This folder contains a few files for the connection class,

REGEX is a list of regular expressions to remove from output by way of deleting it from the message stack

BOTEX is a list of commands to listen for, if a command is detected, the following string starting at a hypen and ending at \n is sent to the connection

LOG.txt keeps a log of the name of each connection to server
