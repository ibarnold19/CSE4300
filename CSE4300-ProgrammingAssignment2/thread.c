/***************
* Bryan Arnold *
*   CSE 4300   *
*   3/23/18    *
*  Assignment2 *
***************/

//PART 1 OF THE ASSIGNMENT

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int printCount[] = {0, 0, 0, 0}; //keep track of how many times each thread's turn happens
int printCountFalse[] = {0, 0, 0, 0}; //keep track of how many times each thread's turn doesn't happen
int currentID = 1; //currentID
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; //lock mutex with default initialization

void *run(void *arg); //Protoype header for the run function

// run Function
// This function is responsible for carrying out what each
// thread will be doing concurrently. Each thread is given an
// ID, and checks whether the gloab variable currentID is the same.
// If it is, print 'My turn!', increment the currentID by 1, then check
// if the currentID > 4 and reset to 1 if so. If the currentID doesn't
// match the thread ID, print 'Not my turn!'. When each thread has been printed
// 'My turn!' 10 times, quit the program.
void *run(void *arg){

	int threadID = (int)arg; //Thread ID for each thread passed as arguments

	while(1){

		pthread_mutex_lock(&lock); //lock the data from other threads accessing it

		if(threadID == currentID){ //If thread Id matches Current ID

			printf("My turn!\n");
			currentID++;
			printCount[threadID - 1]++;
			//printf("Thread ID %d printCount: %d\n", threadID, printCount[threadID - 1]);

			if(currentID > 4){ //If currentID > 4

				currentID = 1;

			}


		} 

		pthread_mutex_unlock(&lock); //release data to other threads

		if(threadID != currentID){ //if thread ID doesn't match currentID

			printf("Not my turn!\n");
			printCountFalse[threadID - 1]++;
			usleep(1);

		} 


		if(printCount[threadID - 1] == 10){ //If all threads have matched the ID 10 times

			break;

		}


	}

	return arg; //exit run

}

// Main Function
// This is where the program will run.
// It creates the threads, pointer for return,
// and any integer tracker value variables.
// The threads are created, returned into the pointer,
// then the number of times each thread printed
// 'Not my turn!' are displayed.
int main(){

	//Threads, variables for tracking, etc.
	pthread_t t[4];
	void* ptr;
	int i;
	int id = 1; 
	int checker = 1;
	
	for(i = 0; i < 4; i++){ //Create 4 threads and make them run the run function

		checker = pthread_create(&t[i], NULL, run, (void *)id);

		if(checker != 0){

			printf("Error creating thread\n");
			exit(1);
	
		}

		id++;

	}

	for(i = 0; i < 4; i++){ //Wait for all threads to finish

		pthread_join(t[i], &ptr);
	
	}

	for(i = 0; i < 4; i++){ //Display how many times 'Not my turn!' printed

		printf("Thread number %d printed 'Not my turn!' %d times\n", i + 1, printCountFalse[i]);

	}
	
	return 0; //exit program

}
