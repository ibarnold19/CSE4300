/***************
* Bryan Arnold *
*   CSE 4300   *
*   2/24/18    *
*  Assignment1 *
***************/

#include <stdio.h>
#include <stdlib.h>

   //Main method, will run program.
   //this program reads the file full
   //of integers, and iterates through
   //them to find the maximum and minimum value,
   //then displays those values as output.
   int main(){

   //File pointer to old file, initialize 
   //i to hold integer being read as well as
   //the minimum and maximum variables.
   FILE *file;
   int min, max;
   int i = 0;   

   //Open the integer file for read only. 
   //If the file doesn't open for some reason,
   //indicate so and exit the program.
   file = fopen("Integers.txt", "r");
   if(file == NULL){

	printf("Error opening file.\n");
	exit(1);

   }

   //Open the file to read first integer into i.
   //Assign max and min to the first value read.
   fscanf(file, "%d", &i);
   min = max = i;

   //While the end of the file has not been reached,
   //check if the value being read is greater than the current
   //maximum value or less than the current minimum value. If i
   //is greater or less than either, minimum or maximum is updated
   //with that new value. Begin scanning again at end.
   while(!feof (file)){

	if(i > max){

		max = i;

	} else if(i < min){

		min = i;

	}

	fscanf(file, "%d", &i);

   }

   //Display the maximum and minimum values in the
   //file being read, close the file, then return.
   printf("Maximum value in file: %d\n", max);
   printf("Minimum value in file: %d\n", min);
   fclose(file);

   return 0;

   }
