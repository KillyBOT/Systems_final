## 1.2.20

I started to work on the bare minimum for the game, such as the header files. In other words, I didn't do anything today.

## 1.3.20

Today, I worked on the makefile and the general structure. I didn't finish the makefile, but the general structure of the program was fairly solidified at this point.

## 1.5.20

I was quite productive today. I finished a majority of the game functions, and got SDL running well. This means that the game engine is now complete! I'm surprised that I finished it so quickly. Hopefully the networking will be as easy...

## 1.7.20

Networking is difficult, especially when you don't have a good idea of what type of structure you want for your server. I started to play around with some different structures, but all of them were insufficient. I wanted to have a sort of queue to accept commands from players without having to wait for the slowest one.

## 1.8.20 & 1.9.20 & 1.10.20

As said earlier, networking is difficult. At this time, I realized that my current networking model didn't work, so I got rid of it. I eventually bit the bullet and decided that I would just wait for each player to respond and would then reply to each player, meaning that the speed of the game is dependent on the slowest player. For a game like this however, I actually think it works alright. It wouldn't be fair if one player was slower than the rest, since they would probably move into a player that they haven't even seen yet.

## 1.12.20

I finished the networking part! I was able to play with myself on localhost, and the latency was actually pretty good: around 40ms I think. I tested the game with my dad on his computer, and I realized that in networking you need to make sure that all of the data has been transferred before reading. If you don't check for this, you'll read garbage. To fix this, I simply switched my reads to recv commands. I thought I would have to use UDP for a fast connection, but it hasn't been that big of a deal.

## 1.13.20 & 1.14.20

I added a way for each player to see their current latency. That's it. I also realized that a latency of 40ms was fairly high, so started to work on lowering it.

## 1.15.20

Now the latency is only 7ms! I did this by having each client have a separate game instance, and I had the server only receive and send the direction of each player, which meant I was not only transferring 16 bytes instead of 20 kilobytes. That made the game way smoother! I also made the server and game more synchronized to make sure no memory leaks happen, and each port is properly closed.

## 1.16.20

I started working on final touches, such as refining the README and this devlog! I also cleaned up some files that I eventually didn't end up using.
