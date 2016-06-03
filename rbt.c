#include "rbt.h"

//designated initializer
const node nil_node = {
	.key = 0,
	.color = COLOR_BLACK,
	.left = NULL,
	.right = NULL,
	.parent = NULL
};



//RBT를 사용할 때, root 를 null 로 초기화 하고, 공통으로 사용할 nil을 넣어주는 함수
void rbtInit(RBT* rbt)
{
	rbt->root = &nil_node;	//Todo: 나중에 root 넣을 때, root 의 부모에 &nil_node 넣어줘야함
	rbt->nil = &nil_node;
	rbt->numOfNodes = 0;
}



//찾으려는 key가 존재하는지 확인, 존재한다면 해당노드 반환, 존재하지않는다면 NULL 반환
//key: 찾으려는 key값, curKey: 현재 노드의 key값
node* isKeyExist(node* nd, int key) {
	if (nd == &nil_node) return NULL;

	int curKey = nd->key;
	if (curKey == key) return nd;
	else if (curKey > key)
		return isKeyExist(nd->left, key);
	else
		return isKeyExist(nd->right, key);
}


//노드 할당받는 함수
node* getNode(int key) {
	node* temp = (node*)malloc(sizeof(node));
	temp->key = key;
	temp->color = COLOR_BLACK;
	temp->left = &nil_node;
	temp->right = &nil_node;
	temp->parent = &nil_node;
	return temp;
}

//y가 x의 오른쪽 자식일 경우로 시작하여 left rotate 진행
void leftRotate(RBT* rbt, node* x) {
	node* y = x->right;

	x->right = y->left;
	if (y->left != rbt->nil) y->left->parent = x;
	y->parent = x->parent;

	if (x->parent == rbt->nil) rbt->root = y;
	else if (x->parent->left == x) x->parent->left = y;
	else x->parent->right = y;

	y->left = x;
	x->parent = y;
}

//y가 x의 왼쪽 자식일 경우로 시작하여 right rotate 진행
void rightRotate(RBT* rbt, node* x) {
	node* y = x->left;

	x->left = y->right;
	if (y->right != rbt->nil) y->right->parent = x;
	y->parent = x->parent;

	if (x->parent == rbt->nil) rbt->root = y;
	else if (x->parent->right == x) x->parent->right = y;
	else x->parent->left = y;

	y->right = x;
	x->parent = y;
}

//insert 함수에서 z노드는 red로 칠해진 상태로 넘어온다. 
//이 때, z->parent->color 가 또 red 라면, RBT의 4번 조건 위반
//위반하는 경우 계속 반복 ==> while(z->parent->color == COLOR_RED)
void RB_insert_fixUp(RBT* T, node* z) {
	node* y;

	while (z->parent->color == COLOR_RED)
	{
		if (z->parent == z->parent->parent->left)
		{
			y = z->parent->parent->right;
			//case 1
			if (y->color == COLOR_RED)
			{
				z->parent->color = COLOR_BLACK;
				y->color = COLOR_BLACK;
				z->parent->parent->color = COLOR_RED;
				z = z->parent->parent;
			}
			else
			{
				//case 2
				if (z == z->parent->right)
				{
					z = z->parent;
					leftRotate(T, z);
				}
				//case 3
				z->parent->color = COLOR_BLACK;
				z->parent->parent->color = COLOR_RED;
				rightRotate(T, z->parent->parent);
			}
		}
		//same as then clause with "right" and "left" exchanged.
		else
		{
			y = z->parent->parent->left;
			//case 1
			if (y->color == COLOR_RED)
			{
				z->parent->color = COLOR_BLACK;
				y->color = COLOR_BLACK;
				z->parent->parent->color = COLOR_RED;
				z = z->parent->parent;
			}
			else
			{
				//case 2
				if (z == z->parent->left)
				{
					z = z->parent;
					rightRotate(T, z);
				}
				//case 3
				z->parent->color = COLOR_BLACK;
				z->parent->parent->color = COLOR_RED;
				leftRotate(T, z->parent->parent);
			}
		}
	}
	T->root->color = COLOR_BLACK;
}

void RB_insert(RBT* T, node* z) {
	T->numOfNodes++;
	node *y = T->nil;
	node *x = T->root;

	while (x != T->nil)
	{
		y = x;
		//BST와 비슷, 현재 위치의 key보다 찾으려는 key가 작다면 왼쪽자식으로, 크다면 오른쪽 자식으로 이동
		if (z->key < x->key)
			x = x->left;
		else
			x = x->right;
	}
	z->parent = y;
	if (y == T->nil)
		T->root = z;
	else if (z->key < y->key)
		y->left = z;
	else
		y->right = z;
	z->left = T->nil;
	z->right = T->nil;
	z->color = COLOR_RED;	//삽입하는 노드의 색깔을 RED로 칠한 뒤에 insertFixUp( ) 함수를 호출한다.
	RB_insert_fixUp(T, z);
}


