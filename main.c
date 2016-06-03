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
int maxHeight;	//tree를 traversal 할 때, 최대 높이를 저장해주기 위한 전역변수



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



//상영 중인 영화 목록 출력
void showRunningMovies()
{
	printf("[]***************************************************************************[]\n");
	for (int i = 1; i <= TOTAL_NUM_OF_MOVIE; i++)
		printf("[]%27d.%20s%29s\n", i, movies[i - 1].name, "[]");
	printf("[]***************************************************************************[]\n");
}

//해당 영화의 스케쥴 출력 함수
void showScheduleOfMovie(Movie m, int date)
{
	system("cls");
	printf("\n\"%s\"의 %d일 후 스케쥴입니다.\n\n", m.name, date + 1);
	printf("[]***************************************************************************[]\n");
	for (int j = 1; j <= 3; j++)
	{
		int time = m.sched[date][j - 1];
		if (time >= 12)
		{
			//오후 0시 말고, 그냥 오후 12시로 처리
			if(time == 12)
				printf("[]%27d.%13s %2d시 ~ %2d시%24s\n", j, "오후", time, time - 11, "[]");
			else
				printf("[]%27d.%13s %2d시 ~ %2d시%24s\n", j, "오후", time - 12, time - 11, "[]");
		}			
		else
			printf("[]%27d.%13s %2d시 ~ %2d시%24s\n", j, "오전", time, time + 1, "[]");
	}
	printf("[]***************************************************************************[]\n");
}
//해당 영화 스케쥴의 좌석표 출력


//예약
void reservation(int movieNum)
{
	int date;
	int order;
	int seatRow;	//좌석표에서 예약 좌석 행 번호
	int seatCol;	//좌석표에서 예약 좌석 열 번호
	int seatNum;	//예약할 좌석번호
	int totalRow;
	int totalCol;
	//해당 영화의 스케쥴
	while (1)
	{
		system("cls");
		showRunningMovies();
		printf("\n\"%s\"를 선택하셨습니다. 며칠 후 예약하시겠습니까?(1~6) \n처음으로 돌아가시려면 0번을 누르세요.", movies[movieNum].name);
		date = _getch();
		date -= 48;

		if (date < 0 || date > 6)
		{
			printf("\n잘못된 입력입니다. 다시 입력해주세요.\n");
			delay(1000);
			continue;
		}
		if (date == 0)
			return;
		date = date - 1;		//Movie 구조체 안의  2차원 배열들은 인덱스가 0부터 시작하므로 조정해준다.
		break;
	}
	while (1) {
		system("cls");
		showScheduleOfMovie(movies[movieNum], date);
		printf("\n몇번째 영화를 보시겠습니까?(1~3) 처음으로 돌아가시려면 0번을 누르세요.");
		order = _getch();
		order -= 48;
		if (order < 0 || order > 3)
		{
			printf("\n잘못된 입력입니다. 다시 입력해주세요.\n");
			delay(1000);
			continue;
		}
		if (order == 0)
			return;
		order = order - 1;	//Movie 구조체 안의  2차원 배열들은 인덱스가 0부터 시작하므로 조정해준다.
		break;
	}


	//seatRow, seatCol 값 찾기 위한 과정, 약간 hardcoded 됨
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
	//해당 스케쥴의 좌석표
	while (1)
	{
		system("cls");
		display_status_of_seats(&movies[movieNum], date, order);
		printf("\n\n");
		//좌석 번호 입력 받기 (행, 열 값을 좌석번호로 치환해야 한다.)
		printf("해당하는 영화의 좌석표 입니다. 어떤 자리를 예약하시겠습니까? 처음으로 돌아가시려면 0번을 입력하세요.\n");
		while (1)
		{
			printf("몇 행?(1~%d) : ", totalRow);
			scanf("%d", &seatRow);
			if (seatRow == 0)
				return;
			if (seatRow < 0 || seatRow > totalRow)
			{
				printf("올바르지 않은 입력입니다.\n");
				delay(1000);	//1초 기다리기
				system("cls");
				display_status_of_seats(&movies[movieNum], date, order);
				printf("\n\n");
				printf("해당하는 영화의 좌석표 입니다. 어떤 자리를 예약하시겠습니까? 처음으로 돌아가시려면 0번을 입력하세요.\n");
				continue;
			}
			printf("몇 열?(1~%d) : ", totalCol);
			scanf("%d", &seatCol);
			if (seatCol == 0)
				return;
			if (seatCol < 0 || seatCol > totalRow)
			{
				printf("올바르지 않은 입력입니다.\n");
				delay(1000);	//1초 기다리기
				system("cls");
				display_status_of_seats(&movies[movieNum], date, order);
				printf("\n\n");
				printf("해당하는 영화의 좌석표 입니다. 어떤 자리를 예약하시겠습니까? 처음으로 돌아가시려면 0번을 입력하세요.\n");
				continue;
			}
			else
				break;
		}



		seatNum = (seatRow - 1)*totalCol + seatCol;
		//좌석표의 행, 열 범위를 넘어가는 경우 처리
		if (seatNum < 0 || seatNum > movies[movieNum].num_of_seats[date][order])
		{
			printf("올바르지 않은 입력입니다.\n");
			delay(1000);	//1초 기다리기
			continue;
		}

		printf("seatNum : %d\n", seatNum);
		node* nd = isKeyExist(movies[movieNum].seats[date][order].root, seatNum);
		if (nd != NULL)
		{
			printf("이미 예약이 되어있는 좌석입니다. 다시 입력해 주세요.\n");
			delay(1000);	//1초 기다리기
		}
		else
		{
			printf("%d행 %d열 좌석으로 예약되었습니다.\n", seatRow, seatCol);
			delay(2000);
			node* ins = getNode(seatNum);
			RB_insert(&movies[movieNum].seats[date][order], ins);
			system("cls");
			afterInsDel(movieNum, date, order);
			printf("처음으로 되돌아 가시려면 아무키나 누르세요.");
			_getch();
			break;
		}
	}
}

