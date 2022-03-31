#ifndef STRUCTS_H
#define STRUCTS_H

#define SIZE 10
#define DEFAULTSIZE 2

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


/*FUNCTION DECLARATIONS*/
/*function declarations*/
list_process_t *make_empty_list_process(void);
list_process_t *insert_at_foot_process(list_process_t *list, process_t* value);
int traverse_count_process(list_process_t *list);
int traverse_count_process_waitlist(list_process_t *list); //used for task2(finding execution time)

/*task1, reading and making list*/
//void read_and_process(FILE* file, char* tmp, list_resource_t* lr, list_process_t* lp, resource_t* r, process_t* p)

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

void free_list_process(list_process_t *list);
void free_list_resource(list_resource_t *list);
void free_visited_process_stack(stack_process_t* stack);

#endif