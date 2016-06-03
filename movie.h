#ifndef _MOVIE_H_
#define _MOVIE_H_

#include "rbt.h"
#include <string.h>
#define TOTAL_NUM_OF_MOVIE 5	//�� ��ȭ�� ��


//��ȭ�� ����ü
typedef struct _movie
{
	char* name;
	RBT seats[6][3];			//������ ��ȭ�� ���� �¼� ������ ��� �ִ�. �� �ȿ� �� �ִ� ������ key�� ����� �¼���ȣ�� ��Ÿ����.
	int sched[6][3];			//[x][y] �� �Ͽ��� ��, x�� ���� �� ��ȭ �� y��° ��ȭ�� ���۽ð��� ��� �ִ�. (���ÿ� �����ϴ� ������ ����)
	int num_of_seats[6][3];		//��ȭ������ �ڸ����� 
} Movie;

//movie ����ü �ʱ�ȭ �Լ�
void init_movie(Movie* m);




/*
* ��ȭ�¼� display �Լ�
* �ش� ��ȭ�� ����ü �ּ�, ��¥, �� ���� �� ��ȭ�� ���ؼ� ���° ��ȭ������ ��Ÿ���� ���� �� ���� �ѱ��.
* �׸��� �� ��ȭ�� �¼� ��Ȳ�� �ð������� �����ش�.
*/
void display_status_of_seats(Movie* m, int date, int order);

//movie ����ü ���� RB_tree �� ���� free �Լ�
void movie_free(Movie* m);

//�����ϰ� ������ �� ��ȭ�� ������ ����ϴ� �Լ�, ���ο��� movie_free() �Լ��� ȣ���Ѵ�.
void all_free(Movie* mpp);

/*
* �����¼� ���� �κ�
* ���� ���:
* ��ȭ�̸�, �󿵽ð�, �ְ� ������(6��ġ�� ��ȭ�̸�, �󿵽ð��� �����ϸ� ����)
* ��ȭ�� �� �ڸ� ����(100~300������ ��)
* �̹� ���ִ� �ڸ�(RB_tree �� ����, 70% �̻�)
*/
Movie* random_generate();

#endif