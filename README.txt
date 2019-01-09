TIC TAC TOE GAME

AUTHOR:
Michael Lindner-D'Addario

DATE:
January 9th 2019

INTRO:
The tic tac toe game is an interactive game where a user can play tic tac toe on a board of any dimension against a computer, which will play perfectly. It can either be played online through a server, or offline through the terminal.

CAUTION:
For dimensions 4 and up, immense computing power is required to craft the game tree necessary for the computer to play perfectly. It is only recommended to use the dimension=3 setting (use dimension=2 if boring).

HOW TO RUN OFFLINE:
- In a terminal, run 'make offline'
- Play by running './offline'

HOW TO RUN ONLINE: 
You need to have set-up an apache2 server with CGI functionality enabled
- In a terminal, run 'ifconfig' and locate the local IP (should be something like 192.XYZ.X.XY)
- Plug this IP into the makefile
	(you may also change the port and board dimension from the makefile)
- Place the files into your apache2 cgi-bin folder, /usr/lib/cgi-bin, and in a terminal run 'make online'
	(you will probably have to run 'sudo make online' if the terminal gives you trouble)
- Begin the server in a terminal by running 'sudo service apache2 start'
- Run the file server in a terminal by typing './server' and ensure server is ready on given port
- Plug the IP into your browser and enjoy!

HOW TO CLOSE ONLINE:
- In the same terminal, input Ctrl+C (^C) to kill the server file
- Type 'sudo service apache2 stop' to stop the apache2 server
- Type 'make clean' to clean the files up

FILE DESCRIPTION:
ttt_game.c			Contains all the basic game logic for ttt
full_solve.c		Algorithm used to fully solve game of ttt and construct game tree
optimal_play.c		Algorithm used to follow game tree and play perfectly

offline.c			Main program to play offline

index.c				Builds the base HTML page for the online play
ttt_cgi.c			Allows HTML page to communicate with the ttt_server brain
ttt_server.c		Server brain that tracks tic tac toe boards and communicates with html page

Makefile			Eases building and saves compilation effort

STRANGE THING:
- The file 'online' generated from the makefile is stricly to save compilation effort. Since
  multiple files need to be ran for the online version to work, it does not make sense to track
  a single file, instead we have a dummy file do this for us

TROUBLESHOOTING:
- A quick little 'sudo chmod 777 *' usually helps

I HOPE YOU ENJOY
If you have any questions, reach me at michael.lindner.daddario@gmail.com