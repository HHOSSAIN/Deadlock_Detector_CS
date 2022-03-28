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
void push_process(stack_process_t* stack, process_t item);
int visited_process_check(stack_process_t* stack, process_t process);

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


                            

                            //list_process->foot->lock2->heldBy = list_process->foot;
                            //printf("TEST FOR TASK 2: PROCESS ER LOCK2 ER RESOURCE HELD BY: ");
                            //if(list_process->foot->lock2->heldBy)

                            //r[resourcenum -1].heldBy = NULL;

                            //r[resourcenum++].resource= strtoul(tmp, &ptr, 10);
                            //r[resourcenum++].resource= strtoul(tmp, NULL, 0);

                            //r[resourcenum++].resource= atoi(tmp);

                            /*printf("resource= %u\n", r[resourcenum -1].resource);
                            printf("resource1= %u, ", r[resourcenum -2].resource);
                            printf("resource2= %u\n", r[resourcenum -1].resource); */

                            //printf("resource= %lu\n", r[resourcenum -1].resource);
                            //printf("resource= %d\n", r[resourcenum -1].resource);
                            count = 0;

                            //associating files to process and vice versa
                            //p[filenum -1].lock1 = &(r[resourcenum - 2]); //transferred to test
                            //p[filenum -1].lock2 = &(r[resourcenum - 1]); //task 2...transferred to test
                            //r[resourcenum -2].heldBy = &(p[filenum -1] ); //task 2

                            /* printf("process=%u, res1=%u\n", (p[filenum-1].file), (*(&(r[resourcenum - 2]))).resource ); //transferred to test
                            printf("process=%u, res1=%u, res2=%u\n",
                                 (p[filenum-1].file),  (*(p[filenum-1].lock1)).resource, (*(p[filenum-1].lock2)).resource ); //transferred to test  */
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

                        /*p[filenum++].file = strtoll(tmp, 0L, 10); //typecasting issue
                        p[filenum - 1].next = NULL;
                        //p[filenum++].file = strtoul(tmp, 0L, 10); //typecasting issue
                        //p[filenum++].file = atoi(tmp); //typecasting issue
                        //printf("file= %d\n", p[filenum - 1].file); //file refers to process
                        printf("file= %u\n", p[filenum - 1].file); //file refers to process
                        list_process = insert_at_foot_process(list_process, &(p[filenum - 1]) );
                        count = 0; */

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


                        //r[resourcenum - 1].next = NULL;

                        //r[resourcenum -1].heldBy = &(p[filenum - 1]);
                        //printf("resource= %u, process holding the resource= %u\n", r[resourcenum-1].resource, r[resourcenum-1].heldBy->file);

                        //list_resource = insert_at_foot_resource(list_resource, &(r[resourcenum - 1]) );
                        //r[resourcenum++].resource= atoi(tmp);
                        //printf("resource= %u\n", r[resourcenum -1].resource);

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

        /*task2,3- calculating execution time and detecting deadlock*/
        //int node_type_marker = 0; //prolly better to make it unsigned
        int waitlist_activator = 0;

        //need to make a stack?


        //iteration/traversal to start from a process
        process_t* process = list_process ->head; 
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


        //TASK-3
        /*if(node_type_marker==0 || ((node_type_marker + 1)%3 == 0) ){ //means it is process_t node
        //if process, then go to process->lock2. if resource, go to heldby
        //process and resource will keep alternating between them, so we can have a counter such that even val hoile process
        //bujhe, so looks for "lock2", else looks for "heldBy"        
        }*/
        //node_type_marker
        printf("node type marker=%d\n", node_type_marker);
        process = list_process ->head; 
        unsigned long long int smallest_process_id = 0;
        unsigned long long int counter = 0; //for checking if it's process or resource
        process_t* task3_process = list_process->head;

        int check = 0;
        stack_process_t* visited_process_stack = makeStack();
        assert(visited_process_stack);

        //a stack or list to keep track of the nodes
        //while (process != NULL){
            /*if(process->file < smallest_process_id || (node_type_marker == 0) ){
                smallest_process_id = process->file;
            }*/


            //1)account for storing the deadlock's process id
            //2)ensure id stored is the smallest id
            //3)
            /*
            void deadlocks(process_t* p, stack_process_t* process_stack, resource_stack, counter, void* process_or_resource){
                void* node_type;
                if ( (counter % 2)==0 ){
                    if(process_or_resource == NULL){
                        return;
                    }
                    (process_t*) process = (process_t*) process_or_resource_stack;
                    if(process not in process_stack){
                        push it;
                    }
                    if(process in process_stack){
                        return deadlock;
                    }
                    counter += 1;
                    deadlocks(process_list, process_stack, resource_stack, counter, void* process_or_resource->lock2);
                }
                else{  //i.e. counter value is odd...(counter+1)%2 == 0
                    (resource_t*) r = (resource_t*) process_or_resource;
                    process_t* process = process_or_resource->heldBy;
                    if(process == NULL){
                        process->next;
                        counter += 1;
                        deadlocks(process_list, process_stack, counter, process_resource)
                    }
                    else{
                        counter += 1;
                        deadlocks(process_list, process_stack, resource_stack, counter, void* process_or_resource);
                    }
                }

                    push (process_t*) node_type->lock2 to resource stack
                    counter += 1;
                    deadlock(process_list, process_stack,  )
               }

                if (process->next == NULL){
                    break or return;
                }
            }
            */


           // push_process(visited_process_stack, *process);
            //if( !(visited_process_check(visited_process_stack, *(process->lock2->heldBy))) ){
             //   push_process(visited_process_stack, *(process->lock2->heldBy) ); //amon akta recursion korte hobe such that it takes the last process
                                                                                //inserted in the stack and oitar respect a cycle pawar try kore
                                                                               //oi check shesh hoile we move next process in our linked list and see
                                                                              //if it's in the stack. if it's in, we skip and move to next. 
                                                                            //if it's not, then we repeat the recursion
            //}
            //if(process->lock2->heldBy)
            

            //if(visited_process_check(visited_process_stack, *process)){
            //else{
              //  printf("DEADLOCK FOUND HHEHEEHEHEH");
            //}

            //process = process->next;
        //} //end of while loop

        //unsigned long long int* a = (unsigned long long int*) malloc(sizeof(unsigned long long int)* (pow(2,(32)) - 1)  );
        //unsigned int* a = (unsigned int*) malloc(sizeof(unsigned int) * (pow(2,(32)) - 1)  );
        //assert(a);
        

        //need a list or process to hold the processes and resources

        //process->lock2

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

