//ver 4.0
#define _CRT_SECURE_NO_WARNINGS
#include "movie.h"
#include <conio.h>

void delay(int milliseconds)
{
	long pause;
	clock_t now, then;

	pause = milliseconds*(CLOCKS_PER_SEC / 1000);
	now = then = clock();
	while ((now - then) < pause)
		now = clock();
}

Movie* movies;
int maxHeight;	//tree�� traversal �� ��, �ִ� ���̸� �������ֱ� ���� ��������



void menu();
void showRunningMovies();
void showScheduleOfMovie(Movie m, int date);
void reservation(int movieNum);
void cancelation(int movieNum);
void showLeaves(RBT* T, node* nd, int* height);
void afterInsDel(int movieNum, int date, int order);



int main(void)
{
	system("mode con lines=35 cols=80");

	movies = random_generate();

	menu();
	all_free(movies);



	return 0;
}



//�� ���� ��ȭ ��� ���
void showRunningMovies()
{
	printf("[]***************************************************************************[]\n");
	for (int i = 1; i <= TOTAL_NUM_OF_MOVIE; i++)
		printf("[]%27d.%20s%29s\n", i, movies[i - 1].name, "[]");
	printf("[]***************************************************************************[]\n");
}

//�ش� ��ȭ�� ������ ��� �Լ�
void showScheduleOfMovie(Movie m, int date)
{
	system("cls");
	printf("\n\"%s\"�� %d�� �� �������Դϴ�.\n\n", m.name, date + 1);
	printf("[]***************************************************************************[]\n");
	for (int j = 1; j <= 3; j++)
	{
		int time = m.sched[date][j - 1];
		if (time >= 12)
		{
			//���� 0�� ����, �׳� ���� 12�÷� ó��
			if(time == 12)
				printf("[]%27d.%13s %2d�� ~ %2d��%24s\n", j, "����", time, time - 11, "[]");
			else
				printf("[]%27d.%13s %2d�� ~ %2d��%24s\n", j, "����", time - 12, time - 11, "[]");
		}			
		else
			printf("[]%27d.%13s %2d�� ~ %2d��%24s\n", j, "����", time, time + 1, "[]");
	}
	printf("[]***************************************************************************[]\n");
}
//�ش� ��ȭ �������� �¼�ǥ ���


