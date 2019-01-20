/***************
* Bryan Arnold *
*   CSE 4300   *
*   2/24/18    *
*  Assignment1 *
***************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

   //Main method, will run program
   //This program creates a file that writes in
   //500 random integers into the file.
   int main(){

   //Initialize counter i, new file pointer, and make it so
   //rand() generates new random integers each time the program
   //is run
   srand(time(0));
   int i;
   FILE *file;

   //Open a new file for writing the integers into it.
   //If the file fails to load, indicate so and exit program.
   file = fopen("Integers.txt", "w");
   if(file == NULL){

	printf("Error creating file.");
	exit(1);

   }

   //Iterate 500 times for 500 integers. Use
   //fprintf to write into the new file an integer
   //as well as a space.
   for(i = 0; i < 500; i++){

	fprintf(file, "%d ", rand() % 10000);

   }

   //Close file and exit program
   fclose(file);

   return 0;

   }

