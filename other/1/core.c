#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>


void test(){
 char name[]="siva";
 printf("name = %p\n", name);
 printf("&name[0] = %p\n", &name[0]);
 printf("name printed as %%s is %s\n",name);
 printf("*name = %c\n",*name);
 printf("name[0] = %c\n", name[0]);
}

//void cpuload(){
//set_display_atrib(B_BLUE);
//char cpu[] = "teeeest";
//system("ps axc --format pid,%cpu,%mem,command --sort=-%cpu --headers | head -n 5");
//printf("%S",cpu[3]);
//}

int popa() {
    FILE *popen_result;
    char buff[512];
    popen_result = popen("ps axc --format pid,%cpu,%mem,command --sort=-%mem --headers | head -n 5", "r");
    
    while(fgets(buff, sizeof(buff), popen_result)!=NULL){
        printf("%s", buff);
    }
    pclose(popen_result);
    return 0;
}

void memload(){
//system("ps axc --format pid,%cpu,%mem,command --sort=-%mem --headers | head -n 5");
char test[] = "FUCKING SYSTEM";
for (int i = 0;i < strlen(test); i++){
printf("%c", test[i]);}
printf("\n");

}

int main (){
  popa();
//while (1){
//  test();
//  memload();
//  cpuload();
//  sleep (1);
//  system("clear");
//}

  return 0;
}