//����
void reservation(int movieNum)
{
	int date;
	int order;
	int seatRow;	//�¼�ǥ���� ���� �¼� �� ��ȣ
	int seatCol;	//�¼�ǥ���� ���� �¼� �� ��ȣ
	int seatNum;	//������ �¼���ȣ
	int totalRow;
	int totalCol;
	//�ش� ��ȭ�� ������
	while (1)
	{
		system("cls");
		showRunningMovies();
		printf("\n\"%s\"�� �����ϼ̽��ϴ�. ��ĥ �� �����Ͻðڽ��ϱ�?(1~6) \nó������ ���ư��÷��� 0���� ��������.", movies[movieNum].name);
		date = _getch();
		date -= 48;

		if (date < 0 || date > 6)
		{
			printf("\n�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n");
			delay(1000);
			continue;
		}
		if (date == 0)
			return;
		date = date - 1;		//Movie ����ü ����  2���� �迭���� �ε����� 0���� �����ϹǷ� �������ش�.
		break;
	}
	while (1) {
		system("cls");
		showScheduleOfMovie(movies[movieNum], date);
		printf("\n���° ��ȭ�� ���ðڽ��ϱ�?(1~3) ó������ ���ư��÷��� 0���� ��������.");
		order = _getch();
		order -= 48;
		if (order < 0 || order > 3)
		{
			printf("\n�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n");
			delay(1000);
			continue;
		}
		if (order == 0)
			return;
		order = order - 1;	//Movie ����ü ����  2���� �迭���� �ε����� 0���� �����ϹǷ� �������ش�.
		break;
	}


	//seatRow, seatCol �� ã�� ���� ����, �ణ hardcoded ��
	for (int i = 1, seats = movies[movieNum].num_of_seats[date][order]; i <= seats; i++)
	{
		if (i*i == seats)
		{
			totalCol = i;
			totalRow = i;
			break;
		}
		else if (i*i > seats)
		{
			totalCol = i;
			totalRow = i-1;
			while(1)
			{
				if (totalCol * totalRow < seats)
				{
					totalRow += 1;
					break;
				}
				else
					totalRow -= 1;
			}
			break;
		}
	}
	//�ش� �������� �¼�ǥ
	while (1)
	{
		system("cls");
		display_status_of_seats(&movies[movieNum], date, order);
		printf("\n\n");
		//�¼� ��ȣ �Է� �ޱ� (��, �� ���� �¼���ȣ�� ġȯ�ؾ� �Ѵ�.)
		printf("�ش��ϴ� ��ȭ�� �¼�ǥ �Դϴ�. � �ڸ��� �����Ͻðڽ��ϱ�? ó������ ���ư��÷��� 0���� �Է��ϼ���.\n");
		while (1)
		{
			printf("�� ��?(1~%d) : ", totalRow);
			scanf("%d", &seatRow);
			if (seatRow == 0)
				return;
			if (seatRow < 0 || seatRow > totalRow)
			{
				printf("�ùٸ��� ���� �Է��Դϴ�.\n");
				delay(1000);	//1�� ��ٸ���
				system("cls");
				display_status_of_seats(&movies[movieNum], date, order);
				printf("\n\n");
				printf("�ش��ϴ� ��ȭ�� �¼�ǥ �Դϴ�. � �ڸ��� �����Ͻðڽ��ϱ�? ó������ ���ư��÷��� 0���� �Է��ϼ���.\n");
				continue;
			}
			printf("�� ��?(1~%d) : ", totalCol);
			scanf("%d", &seatCol);
			if (seatCol == 0)
				return;
			if (seatCol < 0 || seatCol > totalRow)
			{
				printf("�ùٸ��� ���� �Է��Դϴ�.\n");
				delay(1000);	//1�� ��ٸ���
				system("cls");
				display_status_of_seats(&movies[movieNum], date, order);
				printf("\n\n");
				printf("�ش��ϴ� ��ȭ�� �¼�ǥ �Դϴ�. � �ڸ��� �����Ͻðڽ��ϱ�? ó������ ���ư��÷��� 0���� �Է��ϼ���.\n");
				continue;
			}
			else
				break;
		}



		seatNum = (seatRow - 1)*totalCol + seatCol;
		//�¼�ǥ�� ��, �� ������ �Ѿ�� ��� ó��
		if (seatNum < 0 || seatNum > movies[movieNum].num_of_seats[date][order])
		{
			printf("�ùٸ��� ���� �Է��Դϴ�.\n");
			delay(1000);	//1�� ��ٸ���
			continue;
		}

		printf("seatNum : %d\n", seatNum);
		node* nd = isKeyExist(movies[movieNum].seats[date][order].root, seatNum);
		if (nd != NULL)
		{
			printf("�̹� ������ �Ǿ��ִ� �¼��Դϴ�. �ٽ� �Է��� �ּ���.\n");
			delay(1000);	//1�� ��ٸ���
		}
		else
		{
			printf("%d�� %d�� �¼����� ����Ǿ����ϴ�.\n", seatRow, seatCol);
			delay(2000);
			node* ins = getNode(seatNum);
			RB_insert(&movies[movieNum].seats[date][order], ins);
			system("cls");
			afterInsDel(movieNum, date, order);
			printf("ó������ �ǵ��� ���÷��� �ƹ�Ű�� ��������.");
			_getch();
			break;
		}
	}
}