//예약취소
void cancelation(int movieNum)
{
	int date;
	int order;
	int seatRow;	//좌석표에서 예약취소 좌석 행 번호
	int seatCol;	//좌석표에서 예약취소 좌석 열 번호
	int seatNum;	//예약취소할 좌석번호
	int totalRow;
	int totalCol;


	//해당 영화의 스케쥴
	while (1)
	{
		system("cls");
		showRunningMovies();
		printf("\n\"%s\"를 선택하셨습니다. 며칠 후 영화를 예약취소하시겠습니까?(1~6) \n처음으로 돌아가시려면 0번을 누르세요.", movies[movieNum].name);
		date = _getch();
		date -= 48;
		if (date < 0 || date > 6)
		{
			printf("\n잘못된 입력입니다. 다시 입력해주세요.\n");
			delay(1000);
			continue;
		}
		if (date == 0)
			return;
		date = date - 1;		//Movie 구조체 안의  2차원 배열들은 인덱스가 0부터 시작하므로 조정해준다.
		break;
	}
	while (1) {
		system("cls");
		showScheduleOfMovie(movies[movieNum], date);
		printf("\n몇 번째 영화를 예약취소하시겠습니까?(1~3) 처음으로 돌아가시려면 0번을 누르세요.");
		order = _getch();
		order -= 48;
		if (order < 0 || order > 3)
		{
			printf("잘못된 입력입니다.다시 입력해주세요.\n");
			delay(1000);
			continue;
		}
		if (order == 0)
			return;
		order = order - 1;		//Movie 구조체 안의  2차원 배열들은 인덱스가 0부터 시작하므로 조정해준다.
		break;
	}




	//seatRow, seatCol 값 찾기 위한 과정, 약간 hardcoded 됨
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
	//해당 스케쥴의 좌석표
	while (1)
	{
		system("cls");
		display_status_of_seats(&movies[movieNum], date, order);
		printf("\n\n");
		//좌석 번호 입력 받기 (행, 열 값을 좌석번호로 치환해야 한다.)
		printf("해당하는 영화의 좌석표 입니다. 어떤 자리를 예약취소하시겠습니까? 처음으로 돌아가시려면 0번을 입력하세요.\n");
		while (1)
		{
			printf("몇 행?(1~%d) : ", totalRow);
			scanf("%d", &seatRow);
			if (seatRow == 0)
				return;
			if (seatRow < 0 || seatRow > totalRow)
			{
				printf("올바르지 않은 입력입니다.\n");
				delay(1000);	//1초 기다리기
				system("cls");
				display_status_of_seats(&movies[movieNum], date, order);
				printf("\n\n");
				printf("해당하는 영화의 좌석표 입니다. 어떤 자리를 예약취소하시겠습니까? 처음으로 돌아가시려면 0번을 입력하세요.\n");
				continue;
			}
			printf("몇 열?(1~%d) : ", totalCol);
			scanf("%d", &seatCol);
			if (seatCol == 0)
				return;
			if (seatCol < 0 || seatCol > totalRow)
			{
				printf("올바르지 않은 입력입니다.\n");
				delay(1000);	//1초 기다리기
				system("cls");
				display_status_of_seats(&movies[movieNum], date, order);
				printf("\n\n");
				printf("해당하는 영화의 좌석표 입니다. 어떤 자리를 예약취소하시겠습니까? 처음으로 돌아가시려면 0번을 입력하세요.\n");
				continue;
			}
			else
				break;
		}
		seatNum = (seatRow - 1)*totalCol + seatCol;
		//좌석표의 행, 열 범위를 넘어가는 경우 처리
		if (seatNum < 0 || seatNum > movies[movieNum].num_of_seats[date][order])
		{
			printf("올바르지 않은 입력입니다.\n");
			delay(1000);	//1초 기다리기
			continue;
		}

		node* nd = isKeyExist(movies[movieNum].seats[date][order].root, seatNum);
		//reservation( ) 함수와 조건이 다름에 유의
		if (nd == NULL)
		{
			printf("아직 예약 되어있지 않는 좌석입니다. 다시 입력해 주세요.\n");
			delay(1000);	//1초 기다리기
		}
		else
		{
			printf("%d행 %d열 좌석이 예약취소되었습니다.\n", seatRow, seatCol);
			delay(2000);
			RB_delete(&movies[movieNum].seats[date][order], nd);
			system("cls");
			afterInsDel(movieNum, date, order);
			printf("처음으로 되돌아 가시려면 아무키나 누르세요.");
			_getch();
			break;
		}
	}
}



