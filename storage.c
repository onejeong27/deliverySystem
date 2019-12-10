#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

/* 
  definition of storage cell structure ----
  members :
  int building : building number of the destination
  int room : room number of the destination
  int cnt : number of packages in the cell
  char passwd[] : password setting (4 characters)
  char *contents : package context (message string)
*/
typedef struct {
	int building;
	int room;
	int cnt;
	char passwd[PASSWD_LEN+1];
	
	char *context;
} storage_t;


static storage_t** deliverySystem; 			//deliverySystem 
//??deliverysystem이 이중포인터인 이유는 struct안에 또 password나 context배열이 있기 떄문이라고 생 
static int storedCnt = 0;					//number of cells occupied
static int systemSize[2] = {0, 0};  		//row/column of the delivery system
static char masterPassword[PASSWD_LEN+1];	//master password




// ------- inner functions ---------------

//print the inside context of a specific cell
//int x, int y : cell to print the context
static void printStorageInside(int x, int y) {
	printf("\n------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n");
	if (deliverySystem[x][y].cnt > 0)
		printf("<<<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
	else
		printf("<<<<<<<<<<<<<<<<<<<<<<<< empty >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		
	printf("------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n\n");
}

//initialize the storage
//int x, int y : cell coordinate to be initialized
static void initStorage(int x, int y) {

	//set all the member variable as an initial value
	deliverySystem[x][y].building =0;													//??한방에초기화하는 방법  
	deliverySystem[x][y].room =0;
	deliverySystem[x][y].cnt =0;
	deliverySystem[x][y].passwd[0] ='\0';

	//and allocate memory to the context pointer
	deliverySystem[x][y].context = (char*)malloc((sizeof(char)*MAX_MSG_SIZE);			//??char *context를 사용하는 것이 옳은 것인가? 문자하나에 char하나? 
	//메모리가 할당되지 않은 경우: 오류처리 코드 필요                                   //??확인방법 
	if(deliverySystem[x][y].context == NULL){
		printf("allocate memory Errors\n");												
		exit(1);  																		//??return -1: 무슨 종료가 옳은 것일까  
	}
	//메모리 반납  
	free(deliverySystem[x][y].context); 												//??생각해보니 여기서는 메모리 반납이 필요 없지 않나  
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
static int inputPasswd(int x, int y) {
	char getpassword[PASSWD_LEN+1];														//??이것도 초기화가 필요한가(내생각에는 필요X) 
	
	printf(" - input password for (%d, %d) storage : ", x,y);
	scanf("%4s",&getpassword);
	//enter로인한 버퍼 비우기  
	while(getchar() != '\n');															 //??버퍼 비우기 (돈모양 fflush보다 안정강화 버전 )
	
	//return : 0 - password is matching (마스터비밀번호 또는 저장된 비밀번호)	
	if(strcmp(deliverySystem[x][y].passwd,getpassword)==0 || strcmp(masterPassword,getpassword)==0)   //??마스터 비밀번호 진짜 짜증난다. 		
		return 0;
	//return : 0 - password is not matching	
	else{
		printf(" -----------> password is wrong!!/n -----------> Failed to extract my package!")
	} 
		return -1;
}



// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write                           //??name to write는 무슨 뜻일까  
int str_backupSystem(char* filepath) {
	
	//쓰기 모드로 파일여는 스트림 형성 
	FILE *fp = fopen(filepath,"w");                                   //??읽고 쓰기 모드 모두다 가능해야 하는 거 아님? 

	//return -1 : failed to backup(스트림 생성 실패)	                 //??파일열기 실패도 알려줘야 하나? 이건 어떻게 확인하지ㅋ 진짜 내머리는 장식용  
	if(fp==NULL){
		//스트림해제 
		fclose(fp);
		return -1;
	}
	//return : 0 - backup was successfully done(파일에 systemsize,masterpassword,deliverySystem내용  쓰기)	
	fprintf(fp,"%d %d\n", systemSize[0], systemSize[1]);
	fprintf(fp,"%s\n", masterPassword);
	
	int i,j;
	for(i=0;i<systemSize[0];i++){
		for(j=0;j<systemSize[1];j++){
			if (deliverySystem[i][j].cnt>0)
				fprintf(pf,"%d %d %d %d %s %s\n", i, j, deliverySystem[i][j].building, deliverySystem[i][j].room, deliverySystem[i][j].passwd, deliverySystem[i][j].context);
		}
	}
	
	//스트림해제 
	fclose(pf);
	
	return 0; 	
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {
	
	
	//읽기모드로 파일여는 스트림 생성
	FILE *fp = fopen(filepath,"r");
	
	//파일 안열림 
	if(fp==NULL){
		//스트림해제 
		fclose(fp);
		return -1;
	}
	
	fscanf(fp,"%d %d",&systemSize[0],&systemSize[1]);
	fscanf(fp,"%s",masterPassword);
	
	deliverySystem
	
	
	 
	
}

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
}



//print the current state of the whole delivery system (which cells are occupied and the destination of the each occupied cells)
void str_printStorageStatus(void) {
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt, systemSize[0]*systemSize[1]);
	
	printf("\t");
	for (j=0;j<systemSize[1];j++)
	{
		printf(" %i\t\t",j);
	}
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	
	for (i=0;i<systemSize[0];i++)
	{
		printf("%i|\t",i);
		for (j=0;j<systemSize[1];j++)
		{
			if (deliverySystem[i][j].cnt > 0)
			{
				printf("%i,%i\t|\t", deliverySystem[i][j].building, deliverySystem[i][j].room);
			}
			else
			{
				printf(" -  \t|\t");
			}
		}
		printf("\n");
	}
	printf("--------------------------------------- Delivery Storage System Status --------------------------------------------\n\n");
}



//check if the input cell (x,y) is valid and whether it is occupied or not
int str_checkStorage(int x, int y) {
	if (x < 0 || x >= systemSize[0])
	{
		return -1;
	}
	
	if (y < 0 || y >= systemSize[1])
	{
		return -1;
	}
	
	return deliverySystem[x][y].cnt;	
}


//put a package (msg) to the cell
//input parameters
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
	

}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	
	return cnt;
}
