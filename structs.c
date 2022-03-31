#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "structs.h"

#define SIZE 10
#define DEFAULTSIZE 2


/*Initiates a process list*/
list_process_t
*make_empty_list_process(void) {
	list_process_t *list;  //list is a pointer pointing to a block which has elements of list_t datatype,so 1 elem in block
				  //will have 2 node pointers
	list = (list_process_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
   	return list;
}

/*Initiates a resource list*/
list_resource_t
*make_empty_list_resource(void) {
	list_resource_t *list;  //list is a pointer pointing to a block which has elements of list_t datatype,so 1 elem in block
				  //will have 2 node pointers
	list = (list_resource_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
   	return list;
}

/*Inserts process to a list of processes*/
list_process_t
*insert_at_foot_process(list_process_t *list, process_t* value) {
	//process_t *new;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = value;
        //printf("func check: head= %llu, foot= %llu\n", list->head->file, list->foot->file); //IMPORTANT
	} else {
		list->foot->next = value;
		list->foot = value;
        //printf("func check: head= %llu, foot= %llu\n", list->head->file, list->foot->file); //IMPORTANT
	}
	return list;
}

/*Inserts process in the list of processes to be terminated to remove deadlock*/
list_process_t
*insert_at_foot_terminating_process(list_process_t *list, process_t* value) {
	//process_t *new;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = value;
        //printf("Terminating process list check: head= %llu, foot= %llu\n", list->head->file, list->foot->file); //IMPORTANT
	} else {
		list->foot->terminating_next = value;
		list->foot = value;
        //printf("func check: head= %llu, foot= %llu\n", list->head->file, list->foot->file); //IMPORTANT
	}
	return list;
}

/*Inserts process in the list of processes waiting on a resource*/
list_process_t
*insert_at_foot_process_waitlist(list_process_t *list, process_t* value) {
	//process_t *new;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = value;
        //printf("func check: head= %llu, foot= %llu\n", list->head->file, list->foot->file); //IMPORTANT
	} else {
		list->foot->waitlist_next = value;
		list->foot = value;
        printf("func check: head= %llu, foot= %llu\n", list->head->file, list->foot->file);
	}
	return list;
}

/*Inserting resource in a resource list*/
list_resource_t
*insert_at_foot_resource(list_resource_t *list, resource_t* value) {
	//process_t *new;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = value;
        //printf("func check: head= %llu, foot= %llu\n", list->head->resource, list->foot->resource); //IMPORTANT
	} else {
		list->foot->next = value;
		list->foot = value;
        //printf("func check: head= %llu, foot= %llu\n", list->head->resource, list->foot->resource); //IMPORTANT
	}
	return list;
}

/*Finds the total number of processes in a list*/
int traverse_count_process(list_process_t *list){
	process_t *new;
    int count = 0;
	new =list->head; //head is a node pointer
	while(new != NULL){
        count += 1;
		//printf("traverse= %llu\n", new->file);
		//printf("traverse= %u\n", new->file); //IMPORTANT
		new = new->next;
	}
    return count;
}

/*used to find the length of process waitlist, which is basically list of processes that
  are waiting to lock a particular resource*/
int traverse_count_process_waitlist(list_process_t *list){
	process_t *new;
    int count = 0;
	new =list->head; //head is a node pointer
	while(new != NULL){
        count += 1;
		//printf("traverse= %llu\n", new->file); //IMPORTANT
		//new = new->next;
        new = new->waitlist_next;
	}
    return count;
}

/*Finds the total number of resources in a list*/
int traverse_count_resource(list_resource_t *list){
	resource_t *new;
    int count = 0;
	new =list->head; //head is a node pointer
	while(new != NULL){
        count += 1;
		//printf("traverse= %llu, ", new->resource); //IMPORTANT
		new = new->next;
	}
    return count;
}

/* traverse resource list and check if this resource already exists or not */
int traverse_resource_exist(list_resource_t *list, unsigned long long int val, resource_t** r){
	resource_t *new;
    int found = 0;
	new =list->head; //head is a node pointer
	while(new != NULL){
        if(new->resource == val){
            found = 1;
            *r = new;
            return found;
        }
        else{
            new= new->next;
            continue;
        }
	}
    return found;
}

/*Finds maximum number of processes that is waiting on a resource*/
int traverse_find_longest_waitlist(list_resource_t *list){
	resource_t *new;
    int max = 0;
    int current_max = 0;
	new =list->head; //head is a node pointer
	while(new != NULL){
        //count += 1;
        if(new->waitlist_alloced){
            current_max = traverse_count_process_waitlist(new->waitlist_tracker);
        }
        if(current_max > max){
            max = current_max;
        }
		//printf("traverse= %u, ", new->resource);
		new = new->next;
	}
    return max;
}

/*Makes a stack to hold process*/
stack_process_t* makeStack(){
    stack_process_t* retStack = (stack_process_t*) malloc(sizeof(stack_process_t));
	assert(retStack);
	
	retStack->stack = NULL;
	retStack->used = 0;
	retStack->alloced = 0;
	
    return retStack;
}

