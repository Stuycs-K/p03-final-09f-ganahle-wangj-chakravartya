# Final Project Proposal

## Group Members:

Edgar Ganahl, Jophy Wang, Aritro Chakravarty

# Intentions:

We plan on making a game called Higher or Lower, where one client chooses a number from 1-10, the server picks a random number within the same range, and then a second client has to guess whether the number of client #1 was above or below the number chosen by the computer.

# Intended usage:

First, the client will connect, and upon another player's connection to the server, they will be prompted to accept a match via typing Y/N on their computer and return. Then, one of the players (randomly selected) will pick a random number from the range, and upon entering, the server will pick a random number from the range and the other player will have to guess as to whether the computer's number is above or below or equal to the first players' by clicking Y/N on their computer and entering.

# Technical Details:

Our program will utilize Inter-Process Communication (IPC) via sockets to allow for multiplayer, and use named pipes to communicate information between the clients and the server. We will be breaking the project down into both c and h files, for the client, server, and networking, and we will divide ourselves by splitting the different files/features between us. I will work on the client, Jophy will work on the server and the game, and Aritro will work on the networking. We will utilize a make compile to generate both the server and client.

Will use:
- Allocating memory
- Semaphores
- Pipes (named and unnamed)
- Sockets

# Intended pacing:

-Skeleton/Pseudocode done by end of 1/6
-Begin working individually on client/server/networking/game
-Game should be complete by end of 1/7
-Client/server/networking done by end of 1/7
-Put everything together 1/8 and test
-Debug MVP on 1/9 and make video over weekend
-Add high score tracking and streak tracking on 1/12 + 1/13
-Think of other ideas
