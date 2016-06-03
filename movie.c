#include "movie.h"

//랜덤 생성할 영화 이름
char* movie_name_cand[10] = {
	"쇼생크탈출", "대부", "다크나이트", "쉰들러리스트", "펄프픽션",
	"반지의제왕", "파이트클럽", "스타워즈", "포레스트검프", "인셉션"
};

int isRunning[10];	//random_generate() 함수에서 똑같은 이름의 영화를 중복해서 생성하지 않기 위해 사용되는 변수

//13을 넘어가면 12를 빼고 '오후'를 붙여줘서 사용
//시간 중복처리를 위한 flag 변수는 랜덤생성하는 함수 내에서 적용
int time_cand[15] = {
	8, 9, 10, 11, 12,
	13, 14, 15, 16, 17,
	18, 19, 20, 21, 22
};

//Movie 구조체를 사용하기 전에 초기화를 진행
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
	printf("빈 자리: O, 예약된 자리: X\n");
	int seats = m->num_of_seats[date][order];
	printf("   ");
	for (int i = 0; i*i < seats; i++)
		printf("%2d ", i + 1);

	printf("\n%2d ", 1);
	//RB_tree 를 이용하여 num_of_seats 만큼 반복하여 해당 자리가 RB_tree안에 있으면 X 없으면 O 출력
	for (int i = 1, j = 1, count = 1; i <= seats; i++, j++)
	{
		//TODO: 이 자리에서 RB_tree 체크 후, O, X 중에 하나를 print
		printf("%2c ", isKeyExist(m->seats[date][order].root, i) ? 'X' : 'O');
		//적당히 가로세로 길이를 나누기 위한 부분
		//자리가 100~300자리로 변할 수 있으므로 절대적인 숫자로 한 줄씩 라인피드하면 안된다.
		//i !=seats 조건은 만일 해당 영화의 좌석의 총 수가 정확히 어떤 수의 제곱이라면(n^2), n+1행이 내용없이 추가되는 버그가 생긴다.
		//따라서, i==seats 인 경우에는 printf("\n%2 ", count); 가 실행되지 않도록 한다.
		if (j*j >= seats && i !=seats)	
		{
			count++;
			printf("\n%2d ", count);	//세로로 문자출력하기 위한 부분.
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


//TODO: 나중에 main 에서 이 함수 반환된 포인터 반환필요
Movie* random_generate()
{
	//영화를 선택한 후에, isRunning[] 변수도 업데이트하여 이미 선택된 영화를 중복선택하지 않도록 한다.
	srand(time(NULL));
	Movie* gen = (Movie*)malloc(sizeof(Movie) * 5);
	for (int i = 0; i < 5; i++)
		init_movie(&gen[i]);	//Movie 구조체 사용 전 초기화
	//영화이름 정하기
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

	//영화이름이 중복되지 않도록 사용하기 위한 변수
	//하루 사이에서만 중복되지 않게 적용
	//주간 스케쥴 내에서는 중복되어도 됨(실제 영화관에서도 같은 영화가 주간에 여러 번 상영하기도 하므로)
	int is_movie_selected[10] = { 0, };
	//시간대가 중복되지 않도록 사용(사실, 영화 상영실이 여러 개라면, 시간이 중복되어도 상관은 없지만 그냥 중복되지 않게 한다고 가정)
	int is_time_selected[15] = { 0, };

	//스케쥴 정하기
	//i: 6일치  j: 5개의 영화 k: 하루의 3개 스케쥴  --> 5개의 영화가 하루에 스케쥴이 모두 안겹치게 함.
	for (int i = 0; i < 6; i++)
	{
		memset(is_time_selected, 0, 15 * sizeof(int));	//6일 중, 시간대가 하루 중에서만 안겹치면 되므로, 6일치를 만드는 해당 loop 의 처음 부분에 초기화를 진행한다.
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

	//각 영화의 좌석 수 생성하기
	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 6; j++)
			for (int k = 0; k < 3; k++)
				gen[i].num_of_seats[j][k] = rand() % 201 + 100;	//100~300 사이의 범위

	//각 영화의 좌석 수의 70%만큼 RBT에 삽입하기
	int isDuplicate[301];
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				memset(isDuplicate, 0, 301 * sizeof(int));
				int numOfSeats = gen[i].num_of_seats[j][k];
				for (int l = 0; l < numOfSeats * 7 / 10; l++)	//70% 생성
				{
					int idx;
					while (1)
					{
						idx = rand() % numOfSeats + 1; //numOfSeats 가 100자리라면, 1~100번째 까지의 숫자가 랜덤으로 생성됨
						if (isDuplicate[idx] == FALSE)	//만일 중복된 숫자가 안나온다면, isDuplicate[] 을 update한 후에, loop를 빠져나가 해당 자리에 대한 노드를 red black tree에 삽입해 준다.
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

	//5개의 Movie 구조체를 할당하고, 첫번째 Movie를 가리키는 pointer를 반환한다.
	return gen;
}