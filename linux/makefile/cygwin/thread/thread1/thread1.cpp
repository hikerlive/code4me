/**
 * A Simpile thread program
 * linux program p481
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

void* thread_function(void* arg);

char message[] = "Hello world";

int main(int argc, char* argv[])
{
	int res;
	pthread_t a_thread;
	void* thread_result;

	res = pthread_create(&a_thread, NULL, thread_function, (void*) message);
	if (res !=0 )
	{
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}

	printf("Waiting for thread finish...\n");
	res = pthread_join(a_thread, &thread_result);
	if (res !=0 )
	{
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}
	printf("Thread joined, it returned %s\n", (char*)thread_result);
	printf("Message is now %s\n", message);
	exit(EXIT_FAILURE);
    
    return 0;
}

void* thread_function(void* arg)
{
	printf("thread_function is running. Argument was %s\n", (char*)arg);
	sleep(3);
	strcpy(message, "Bye!");
	pthread_exit((void*)"Thank you for the CPU time");
}
















 