//메뉴 출력함수
void menu()
{
	int choice;
	int num;
	while (1)
	{


		while (1)
		{
			system("cls");
			printf("%60s", "저희 영화관에 오신걸 환영합니다!\n");
			printf("%67s", "저희 영화관의 이번 주 상영작은 다음과 같습니다.\n\n");
			showRunningMovies();


			printf("\n%-15s%-10s%13s\n", "1. 예약", "2. 예약 취소", "3. 종료");
			printf("\n메뉴를 선택해 주세요.");

			choice = _getch();
			choice -= 48; // _getch() 함수는 문자을 받는데 choice는 int형이므로 해당하는 ascii값 조정
			if (choice < 1 || choice > 3)
			{
				printf("\n잘못된 입력입니다. 다시 입력해 주세요.\n");
				delay(1000);	//1초 기다리기
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
				printf("\n예약하실 영화의 번호를 누르세요.");
				num = _getch();
				num -= 48;
				if (num < 1 || num>5)
				{
					printf("\n잘못된 입력입니다. 다시 입력해 주세요.\n");
					delay(1000);	//1초 기다리기
				}
				else
					break;
			}
			reservation(num - 1);	//Movie* movie 이 변수가 가리키는 배열의 인덱스는 0부터 시작하기 때문에 1을 빼주어 사용
			break;
		case 2:
			while (1)
			{
				system("cls");
				showRunningMovies();
				printf("\n예약취소하실 영화의 번호를 누르세요. : ");
				num = _getch();
				num -= 48;
				if (num < 1 || num>5)
				{
					printf("\n잘못된 입력입니다. 다시 입력해 주세요.\n");
					delay(1000);	//1초 기다리기
				}
				else
					break;
			}
			cancelation(num - 1);	//Movie* movie 이 변수가 가리키는 배열의 인덱스는 0부터 시작하기 때문에 1을 빼주어 사용
			break;
		case 3:
			printf("감사합니다. 다음에 또 오세요.\n");
			delay(1000);	//1초 기다리기
			break;

		}
		if (choice == 3)
			break;
	}
}





void showLeaves(RBT* T, node* nd, int* height)
{
	(*height)++;
	//현재 노드는 T->nil 인데, 이 함수를 호출한 부모노드가 자식이 하나 있는 경우에 해당한다.
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



//RBT의 insertion, deletion 이 이루어질 때, 노드의 숫자, root 와 leaves, tree 의 높이 출력
//RBT의 insertion, deletion 이 이루어질 때, reservation 결과를 출력(좌석 보여주면 될 듯)
void afterInsDel(int movieNum, int date, int order)
{
	Movie mv = movies[movieNum];
	RBT rbt = mv.seats[date][order];
	int height = 0;
	maxHeight = 0;
	printf("\"%s\"  %d일 후 %d번째 영화\n", mv.name, date + 1, order + 1);
	printf("예약된 좌석의 수(RBT 노드의 숫자) : %d\n", rbt.numOfNodes);
	if (rbt.numOfNodes == 0)
	{
		printf("tree의 높이 : 0\n");
		return;
	}

	printf("RBT의 root의 key값(좌석번호) : %d\n", rbt.root->key);
	delay(1000);
	printf("RBT의 leaves의 key값들(좌석번호)\n");
	delay(1000);
	showLeaves(&rbt, rbt.root, &height);
	delay(1000);
	printf("\nRBT의 높이 : %d\n\n\n", maxHeight);
	delay(1000);

	printf("예약된 후의 좌석표 입니다.\n\n");
	display_status_of_seats(&mv, date, order);
	printf("\n\n");
}