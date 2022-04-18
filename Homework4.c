// Stephanie Cook
// Project 4 - CS332
// 4/17/22
// BlazerID: secook2
// This program is a job scheduler that executes non-interactive jobs, and takes the
// number of processes you want running at a time as a command line argument.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>

typedef struct _queue {
	int size;    /* maximum size of the queue */
	int *buffer; /* queue buffer */
	int start;   /* index to the start of the queue */
	int end;     /* index to the end of the queue */
	int count;   /* no. of elements in the queue */
} queue;

// global variables
int P;
queue *q, *q2, *q3;

/* create the queue data structure and initialize it */
queue *queue_init(int n) {
	queue *q = (queue *)malloc(sizeof(queue));
	q->size = n;
	q->buffer = malloc(sizeof(int)*n);
	q->start = 0;
	q->end = 0;
	q->count = 0;

	return q;
}

/* insert an item into the queue, update the pointers and count, and
   return the no. of items in the queue (-1 if queue is null or full) */
int queue_insert(queue *q, int item) {
	if ((q == NULL) || (q->count == q->size))
	   return -1;

	q->buffer[q->end % q->size] = item;	
	q->end = (q->end + 1) % q->size;
	q->count++;

	return q->count;
}


// from queue.c
/* delete an item from the queue, update the pointers and count, and 
   return the item deleted (-1 if queue is null or empty) */
int queue_delete(queue *q) {
	if ((q == NULL) || (q->count == 0))
	   return -1;

	int x = q->buffer[q->start];
	q->start = (q->start + 1) % q->size;
	q->count--;

	return x;
}

// from queue.c
/* display the contents of the queue data structure */
void queue_display(queue *q) {
	int i;
	if (q != NULL && q->count != 0) {
		printf("queue has %d elements, start = %d, end = %d\n", 
			q->count, q->start, q->end);
		printf("queue contents: ");
		for (i = 0; i < q->count; i++)
	    		printf("%d ", q->buffer[(q->start + i) % q->size]);
		printf("\n");
	} else
		printf("queue empty, nothing to display\n");
}

// from queue.c
/* delete the queue data structure */
void queue_destroy(queue *q) {
	free(q->buffer);
	free(q);
}


void *submit(){
	pid_t pid, apid;
	int status; 
	char *line, buf[BUFSIZ], *args[BUFSIZ], *input;
	int num;

	// deleting element from the queue and storing it in line	
	line = queue_delete(q);
	input = queue_delete(q2);
	num = input[0] - 48;
	queue_insert(q3, input);
	
	// create execvp input
	int i, count, len;
	len = strlen(line);

	for(i = 1, args[0] = &line[1], count = 1; i < len; i++){
		if(line[i] == ' '){
			line[i] = '\0';
			args[count++] = &line[i+1];
		}
	}

	pid = fork();
	if(pid == 0) {
		int fdout, fderr;
		char outFileName[BUFSIZ], errFileName[BUFSIZ];
		
		// idea from lab 10
		// making file names
		sprintf(outFileName, "%d.out", num);
		sprintf(errFileName, "%d.err", num);
		
		// idea from lab 10
		// open file to write stdout to	
		if((fdout = open(outFileName, O_CREAT | O_APPEND | O_WRONLY, 0755)) == -1){
			printf("Error opening file %s for output\n", outFileName);
			exit(-1);
		}

		// open file to write stderr to
		if((fderr = open(errFileName, O_CREAT | O_APPEND | O_WRONLY, 0755)) == -1){
			printf("Error opening file %s for output\n", errFileName);
			exit(-1);
		}

		dup2(fdout, 1);
		dup2(fderr, 2);

		execvp(args[0], args);
		perror("exec");
		exit(-1);
	} else if (pid > 0) {
		printf("Child process %ld started \n", (long)pid);
		count++;
	} else {
		perror("fork"); 
		exit(EXIT_FAILURE);
	}


	queue_delete(q3);
}

void showjobs(char *args){
	printf("jobid \t  command \t \t  status \n");
	queue_display(q3);
	queue_display(q2);
}

// idea from driver
int main(int argc, char **argv) {
	queue *q;
	int i;

	/* create a queue data structure */
	q = queue_init(10);

	/* insert 5 items to the queue */
	for (i = 0; i < 5; i++) {
	    queue_insert(q, 100+i);
	    queue_display(q);
	}
	/* delete 5 items from the queue */
	for (i = 0; i < 5; i++) {
	    queue_delete(q);
	    queue_display(q);
	}
	/* insert 11 items to the queue */
	for (i = 0; i < 11; i++) {
	    if (queue_insert(q, 200+i) != -1)
	        queue_display(q);
	    else
		printf("queue full, insertion failed!\n");
	}
	/* delete 11 items from the queue */
	for (i = 0; i < 11; i++) {
	    if (queue_delete(q) != -1) 
	        queue_display(q);
	    else
		printf("queue empty, deletion failed!\n");
	}
	/* delete the queue */
	queue_destroy(q);

	return 0;
}