#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h> 

#define PRINT_INT(x) printf("\n"#x" : %d\n", x);
#define PRINT_CHAR(x) printf("\n"#x" : %c\n", x);
#define PRINT_STRING(x) printf("\n"#x" : %s\n", x);
/////////////////////////////////출력 관련 매크로
#define SCAN_INT(x) scanf("%d", &x);
#define SCAN_CHAR(x) scanf("%c", &x);
#define SCAN_STRING(x) scanf("%s", s);
/////////////////////////////////입력 관련 매크로
#define PATH_CLIENT "client.txt"
#define PATH_BOOK "book.txt"
#define PATH_BORROW "borrow.txt"
#define READ_MOD "rt"
#define WRITE_MOD "wt"
/////////////////////////////////파일 관련 매크로
#define MALLOC_CHAR(x, y) x -> y = (char *) malloc(strlen(y) * sizeof(char))
#define MALLOC_STRUCT(x,y) y = (x *) malloc(sizeof(x))
/////////////////////////////////동적 메모리 할당 관련 매크로


/////////////////////////////////매크로

typedef struct client{ //client 파일의 정보를 저장하기 위한 구조체
	unsigned id; //학번 (정수 8자리)
	char *password; //비밀번호
	char *name; //이름
	char *address; //주소
	char *phone_number; //전화번호
	struct client *next; //자기 참조 구조체 구현
} CLIENT;

typedef struct book{ //book 파일의 정보를 저장하기 위한 구조체
	unsigned number; //도서번호 (정수 7자리)
	char *name; //도서명
	char *publisher; //출판사
	char *writer; //저자명
 	unsigned long ISBN; //ISBN(정수 13자리)
	char *location; //소장처
	char borrow; //대여가능 여부
	struct book *next; //자기 참조 구조체 구현
} BOOK;

typedef struct borrow{ //borrow 파일의 정보를 저장하기 위한 구조체
	unsigned client_id; //학번 (정수 8자리)
	unsigned book_number; //도서번호 (정수 7자리)
	time_t borrow_date; //대여일자
	time_t return_date; //반납일자
	struct borrow *next; //자기 참조 구조체 구현
} BORROW;

/////////////////////////////////구조체 선언

CLIENT *client_read(void);

CLIENT *add_client(CLIENT *new_client, CLIENT **head, CLIENT *temp);

CLIENT *create_client(unsigned id, char password[], char name[], char address[], char 
phone_number[]);

CLIENT *sort_client(CLIENT *head);

/////////////////////////////////링크드 리스트 함수 선언



/////////////////////////////////함수 선언 

int main(void){
	CLIENT *client = NULL;
	MALLOC_STRUCT(CLIENT, client);

	client = client_read();
	
	return 0;
}

CLIENT *client_read(void){ //함수 안에서 client 파일 내용 받아와서 CLIENT 구조체에 내용 넣는 함수 (만드는 중)
	FILE *client_ifp; //client 파일 내용 받아오는 포인터 변수
	
	if ((client_ifp = fopen(PATH_CLIENT, READ_MOD)) == NULL){ //client 파일 읽어오기
		printf("client.txt 파일이 존재하지 않습니다.\n"); //파일 없으면 오류 메시지 출력
		exit(1); //프로그램 종료
	}
	
	
	CLIENT *head; //client 구조체 포인터 변수
	CLIENT *temp; //client 구조체 포인터 변수
	
	unsigned id; //학번 (정수 8자리)
	char password[20]; //비밀번호
	char name[10]; //이름
	char address[30]; //주소
	char phone_number[20]; //전화번호	
	
	MALLOC_STRUCT(CLIENT, head); //client 구조체 포인터변수에 메모리 할당
	MALLOC_STRUCT(CLIENT, temp); //client 구조체 포인터변수에 메모리 할당
	
	head -> next = NULL; //시작 부분
	temp -> next = NULL; //이전 노드 저장
	
	// id | password | name | address | phone_number : client 파일 데이터 형식
	while (fscanf(client_ifp, "%u | %s | %s | %[^|] | %s", &id, password,
	name, address, phone_number) != EOF){  //client 파일에서 한줄 fscanf로 받아와서 자료형에 넣어주기
	
	temp = add_client(create_client(id, password, name, address, phone_number), 
	&head, temp);	
	}
	// head = sort_client(head);
	temp = head;	//디버깅 코드 
	while (temp){	//마지막 구조체가 가리키는게 NULL값일 때까지 ( 전까지만)	
		printf("%u | %s | %s | %s| %s\n", temp -> id, temp -> password,
		temp -> name, temp -> address, temp -> phone_number);
		temp = temp -> next; //다음 구조체로 넘어가기
	}
		
	
	
	fclose(client_ifp);	
	
	return head;
}

CLIENT *add_client(CLIENT *new_client, CLIENT **head_p, CLIENT *temp){
	//**head_p로 head 포인터의 주소값을 받아와서 *head_p가 가리키는 구조체의 값을 변경해줘야됨
	//*head로 받아오면 값에 의한 호출이라 안바뀜
	//head는 포인터이므로 **으로 이중포인터 표현 사용
	//add_client를 호출할 때 매개인자에 head의 주소값을 넣어주면 됨
	if ((*head_p) -> next == NULL){		//자료가 첫번째로 입력될 때
		new_client -> next = new_client;			//새로 추가된 노드는 첫번째 노드임		
		*head_p = new_client; //첫번째 노드는 새로 추가된 노드와 같음
	}
	else {							//자료가 두번째 이후로 입력될 때
		temp -> next = new_client;  //이전의 노드는 새로 추가된 노드를 가리킴
		new_client -> next = NULL;  //새로 추가된 노드는 마지막 노드를 가리킴
	}
		
	return new_client; //만들어진 노드를 가리키는 포인터를 리턴
		
}

CLIENT *create_client(unsigned id, char password[], char name[], char address[], char 
phone_number[]){	
	CLIENT *new_client; 
	
	MALLOC_STRUCT(CLIENT, new_client); //client 구조체 포인터변수에 메모리 할당
	MALLOC_CHAR(new_client, password); //new_client -> password에 메모리 할당
	MALLOC_CHAR(new_client, name); //new_client -> name에 메모리 할당
	MALLOC_CHAR(new_client, address); //new_client -> address에 메모리 할당
	MALLOC_CHAR(new_client, phone_number); //new_client -> phone_number에 메모리 할당
	
	new_client -> id = id;
	strcpy(new_client -> password, password);
	strcpy(new_client -> name, name);
	strcpy(new_client -> address, address);
	strcpy(new_client -> phone_number, phone_number);
	new_client -> next = NULL;
	
	return new_client;
}

    // for(int i = 0; i < N - 1; i++)//비교횟수는 데이터갯수-1, 자기 자신과는 비교 하지 않음
        // for(int j = 0; j < N - 1 - i; j++)
            // if(num[j] > num[j + 1]){
                // temp = num[j];//앞자리의 수가 더 크면 교체
                // num[j] = num[j + 1];
                // num[j + 1] = temp;
			// }



CLIENT *sort_client(CLIENT *head){
	
	}
		
		
	
	
	
	

	
	


	
	
	
	
	
	
	