void RB_delete_fixup(RBT* T, node* x)
{
	node* w;
	while (x != T->root && x->color == COLOR_BLACK)
	{
		if (x == x->parent->left)
		{
			w = x->parent->right;
			//case 1
			if (w->color == COLOR_RED)
			{
				w->color = COLOR_BLACK;
				x->parent->color = COLOR_RED;
				leftRotate(T, x->parent);
				w = x->parent->right;
			}
			//case 2
			if (w->left->color == COLOR_BLACK && w->right->color == COLOR_BLACK)
			{
				w->color = COLOR_RED;
				x = x->parent;
			}
			else
			{
				//case 3
				if (w->right->color == COLOR_BLACK)
				{
					w->left->color = COLOR_BLACK;
					w->color = COLOR_RED;
					rightRotate(T, w);
					w = x->parent->right;
				}
				//case 4
				w->color = x->parent->color;
				x->parent->color = COLOR_BLACK;
				w->right->color = COLOR_BLACK;
				leftRotate(T, x->parent);
				x = T->root;
			}


		}

		//same as then clause with "right" and "left" exchanged.
		else
		{
			w = x->parent->left;
			//case 1
			if (w->color == COLOR_RED)
			{
				w->color = COLOR_BLACK;
				x->parent->color = COLOR_RED;
				rightRotate(T, x->parent);
				w = x->parent->left;
			}
			//case 2
			if (w->right->color == COLOR_BLACK && w->left->color == COLOR_BLACK)
			{
				w->color = COLOR_RED;
				x = x->parent;
			}
			else
			{
				//case 3
				if (w->left->color == COLOR_BLACK)
				{
					w->right->color = COLOR_BLACK;
					w->color = COLOR_RED;
					leftRotate(T, w);
					w = x->parent->left;
				}
				//case 4
				w->color = x->parent->color;
				x->parent->color = COLOR_BLACK;
				w->left->color = COLOR_BLACK;
				rightRotate(T, x->parent);
				x = T->root;
			}


		}
	}
	x->color = COLOR_BLACK;
}


node* minimum(node* x) {
	node* temp = x;
	while (temp->left != &nil_node) temp = temp->left;
	return temp;
}


//RB_transplant ( ) 함수는 v.left 나 v.right 를 update 하지 않는다.
//그 부분은 이 함수의 호출자가 책임진다.
//현재 u와 u의 부모와의 관계를 v와의 관계로 바꾼다.
//v의 부모가 u의 부모가 되고, u의 부모의 자식은 더이상 u가 아니라 v가 된다.
//v는 nil_node 일 수 있다. 
void RB_transplant(RBT* T, node* u, node* v)
{
	if (u->parent == T->nil)
		T->root = v;
	else if (u == u->parent->left)
		u->parent->left = v;
	else
		u->parent->right = v;
	if(v != T->nil)
		v->parent = u->parent;
}

/*
 */
void RB_delete(RBT* T, node* z)
{
	T->numOfNodes--;
	node* x;
	node* y = z;
	int y_original_color = y->color;
	if (z->left == T->nil)
	{
		x = z->right;
		RB_transplant(T, z, z->right);
	}
	else if (z->right == T->nil)
	{
		x = z->left;
		RB_transplant(T, z, z->left);
	}
	else
	{
		//이 부분에서 z는 삭제할 노드를, y는 삭제되고 그 자리를 채울 노드를 나타낸다.

		//z->left 와 z->right 가 T->nil 이 아니므로, 자식은 반드시 존재한다. 따라서, minimum 함수를 사용할 수 있다.
		//successor 를 사용할 필요가 없다. 자식이 있으므로, 부모를 찾아갈 필요가 없다.
		y = minimum(z->right);
		y_original_color = y->color;
		x = y->right;
		//없애려는 노드(==z)의 successor(==y) 가 바로 오른쪽 자식인 경우
		//(x는 nil_node 이면 안된다.)
		if (x!=T->nil && y->parent == z)
			x->parent = y;
		//그렇지 않은 경우
		else
		{
			RB_transplant(T, y, y->right);
			y->right = z->right;		//z자리로 y를 채우기 위해, y의 오른쪽 자식을 z의 오른쪽 자식을 가리키게 한다.
			y->right->parent = y;	//바뀐 y의 오른쪽 자식의 부모를 y를 가리키게 한다.(아직 z를 가리키고 있으므로)
		}
		//지금까지 과정으로 삭제할 노드의 오른쪽 자식트리를 이용하여 z자리를 채워 줬고, 새로 채운 노드와 삭제되는 노드의 왼쪽 자식트리를 이어주는 과정을 진행한다.
		RB_transplant(T, z, y);		//z 자리에 y를 채워주기 위해서 z의 부모와 z의 관계를 y와의 관계로 바꿔준다.
		y->left = z->left;			//y의 왼쪽 자식을 z의 왼쪽 자식으로 채운다.
		y->left->parent = y;			//y->left 가 nil_node 일 수는 없다. 앞의 if 조건문에서 처리가 되었기 때문이다. 따라서, y->left->parent 접근은 valid하다.
		y->color = z->color;			//색깔을 일치시킨다.
	}
	if (y_original_color == COLOR_BLACK)	//if 조건문에 따라 y가 z노드를 가리킬 수도 있고, 새로 채워질 노드를 가리킬 수도 있다. 경우에 따른 y에 대하여 그 색깔을 나타낸다.
		RB_delete_fixup(T, x);
	free(z);	//제거된 노드의 메모리 해제
}


//할당해준 모든 노드를 반환하기 위한 함수
void RB_free_all_nodes(node* nd)
{
	if (nd == &nil_node) return;
	//post-order traversal
	RB_free_all_nodes(nd->left);
	RB_free_all_nodes(nd->right);
	free(nd);
}