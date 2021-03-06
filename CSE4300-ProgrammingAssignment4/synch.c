/*
 * Synchronization primitives.
 * See synch.h for specifications of the functions.
 */

#include <types.h>
#include <lib.h>
#include <synch.h>
#include <thread.h>
#include <curthread.h>
#include <machine/spl.h>

////////////////////////////////////////////////////////////
//
// Semaphore.

struct semaphore *
sem_create(const char *namearg, int initial_count)
{
	struct semaphore *sem;

	assert(initial_count >= 0);

	sem = kmalloc(sizeof(struct semaphore));
	if (sem == NULL) {
		return NULL;
	}

	sem->name = kstrdup(namearg);
	if (sem->name == NULL) {
		kfree(sem);
		return NULL;
	}

	sem->count = initial_count;
	return sem;
}

void
sem_destroy(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	spl = splhigh();
	assert(thread_hassleepers(sem)==0);
	splx(spl);

	/*
	 * Note: while someone could theoretically start sleeping on
	 * the semaphore after the above test but before we free it,
	 * if they're going to do that, they can just as easily wait
	 * a bit and start sleeping on the semaphore after it's been
	 * freed. Consequently, there's not a whole lot of point in 
	 * including the kfrees in the splhigh block, so we don't.
	 */

	kfree(sem->name);
	kfree(sem);
}

void 
P(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);

	/*
	 * May not block in an interrupt handler.
	 *
	 * For robustness, always check, even if we can actually
	 * complete the P without blocking.
	 */
	assert(in_interrupt==0);

	spl = splhigh();
	while (sem->count==0) {
		thread_sleep(sem);
	}
	assert(sem->count>0);
	sem->count--;
	splx(spl);
}

void
V(struct semaphore *sem)
{
	int spl;
	assert(sem != NULL);
	spl = splhigh();
	sem->count++;
	assert(sem->count>0);
	thread_wakeup(sem);
	splx(spl);
}

////////////////////////////////////////////////////////////
//
// Lock.

struct lock *
lock_create(const char *name)
{
	struct lock *lock;

	lock = kmalloc(sizeof(struct lock));
	if (lock == NULL) {
		return NULL;
	}

	lock->name = kstrdup(name);
	if (lock->name == NULL) {
		kfree(lock);
		return NULL;
	}

	//Initialize the values of the lock struct to default values

	lock->lock_acquired = 0; //Lock isn't being used, so 0.
	lock->lock_acquire = NULL; //Lock currently isn't held by any thread or process.
	
	return lock;

}

void
lock_destroy(struct lock *lock)
{
	assert(lock != NULL);
	
	kfree(lock->name);
	kfree(lock);

	lock = NULL; //Back up destruction, just incase kfree() manages to fail. Nothing happens if klfree 				succeeds.	

}

void
lock_acquire(struct lock *lock)
{
	
	int spl;
	assert(lock != NULL);

	spl = splhigh(); //Disable all interrupts

	while(lock->lock_acquired == 1 && lock->lock_acquire != curthread){ //If the lock is held and it isn't 										the current thread...

		thread_sleep(lock); //...make the thread sleep

	}
	

	lock->lock_acquired = 1; //Thread acquired lock
	lock->lock_acquire = curthread; //The current thread has the lock

	splx(spl); //Reenable all interrupts

	//(void)lock;

}

void
lock_release(struct lock *lock)
{
	
	int spl;
	assert(lock != NULL);

	spl = splhigh(); //Disable all interrupts

	lock->lock_acquired = 0; //Lock isn't held by the thread anymore
	lock->lock_acquire = NULL; //The current thread no longer holds the lock, no thread does

	thread_wakeup(lock); //Wakeup the next thread waiting to get the lock

	splx(spl); //Reenable all interrupts

	//(void)lock;

}

int
lock_do_i_hold(struct lock *lock)
{
	
	return (lock->lock_acquire == curthread); //If the current thread holds the lock, return true

	//(void)lock;
	//return lock;

}

////////////////////////////////////////////////////////////
//
// CV


struct cv *
cv_create(const char *name)
{
	struct cv *cv;

	cv = kmalloc(sizeof(struct cv));
	if (cv == NULL) {
		return NULL;
	}

	cv->name = kstrdup(name);
	if (cv->name==NULL) {
		kfree(cv);
		return NULL;
	}
	
	// add stuff here as needed
	
	return cv;
}

void
cv_destroy(struct cv *cv)
{
	assert(cv != NULL);

	// add stuff here as needed
	
	kfree(cv->name);
	kfree(cv);
}

void
cv_wait(struct cv *cv, struct lock *lock)
{
	// Write this
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}

void
cv_signal(struct cv *cv, struct lock *lock)
{
	// Write this
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}

void
cv_broadcast(struct cv *cv, struct lock *lock)
{
	// Write this
	(void)cv;    // suppress warning until code gets written
	(void)lock;  // suppress warning until code gets written
}
