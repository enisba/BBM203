#include "Queue.h"
#include <iostream>

// Constructor to initialize an empty queue
Queue::Queue() {
    front= -1;
    rear = -1;
}

// Adds a province to the end of the queue
void Queue::enqueue(int province) {
    // Implement circular structure 
    // Add the province  
    if(front ==  (rear+1) %MAX_QUEUE_SIZE)return;
    
    if (front == -1){
        front= 0;
    }
    rear =  (rear +1) % MAX_QUEUE_SIZE;
    data[rear] = province;
    
}

// Removes and returns the front province from the queue
int Queue::dequeue() {
    if(front == -1) return -1;
    int x = data[front];
    if(front ==  rear){  
        front = rear = -1;
    }
    else front = (front+1) % MAX_QUEUE_SIZE;
    return x;
}

// Returns the front province without removing it
int Queue::peek() const {
    if (isEmpty())return -1;
    return data[front];
}

// Checks if the queue is empty
bool Queue::isEmpty() const {
    if(front == -1 )return true;
    return false;
}

// Add a priority neighboring province in a way that will be dequeued and explored before other non-priority neighbors
void Queue::enqueuePriority(int province) {
    if(front == (rear + 1) % MAX_QUEUE_SIZE)return;
    if(isEmpty()){
        enqueue(province);
        return;
    }
    front = (front -1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    data[front] = province;
}

