#include "rbt.h"

//designated initializer
const node nil_node = {
	.key = 0,
	.color = COLOR_BLACK,
	.left = NULL,
	.right = NULL,
	.parent = NULL
};



//RBT�� ����� ��, root �� null �� �ʱ�ȭ �ϰ�, �������� ����� nil�� �־��ִ� �Լ�
void rbtInit(RBT* rbt)
{
	rbt->root = &nil_node;	//Todo: ���߿� root ���� ��, root �� �θ� &nil_node �־������
	rbt->nil = &nil_node;
	rbt->numOfNodes = 0;
}



//ã������ key�� �����ϴ��� Ȯ��, �����Ѵٸ� �ش��� ��ȯ, ���������ʴ´ٸ� NULL ��ȯ
//key: ã������ key��, curKey: ���� ����� key��
node* isKeyExist(node* nd, int key) {
	if (nd == &nil_node) return NULL;

	int curKey = nd->key;
	if (curKey == key) return nd;
	else if (curKey > key)
		return isKeyExist(nd->left, key);
	else
		return isKeyExist(nd->right, key);
}


//��� �Ҵ�޴� �Լ�
node* getNode(int key) {
	node* temp = (node*)malloc(sizeof(node));
	temp->key = key;
	temp->color = COLOR_BLACK;
	temp->left = &nil_node;
	temp->right = &nil_node;
	temp->parent = &nil_node;
	return temp;
}

//y�� x�� ������ �ڽ��� ���� �����Ͽ� left rotate ����
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

//y�� x�� ���� �ڽ��� ���� �����Ͽ� right rotate ����
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

//insert �Լ����� z���� red�� ĥ���� ���·� �Ѿ�´�. 
//�� ��, z->parent->color �� �� red ���, RBT�� 4�� ���� ����
//�����ϴ� ��� ��� �ݺ� ==> while(z->parent->color == COLOR_RED)
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
		//BST�� ���, ���� ��ġ�� key���� ã������ key�� �۴ٸ� �����ڽ�����, ũ�ٸ� ������ �ڽ����� �̵�
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
	z->color = COLOR_RED;	//�����ϴ� ����� ������ RED�� ĥ�� �ڿ� insertFixUp( ) �Լ��� ȣ���Ѵ�.
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


//RB_transplant ( ) �Լ��� v.left �� v.right �� update ���� �ʴ´�.
//�� �κ��� �� �Լ��� ȣ���ڰ� å������.
//���� u�� u�� �θ���� ���踦 v���� ����� �ٲ۴�.
//v�� �θ� u�� �θ� �ǰ�, u�� �θ��� �ڽ��� ���̻� u�� �ƴ϶� v�� �ȴ�.
//v�� nil_node �� �� �ִ�. 
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
		//�� �κп��� z�� ������ ��带, y�� �����ǰ� �� �ڸ��� ä�� ��带 ��Ÿ����.

		//z->left �� z->right �� T->nil �� �ƴϹǷ�, �ڽ��� �ݵ�� �����Ѵ�. ����, minimum �Լ��� ����� �� �ִ�.
		//successor �� ����� �ʿ䰡 ����. �ڽ��� �����Ƿ�, �θ� ã�ư� �ʿ䰡 ����.
		y = minimum(z->right);
		y_original_color = y->color;
		x = y->right;
		//���ַ��� ���(==z)�� successor(==y) �� �ٷ� ������ �ڽ��� ���
		//(x�� nil_node �̸� �ȵȴ�.)
		if (x!=T->nil && y->parent == z)
			x->parent = y;
		//�׷��� ���� ���
		else
		{
			RB_transplant(T, y, y->right);
			y->right = z->right;		//z�ڸ��� y�� ä��� ����, y�� ������ �ڽ��� z�� ������ �ڽ��� ����Ű�� �Ѵ�.
			y->right->parent = y;	//�ٲ� y�� ������ �ڽ��� �θ� y�� ����Ű�� �Ѵ�.(���� z�� ����Ű�� �����Ƿ�)
		}
		//���ݱ��� �������� ������ ����� ������ �ڽ�Ʈ���� �̿��Ͽ� z�ڸ��� ä�� ���, ���� ä�� ���� �����Ǵ� ����� ���� �ڽ�Ʈ���� �̾��ִ� ������ �����Ѵ�.
		RB_transplant(T, z, y);		//z �ڸ��� y�� ä���ֱ� ���ؼ� z�� �θ�� z�� ���踦 y���� ����� �ٲ��ش�.
		y->left = z->left;			//y�� ���� �ڽ��� z�� ���� �ڽ����� ä���.
		y->left->parent = y;			//y->left �� nil_node �� ���� ����. ���� if ���ǹ����� ó���� �Ǿ��� �����̴�. ����, y->left->parent ������ valid�ϴ�.
		y->color = z->color;			//������ ��ġ��Ų��.
	}
	if (y_original_color == COLOR_BLACK)	//if ���ǹ��� ���� y�� z��带 ����ų ���� �ְ�, ���� ä���� ��带 ����ų ���� �ִ�. ��쿡 ���� y�� ���Ͽ� �� ������ ��Ÿ����.
		RB_delete_fixup(T, x);
	free(z);	//���ŵ� ����� �޸� ����
}


//�Ҵ����� ��� ��带 ��ȯ�ϱ� ���� �Լ�
void RB_free_all_nodes(node* nd)
{
	if (nd == &nil_node) return;
	//post-order traversal
	RB_free_all_nodes(nd->left);
	RB_free_all_nodes(nd->right);
	free(nd);
}