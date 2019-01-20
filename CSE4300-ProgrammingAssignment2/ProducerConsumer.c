/***************
* Bryan Arnold *
*   CSE 4300   *
*   2/24/18    *
*  Assignment1 *
***************/

//PART 2 OF ASSIGNMENT

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; //lock mutex with default initialization
sem_t lineFull, lineEmpty; //Two semaphores, one for tracking empty or full lines of items

typedef int queue; //queue structure, the size of the queue of items
queue line[30]; //default queue size is 30

int counter = 0; //Track what's in queue
int pSleep = 0; //How many times producer's had to sleep
int cSleep = 0; //How many times consumers had to sleep
queue item = 0;

// Producer Function
// This function is responsible for how the producer
// threads will operate. They enter the function,
// are assigned a thread ID. They will then infinitely
// do the producer protocol, with sleeping before they act.
// If the semaphore blocks, increment the sleep count. 
// Now, use the mutex to lock data from other producer,
// add onto the queue, release the data and semaphore, and finally
// continue.
void *producer(void *arg) {

	int pid = (int)arg + 1; //Producer number

	while(1) { //run forever until producings > 100

		int waitTime = rand() / 1000000000 + 1; //Number from 1-3
		sleep(waitTime); //Sleep to allow queues to fill up
		item++; //Increase item number

		if (sem_wait(&lineEmpty) == 0){ //Sleep, so increment

			pSleep++;

		}

		pthread_mutex_lock(&lock); //Lock item data

		if(addNewItem(item)) { //If failure to produce

			printf("Producer failed to produce an item\n");

		} else { //Item produced, so displayed

			printf("Producer number %d produced item number: %d\n", pid, item);

		}

		pthread_mutex_unlock(&lock); //release data 
		sem_post(&lineFull); //Increment semaphore
	
	}

}

// addNewItem Function
// This function is responsible for adding new items onto the queue.
// If the queue isn't full, add item onto queue, otherwise error.
int addNewItem(queue item) {

	if(counter < 30) { //Queue can acquire new item

		line[counter] = item;
		counter++;
		return 0;

	} else { //Queue can't add new item

		return -1;

	}

}

// Consumer Function
// This function is responsible for how the consumer
// threads will operate. They enter the function,
// are assigned a thread ID. They will then infinitely
// do the consumer protocol, with sleeping before they act.
// If the semaphore blocks, increment the sleep count. 
// Now, use the mutex to lock data from other consumer,
// take from the queue, release the data and semaphore, and finally
// continue.
void *consumer(void *arg) {

	int cid = (int)arg + 1; //Thread ID assigned to each thread

	while(1) { //Runs forever until consumed > 100

		int waitTime = rand() / 1000000000 + 2; //Random number, slightly longer than producer, to reflect how this problem works in real life
		sleep(waitTime); //Sleep

		if(sem_wait(&lineFull) == 0){ //Increment sleep counter for consumers

			cSleep++;

		}
      
		pthread_mutex_lock(&lock); //Protect data from other thread

		if(consumeOldItem(&item)) { //Failed consumption of item

			printf("Consumer failed to consume an item\n");

		} else { //Item consumed, display

			printf("Consumer number %d consumed item number: %d\n", cid, item);

		}
      
		pthread_mutex_unlock(&lock); //Realse data to other thread
		sem_post(&lineEmpty); //Release semaphore

	}

}

// addNewItem Function
// This function is responsible for removing old items from the queue.
// If the queue isn't empty, remove the item first put into the queue, otherwise error.
int consumeOldItem(queue *item) {

	if(counter > 0) { //Queue not empty, so remove oldest one

		*item = line[(counter-1)];
		counter--;
		return 0;

	} else { //Otherwise error

		return -1;

	}

}

// Main Function
// This runs the entire program. Creates threads,
// initializes semaphore, and terminates when the number
// of produced and consumed are over 120.
int main(int argc, char *argv[]) {

	int i; // Incremenet variable

	pthread_t tidP[2]; //Producer threads
	pthread_t tidC[2]; //Consumer threads
	sem_init(&lineFull, 0, 0); //Empty emaphore
	sem_init(&lineEmpty, 0, 30); //Full semaphore

	for(i = 0; i < 2; i++) { //Create producers

		pthread_create(&tidP[i], NULL, producer, (void*)i);

	}

	for(i = 0; i < 2; i++) { //Create consumers

		pthread_create(&tidC[i], NULL, consumer, (void*)i);

	}

	while(1){ //Loop forever

		if(pSleep && cSleep > 120){ //If consumed/produced both over 120, break loop

			break;

		}

	}

	printf("Number of times producers put to sleep: %d\n", pSleep); //Display number of times slept
	printf("Number of times consumers put to sleep: %d\n", cSleep); //Display number of times slept

	return 0; //Exit program

}
