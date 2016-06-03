#include "movie.h"

//���� ������ ��ȭ �̸�
char* movie_name_cand[10] = {
	"���ũŻ��", "���", "��ũ����Ʈ", "���鷯����Ʈ", "�����ȼ�",
	"����������", "����ƮŬ��", "��Ÿ����", "������Ʈ����", "�μ���"
};

int isRunning[10];	//random_generate() �Լ����� �Ȱ��� �̸��� ��ȭ�� �ߺ��ؼ� �������� �ʱ� ���� ���Ǵ� ����

//13�� �Ѿ�� 12�� ���� '����'�� �ٿ��༭ ���
//�ð� �ߺ�ó���� ���� flag ������ ���������ϴ� �Լ� ������ ����
int time_cand[15] = {
	8, 9, 10, 11, 12,
	13, 14, 15, 16, 17,
	18, 19, 20, 21, 22
};

//Movie ����ü�� ����ϱ� ���� �ʱ�ȭ�� ����
void init_movie(Movie* m)
{
	m->name = NULL;
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 3; j++)
		{
			rbtInit(&m->seats[i][j]);
			m->sched[i][j] = 0;
			m->num_of_seats[i][j] = 0;
		}
}

void display_status_of_seats(Movie* m, int date, int order)
{
	printf("�� �ڸ�: O, ����� �ڸ�: X\n");
	int seats = m->num_of_seats[date][order];
	printf("   ");
	for (int i = 0; i*i < seats; i++)
		printf("%2d ", i + 1);

	printf("\n%2d ", 1);
	//RB_tree �� �̿��Ͽ� num_of_seats ��ŭ �ݺ��Ͽ� �ش� �ڸ��� RB_tree�ȿ� ������ X ������ O ���
	for (int i = 1, j = 1, count = 1; i <= seats; i++, j++)
	{
		//TODO: �� �ڸ����� RB_tree üũ ��, O, X �߿� �ϳ��� print
		printf("%2c ", isKeyExist(m->seats[date][order].root, i) ? 'X' : 'O');
		//������ ���μ��� ���̸� ������ ���� �κ�
		//�ڸ��� 100~300�ڸ��� ���� �� �����Ƿ� �������� ���ڷ� �� �پ� �����ǵ��ϸ� �ȵȴ�.
		//i !=seats ������ ���� �ش� ��ȭ�� �¼��� �� ���� ��Ȯ�� � ���� �����̶��(n^2), n+1���� ������� �߰��Ǵ� ���װ� �����.
		//����, i==seats �� ��쿡�� printf("\n%2 ", count); �� ������� �ʵ��� �Ѵ�.
		if (j*j >= seats && i !=seats)	
		{
			count++;
			printf("\n%2d ", count);	//���η� ��������ϱ� ���� �κ�.
			j = 0;
		}
	}
}

void movie_free(Movie* m)
{
	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 3; j++)
			RB_free_all_nodes(m->seats[i][j].root);
}

void all_free(Movie* mpp)
{
	for (int i = 0; i < TOTAL_NUM_OF_MOVIE; i++)
		movie_free(&mpp[i]);
	free(mpp);
}


//TODO: ���߿� main ���� �� �Լ� ��ȯ�� ������ ��ȯ�ʿ�
Movie* random_generate()
{
	//��ȭ�� ������ �Ŀ�, isRunning[] ������ ������Ʈ�Ͽ� �̹� ���õ� ��ȭ�� �ߺ��������� �ʵ��� �Ѵ�.
	srand(time(NULL));
	Movie* gen = (Movie*)malloc(sizeof(Movie) * 5);
	for (int i = 0; i < 5; i++)
		init_movie(&gen[i]);	//Movie ����ü ��� �� �ʱ�ȭ
	//��ȭ�̸� ���ϱ�
	for (int i = 0; i < 5; i++)
	{
		int idx;
		while (1)
		{
			idx = rand() % 10;
			if (isRunning[idx] == FALSE)
			{
				isRunning[idx] = TRUE;
				gen[i].name = movie_name_cand[idx];
				break;
			}
		}
	}

	//��ȭ�̸��� �ߺ����� �ʵ��� ����ϱ� ���� ����
	//�Ϸ� ���̿����� �ߺ����� �ʰ� ����
	//�ְ� ������ �������� �ߺ��Ǿ ��(���� ��ȭ�������� ���� ��ȭ�� �ְ��� ���� �� ���ϱ⵵ �ϹǷ�)
	int is_movie_selected[10] = { 0, };
	//�ð��밡 �ߺ����� �ʵ��� ���(���, ��ȭ �󿵽��� ���� �����, �ð��� �ߺ��Ǿ ����� ������ �׳� �ߺ����� �ʰ� �Ѵٰ� ����)
	int is_time_selected[15] = { 0, };

	//������ ���ϱ�
	//i: 6��ġ  j: 5���� ��ȭ k: �Ϸ��� 3�� ������  --> 5���� ��ȭ�� �Ϸ翡 �������� ��� �Ȱ�ġ�� ��.
	for (int i = 0; i < 6; i++)
	{
		memset(is_time_selected, 0, 15 * sizeof(int));	//6�� ��, �ð��밡 �Ϸ� �߿����� �Ȱ�ġ�� �ǹǷ�, 6��ġ�� ����� �ش� loop �� ó�� �κп� �ʱ�ȭ�� �����Ѵ�.
		for (int j = 0; j < 5; j++)
			for (int k = 0; k < 3; k++)
			{
				int idx;
				while (1)
				{
					idx = rand() % 15;
					if (is_time_selected[idx] == FALSE)
					{
						is_time_selected[idx] = TRUE;
						gen[j].sched[i][k] = time_cand[idx];
						break;
					}
				}
			}
	}

	//�� ��ȭ�� �¼� �� �����ϱ�
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 6; j++)
			for (int k = 0; k < 3; k++)
				gen[i].num_of_seats[j][k] = rand() % 201 + 100;	//100~300 ������ ����

	//�� ��ȭ�� �¼� ���� 70%��ŭ RBT�� �����ϱ�
	int isDuplicate[301];
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				memset(isDuplicate, 0, 301 * sizeof(int));
				int numOfSeats = gen[i].num_of_seats[j][k];
				for (int l = 0; l < numOfSeats * 7 / 10; l++)	//70% ����
				{
					int idx;
					while (1)
					{
						idx = rand() % numOfSeats + 1; //numOfSeats �� 100�ڸ����, 1~100��° ������ ���ڰ� �������� ������
						if (isDuplicate[idx] == FALSE)	//���� �ߺ��� ���ڰ� �ȳ��´ٸ�, isDuplicate[] �� update�� �Ŀ�, loop�� �������� �ش� �ڸ��� ���� ��带 red black tree�� ������ �ش�.
						{
							isDuplicate[idx] = TRUE;
							break;
						}
					}
					node* ins = getNode(idx);
					RB_insert(&gen[i].seats[j][k], ins);
				}
			}
		}
	}

	//5���� Movie ����ü�� �Ҵ��ϰ�, ù��° Movie�� ����Ű�� pointer�� ��ȯ�Ѵ�.
	return gen;
}