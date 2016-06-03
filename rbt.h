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
	unsigned key; //key�� 0 �̸� NULL�̶�� ����, RBT�� �Ϲ����� �����̹Ƿ�, position ���ٴ� key ��� �̸��� ���
	int color;
	node  *left, *right;
	node  *parent;
}node;


//RB tree
typedef struct _rbtree {
	node* root;
	node* nil;	//rbt.c ���� ������ nil_node�� �� ������, �̰����� red black tree �� Nil node�� �� ���� �����Ͽ� �̿��� �� �ִ�.
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
void RB_free_all_nodes(node* nd);	//�� �Լ��� ����� ������ ��� ��带 ��ȸ�ϸ� ������ �� �ֵ��� T->root �� ���ڷ� �־��ش�.


#endif