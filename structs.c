#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "structs.h"

#define SIZE 10
#define DEFAULTSIZE 2



list_process_t
*make_empty_list_process(void) {
	list_process_t *list;  //list is a pointer pointing to a block which has elements of list_t datatype,so 1 elem in block
				  //will have 2 node pointers
	list = (list_process_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
   	return list;
}
list_resource_t
*make_empty_list_resource(void) {
	list_resource_t *list;  //list is a pointer pointing to a block which has elements of list_t datatype,so 1 elem in block
				  //will have 2 node pointers
	list = (list_resource_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
   	return list;
}

list_process_t
*insert_at_foot_process(list_process_t *list, process_t* value) {
	//process_t *new;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = value;
        printf("func check: head= %llu, foot= %llu\n", list->head->file, list->foot->file);
	} else {
		list->foot->next = value;
		list->foot = value;
        printf("func check: head= %llu, foot= %llu\n", list->head->file, list->foot->file);
	}
	return list;
}
list_process_t
*insert_at_foot_terminating_process(list_process_t *list, process_t* value) {
	//process_t *new;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = value;
        printf("Terminating process list check: head= %llu, foot= %llu\n", list->head->file, list->foot->file);
	} else {
		list->foot->terminating_next = value;
		list->foot = value;
        printf("func check: head= %llu, foot= %llu\n", list->head->file, list->foot->file);
	}
	return list;
}
list_process_t
*insert_at_foot_process_waitlist(list_process_t *list, process_t* value) {
	//process_t *new;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = value;
        printf("func check: head= %llu, foot= %llu\n", list->head->file, list->foot->file);
	} else {
		list->foot->waitlist_next = value;
		list->foot = value;
        printf("func check: head= %llu, foot= %llu\n", list->head->file, list->foot->file);
	}
	return list;
}
list_resource_t
*insert_at_foot_resource(list_resource_t *list, resource_t* value) {
	//process_t *new;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = value;
        printf("func check: head= %llu, foot= %llu\n", list->head->resource, list->foot->resource);
	} else {
		list->foot->next = value;
		list->foot = value;
        printf("func check: head= %llu, foot= %llu\n", list->head->resource, list->foot->resource);
	}
	return list;
}

/* added by me */
int traverse_count_process(list_process_t *list){
	process_t *new;
    int count = 0;
	new =list->head; //head is a node pointer
	while(new != NULL){
        count += 1;
		printf("traverse= %llu\n", new->file);
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
		printf("traverse= %llu\n", new->file);
		//new = new->next;
        new = new->waitlist_next;
	}
    return count;
}
int traverse_count_resource(list_resource_t *list){
	resource_t *new;
    int count = 0;
	new =list->head; //head is a node pointer
	while(new != NULL){
        count += 1;
		printf("traverse= %llu, ", new->resource);
		new = new->next;
	}
    return count;
}

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














