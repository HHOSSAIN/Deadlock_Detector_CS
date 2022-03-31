#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include "structs.h"

#define SIZE 10
#define DEFAULTSIZE 2

//void read_and_process(FILE* file, char* tmp, list_resource_t* lr, list_process_t* lp, resource_t* r, process_t* p);

int main(int argc, char** argv){
    //format1: ./detect -f resources.txt
    //format2: ./detect -e -f resources.txt
    int execution_time = 1; //coz 2nd col er shobai unique, so guaranteed time from that column is 1
    //int current_mutual_maxtime=0; //prolly better to make it unsigned int
    unsigned long long int resource_val;
    int node_type_marker = 0;

    /*format 1*/
    FILE* file; //int lines =0;
    char* tmp = malloc(sizeof(char)*5);
    char* ptr;

    list_process_t* list_process = make_empty_list_process();
    //list_process_t* list_terminating_process = make_empty_list_process();
    list_resource_t* list_resource = make_empty_list_resource();

    process_t* p =(process_t*)  malloc(sizeof(process_t)*SIZE);
    //int p_size = SIZE; //handle realloc
    //int r_size = SIZE;

    resource_t* r =(resource_t*) malloc(sizeof(resource_t)*SIZE);
    resource_t* task2_r;


    assert(tmp);
    assert(p);
    assert(r);

    int arguments = argc; 
    int f_flag=0, e_flag=0, c_flag=0;
    char* filename;
    printf("ARGUMENTS=%d\n\n", arguments);
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
    printf("f_flag=%d, e_flag=%d, c_flag=%d, filename=%s\n", f_flag, e_flag, c_flag, filename);

    //printf("test= %d\n", lines);
    //if((strcmp(argv[1], "-f") == 0)){
    if(f_flag){
        //file = fopen(argv[2], "r");
        file = fopen(filename, "r");
        int c=0, count=0, spaces=0; //filenum=0; //resourcenum=0; //lines=0; //lines=processes
        //int p_size=SIZE, r_size=SIZE;

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
                                printf("\nEXISTING RESOURCE NUM= %llu\n\n", resource_val);
                                printf("RESOURCE= %llu\n", task2_r->resource );                                
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


                                printf("resource1= %llu, ", list_resource->foot->resource);
                                printf("process=%llu, res1=%llu\n", list_process->foot->file, list_resource->foot->resource );

                                list_process->foot->lock1 = list_resource->foot; //lock1 pointing to resource1...after that we add resource2 to the resource list

                                list_resource = insert_at_foot_resource(list_resource, r );
                                printf("resource= %llu\n", r->resource);
                                //printf("resource1= %u, ", r[resourcenum -2].resource);
                                printf("resource2= %llu\n", list_resource->foot->resource);
                                printf("process=%llu, res2=%llu\n", list_process->foot->file, list_resource->foot->resource );

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
                        printf("file= %llu\n", p->file); //file refers to process
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
                                printf("\nEXISTING RESOURCE NUMMM= %llu\n", resource_val);
                                printf("RESOURCE= %llu\n", task2_r->resource );

                                //task2_r->heldBy = list_process->foot; //already heldBy blocked by prev process
                                //task2_r->next = NULL;
                                printf("resource= %llu, process holding the resource= %llu\n", task2_r->resource, list_process->foot->file);
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
                                printf("resource= %llu, process holding the resource= %llu\n", r->resource, list_process->foot->file);
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
                printf("check:: resource= %llu, process holding the resource= %llu\n", ress->resource,  ress->heldBy->file);
            }
            ress = ress->next;
            printf("next ress= %llu\n", ress->resource);
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
        if(e_flag == 0){
            int loop_number = 1;
            printf("node type marker=%d\n", node_type_marker);
            //process_t process = list_process ->head; 
            //unsigned long long int smallest_process_id = 0;
            //unsigned long long int counter = 0; //for checking if it's process or resource
            process_t* task3_process = list_process->head;
            list_process_t* terminate_process_list= make_empty_list_process();

            //int check = 0;
            stack_process_t* visited_process_stack = makeStack();
            assert(visited_process_stack);

            //deadlocks(task3_process, task3_process, visited_process_stack, 0, task3_process->file, task3_process, terminate_process_list);
            deadlock_detector(task3_process, task3_process, visited_process_stack, 0, task3_process->file, task3_process, terminate_process_list, loop_number);
            printf("CHECKING Terminating LIST:\n");

            if(terminate_process_list->head != NULL){
                printf("CHECKING Terminating LIST= %llu\n\n", terminate_process_list->head->file);
                //printf("TERMINATE PROCESS=%u\n", terminate_process_list->head->file);
            }
            print_processes_to_terminate(terminate_process_list);
        }






        //CHALLENGE TASK
        if(c_flag){
            int total_processes = traverse_count_process(list_process);
            int challenge_exec_time = 0;
            challenge_exec_time = challenge_deadlock_avoider(list_process, total_processes);
            printf("SIMULATION TIME IS %d\n\n", challenge_exec_time);
        }





        /*task2,3- calculating execution time and detecting deadlock*/
        //int node_type_marker = 0; //prolly better to make it unsigned

        if(e_flag){
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
                        printf("resource held: %llu\n", process->lock2->resource);
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
                        printf("resource held in else: %llu\n", process->lock2->resource);
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
    if(e_flag){
        printf("execution time= %d\n", execution_time);
    }
    printf("node type marker=%d\n", node_type_marker);
    ///printf("%u\n", )

    fclose(file);

    return 0;
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
}
