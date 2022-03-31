#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>

#define SIZE 10
#define DEFAULTSIZE 2


/*typedef struct node{
    resource_t resource;
    process_t process;
}node_t; */

typedef struct resource resource_t;
typedef struct process process_t;
typedef struct list_process list_process_t;
typedef struct stack_process stack_process_t; 

struct resource{
    //unsigned int resource;
    unsigned long long int resource;
    //unsigned long int resource;
    //int resource;
    //process_t* p;
    struct process* heldBy;

    //for waitlist
    struct process* waitlist;
    int waitlist_alloced;
    int waitlist_size;
    list_process_t* waitlist_tracker;
    
    resource_t* next; //for linkedlist

    //challenge task
    process_t* challenge_held_by;
};

struct process{
    int count; //to count and ensure exec tie for both resources are calculated
    //unsigned int file;
    unsigned long long int file;
    //unsigned long int file;
    //int file;
    int resource_count;
    resource_t* lock1;
    resource_t* lock2;
    process_t* next;
    process_t* waitlist_next;
    process_t* terminating_next;
    int loop_number;

    //challenge task
    int challenge_process_terminated;
};

struct list_process{ 
	process_t *head;
	process_t *foot;
};

typedef struct { 
	resource_t *head;
	resource_t *foot;
} list_resource_t;

//task 3
struct stack_process{
    process_t* stack;
	int used;
	int alloced; //alloced is the size of the stack
};


/*function declarations*/
list_process_t *make_empty_list_process(void);
list_process_t *insert_at_foot_process(list_process_t *list, process_t* value);
int traverse_count_process(list_process_t *list);

list_resource_t *insert_at_foot_resource(list_resource_t *list, resource_t* value);
int traverse_count_resource(list_resource_t *list);
list_resource_t *make_empty_list_resource(void);
int traverse_find_longest_waitlist(list_resource_t *list);
int traverse_resource_exist(list_resource_t *list, unsigned long long int val, resource_t** r);
list_process_t *insert_at_foot_process_waitlist(list_process_t *list, process_t* value);

//task-3
stack_process_t* makeStack();
//void push_process(stack_process_t* stack, process_t item);
stack_process_t* push_process(stack_process_t* stack, process_t item, int loop_number);
int visited_process_check(stack_process_t* stack, process_t process);
list_process_t* insert_at_foot_terminating_process(list_process_t *list, process_t* value);
void deadlock_detector(process_t* p, process_t* smallest_process, stack_process_t* process_stack, unsigned long long int counter, unsigned long long int smallest_pid, 
                void* process_or_resource, list_process_t* terminate_process_list, int loop_number);
int loop_number_stack(stack_process_t* stack, process_t process);
void print_processes_to_terminate(list_process_t* terminate_process_list);

//challenge task
//void challenge_deadlock_avoider(list_process_t* list, int total_processes);
int challenge_deadlock_avoider(list_process_t* list, int total_processes);