void push_process(stack_process_t* stack, process_t item){
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
	(stack->stack)[stack->used] = item; //stac->stack is an int arr.So stack->used is the pos of the arr
	(stack->used)++;
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




//task3
/*void deadlocks(process_t* p, stack_process_t* process_stack, resource_stack, counter, void* process_or_resource){
                void* node_type;
                if ( (counter % 2)==0 ){
                    if(process_or_resource == NULL){
                        return;
                    }
                    (process_t*) process = (process_t*) process_or_resource_stack;
                    if(process not in process_stack){
                        push it;
                    }
                    if(process in process_stack){
                        return deadlock;
                    }
                    counter += 1;
                    deadlocks(process_list, process_stack, resource_stack, counter, void* process_or_resource->lock2);
                }
                else{  //i.e. counter value is odd...(counter+1)%2 == 0
                    (resource_t*) r = (resource_t*) process_or_resource;
                    process_t* process = process_or_resource->heldBy;
                    if(process == NULL){
                        process->next;
                        counter += 1;
                        deadlocks(process_list, process_stack, counter, process_resource)
                    }
                    else{
                        counter += 1;
                        deadlocks(process_list, process_stack, resource_stack, counter, void* process_or_resource);
                    }
                }

                    push (process_t*) node_type->lock2 to resource stack
                    counter += 1;
                    deadlock(process_list, process_stack,  )
} */

                //if (process->next == NULL){
                  //  break or return;
                //}
            //}