/*Pushing process to a stack*/
stack_process_t* push_process(stack_process_t* stack, process_t item, int loop_number){
    if(stack->alloced == 0){
		stack->alloced = DEFAULTSIZE;
		stack->stack = (process_t*) malloc(sizeof(process_t)*(stack->alloced));
		assert(stack->stack);
	} else if (stack->alloced <= (stack->used + 1)){ //i.e. if stack full hoye jai
		stack->alloced *= 2;
		stack->stack = (process_t*) realloc(stack->stack, 
			sizeof(process_t)*(stack->alloced));
		assert(stack->stack);
	}

    item.loop_number = loop_number;
	(stack->stack)[stack->used] = item; //stac->stack is an int arr.So stack->used is the pos of the arr
	(stack->used)++;
    return stack;
}

/*Checks if the process has already been visited*/
int visited_process_check(stack_process_t* stack, process_t process){
    int found = 0;
    for(int i=0; i< stack->used; i++){
        if( (stack->stack)[i].file == process.file ){
            found =1;
            return found;
        }
    }
    return found;
}

/*Finds the loop number of a process from the stack */
int loop_number_stack(stack_process_t* stack, process_t process){
    int loop_number=0;
    for(int i=0; i< stack->used; i++){
        if( (stack->stack)[i].file == process.file ){
            loop_number = stack->stack[i].loop_number;
            return loop_number;
        }
    }
    return loop_number;
}

/*Prints the processes we need to terminate*/
void print_processes_to_terminate(list_process_t* terminate_process_list){ 
    if(terminate_process_list->head){
        printf("Terminate");
    }
    process_t* process = terminate_process_list->head;
    while(process != NULL){
        printf(" %llu", process->file); //IMPORTANT
        process = process->terminating_next;
    }
    
    /*for(int i=0; i< stack->used; i++){
        printf("%u, ", stack->stack[i].file);
    } */

    if(terminate_process_list->head){
        printf("\n");
    }
}


/*For task3, 4, 5. Detects deadlock and makes a list of processes to terminate */
void deadlock_detector(process_t* p, process_t* smallest_process, stack_process_t* process_stack, unsigned long long int counter, unsigned long long int smallest_pid, 
                void* process_or_resource, list_process_t* terminate_process_list, int loop_number){

                    //done with looping through list of processes
                    if(p == NULL){
                        return;
                    }

                    //null can only happen if it is a resource..ignore this comment
                    if(process_or_resource == NULL){
                        p = p->next;

                        if(p == NULL){ //eta ekhane use na koRLE SEG FAULT COZ WE CAN'T FIND SMALLEST PID FOR NULL TYPE PROCESS AT THE END
                            return;
                        }

                        process_t* process_or_resource = (process_t*) p;
                        counter = 0;
                        //not sure if we should change the loop number here
                        loop_number += 1;
                        //printf("STARTING NEW LOOP: LOOP NUM=%d , PROCES=%llu \n\n", loop_number, p->file);


                        smallest_process = p;
                        smallest_pid = p->file; 

                        deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process_or_resource, terminate_process_list, loop_number);
                        return;
                    }

                    if ( (counter % 2)==0 ){ //making sure it's a process
                        process_t* process = (process_t*) process_or_resource;

                        if(process->file < smallest_pid){
                            smallest_pid = process->file;
                            smallest_process = process;
                            //printf("CURRENT SMALLEST PROCESS IS %llu\n\n", smallest_pid); //IMPORTANT
                        }

                        if( !(visited_process_check(process_stack,  *process)) ){
                            push_process(process_stack, *process, loop_number); //when we push to the stack, we should also pass the loop number so that we can compare
                            //printf("PUSHED PROCESS =%llu\n", process->file); //IMPORTANT
                            counter += 1;
                            deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process->lock2, terminate_process_list, loop_number);
                            return; //testing for res7

                        }

                        else{
                            //else a asha does not guarantee a deadlock
                            //need to a comparison function to compare loop numbers
                            if( (loop_number_stack(process_stack, *process)) == loop_number ){ //mane this process was prev visited + part of same loop
                                //printf("FOUNDDD DEADDDLOOOOCCKCK!!!!\n"); //IMPORTANT
                                terminate_process_list = insert_at_foot_terminating_process(terminate_process_list, smallest_process); //only executed if deadlock found
                                //terminate_process_list = insert_at_foot_terminating_process(terminate_process_list, process); //only executed if deadlock found
                            }
                            
                            
                            //return;

                            //even if it's not a deadlock, we still want to start a new loop coz the adjacency list of the process has already been visited
                            p = p->next;

                            
                            if(p == NULL){ //eta ekhane use na koRLE SEG FAULT COZ WE CAN'T FIND SMALLEST PID FOR NULL TYPE PROCESS AT THE END
                                return;
                            }


                            process = p;
                            loop_number += 1; //even if it's not a deadlock, we still want to start a new loop coz the adjacency list of the process has already been visited
                            counter = 0;

                            smallest_process = p;
                            smallest_pid = p->file; //i think this is what is causing the segmentation error as p null HOILE IT'S SEG FAULT
                            //printf("NEW PROCESS FOR NEW LOOP IS %llu AND NEW SMALLEST PROCESS IS %llu\n\n", p->file, smallest_process->file); //IMPORTANT
                            //deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process, terminate_process_list, loop_number);
                            deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process, terminate_process_list, loop_number);
                            return;
                        }   
                    }

                    else{
                        resource_t* r = (resource_t*) process_or_resource;
                        //printf("GOT INTO RESOURCE NUM=%llu\n", r->resource); //IMPORTANT
                        process_t* process = r->heldBy ; //say res7 er last a eta null..p=400

                        //adding for res7
                        if(process == NULL){
                            p = p->next;
                            if(p == NULL){ //eta ekhane use na koRLE SEG FAULT COZ WE CAN'T FIND SMALLEST PID FOR NULL TYPE PROCESS AT THE END
                                //printf("ARRIVED AT END OF PROCESS LIST\n"); //IMPORTANT
                                return;
                            }
                            
                            process = p;
                            loop_number += 1; //even if it's not a deadlock, we still want to start a new loop coz the adjacency list of the process has already been visited
                            counter = 0;

                            smallest_process = p;
                            smallest_pid = p->file; //i think this is what is causing the segmentation error as p null HOILE IT'S SEG FAULT
                            //printf("NEW PROCESS FOR NEW LOOP IS %llu AND NEW SMALLEST PROCESS IS %llu\n\n", p->file, smallest_process->file); //IMPORTANT
                            //deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process, terminate_process_list, loop_number);
                            deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process, terminate_process_list, loop_number);
                            return;
                        }

                        counter += 1;
                        deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process, terminate_process_list, loop_number);
                        return;
                    }

                }