/*task1, reading and making list*/
/*void read_and_process(FILE* file, char* tmp, list_resource_t* lr, list_process_t* lp, resource_t* r, process_t* p){
    int spaces=0, count=0, c=0, resourcenum=0, filenum=0;
    char* ptr;
    while ( (c=fgetc(file)) !=EOF ){ //last line er '\n' er time a ber hoye jai, so last line count 
                                                //hoy na
                    if(c != ' '){ //if c is not a space
                        if(c == '\r' || c=='\n'){ //if c is new line or \r

                            if(c == '\n'){
                                //tmp[count] = '\0';
                                count = 0;
                                spaces = 0;
                                //continue;
                                
                                //spaces += 1; //write in file when space=1, resource when space=2
                                //tmp[count]='\0';
                                printf("tmp= %s, spaces=%d\n", tmp, spaces);
                                
                                r[resourcenum++].resource= strtoll(tmp, &ptr, 10);
                                r[resourcenum - 1].next = NULL;
                                lr = insert_at_foot_resource(lr, &(r[resourcenum - 1]) );
                                //r[resourcenum++].resource= strtoul(tmp, &ptr, 10);
                                //r[resourcenum++].resource= strtoul(tmp, NULL, 0);

                                //r[resourcenum++].resource= atoi(tmp);

                                printf("resource= %llu\n", r[resourcenum -1].resource);
                                printf("resource1= %llu, ", r[resourcenum -2].resource);
                                printf("resource2= %llu\n", r[resourcenum -1].resource);

                                //printf("resource= %lu\n", r[resourcenum -1].resource);
                                //printf("resource= %d\n", r[resourcenum -1].resource);
                                count = 0;

                                //associating files to process
                                p[filenum -1].lock1 = &(r[resourcenum - 2]);
                                p[filenum -1].lock2 = &(r[resourcenum - 1]);
                                r[resourcenum -2].heldBy = &(p[filenum -1] );

                                printf("process=%llu, res1=%llu\n", (p[filenum-1].file), (*(&(r[resourcenum - 2]))).resource );
                                printf("process=%llu, res1=%llu, res2=%llu\n",
                                    (p[filenum-1].file),  (*(p[filenum-1].lock1)).resource, (*(p[filenum-1].lock2)).resource );
                                //printf("process=%u, resource_locked=%u, resource_req=%u\n", 
                                        //(p[filenum-1].file), p[filenum - 1].lock1->resource, p[filenum - 2].lock2->resource );
                            }



                            //spaces = 0;
                            //count = 0;
                            else{
                                tmp[count] = '\0'; //closing string at '/r'
                                printf("check=%c, ", c );
                                continue;
                            }
                        }

                        else{
                            tmp[count++] = c;
                            //printf("check=%c %d\n", c, (c == ' ') );
                            printf("check=%c, ", c );
                        }
                    }
                    else{
                        spaces += 1; //write in file when space=1, resource when space=2
                        tmp[count]='\0';
                        printf("tmp= %s %d, spaces=%d\n", tmp, atoi(tmp), spaces);
                        if( (spaces+1)%2 == 0){ //odd space mane it's a process
                        //if(spaces == 1){
                            p[filenum++].file = strtoll(tmp, 0L, 10); //typecasting issue
                            p[filenum - 1].next = NULL;
                            //p[filenum++].file = strtoul(tmp, 0L, 10); //typecasting issue
                            //p[filenum++].file = atoi(tmp); //typecasting issue
                            //printf("file= %d\n", p[filenum - 1].file); //file refers to process
                            printf("file= %llu\n", p[filenum - 1].file); //file refers to process
                            lp = insert_at_foot_process(lp, &(p[filenum - 1]) );
                            count = 0;
                        }
                        //break;
                        else{
                            //r[resourcenum++].resource= strtoul(tmp, 0L, 10);
                            r[resourcenum++].resource= strtoll(tmp, 0L, 10);
                            r[resourcenum - 1].next = NULL;
                            lr = insert_at_foot_resource(lr, &(r[resourcenum - 1]) );
                            //r[resourcenum++].resource= atoi(tmp);
                            printf("resource= %llu\n", r[resourcenum -1].resource);
                            count = 0;
                        }
                    }
    }
} */

stack_process_t* makeStack(){
    stack_process_t* retStack = (stack_process_t*) malloc(sizeof(stack_process_t));
	assert(retStack);
	
	retStack->stack = NULL;
	retStack->used = 0;
	retStack->alloced = 0;
	
    return retStack;
}

/*
struct stack_process{ //stack_process_t*
    process_t* stack;
	int used;
	int alloced; //alloced is the size of the stack
};
*/

//void push_process(stack_process_t* stack, process_t item){
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
void print_processes_to_terminate(list_process_t* terminate_process_list){ 
    printf("PROCESSES TO TERMINATE ARE: ");
    process_t* process = terminate_process_list->head;
    while(process != NULL){
        printf("%llu, ", process->file);
        process = process->terminating_next;
    }
    
    /*for(int i=0; i< stack->used; i++){
        printf("%u, ", stack->stack[i].file);
    } */
    printf("\n\n");
}




