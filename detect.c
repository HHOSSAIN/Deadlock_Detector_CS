#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include <math.h>
#include <stdint.h>
#include <inttypes.h>

#include "structs.h"

#define SIZE 10
#define DEFAULTSIZE 2

//void read_and_process(FILE* file, char* tmp, list_resource_t* lr, list_process_t* lp, resource_t* r, process_t* p);

int main(int argc, char** argv){

    //coz every resource in 2nd column is unique, so guaranteed time from that column is 1
    int execution_time = 1; 
    unsigned long long int resource_val;
    //int node_type_marker = 0;

    /*format 1*/
    FILE* file; //int lines =0;
    char* tmp = malloc(sizeof(char)*5);
    char* ptr;

    list_process_t* list_process = make_empty_list_process();
    //list_process_t* list_terminating_process = make_empty_list_process();
    list_resource_t* list_resource = make_empty_list_resource();

    process_t* p =(process_t*)  malloc(sizeof(process_t)*SIZE);

    resource_t* r =(resource_t*) malloc(sizeof(resource_t)*SIZE);
    resource_t* task2_r;


    assert(tmp);
    assert(p);
    assert(r);

    int arguments = argc; 
    int f_flag=0, e_flag=0, c_flag=0;
    char* filename;
    for(int i=0; i<arguments; i++){
        //f flag er porer tai filename hobe
        if((strcmp(argv[i], "-f") == 0)){
            filename = argv[i+1];
            f_flag = 1;
        }
        else if((strcmp(argv[i], "-e") == 0)){
            e_flag = 1;
        }
        else if((strcmp(argv[i], "-c") == 0)){
            c_flag = 1;
        }
    }

    if(f_flag){
        file = fopen(filename, "r");
        int c=0, count=0, spaces=0;

        /*On Windows, lines in text files are separated by \r\n, not just \n*/
        while ( (c=fgetc(file)) !=EOF ){ //Exits during last line's '\n' , so last line count is not counted
                if(c != ' '){
                    if(c == '\r' || c=='\n'){

                        if(c == '\n'){
                            count = 0;
                            spaces = 0;
                            
                            /*test*/
                            resource_val = strtoll(tmp, &ptr, 10);
                            
                            //traverse resource list and check if this resource already exists or not
                            int testt = traverse_resource_exist(list_resource, resource_val, &task2_r);
                            if(testt == 1){

                                /*PRINT COMMANDS FOR DEBUGGING*/
                                //printf("\nEXISTING RESOURCE NUM= %llu\n\n", resource_val);
                                //printf("RESOURCE= %llu\n", task2_r->resource );                                

                                list_process->foot->lock1 = list_resource->foot; //lock1 pointing to resource1...after that we add resource2 to the resource list
                                list_process->foot->lock2 = task2_r;

                            } 


                            else{
                                resource_t* r = (resource_t*) malloc(sizeof(resource_t)); //can't always malloc as same resource may have been created previously
                                assert(r);
                                
                                r->resource = strtoll(tmp, &ptr, 10);
                                r->next = NULL;

                                /*PRINT COMMANDS FOR DEBUGGING*/
                                //printf("resource1= %llu, ", list_resource->foot->resource);
                                //printf("process=%llu, res1=%llu\n", list_process->foot->file, list_resource->foot->resource );

                                list_process->foot->lock1 = list_resource->foot; //lock1 pointing to resource1...after that we add resource2 to the resource list

                                list_resource = insert_at_foot_resource(list_resource, r );

                                /*PRINT COMMANDS FOR DEBUGGING*/
                                //printf("resource= %llu\n", r->resource);
                                //printf("resource1= %u, ", r[resourcenum -2].resource);
                                //printf("resource2= %llu\n", list_resource->foot->resource);
                                //printf("process=%llu, res2=%llu\n", list_process->foot->file, list_resource->foot->resource );

                                list_process->foot->lock2 = r;
                                //printf("TASK2 CHECKKKK FOR HELDBY")
                            }
                            
                            count = 0;
                        }

                        else{
                            tmp[count] = '\0';
                            
                            /*PRINT COMMANDS FOR DEBUGGING*/    
                            //printf("check=%c, ", c );
                            
                            continue;
                        }
                    }

                    else{
                        tmp[count++] = c;

                        /*PRINT COMMANDS FOR DEBUGGING*/
                        //printf("check=%c %d\n", c, (c == ' ') );
                        //printf("check=%c, ", c );
                    }
                }
                else{
                    //write in file when space=1, resource when space=2
                    spaces += 1; 
                    tmp[count]='\0';

                    /*PRINT COMMANDS FOR DEBUGGING*/
                    //printf("tmp= %s %d, spaces=%d\n", tmp, atoi(tmp), spaces);

                    if( (spaces+1)%2 == 0){ //odd space mane it's a process

                        /*test*/
                        process_t* p = (process_t*) malloc(sizeof(process_t));
                        assert(p);
                        p->file = strtoll(tmp, 0L, 10);
                        p->next = NULL;

                        /*PRINT COMMANDS FOR DEBUGGING*/
                        //printf("file= %llu\n", p->file); //file refers to process

                        list_process = insert_at_foot_process(list_process, p );
                        count = 0;

                    }
                    
                    else{
                        /*test*/
                        resource_val = strtoll(tmp, &ptr, 10);
                        if(list_resource != NULL){
                            if(traverse_resource_exist(list_resource, resource_val, &task2_r) == 1){

                                /*PRINT COMMANDS FOR DEBUGGING*/
                                //printf("\nEXISTING RESOURCE NUMMM= %llu\n", resource_val);
                                //printf("RESOURCE= %llu\n", task2_r->resource );
                                //printf("resource= %llu, process holding the resource= %llu\n", task2_r->resource, list_process->foot->file);
                                
                                task2_r->heldBy = list_process->foot;

                                /*PRINT COMMANDS FOR DEBUGGING*/
                                //printf("\nNOT INSERTED AGAIN IN RESOURCE LIST\n\n");
                                //list_resource = insert_at_foot_resource(list_resource, r ); already in list
                            } 
                            else{
                                resource_t* r = (resource_t*) malloc(sizeof(resource_t));
                                assert(r);
                        
                                r->resource= strtoll(tmp, 0L, 10);
                                r->heldBy = list_process->foot;
                                r->next = NULL;

                                /*PRINT COMMANDS FOR DEBUGGING*/
                                //printf("resource= %llu, process holding the resource= %llu\n", r->resource, list_process->foot->file);

                                list_resource = insert_at_foot_resource(list_resource, r );
                            }
                        }   

                        count = 0;
                    }
                }
        }

        int z1 = traverse_count_process(list_process);
        int z2 = traverse_count_resource(list_resource);

        /*.............TASK-1.................*/
        if(c_flag == 0){
            printf("Processes %d\n", z1);
            printf("Files %d\n", z2);
        }
            
        /*PRINT COMMANDS FOR DEBUGGING*/
        //printf("check:: resource= %u, process holding the resource= %u\n", r[1].resource, r[1].heldBy->file);



        /*...........TASK-3, 4, 5..........*/
        if(e_flag == 0){
            int loop_number = 1;
            
            process_t* task3_process = list_process->head;
            list_process_t* terminate_process_list= make_empty_list_process();

            //int check = 0;
            stack_process_t* visited_process_stack = makeStack();
            assert(visited_process_stack);

            deadlock_detector(task3_process, task3_process, visited_process_stack, 0, task3_process->file, task3_process, terminate_process_list, loop_number);

            if(terminate_process_list->head != NULL){

                /*PRINT COMMANDS FOR DEBUGGING*/
                //printf("CHECKING Terminating LIST= %llu\n\n", terminate_process_list->head->file); //IMPORTANT
                //printf("TERMINATE PROCESS=%u\n", terminate_process_list->head->file);

                if(c_flag == 0){
                    printf("Deadlock detected\n");
                }
            }
            else{
                printf("No deadlocks\n");
            }

            if(c_flag == 0){
                print_processes_to_terminate(terminate_process_list);
            }
        }


        /*.................CHALLENGE TASK.................*/
        if(c_flag){
            int total_processes = traverse_count_process(list_process);
            int challenge_exec_time = 0;
            challenge_exec_time = challenge_deadlock_avoider(list_process, total_processes);
            printf("Simulation time %d\n", challenge_exec_time);
        }


        /*..........TASK-2- calculating execution time.......... */
        if(e_flag){
            int waitlist_activator = 0;

            //iteration/traversal to start from a process
            process_t* process = list_process ->head; 
            
            /*given that there is no deadlock for task 2, it is guaranteed in such case that there is at least 1 
              resource/file in the last column which is not present in the 2nd last column */
            execution_time += 1; 

            //we are bothered to check time from last columnn only as column 2's time can be predetermined
            while(process != NULL){
                
                if(process->count < 1){
                    //printf("got in\n");
                    if(process->lock2->heldBy == NULL){

                        //printf("held by process: %u\n", process->lock2->heldBy->file);
                        //printf("resource held: %llu\n", process->lock2->resource); //IMPORTANT

                        process->lock1 = process->lock2;  

                        //updating the resource's "heldby"          
                        process->lock1->heldBy = process; 
                        process->count += 1;
                        printf("successful3\n"); 
                    }
                    
                    else{
                        
                        if( !(process->lock2->waitlist_alloced) ){ 

                            //when it is 1, it means there's at least 1 resource on which a process is waiting
                            waitlist_activator = 1; 
                            
                            process->lock2->waitlist_tracker = make_empty_list_process();

                            // 1 means list's malloc is done, don't have to do again
                            process->lock2->waitlist_alloced = 1; 
                        }
                        
                        //insert process to the list of processes waiting on a resource
                        process->lock2->waitlist_tracker = insert_at_foot_process_waitlist(process->lock2->waitlist_tracker, process );
                    }
                }
                process = process->next;
            }

            //traverse thorugh the resources to find the longest waitlist
            if(waitlist_activator){
                execution_time += traverse_find_longest_waitlist(list_resource); 
            }
        }
    }

    if(e_flag && c_flag==0){
        printf("Execution time %d\n", execution_time);
    }

    fclose(file);

    return 0;
}


/*FOR USE IN FUTURE FOR task1-> reading and making list TO REDUCE THE NUMBER OF LINES IN main() */
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
                            //printf("check=%c, ", c );
                        }
                    }
                    else{
                        spaces += 1; //write in file when space=1, resource when space=2
                        tmp[count]='\0';

                        //printf("tmp= %s %d, spaces=%d\n", tmp, atoi(tmp), spaces);

                        if( (spaces+1)%2 == 0){ //odd space mane it's a process
                            p[filenum++].file = strtoll(tmp, 0L, 10); //typecasting issue
                            p[filenum - 1].next = NULL;

                            //printf("file= %llu\n", p[filenum - 1].file); //file refers to process

                            lp = insert_at_foot_process(lp, &(p[filenum - 1]) );
                            count = 0;
                        }
                        //break;
                        else{
                            r[resourcenum++].resource= strtoll(tmp, 0L, 10);
                            r[resourcenum - 1].next = NULL;
                            lr = insert_at_foot_resource(lr, &(r[resourcenum - 1]) );
                            //r[resourcenum++].resource= atoi(tmp);
                            //printf("resource= %llu\n", r[resourcenum -1].resource);
                            count = 0;
                        }
                    }
    }
 } */