//�������
void cancelation(int movieNum)
{
	int date;
	int order;
	int seatRow;	//�¼�ǥ���� ������� �¼� �� ��ȣ
	int seatCol;	//�¼�ǥ���� ������� �¼� �� ��ȣ
	int seatNum;	//��������� �¼���ȣ
	int totalRow;
	int totalCol;


	//�ش� ��ȭ�� ������
	while (1)
	{
		system("cls");
		showRunningMovies();
		printf("\n\"%s\"�� �����ϼ̽��ϴ�. ��ĥ �� ��ȭ�� ��������Ͻðڽ��ϱ�?(1~6) \nó������ ���ư��÷��� 0���� ��������.", movies[movieNum].name);
		date = _getch();
		date -= 48;
		if (date < 0 || date > 6)
		{
			printf("\n�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n");
			delay(1000);
			continue;
		}
		if (date == 0)
			return;
		date = date - 1;		//Movie ����ü ����  2���� �迭���� �ε����� 0���� �����ϹǷ� �������ش�.
		break;
	}
	while (1) {
		system("cls");
		showScheduleOfMovie(movies[movieNum], date);
		printf("\n�� ��° ��ȭ�� ��������Ͻðڽ��ϱ�?(1~3) ó������ ���ư��÷��� 0���� ��������.");
		order = _getch();
		order -= 48;
		if (order < 0 || order > 3)
		{
			printf("�߸��� �Է��Դϴ�.�ٽ� �Է����ּ���.\n");
			delay(1000);
			continue;
		}
		if (order == 0)
			return;
		order = order - 1;		//Movie ����ü ����  2���� �迭���� �ε����� 0���� �����ϹǷ� �������ش�.
		break;
	}




	//seatRow, seatCol �� ã�� ���� ����, �ణ hardcoded ��
	for (int i = 1, seats = movies[movieNum].num_of_seats[date][order]; i <= seats; i++)
	{
		if (i*i == seats)
		{
			totalCol = i;
			totalRow = i;
			break;
		}
		else if (i*i > seats)
		{
			totalCol = i;
			totalRow = i - 1;
			while (1)
			{
				if (totalCol * totalRow < seats)
				{
					totalRow += 1;
					break;
				}
				else
					totalRow -= 1;
			}
			break;
		}
	}
	//�ش� �������� �¼�ǥ
	while (1)
	{
		system("cls");
		display_status_of_seats(&movies[movieNum], date, order);
		printf("\n\n");
		//�¼� ��ȣ �Է� �ޱ� (��, �� ���� �¼���ȣ�� ġȯ�ؾ� �Ѵ�.)
		printf("�ش��ϴ� ��ȭ�� �¼�ǥ �Դϴ�. � �ڸ��� ��������Ͻðڽ��ϱ�? ó������ ���ư��÷��� 0���� �Է��ϼ���.\n");
		while (1)
		{
			printf("�� ��?(1~%d) : ", totalRow);
			scanf("%d", &seatRow);
			if (seatRow == 0)
				return;
			if (seatRow < 0 || seatRow > totalRow)
			{
				printf("�ùٸ��� ���� �Է��Դϴ�.\n");
				delay(1000);	//1�� ��ٸ���
				system("cls");
				display_status_of_seats(&movies[movieNum], date, order);
				printf("\n\n");
				printf("�ش��ϴ� ��ȭ�� �¼�ǥ �Դϴ�. � �ڸ��� ��������Ͻðڽ��ϱ�? ó������ ���ư��÷��� 0���� �Է��ϼ���.\n");
				continue;
			}
			printf("�� ��?(1~%d) : ", totalCol);
			scanf("%d", &seatCol);
			if (seatCol == 0)
				return;
			if (seatCol < 0 || seatCol > totalRow)
			{
				printf("�ùٸ��� ���� �Է��Դϴ�.\n");
				delay(1000);	//1�� ��ٸ���
				system("cls");
				display_status_of_seats(&movies[movieNum], date, order);
				printf("\n\n");
				printf("�ش��ϴ� ��ȭ�� �¼�ǥ �Դϴ�. � �ڸ��� ��������Ͻðڽ��ϱ�? ó������ ���ư��÷��� 0���� �Է��ϼ���.\n");
				continue;
			}
			else
				break;
		}
		seatNum = (seatRow - 1)*totalCol + seatCol;
		//�¼�ǥ�� ��, �� ������ �Ѿ�� ��� ó��
		if (seatNum < 0 || seatNum > movies[movieNum].num_of_seats[date][order])
		{
			printf("�ùٸ��� ���� �Է��Դϴ�.\n");
			delay(1000);	//1�� ��ٸ���
			continue;
		}

		node* nd = isKeyExist(movies[movieNum].seats[date][order].root, seatNum);
		//reservation( ) �Լ��� ������ �ٸ��� ����
		if (nd == NULL)
		{
			printf("���� ���� �Ǿ����� �ʴ� �¼��Դϴ�. �ٽ� �Է��� �ּ���.\n");
			delay(1000);	//1�� ��ٸ���
		}
		else
		{
			printf("%d�� %d�� �¼��� ������ҵǾ����ϴ�.\n", seatRow, seatCol);
			delay(2000);
			RB_delete(&movies[movieNum].seats[date][order], nd);
			system("cls");
			afterInsDel(movieNum, date, order);
			printf("ó������ �ǵ��� ���÷��� �ƹ�Ű�� ��������.");
			_getch();
			break;
		}
	}
}