//task3
//task3
void deadlock_detector(process_t* p, process_t* smallest_process, stack_process_t* process_stack, unsigned long long int counter, unsigned long long int smallest_pid, 
                void* process_or_resource, list_process_t* terminate_process_list, int loop_number){

                    //p and smallest_process are same at the start, i.e. both are the 1st process from the process's linked list

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
                        printf("STARTING NEW LOOP: LOOP NUM=%d , PROCES=%llu \n\n", loop_number, p->file);


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
                            printf("CURRENT SMALLEST PROCESS IS %llu\n\n", smallest_pid);
                        }

                        if( !(visited_process_check(process_stack,  *process)) ){
                            push_process(process_stack, *process, loop_number); //when we push to the stack, we should also pass the loop number so that we can compare
                            printf("PUSHED PROCESS =%llu\n", process->file);
                            counter += 1;
                            deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process->lock2, terminate_process_list, loop_number);
                            return; //testing for res7

                        }

                        else{
                            //else a asha does not guarantee a deadlock
                            //need to a comparison function to compare loop numbers
                            if( (loop_number_stack(process_stack, *process)) == loop_number ){ //mane this process was prev visited + part of same loop
                                printf("FOUNDDD DEADDDLOOOOCCKCK!!!!\n");    
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
                            printf("NEW PROCESS FOR NEW LOOP IS %llu AND NEW SMALLEST PROCESS IS %llu\n\n", p->file, smallest_process->file);
                            //deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process, terminate_process_list, loop_number);
                            deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process, terminate_process_list, loop_number);
                            return;
                        }   
                    }

                    else{
                        resource_t* r = (resource_t*) process_or_resource;
                        printf("GOT INTO RESOURCE NUM=%llu\n", r->resource);
                        process_t* process = r->heldBy ; //say res7 er last a eta null..p=400

                        //adding for res7
                        if(process == NULL){
                            p = p->next;
                            if(p == NULL){ //eta ekhane use na koRLE SEG FAULT COZ WE CAN'T FIND SMALLEST PID FOR NULL TYPE PROCESS AT THE END
                                printf("ARRIVED AT END OF PROCESS LIST\n");
                                return;
                            }
                            
                            process = p;
                            loop_number += 1; //even if it's not a deadlock, we still want to start a new loop coz the adjacency list of the process has already been visited
                            counter = 0;

                            smallest_process = p;
                            smallest_pid = p->file; //i think this is what is causing the segmentation error as p null HOILE IT'S SEG FAULT
                            printf("NEW PROCESS FOR NEW LOOP IS %llu AND NEW SMALLEST PROCESS IS %llu\n\n", p->file, smallest_process->file);
                            //deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process, terminate_process_list, loop_number);
                            deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process, terminate_process_list, loop_number);
                            return;
                        }

                        counter += 1;
                        deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process, terminate_process_list, loop_number);
                        return;
                    }

                }

//void challenge_deadlock_avoider(list_process_t* list, int total_processes){
int challenge_deadlock_avoider(list_process_t* list, int total_processes){
    process_t* process;
    int execution_time = 0;
    int terminated_processes = 0;
    //int new_loop =1; //
    while(terminated_processes != total_processes){
        process = list->head;
        while(process != NULL){
            printf("PROCESS=%llu\n", process->file);
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
                printf("time=%d process=%llu, resource1=%llu, resource2=%llu\n", execution_time, process->file, process->lock1->resource, process->lock2->resource);

                process = process->next;
                //printf("CURRENT PROCESS IS:%u\n", process->file);

                if(process == NULL){ //means came at end of d process list
                    execution_time += 1;
                    break;
                }
                else{
                    printf("CURRENT PROCESS IS:%llu\n", process->file);
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

        //getting out of this while loop means we have traversed the whole list. so, now, we want to free the resources held by terminating processes from loop1
        process = list->head;
        while(process != NULL){
            printf("GOTTT INNN....process=%llu\n", process->file);
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

                printf("NEXT 4m INSIDE IS....process=%llu\n", process->file);
                continue;
            }
            else{
                process = process->next;
                if(process != NULL){
                    printf("NEXT IS....process=%llu\n", process->file);
                    continue;
                }
                else{
                    break;
                }
            }
        }
    }
    printf("SIMULATION TIME=%d\n\n", execution_time);
    return execution_time;
    //return;
}