/*For the challenge task to run processes concurrently and avoiding deadlocks*/
int challenge_deadlock_avoider(list_process_t* list, int total_processes){
    process_t* process;
    int execution_time = 0;
    int terminated_processes = 0;
    //int new_loop =1; //
    while(terminated_processes != total_processes){
        process = list->head;
        while(process != NULL){
            //printf("PROCESS=%llu\n", process->file); //IMPORTANT
            if(process->challenge_process_terminated){ //i.e. if the current process has already been terminated
                process = process->next;
                if(process == NULL){ //means came at end of d process list
                    execution_time += 1;
                    break;
                }

                else{ //extra
                    continue;
                }
            }

            if( (process->lock1->challenge_held_by == NULL) && (process->lock2->challenge_held_by == NULL) ){
                process->lock1->challenge_held_by = process;
                process->lock2->challenge_held_by = process;
                process->challenge_process_terminated = 1;
                terminated_processes += 1;
                //printf("time=%d process=%llu, resource1=%llu, resource2=%llu\n", execution_time, process->file, process->lock1->resource, process->lock2->resource);
                printf("%d %llu %llu, %llu\n", execution_time, process->file, process->lock1->resource, process->lock2->resource); //IMPORTANT

                process = process->next;
                //printf("CURRENT PROCESS IS:%u\n", process->file);

                if(process == NULL){ //means came at end of d process list
                    execution_time += 1;
                    break;
                }
                else{
                    //printf("CURRENT PROCESS IS:%llu\n", process->file); //IMPORTANT
                    continue;
                }
            }
            process = process->next;
            if(process == NULL){ //means came at end of d process list
                execution_time += 1;
                break;
            }
            else{
                continue;
            }
            
        }

        /*getting out of this while loop means we have traversed the whole list. so, now, we want to free the resources
          held by terminating processes from loop1 */
        process = list->head;
        while(process != NULL){
            //printf("GOTTT INNN....process=%llu\n", process->file); //IMPORTANT
            if(process->challenge_process_terminated){ //i.e. if the current process has already been terminated
                process->lock1->challenge_held_by = NULL;
                process->lock2->challenge_held_by = NULL;
                process = process->next;

                if(process != NULL){
                    //printf("NEXT IS....process=%u\n", process->file);
                    continue;
                }
                else{
                    break;
                }

                //printf("NEXT 4m INSIDE IS....process=%llu\n", process->file); //IMPORTANT
                continue;
            }
            else{
                process = process->next;
                if(process != NULL){
                    //printf("NEXT IS....process=%llu\n", process->file); //IMPORTANT
                    continue;
                }
                else{
                    break;
                }
            }
        }
    }
    //printf("SIMULATION TIME=%d\n", execution_time);
    return execution_time;
    //return;
}

void
free_list_process(list_process_t *list) {
	process_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr; //prev initially not pointing to anything
		curr = curr->next;
		free(prev);
	}
	free(list);
}

void
free_list_resource(list_resource_t *list) {
	resource_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr; //prev initially not pointing to anything
		curr = curr->next;
        if(prev->waitlist_tracker){
            free(prev->waitlist_tracker);
        }
		free(prev);
	}
	free(list);
}

void free_visited_process_stack(stack_process_t* stack){
    if(stack->stack){
        free(stack->stack);
    }
    free(stack);
}