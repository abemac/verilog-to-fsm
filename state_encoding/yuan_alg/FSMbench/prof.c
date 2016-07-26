//
//  prof.c
//  FaultFSM 
//
//  Created by Zhijia Zhao on May 26, 2015.
//  Copyright (c) 2015 Zhijia Zhao. All rights reserved.
//

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

/* input params */
int   state_num;
int   condition_num;
int   start_state;
 
/* runtime data */
int **dfa;      // dfa table    
char* input;    // input array
int len;        // input array length

/* to-profile data */	
long* w_node;   // node weights
long** w_edge;  // edge weights

int token(char);

/* 
 * read dfa table from a file 
 */
void read_dfa_table(char* fname)
{
    int i, j;
    char buff_in[100];
    FILE* fp;
    
    fp = fopen(fname, "r");
    
    if(fp == NULL)
        fprintf(stderr, "Unable to read DFA table file %s\n", fname);
	else
        fprintf(stderr, "Reading DFA table file %s\n", fname);
    
    /* dynamiclly allocate memory for DFA table */
    dfa = malloc(sizeof(int *) * state_num);

    for(i = 0; i < state_num; i++)
        dfa[i] = malloc( sizeof(int) * condition_num);
    
    for(i = 0; i < state_num; i++)
        for(j = 0; j < condition_num; j++)
        {
            fscanf(fp, "%s", buff_in);
            dfa[i][j] = atoi(buff_in);
        }
}

/*
 * read input to memory
 */
void load_input(char* fname) {

	FILE* fp;
    fp = fopen(fname, "r");

    /* get file size */
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

	input = (char*)malloc(sizeof(char) * len);
	fread(input, 1, len, fp);
}

/* 
 * lookup DFA table and return next state.
 */
int lookup_dfa_table(int current_state, int condition)
{
	if( current_state < 0)
	{
		fprintf(stderr, "current_state is %d!\n", current_state);
		return -1;
	}
	else if (condition < 0) {
		fprintf(stderr, "condition is %d!\n", condition);
		return -1;
	}
	else if (current_state > state_num)
	{
		fprintf(stderr, "too large current_state\n");
		return -1;
	}
	else if (condition > condition_num)
	{
		fprintf(stderr, "too large condition!\n");
		return -1;
	}
    
    return dfa[current_state][condition];
}


/* write state/node weight to file 
 */
void write_long(char* file, long* array) {
	FILE* out;
	out = fopen(file, "w");
	long idx;
	for(idx=0; idx < state_num; idx++)
		fprintf(out, "%ld ", array[idx]);
	fprintf(out, "\n");
	fclose(out);
}


/* write transition/edge weight to file 
 */
void write_longlong(char* file, long** array) {
	FILE* out;
	out = fopen(file, "w");
	long i, j;
	for(i=0; i < state_num; i++) {
		for(j=0; j < condition_num; j++)
			fprintf(out, "%ld ", array[i][j]);
		fprintf(out, "\n");
	}
	fclose(out);
}


/*
 * do dfa transitions from state 'start_state' 
 */
void scan()
{
	/* allocate memory for weights */
	int j,k;
	w_node = (long*)malloc(sizeof(long)*state_num);
	for(j=0; j<state_num; j++)
		w_node[j] = 0;

	w_edge = (long**)malloc(sizeof(long*)*state_num);
	for(j = 0; j < state_num; j++)
		w_edge[j] = (long*)malloc(sizeof(long)*condition_num);
	for(j = 0; j < state_num; j++)
		for(k = 0; k < condition_num; k++)
			w_edge[j][k] = 0;

    int current = start_state;
    int next;
    
    int condition;
    
    long i;
    for(i = 0; i < len; i++)
    {
		//printf("%d ", current);
        condition = token(input[i]);
       
		w_edge[current][condition]++;

        next = lookup_dfa_table(current, condition);
        current = next;
        
		w_node[current]++;
    }
}


/* scan the input, collect weights for each edge
 * each node.
 */
int main(int argc, char** argv)
{
	char* dfa_file;
	char* input_file;

    dfa_file = argv[1];
    state_num = atoi(argv[2]);
    condition_num = atoi(argv[3]);
    start_state = atoi(argv[4]);
    input_file = argv[5];
    
    read_dfa_table(dfa_file);
	load_input(input_file);
   
    scan();
	char fnode[100];
	char fedge[100];
	strcpy(fnode, input_file);
	strcpy(fedge, input_file);
	write_long(strcat(fnode, "w_node"), w_node);
	write_longlong(strcat(fedge, "w_edge"), w_edge);

    return 0;

}