//�޴� ����Լ�
void menu()
{
	int choice;
	int num;
	while (1)
	{


		while (1)
		{
			system("cls");
			printf("%60s", "���� ��ȭ���� ���Ű� ȯ���մϴ�!\n");
			printf("%67s", "���� ��ȭ���� �̹� �� ������ ������ �����ϴ�.\n\n");
			showRunningMovies();


			printf("\n%-15s%-10s%13s\n", "1. ����", "2. ���� ���", "3. ����");
			printf("\n�޴��� ������ �ּ���.");

			choice = _getch();
			choice -= 48; // _getch() �Լ��� ������ �޴µ� choice�� int���̹Ƿ� �ش��ϴ� ascii�� ����
			if (choice < 1 || choice > 3)
			{
				printf("\n�߸��� �Է��Դϴ�. �ٽ� �Է��� �ּ���.\n");
				delay(1000);	//1�� ��ٸ���
			}
			else
				break;
		}
		printf("\n\n");

		switch (choice)
		{
		case 1:
			while (1)
			{
				system("cls");
				showRunningMovies();
				printf("\n�����Ͻ� ��ȭ�� ��ȣ�� ��������.");
				num = _getch();
				num -= 48;
				if (num < 1 || num>5)
				{
					printf("\n�߸��� �Է��Դϴ�. �ٽ� �Է��� �ּ���.\n");
					delay(1000);	//1�� ��ٸ���
				}
				else
					break;
			}
			reservation(num - 1);	//Movie* movie �� ������ ����Ű�� �迭�� �ε����� 0���� �����ϱ� ������ 1�� ���־� ���
			break;
		case 2:
			while (1)
			{
				system("cls");
				showRunningMovies();
				printf("\n��������Ͻ� ��ȭ�� ��ȣ�� ��������. : ");
				num = _getch();
				num -= 48;
				if (num < 1 || num>5)
				{
					printf("\n�߸��� �Է��Դϴ�. �ٽ� �Է��� �ּ���.\n");
					delay(1000);	//1�� ��ٸ���
				}
				else
					break;
			}
			cancelation(num - 1);	//Movie* movie �� ������ ����Ű�� �迭�� �ε����� 0���� �����ϱ� ������ 1�� ���־� ���
			break;
		case 3:
			printf("�����մϴ�. ������ �� ������.\n");
			delay(1000);	//1�� ��ٸ���
			break;

		}
		if (choice == 3)
			break;
	}
}





void showLeaves(RBT* T, node* nd, int* height)
{
	(*height)++;
	//���� ���� T->nil �ε�, �� �Լ��� ȣ���� �θ��尡 �ڽ��� �ϳ� �ִ� ��쿡 �ش��Ѵ�.
	if (nd == T->nil)
		return;
	if (nd->left == T->nil && nd->right == T->nil)
	{
		if (maxHeight < *height)
			maxHeight = *height;
		printf("%4d ", nd->key);
		return;
	}
	else
	{
		showLeaves(T, nd->left, height);
		(*height)--;
		showLeaves(T, nd->right, height);
		(*height)--;
	}
}



//RBT�� insertion, deletion �� �̷���� ��, ����� ����, root �� leaves, tree �� ���� ���
//RBT�� insertion, deletion �� �̷���� ��, reservation ����� ���(�¼� �����ָ� �� ��)
void afterInsDel(int movieNum, int date, int order)
{
	Movie mv = movies[movieNum];
	RBT rbt = mv.seats[date][order];
	int height = 0;
	maxHeight = 0;
	printf("\"%s\"  %d�� �� %d��° ��ȭ\n", mv.name, date + 1, order + 1);
	printf("����� �¼��� ��(RBT ����� ����) : %d\n", rbt.numOfNodes);
	if (rbt.numOfNodes == 0)
	{
		printf("tree�� ���� : 0\n");
		return;
	}

	printf("RBT�� root�� key��(�¼���ȣ) : %d\n", rbt.root->key);
	delay(1000);
	printf("RBT�� leaves�� key����(�¼���ȣ)\n");
	delay(1000);
	showLeaves(&rbt, rbt.root, &height);
	delay(1000);
	printf("\nRBT�� ���� : %d\n\n\n", maxHeight);
	delay(1000);

	printf("����� ���� �¼�ǥ �Դϴ�.\n\n");
	display_status_of_seats(&mv, date, order);
	printf("\n\n");
}