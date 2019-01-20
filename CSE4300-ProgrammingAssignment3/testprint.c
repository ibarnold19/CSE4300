#include <types.h>
#include <kern/errno.h>
#include <kern/unistd.h>
#include <kern/limits.h>
#include <lib.h>
#include <clock.h>
#include <thread.h>
#include <syscall.h>
#include <uio.h>
#include <vfs.h>
#include <sfs.h>
#include <test.h>
#include "opt-synchprobs.h"
#include "opt-sfs.h"
#include "opt-net.h"

int main(){

	int test[5] = {4, 167, 1253, 12, 9};
	int i = 0;

	for(i = 0; i < 5; i++){

		printint(test[i]);

	}

	_exit(0);

	return 0;

}
