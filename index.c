/*********************************************************
* FILE: index.c
*
* Outputs the index.html page which serves as the base
* html page for tic tac toe.
*
* AUTHOR: Michael Lindner-D'Addario
* DATE: Jan 09 2019
***********************************************************/

#include <stdio.h>
#include <stdlib.h>

int main(){

	FILE* myfile = fopen("index.html", "w+");

	if (myfile == NULL){
		printf("Index.html failed to open.\n");
		return -1;
	}

	fprintf(myfile, "<html>\n<body>\n<h1>Perfect Tic Tac Toe Play</h1>\n");
	fprintf(myfile, "<h2>Dimension: %d</h2>\n", DIMENSION);
	fprintf(myfile, "<form action=\"cgi-bin/ttt.cgi\">\n");
	fprintf(myfile, "\t<b>Username: <input type=\"text\" name=\"username\" size=\"20\"><br />\n");
	fprintf(myfile, "\t<b>Password: <input type=\"text\" name=\"password\" size=\"20\"><br />\n");
	fprintf(myfile, "\t<b>Gamename: <input type=\"text\" name=\"gamename\" size=\"20\"><br />\n");
	fprintf(myfile, "\t<b>Square: <input type=\"text\" name=\"square\" size=\"20\"><br />\n");
	fprintf(myfile, "\t<input type=\"submit\" value=\"LOGIN\" name=\"LOGIN\">\n");
	fprintf(myfile, "\t<input type=\"submit\" value=\"CREATE\" name=\"CREATE\">\n");
	fprintf(myfile, "\t<input type=\"submit\" value=\"MOVE\" name=\"MOVE\">\n");
	fprintf(myfile, "</form>\n</body>\n</html>\n");

	fclose(myfile);
	return 0;
}
