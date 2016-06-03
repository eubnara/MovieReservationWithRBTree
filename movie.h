#ifndef _MOVIE_H_
#define _MOVIE_H_

#include "rbt.h"
#include <string.h>
#define TOTAL_NUM_OF_MOVIE 5	//총 영화의 수


//영화관 구조체
typedef struct _movie
{
	char* name;
	RBT seats[6][3];			//각각의 영화에 대한 좌석 정보를 담고 있다. 이 안에 들어가 있는 노드들의 key는 예약된 좌석번호를 나타낸다.
	int sched[6][3];			//[x][y] 라 하였을 때, x일 뒤의 이 영화 중 y번째 영화의 시작시간을 담고 있다. (정시에 시작하는 것으로 가정)
	int num_of_seats[6][3];		//영화관마다 자리개수 
} Movie;

//movie 구조체 초기화 함수
void init_movie(Movie* m);




/*
* 영화좌석 display 함수
* 해당 영화의 구조체 주소, 날짜, 그 날의 그 영화에 관해서 몇번째 영화인지를 나타내는 변수 세 개를 넘긴다.
* 그리고 그 영화의 좌석 현황을 시각적으로 보여준다.
*/
void display_status_of_seats(Movie* m, int date, int order);

//movie 구조체 내의 RB_tree 에 대한 free 함수
void movie_free(Movie* m);

//랜덤하게 생성된 총 영화의 해제를 담당하는 함수, 내부에서 movie_free() 함수를 호출한다.
void all_free(Movie* mpp);

/*
* 랜덤좌석 생성 부분
* 랜덤 요소:
* 영화이름, 상영시간, 주간 스케쥴(6일치의 영화이름, 상영시간을 생성하면 충족)
* 영화의 총 자리 개수(100~300사이의 값)
* 이미 차있는 자리(RB_tree 에 삽입, 70% 이상)
*/
Movie* random_generate();

#endif