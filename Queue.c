/*
 * Queue.c
 *
 *  Created on: Jun 13, 2023
 *      Author: Justin
 */

#include "Queue.h"

void initQueue(Queue* queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
}

int isFull(const Queue* queue) {
    return queue->size == QUEUE_SIZE;
}

int isEmpty(const Queue* queue) {
    return queue->size == 0;
}

char dequeue(Queue* queue) {
    if(isEmpty(queue)) {
        return '\0';
    }

    char item = queue->data[queue->front];
    queue->front = (queue->front + 1) % QUEUE_SIZE;
    queue->size--;

    return item;
}

// If queue is full returns the dequeued item else 0;
char enqueue(Queue* queue, const char item) {
    char dequeued = '\0';
    if (isFull(queue)) {
        // Removes oldest item from queue if the queue is full
        dequeued = dequeue(queue);
    }

    queue->rear = (queue->rear + 1) % QUEUE_SIZE;
    queue->data[queue->rear] = item;

    if (isEmpty(queue)) {
        queue->front = queue->rear;
    }

    if (queue->size < QUEUE_SIZE) {
        queue->size++;
    }

    return dequeued;
}

