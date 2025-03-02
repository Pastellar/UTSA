/////////////////////////////////////////////////////////////////////////////////////////
/*
 * @file: File: helper.h
 * @author: HAVEN KOTARA
 * @instructions: http://www.cs.utsa.edu/~korkmaz/teaching/resources-os-ug/tk-assign/assign5.html
 *
 * --------------------------------------------------------------------------------------
 * This program serves as an interface for the doubly linked list and helper functions.
 *
 */
/////////////////////////////////////////////////////////////////////////////////////////

#ifndef _helper_h
#define _helper_h

/*
 * Type: PCB
 * -----------------------------------------------------
 * This type defines the cells used for the linked list that
 * stores the register and process information.
 */
typedef struct PCB
{
    int procID, procPR;             //process ID & priority
    int numCpuBurst, numIoBurst;
    int *cpuBurst, *ioBurst;        //to create dynamic arrays to store burst times
    int cpuIndex, ioIndex;
    double totalWait;
    struct timespec tsBegin, tsEnd, queueEnterClock;    //queueEnterClock to store ready Q time
    struct PCB *head, *tail, *prev, *next;
} PCB;


/*
 * Function: NewPCB
 * Usage: PCB *node;
 *        node = NewPCB(procID, procPR, cpuBurst, ioBurst);
 * --------------------------------------------------------------
 * This function allocates space for a PCB node and initialize its fields
 */
PCB *NewPCB(int procID, int procPR, int *cpuBurst, int *ioBurst);


/*
 * Function: Enlist
 * Usage: Enlist(queue, newNode);
 * -------------------------------
 * This function adds a new PCB node 
 * to the queue.
 */
void Enlist(PCB *queue, PCB *newNode);


/*
 * Function: Delist
 * Usage: element = Delist(queue);
 * --------------------------------
 * This function removes the node at the head of the queue
 * and returns it.
 */
PCB *Delist(PCB *queue);


/*
 * Function: DelistNode
 * Usage: Delist(queue);
 * -------------------------------------------
 * This function deletes a specified PCB node 
 * from the queue.
 */
PCB *DelistNode(PCB *queue, PCB* nodeToDelete);


/*
 * Function: GetNodeAtIndex
 * Usage: node = GetNodeAtIndex(list, index);
 * -----------------------------------------------
 * This function returns the element at the specified index in the
 * list, where the head of the list is defined as index 0. For
 * example, calling GetNodeAtIndex(list, 0) returns the initial
 * element from the list without removing it. 
 */
PCB *GetNodeAtIndex(PCB *queue, int index);


/*
 * Functions: QueueIsEmpty
 * Usage: if (QueueIsEmpty(queue)) . . .
 * -------------------------------------------------
 * These functions test whether the queue is empty
 */
int QueueIsEmpty(PCB *queue);


/*
 * Function: QueueSize
 * Usage: int length = QueueSize(queue);
 * -------------------------------------------------
 * This function returns the number of nodes in the queue.
 */
int QueueSize(PCB *queue);


/*
 * Function: FreeQueue
 * Usage: FreeQueue(list);
 * -------------------------------------------------
 * This function frees the storage associated with list.
 */
void FreeQueue(PCB *queue);


/* 
 * Function: PrintError
 * Usage: PrintError();
 * --------------------------------------------
 * When called, PrintError() prints the appropriate
 * error messages. 
 */
void PrintError();


/* Function: Usage
 * Usage: Usage();
 * --------------------------------------------
 * This function prints the correct Usage. It is
 * mostly used for alerting users of incorrect input
 * when attempting to run the program.
 */
void Usage();

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
void PrintQueue(PCB *queue);

#endif