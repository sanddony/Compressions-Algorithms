#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define byte unsigned char

typedef struct node node;
struct node {
    node* left_leaf;
    node* right_leaf;
    int is_root;
    byte symb;
    byte code;
    int code_len;  
    size_t weight;
};

int main(){
	tc_move_cursor(128,0);
	printf("asd");
	tc_move_cursor(128,0);
	printf("s");
	tc_move_cursor(128,0);
	printf("d");
	tc_move_cursor(128,0);
	printf("0");
}

