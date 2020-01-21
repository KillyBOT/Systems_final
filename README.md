# Systems Final Project : Snake Eater

## Creator

* Kyle Edwards

## Description

The game is basically a competitive version of snake. Each player starts at the corner of a map, and starts to move forward. They can turn left or right, like in snake. However, instead of Snake, each player leaves a line behind them. The goal of the game is to survive the longest without running into
your or into another person's line. This game will use some networking for local multiplayer, and maybe I'll shoehorn in some other systems related things.

Note: This game's original idea is based on an old game I played when I was younger called Armageddon, I think. I can't find the original game, but the best replica of the game I can
find online is here:

https://www.thepcmanwebsite.com/media/flash_tron/

## Problem that this project addresses

Uh, the problem of being bored?

## Technical description

Since this game is an online multiplayer game, it will utilize a client-server based architecture. The server will consist of one main server, which all clients will contact and send commands to, and several subservers, each one tasked with sending data to a specific client. This is to allow for a more streamlined design, since the main server doesn't need to concern itself over each socket; all the main server will do is read from one single socket. The port the server reads from is port 8080.
Each player will have a seperate instance of the game running on their computer, and will be controlling one of the players. Each game tic, the player will move, and then they will send that movement to the main server. The main server will then send that movement over to all of the other players. Finally, each client will read from their respective socket to the server, and will carry out all of the commands that were sent to it. Of course, the game will have to be synchronized so that each player is playing the game at the same time, meaning sadly that the speed of the game will be dependent on the slowest player

![Project Description Image](https://docs.google.com/drawings/d/1kOjkShlz_DWqw3qADPOKS9sCx4GJA3JeWSxvO43LZS4/edit?usp=sharing)

## Topics taught in class that this project will use

* Pipes (between the main and sub servers)
* Processes (Forking the main server to each sub server)
* Memory allocation
* Networking

## Rough outline of project

* Day 1: Main game engine; graphical output
* Day 2-5: Networking
* Day 6: Debugging (If needed)
* Day 7: Finishing touches

## Required libraries

Simple DirectMedia Layer (Or SDL)

Installation (Run in terminal):

### Linux

```
sudo apt-cache search libsdl2
sudo apt-get install libsdl2-dev
```

### Mac

Look at https://lazyfoo.net/tutorials/SDL/01_hello_SDL/mac/index.php

## Running the game

To compile the game, go to the ```src/``` directory, and type ```make server``` and ```make game```.

To run the server, run this in src/
```
./server [MAX_PLAYERS]
```
MAX_PLAYERS is how many players can connect to the server at one time. If it's not specified, then it will default to 2. The maximum that you could make it is 8.

To run the game, run this in src/
```
./game [IP]
```
IP is the IP address of the server. If it's not specified, then it will try to connect to the localhost (127.0.0.1).

## Known bugs

* If each of the players choose to reconnect before the server decides to accept connections again, then all of the characters won't be accepted.


