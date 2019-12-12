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
	deliverySystem[x][y].building =0;													 
	deliverySystem[x][y].room =0;
	deliverySystem[x][y].cnt =0;
	deliverySystem[x][y].passwd[0] ='\0';
	deliverySystem[x][y].context = NULL;

}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
static int inputPasswd(int x, int y) {
	char getpassword[PASSWD_LEN+1];														 
	
	printf(" - input password for (%d, %d) storage : ", x,y);
	scanf("%4s",&getpassword);
	//enter������ ���� ���� 
	fflush(stdin);														 
	
	//password is matching (�����ͺ�й�ȣ �Ǵ� ����� ��й�ȣ)	
	if(strcmp(deliverySystem[x][y].passwd,getpassword)==0 || strcmp(masterPassword,getpassword)==0)    		
		return 0;
	//password is not matching	
	else
		printf(" -----------> password is wrong!!/n -----------> Failed to extract my package!");
		return -1;
}



// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write                            
int str_backupSystem(char* filepath) {
	
	//���� ���� ���Ͽ��� ��Ʈ�� ���� 
	FILE *fp = fopen(filepath,"w");                                   

	//failed to backup(��Ʈ�� ���� ����)	   
	if(fp==NULL){
		//��Ʈ������ 
		return -1;														 
	}
	//backup was successfully done(���Ͽ� systemsize,masterpassword,deliverySystem����  ����)	
	fprintf(fp,"%d %d\n", systemSize[0], systemSize[1]);
	fprintf(fp,"%s\n", masterPassword);
	
	int i,j;
	for(i=0;i<systemSize[0];i++){
		for(j=0;j<systemSize[1];j++){
			if (deliverySystem[i][j].cnt>0)
				fprintf(fp,"%d %d %d %d %s %s\n", i, j, deliverySystem[i][j].building, deliverySystem[i][j].room, deliverySystem[i][j].passwd, deliverySystem[i][j].context);
		}
	}
	
	//��Ʈ������ 
	fclose(fp);
	
	return 0; 	
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
int str_createSystem(char* filepath) {
	
	int i;
	int x,y; 
	
	//�б���� ���Ͽ��� ��Ʈ�� ����
	FILE *fp = fopen(filepath,"r");
	
	//���� �ȿ����� ���  
	if(fp==NULL){												 
		//failed to create the system
		return -1;
	}
	
	fscanf(fp,"%d %d",&systemSize[0],&systemSize[1]);
	fscanf(fp,"%s",masterPassword);
	
	//sizesystem[0]���� ���� ����Ű�� �޸� �Ҵ��Ͽ� ������ �����                     
	deliverySystem = (storage_t**)malloc(systemSize[0]*sizeof(storage_t*));
	//�� �࿡ �ش��ϴ� sizesystem[1]���� ���� ����Ű�� �޸� �Ҵ��Ͽ� ������ ����� 
	for(i=0;i<systemSize[0];i++){  
		deliverySystem[i] = (storage_t*)malloc(systemSize[1]*sizeof(storage_t));            
	}

	//�� �������� �ʱ�ȭ ���ֱ� 
	for(x=0;x<systemSize[0];x++){
		for (y=0;y<systemSize[1];y++){
			initStorage(x,y);
		}
	} 

	//���� �о �ù躸���Կ�  ���� �����ϱ�
	while(!feof(fp)){ //����ȵ� �ٸ�������� �غ�����!  
	
		char getcontext[MAX_MSG_SIZE+1];
		char length;
		//��� �� �б�(�ù躸����) 
		fscanf(fp,"%d %d", &x, &y);
		//�ý��� ���� �б�
		fscanf(fp,"%d %d %s %s",&deliverySystem[x][y].building,&deliverySystem[x][y].room,deliverySystem[x][y].passwd,getcontext);
		//���ڿ�����+1(�������)  
		length =strlen(getcontext)+1;              
		//���ڿ� ���̸�ŭ �޸� �Ҵ� 
		deliverySystem[x][y].context = (char*)malloc(sizeof(char)*length);	//������ڵ� char�� �����ǰ�		
		
		//�޸𸮰� �Ҵ���� ���� ���                                    
		if(deliverySystem[x][y].context == NULL){
		printf("allocate memory Errors\n");              	
		return -1;}
		
		deliverySystem[x][y].cnt++;
		storedCnt++;

	}
	fclose(fp);											 
	//successfully created
	return 0; 
}

//free the memory of the deliverySystem 
void str_freeSystem(void) {                  
	
	int i,j;
	
	//deliverysystem.context memory free(�ּҵ��� ���� �ʿ� ���� �ǰ�)  
	for(i=0;i<systemSize[0];i++){
		for(j=0;j<systemSize[1];j++){
			if(deliverySystem[i][j].cnt != 0)
			   free(deliverySystem[i][j].context);}	
	}
	
	//�ù躸���� '��'�޸� free
	for(j=0;j<systemSize[1];j++){
		free(deliverySystem[j]);
	}
	//�ù躸���� '��'�޸� free 
	free(deliverySystem);
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
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
	
	char length;
	
	//�Է¹��� ���� �־� �ֱ�  
	deliverySystem[x][y].building = nBuilding;
	deliverySystem[x][y].room = nRoom;
	strcpy(deliverySystem[x][y].passwd,passwd);
	
	//���ڿ� ����+1(�������) 
	length = strlen(msg)+1;
	
	//���ڿ� ���̸�ŭ �޸� �Ҵ� 
	deliverySystem[x][y].context = (char*)malloc(sizeof(char)*length);
	
	//�Ҵ����ְ� copy 
	strcpy(deliverySystem[x][y].context,msg);               
	              
	//������ �Ǵ��ϱ�(deliverySystem�� ���� �ʱ�ȭ����) 	
	if(deliverySystem[x][y].building==0 || deliverySystem[x][y].room ==0 || deliverySystem[x][y].passwd == NULL || deliverySystem[x][y].context == NULL)
		//successfully put the package
		return 0;	
	else 
		//failed to put
		return -1;
}



//extract the package context with password checking
//int x, int y : coordinate of the cell to extract
int str_extractStorage(int x, int y) {
	
	//password checking
	inputPasswd(x,y);
	
	//put the msg string on the screen and re_initialize the storage 
	if(inputPasswd == 0){
		printStorageInside(x,y);
		initStorage(x,y);
	//.context memory free 
		free(deliverySystem[x][y].context);
		//successfully extracted
		return 0;
	}
	//failed to extract
	else return -1;
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	
	int x,y;
	int cnt = 0;
	 
	for(x=0;x<systemSize[0];x++){
		for(y=0;y<systemSize[1];y++){
			if(deliverySystem[x][y].building == nBuilding && deliverySystem[x][y].room == nRoom)				
					printf(" -----------> Found a package in (%d, %d)", &x,&y);	
					cnt++;			
			}
		}
	return cnt;
	}	                