int main(int argc, char** argv){
    //format1: ./detect -f resources.txt
    //format2: ./detect -e -f resources.txt
    int execution_time = 1; //coz 2nd col er shobai unique, so guaranteed time from that column is 1
    int current_mutual_maxtime=0; //prolly better to make it unsigned int
    unsigned long long int resource_val;
    int node_type_marker = 0;

    /*format 1*/
    FILE* file; int lines =0;
    char* tmp = malloc(sizeof(char)*5);
    char* ptr;

    list_process_t* list_process = make_empty_list_process();
    list_process_t* list_terminating_process = make_empty_list_process();
    list_resource_t* list_resource = make_empty_list_resource();

    process_t* p =(process_t*)  malloc(sizeof(process_t)*SIZE);
    int p_size = SIZE; //handle realloc
    int r_size = SIZE;

    resource_t* r =(resource_t*) malloc(sizeof(resource_t)*SIZE);
    resource_t* task2_r;


    assert(tmp);
    assert(p);
    assert(r);

    //printf("test= %d\n", lines);
    if((strcmp(argv[1], "-f") == 0)){
        file = fopen(argv[2], "r");
        int c=0, count=0, spaces=0, filenum=0, resourcenum=0; //lines=0; //lines=processes
        int p_size=SIZE, r_size=SIZE;

        /*On Windows, lines in text files are separated by \r\n, not just \n*/
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
                            
                            /*r[resourcenum++].resource= strtoll(tmp, &ptr, 10);
                            r[resourcenum - 1].next = NULL;
                            list_resource = insert_at_foot_resource(list_resource, &(r[resourcenum - 1]) ); */

                            
                            /*test*/
                            resource_val = strtoll(tmp, &ptr, 10);
                            //unsigned long long int test1 = 4;
                            //unsigned long long int test2 = 4;
                            //int task2 = test1==resource_val;
                            //printf("TEST RESULT= %d\n", task2);
                            
                            //traverse resource list and check if this resource already exists or not
                            //int test = traverse_resource_exist(list_resource, resource_val);
                            //printf("TASK2 TEST: %d\n", test);
                            int testt = traverse_resource_exist(list_resource, resource_val, &task2_r);
                            if(testt == 1){
                                printf("\nEXISTING RESOURCE NUM= %u\n\n", resource_val);
                                printf("RESOURCE= %u\n", task2_r->resource );                                
                                list_process->foot->lock1 = list_resource->foot; //lock1 pointing to resource1...after that we add resource2 to the resource list
                                list_process->foot->lock2 = task2_r;

                            } 


                            else{
                                resource_t* r = (resource_t*) malloc(sizeof(resource_t)); //can't always malloc as same resource may have been created previously
                                assert(r);
                                //r->waitlist_tracker = (list_process_t*) malloc(sizeof(list_process_t));
                                //assert(r->waitlist_tracker);
                                r->resource = strtoll(tmp, &ptr, 10);
                                r->next = NULL;


                                printf("resource1= %u, ", list_resource->foot->resource);
                                printf("process=%u, res1=%u\n", list_process->foot->file, list_resource->foot->resource );

                                list_process->foot->lock1 = list_resource->foot; //lock1 pointing to resource1...after that we add resource2 to the resource list

                                list_resource = insert_at_foot_resource(list_resource, r );
                                printf("resource= %u\n", r->resource);
                                //printf("resource1= %u, ", r[resourcenum -2].resource);
                                printf("resource2= %u\n", list_resource->foot->resource);
                                printf("process=%u, res2=%u\n", list_process->foot->file, list_resource->foot->resource );

                                list_process->foot->lock2 = r;
                                //printf("TASK2 CHECKKKK FOR HELDBY")
                            }


                            

                            
                            count = 0;
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

                        /*test*/
                        process_t* p = (process_t*) malloc(sizeof(process_t));
                        assert(p);
                        p->file = strtoll(tmp, 0L, 10);
                        p->next = NULL;
                        printf("file= %u\n", p->file); //file refers to process
                        list_process = insert_at_foot_process(list_process, p );
                        count = 0;

                    }
                    //break;
                    else{
                        //r[resourcenum++].resource= strtoul(tmp, 0L, 10);
                        //r[resourcenum++].resource= strtoll(tmp, 0L, 10);

                        /*test*/

                        resource_val = strtoll(tmp, &ptr, 10);
                        if(list_resource != NULL){
                            if(traverse_resource_exist(list_resource, resource_val, &task2_r) == 1){
                                printf("\nEXISTING RESOURCE NUMMM= %u\n", resource_val);
                                printf("RESOURCE= %u\n", task2_r->resource );

                                //task2_r->heldBy = list_process->foot; //already heldBy blocked by prev process
                                //task2_r->next = NULL;
                                printf("resource= %u, process holding the resource= %u\n", task2_r->resource, list_process->foot->file);
                                task2_r->heldBy = list_process->foot;
                                printf("\nNOT INSERTED AGAIN IN RESOURCE LIST\n\n");
                                //list_resource = insert_at_foot_resource(list_resource, r ); already in list
                            } 
                            else{
                                //resource_t* r = (resource_t*) malloc(sizeof(resource_t));
                                //assert(r);
                                resource_t* r = (resource_t*) malloc(sizeof(resource_t));
                                //r->waitlist_tracker = (list_process_t*) malloc(sizeof(list_process_t));
                                //assert(r->waitlist_tracker);
                                assert(r);
                        
                                r->resource= strtoll(tmp, 0L, 10);
                                r->heldBy = list_process->foot;
                                r->next = NULL;
                                printf("resource= %u, process holding the resource= %u\n", r->resource, list_process->foot->file);
                                list_resource = insert_at_foot_resource(list_resource, r );
                                printf("\nINSERTED IN RESOURCE LIST\n\n");
                            }
                        }   

                        count = 0;
                    }
                }
        }

        //for(int i=0; i<4; i++){
        resource_t* ress = list_resource->head;    
        while(ress->next != NULL){
            if(ress->heldBy != NULL){
                printf("check:: resource= %u, process holding the resource= %u\n", ress->resource,  ress->heldBy->file);
            }
            ress = ress->next;
            printf("next ress= %u\n", ress->resource);
            //printf("check2:: resource= %u, process holding the resource= %u\n", ress->resource,  ress->heldBy->file);
        }

            //printf("check:: resource= %u, process holding the resource= %u\n", r[1].resource, r[1].heldBy->file);
        //}






        //TASK-3
        /*if(node_type_marker==0 || ((node_type_marker + 1)%3 == 0) ){ //means it is process_t node
        //if process, then go to process->lock2. if resource, go to heldby
        //process and resource will keep alternating between them, so we can have a counter such that even val hoile process
        //bujhe, so looks for "lock2", else looks for "heldBy"        
        }*/
        //node_type_marker
        int loop_number = 1;
        printf("node type marker=%d\n", node_type_marker);
        //process_t process = list_process ->head; 
        unsigned long long int smallest_process_id = 0;
        unsigned long long int counter = 0; //for checking if it's process or resource
        process_t* task3_process = list_process->head;
        list_process_t* terminate_process_list= make_empty_list_process();

        int check = 0;
        stack_process_t* visited_process_stack = makeStack();
        assert(visited_process_stack);

        //deadlocks(task3_process, task3_process, visited_process_stack, 0, task3_process->file, task3_process, terminate_process_list);
        deadlock_detector(task3_process, task3_process, visited_process_stack, 0, task3_process->file, task3_process, terminate_process_list, loop_number);
        printf("CHECKING Terminating LIST:\n");

        if(terminate_process_list->head != NULL){
            printf("CHECKING Terminating LIST= %u\n\n", terminate_process_list->head->file);
            //printf("TERMINATE PROCESS=%u\n", terminate_process_list->head->file);
        }
        print_processes_to_terminate(terminate_process_list);







        //CHALLENGE TASK
        int total_processes = traverse_count_process(list_process);
        int challenge_exec_time = 0;
        challenge_exec_time = challenge_deadlock_avoider(list_process, total_processes);
        printf("SIMULATION TIME IS %d\n\n", challenge_exec_time);






        /*task2,3- calculating execution time and detecting deadlock*/
        //int node_type_marker = 0; //prolly better to make it unsigned
        int waitlist_activator = 0;

        //need to make a stack?


        //iteration/traversal to start from a process
        process_t* process = list_process ->head; 
        /*given that there is no deadlock for task 2, it is guaranteed in such case that there is at least 1 resource/file in the last column which is not
          present in the 2nd last column */
        execution_time += 1; //t=2 hoilo here as col 2 er jonno t=1,then col 3 er kichu resource er jonoo it moves to t=2

        //we are bothered to check time from last columnn as col 1's time can be predetermined
        while(process != NULL){
            printf("p->count = %d\n", process->count);
            if(process->count < 1){
                printf("got in\n");
                if(process->lock2->heldBy == NULL){
                    //printf("held by process: %u\n", process->lock2->heldBy->file);
                    printf("resource held: %u\n", process->lock2->resource);
                    process->lock1 = process->lock2;            
                    process->lock1->heldBy = process; //updating the resource's "heldby"
                    process->count += 1;
                    printf("successful3\n");
                }
                //else{
                    //printf()
                //}
            //}
            //printf("successful2/\n");
            //process = process->next;
        //}
                else{
                    //execution_time += 1;
                    printf("resource held in else: %u\n", process->lock2->resource);
                    if( !(process->lock2->waitlist_alloced) ){ //what's the difference between waitlist_alloced and waitlist_activator?
                        //process->lock2->waitlist = (process_t*) malloc(sizeof(process_t)*5); 
                        waitlist_activator = 1; //eta 1 thakle we will try finding the length of this list, else we skip
                        //assert(process->lock2->waitlist);
                        process->lock2->waitlist_tracker = make_empty_list_process();
                        process->lock2->waitlist_alloced = 1; //eta 1 means list er malloc done, r malloc lagbe na
                    }
                    //if( (process->lock2->waitlist_size + 1) == (process->lock2->waitlist_alloced) ){ //prolly use sizeof()/sizeof()
                        //realloc
                    //}
                    //insert at foot function banate hobe for each waitlist
                    process->lock2->waitlist_tracker = insert_at_foot_process_waitlist(process->lock2->waitlist_tracker, process );
                }
            }
            process = process->next;
        }
        //traverse thorugh the resources to find the longest waitlist
        if(waitlist_activator){
            printf("finding longest waitlist: size=%d\n", traverse_find_longest_waitlist(list_resource) );
            execution_time += traverse_find_longest_waitlist(list_resource); 
        }
        

        /*while(!feof(file)){
            fscanf(file, "%d", &temp);
            printf("c= %d\n", temp);
            if(temp == '\n'){
                lines += 1;
            }
        }*/

        
        //printf("lines read= %d", lines);
        
    }

    /*counting process and resource numbers*/
    //processes
    int z1 = traverse_count_process(list_process);
    int z2 = traverse_count_resource(list_resource);
    printf("total process= %d\n", z1);
    printf("total resources= %d\n", z2);
    printf("execution time= %d\n", execution_time);
    printf("node type marker=%d\n", node_type_marker);
    ///printf("%u\n", )

    fclose(file);

    return 0;
}

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
        printf("func check: head= %u, foot= %u\n", list->head->file, list->foot->file);
	} else {
		list->foot->next = value;
		list->foot = value;
        printf("func check: head= %u, foot= %u\n", list->head->file, list->foot->file);
	}
	return list;
}
list_process_t
*insert_at_foot_terminating_process(list_process_t *list, process_t* value) {
	//process_t *new;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = value;
        printf("Terminating process list check: head= %u, foot= %u\n", list->head->file, list->foot->file);
	} else {
		list->foot->terminating_next = value;
		list->foot = value;
        printf("func check: head= %u, foot= %u\n", list->head->file, list->foot->file);
	}
	return list;
}
list_process_t
*insert_at_foot_process_waitlist(list_process_t *list, process_t* value) {
	//process_t *new;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = value;
        printf("func check: head= %u, foot= %u\n", list->head->file, list->foot->file);
	} else {
		list->foot->waitlist_next = value;
		list->foot = value;
        printf("func check: head= %u, foot= %u\n", list->head->file, list->foot->file);
	}
	return list;
}
list_resource_t
*insert_at_foot_resource(list_resource_t *list, resource_t* value) {
	//process_t *new;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = value;
        printf("func check: head= %u, foot= %u\n", list->head->resource, list->foot->resource);
	} else {
		list->foot->next = value;
		list->foot = value;
        printf("func check: head= %u, foot= %u\n", list->head->resource, list->foot->resource);
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
		printf("traverse= %u\n", new->file);
		new = new->next;
	}
    return count;
}
int traverse_count_process_waitlist(list_process_t *list){
	process_t *new;
    int count = 0;
	new =list->head; //head is a node pointer
	while(new != NULL){
        count += 1;
		printf("traverse= %u\n", new->file);
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
		printf("traverse= %u, ", new->resource);
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
void read_and_process(FILE* file, char* tmp, list_resource_t* lr, list_process_t* lp, resource_t* r, process_t* p){
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

                                printf("resource= %u\n", r[resourcenum -1].resource);
                                printf("resource1= %u, ", r[resourcenum -2].resource);
                                printf("resource2= %u\n", r[resourcenum -1].resource);

                                //printf("resource= %lu\n", r[resourcenum -1].resource);
                                //printf("resource= %d\n", r[resourcenum -1].resource);
                                count = 0;

                                //associating files to process
                                p[filenum -1].lock1 = &(r[resourcenum - 2]);
                                p[filenum -1].lock2 = &(r[resourcenum - 1]);
                                r[resourcenum -2].heldBy = &(p[filenum -1] );

                                printf("process=%u, res1=%u\n", (p[filenum-1].file), (*(&(r[resourcenum - 2]))).resource );
                                printf("process=%u, res1=%u, res2=%u\n",
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
                            printf("file= %u\n", p[filenum - 1].file); //file refers to process
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
                            printf("resource= %u\n", r[resourcenum -1].resource);
                            count = 0;
                        }
                    }
    }
}

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
        printf("%u, ", process->file);
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
                        printf("STARTING NEW LOOP: LOOP NUM=%d , PROCES=%u \n\n", loop_number, p->file);


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
                            printf("CURRENT SMALLEST PROCESS IS %u\n\n", smallest_pid);
                        }

                        if( !(visited_process_check(process_stack,  *process)) ){
                            push_process(process_stack, *process, loop_number); //when we push to the stack, we should also pass the loop number so that we can compare
                            printf("PUSHED PROCESS =%u\n", process->file);
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
                            printf("NEW PROCESS FOR NEW LOOP IS %u AND NEW SMALLEST PROCESS IS %u\n\n", p->file, smallest_process->file);
                            //deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process, terminate_process_list, loop_number);
                            deadlock_detector(p, smallest_process, process_stack, counter, smallest_pid, process, terminate_process_list, loop_number);
                            return;
                        }   
                    }

                    else{
                        resource_t* r = (resource_t*) process_or_resource;
                        printf("GOT INTO RESOURCE NUM=%u\n", r->resource);
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
                            printf("NEW PROCESS FOR NEW LOOP IS %u AND NEW SMALLEST PROCESS IS %u\n\n", p->file, smallest_process->file);
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
    int new_loop =1; //
    while(terminated_processes != total_processes){
        process = list->head;
        while(process != NULL){
            printf("PROCESS=%u\n", process->file);
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
                printf("time=%d process=%u, resource1=%u, resource2=%u\n", execution_time, process->file, process->lock1->resource, process->lock2->resource);

                process = process->next;
                //printf("CURRENT PROCESS IS:%u\n", process->file);

                if(process == NULL){ //means came at end of d process list
                    execution_time += 1;
                    break;
                }
                else{
                    printf("CURRENT PROCESS IS:%u\n", process->file);
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
            printf("GOTTT INNN....process=%u\n", process->file);
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

                printf("NEXT 4m INSIDE IS....process=%u\n", process->file);
                continue;
            }
            else{
                process = process->next;
                if(process != NULL){
                    printf("NEXT IS....process=%u\n", process->file);
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