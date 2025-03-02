/////////////////////////////////////////////////////////////////////////////////////////
/*
 * @file: File: helper.c
 * @author: HAVEN KOTARA
 * @instructions: http://www.cs.utsa.edu/~korkmaz/teaching/resources-os-ug/tk-assign/assign5.html
 *
 * --------------------------------------------------------------------------------------------------
 * This program implements the interface defined in helper.h. All linked list and struct
 * related functions can be found here, along with a few other helper functions.
 *
 */
/////////////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include "helper.h"


/*
 * Function: NewPCB
 * Usage: PCB *node;
 *        node = NewPCB(procID, procPR, cpuIndex, ioIndex, numRemaining);
 * --------------------------------------------------------------
 * This function allocates space for a PCB node and initialize its fields
 */
PCB *NewPCB(int procID, int procPR, int *cpuBurst, int *ioBurst)
{
    PCB *node = (PCB *)malloc( sizeof(PCB));

    if (!node)                          //check if the node couldn't create a new item
    {
        PrintError();
        return NULL;
    }

    node->procID = procID;              //initialize the variables
    node->procPR = procPR;
    node->cpuBurst = cpuBurst;
    node->ioBurst = ioBurst;

    return node;
}


/*
 * Function: Enlist
 * Usage: Enlist(queue, newNode);
 * -------------------------------
 * This function adds a new PCB node 
 * to the end of the queue.
 */
void Enlist(PCB *queue, PCB *newNode)
{
    if (queue->head == NULL && queue->tail == NULL) 
	{                                           //if the queue is empty
		queue->head = queue->tail = newNode;    //set the newNode as the head
        newNode->next = NULL;
        newNode->prev = NULL;
	}

    queue->tail->next = newNode;                //else: add new node to the end
    newNode->prev = queue->tail;
    queue->tail = newNode;
    queue->tail->next = NULL;
}


/*
 * Function: Delist
 * Usage: element = Delist(queue);
 * --------------------------------
 * This function removes the node at the head of the queue
 * and returns it.
 */
PCB *Delist(PCB *queue)
{
    if (QueueIsEmpty(queue))                    //check if the queue is empty
    { 
        printf ("No cells to Delist\n");
        return NULL;
    }

    PCB *node = queue->head;

    if (node == queue->head && node == queue->tail) //check if only one node in queue
    {
        queue->head = NULL;
        queue->tail = NULL;
        node->next = NULL;
        node->prev = NULL;

        return node;
    }

    if (queue->head != NULL)    //if a head exists
    {
        queue->head = queue->head->next;
        queue->head->prev = NULL;

	    return node;
    }
    return NULL;
}


/*
 * Function: DelistNode
 * Usage: Delist(queue);
 * -------------------------------------------
 * This function deletes a specified PCB node 
 * from the queue.
 */
PCB *DelistNode(PCB *queue, PCB* nodeToDelete)
{
    if (QueueIsEmpty(queue))
    {                                   //check if the list is empty
        printf ("No nodes to Delist\n");
        return NULL;
    }

    PCB *node = queue->head;                //[head]

    if (queue->head == queue->tail && queue->head == nodeToDelete)
    {
        queue->head = NULL;
        queue->tail = NULL;
        return nodeToDelete;
    }


    if (queue->head == nodeToDelete)               //if nodeToDelete is the head
    {
        queue->head = queue->head->next;
        return node;
    }

    while (node != NULL)
    {
        if (node->next == nodeToDelete)     //match is found
        {
            node->next = nodeToDelete->next;

            if (node->next == NULL)
            {
                queue->tail = node;
            }
            return node;
        }

        node = node->next;
    }
    return node;
}


/*
 * Function: GetNodeAtIndex
 * Usage: node = GetNodeAtIndex(list, index);
 * -----------------------------------------------
 * This function returns the element at the specified index in the
 * list, where the head of the list is defined as index 0. For
 * example, calling GetNodeAtIndex(list, 0) returns the initial
 * element from the list without removing it. 
 */
PCB *GetNodeAtIndex(PCB *queue, int index)
{
    PCB *node = queue->head;
    int i;

    if (QueueIsEmpty(queue))
    {
        return NULL;
    }

    for (i = 0; i < index && node != NULL; i++)
    {
        node = node->next;
    }

    return node;
}


/*
 * Functions: QueueIsEmpty
 * Usage: if (QueueIsEmpty(queue)) . . .
 * -------------------------------------------------
 * These functions test whether the queue is empty
 */
int QueueIsEmpty(PCB *queue)
{
  return (queue->head == NULL && queue->tail == NULL);
}


/*
 * Function: QueueSize
 * Usage: int length = QueueSize(queue);
 * -------------------------------------------------
 * This function returns the number of nodes in the queue.
 */
int QueueSize(PCB *queue)
{
    PCB *temp = NULL;
    int numNodes = 0;

    for (temp = queue->head; temp != NULL; temp = temp->next)
    {                                   //loop through queue until all nodes have been counted
        numNodes++;                     //count the number of nodes in the queue
    }

    free(temp);
    return numNodes;
}


/*
 * Function: FreeQueue
 * Usage: FreeQueue(list);
 * -------------------------------------------------
 * This function frees the storage associated with list.
 */
void FreeQueue(PCB *queue)
{   
    if (QueueIsEmpty(queue))        //check if the list is empty
    {
        free(queue);
        return;
    }

    else
    {
        while (!QueueIsEmpty(queue))
        {                           //loop until the queue is empty
            Delist(queue);
        }
            free(queue);            //free all nodes
    }
}


/* 
 * Function: PrintError
 * Usage: PrintError();
 * --------------------------------------------
 * When called, PrintError() prints the appropriate
 * error messages. 
 */
void PrintError()
{
  perror("Error");
  printf("Value of errno: %d\n", errno);
}

/* Function: Usage
 * Usage: Usage();
 * --------------------------------------------
 * This function prints the correct Usage. It is
 * mostly used for alerting users of incorrect input
 * when attempting to run the program.
 */
void Usage() 
{
    printf("Usage: ./prog -alg [FIFO|SJF|PR|RR] [-quantum integer(ms)] -input [file name]\n");
}

/////////////////////////////////////////////////////////////////////////////////////////
/*
 *                              DEBUGGING FUNCTIONS
 */
/////////////////////////////////////////////////////////////////////////////////////////

/* Function: PrintQueue
 * Usage: PrintQueue(queue)
 * --------------------------------------------
 * This function prints the current order of nodes
 * in the specified queue. The nodes are identified 
 * by their PID. This allows the user to see if nodes
 * are being stored, and in what order.
 */
void PrintQueue(PCB *queue) 
{
	PCB *node = queue->head;

	while(node != NULL) 
    {
		if(node->next != NULL) 
        {
			printf("p%d -> ", node->procID);
		}
        else
        {
		    printf("%d\n", node->procID);
        }

		node = node->next;
    }
}