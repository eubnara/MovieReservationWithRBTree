#ifndef _RBT_H_
#define _RBT_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define COLOR_RED 0
#define COLOR_BLACK 1
#define TRUE 1
#define FALSE 0

typedef struct _node node;
typedef struct _node {
	unsigned key; //key가 0 이면 NULL이라고 가정, RBT는 일반적인 정의이므로, position 보다는 key 라고 이름을 명명
	int color;
	node  *left, *right;
	node  *parent;
}node;


//RB tree
typedef struct _rbtree {
	node* root;
	node* nil;	//rbt.c 에서 생성될 nil_node가 들어갈 포인터, 이것으로 red black tree 의 Nil node를 한 개만 생성하여 이용할 수 있다.
	int numOfNodes;
} RBT;



void rbtInit(RBT* rbt);
node* isKeyExist(node* nd, int key);
node* getNode(int key);
void leftRotate(RBT* rbt, node* x);
void rightRotate(RBT* rbt, node* x);
void RB_insert(RBT* T, node* z);
void RB_insert_fixUp(RBT* T, node* z);
node* minimum(node* x);
void RB_transplant(RBT* T, node* u, node* v);
void RB_delete(RBT* T, node* z);	
void RB_delete_fixup(RBT* T, node* x);
void RB_free_all_nodes(node* nd);	//이 함수를 사용할 때에는 모든 노드를 순회하며 삭제할 수 있도록 T->root 를 인자로 넣어준다.


#endif