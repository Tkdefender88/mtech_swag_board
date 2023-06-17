/*
 * Queue.h
 *
 *  Created on: Jun 13, 2023
 *      Author: Justin
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#define QUEUE_SIZE 5

typedef struct {
    char data[QUEUE_SIZE];
    int front;
    int rear;
    int size;
} Queue;

void initQueue(Queue* queue);
int isFull(const Queue* queue);
int isEmpty(const Queue* queue);
char dequeue(Queue* queue);

// If queue is full returns the dequeued item else 0;
char enqueue(Queue* queue, const char item);



#endif /* QUEUE_H_ */
