#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include<errno.h>
//function 1
int input(){
int fd2;
char susn[12],book_name[30],email[20];
char due_day[2],due_month[2],due_year[4];
printf("enter USN,book name,email\n");
scanf("%s",susn);
scanf("%s",book_name);
scanf("%s",email);
struct stat fileStat;
stat("userinfo.txt", &fileStat);

time_t currentTime;
 time(&currentTime);
 struct tm *myTime = localtime(&currentTime);
   myTime->tm_mday +=7;
    mktime(myTime);
 



fd2=open("userinfo.txt",O_APPEND|O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);


if(stat("userinfo.txt",&fileStat))
 printf("%s", strerror(errno));
else if(fileStat.st_size <=1)
    printf("file is empty");
else {
printf("added nextline\n");
//write(fd2,"\n",1);
}


write(fd2,susn,strlen(susn));
write(fd2,";",1);
write(fd2,book_name,strlen(book_name));
write(fd2,";",1);
write(fd2,email,strlen(email));
write(fd2,";",1);

sprintf(due_day, "%d", (myTime->tm_mday));
write(fd2,due_day,strlen(due_day));
sprintf(due_month, "%d", (myTime->tm_mon +1));
write(fd2,"/",1);
write(fd2,due_month,strlen(due_month));
write(fd2,"/",1);

sprintf(due_year, "%d", (myTime->tm_year +1900));
write(fd2,due_year,strlen(due_year));
write(fd2,"\n",1);



close(fd2);
return 0;
}

int compare_currentdate_and_duedate(char date[]){
//dd/mm/yyyy;
 int flag=0,today_day,today_month,today_year;
 int  date_after_token[3];
 int i=0;
 time_t currentTime;
 time(&currentTime);
 struct tm *myTime = localtime(&currentTime);
 printf("%i/%i/%i\n",myTime->tm_mday,myTime->tm_mon +1,myTime->tm_year +1900);
 today_day=myTime->tm_mday;
 today_month=myTime->tm_mon +1;
 today_year=myTime->tm_year +1900;

 char* token =strtok(date,"/");   

  while(token!=NULL){
   date_after_token[i]=atoi(token);
   token =strtok(NULL,"/");
   i++;
 }
 for(int i=0;i<3;i++){
 printf("%d/",date_after_token[i]);

 }
 printf("\n");
 if(today_year<date_after_token[2]){flag=0;}
   else if(today_year>date_after_token[2]){flag=1;}
   else{ 
     if(today_month<date_after_token[1]){flag=0;}
     else if( today_month>date_after_token[1]){flag=1;}
      else{
        if (today_day<date_after_token[0]){flag=0;}
        else if(today_day>date_after_token[0]){flag=1;}
        else
           { 
         flag=1;  
         }
        }
     }
 



return flag;
}
//function 2
int putin_remainder_or_not(){
 int ctrl=0,ctrl2=0,fd2,i=0;
 char id[10], email[30],name[20];
 char date[11];
 size_t len=0;
 ssize_t read; 
 FILE * f1 = fopen("userinfo.txt","r");
     //char line[1024];
     char *line =NULL;
     char line1[1024];
    while ((read = getline(&line,&len,f1))!=-1){
           ctrl=0;
            strcpy(line1,line);
     char* token =strtok(line,";");
           strcpy(id,token);
           token=strtok(NULL,";");
           strcpy(name,token);
           
           token=strtok(NULL,";");
           strcpy(email,token);
           token=strtok(NULL,";");
           strcpy(date,token);
          // token=strtok(NULL,";");
           
             i=0;
           while(line1[i]!='\n'){
            i++;
            }
     ctrl=compare_currentdate_and_duedate(date);
         if(ctrl==1){
           ctrl2++;
           /* i=0;
           printf("same");
           while(line1[i]!='\n'){
            i++;
            }*/
          fd2=open("Remainder.txt",O_APPEND|O_WRONLY|O_CREAT,S_IRWXU|S_IRWXG|S_IRWXO);
           if(write(fd2,line1,i)==-1){
            perror("write error");
             exit(1);
           }
           if(write(fd2,"\n",1)==-1){
            perror("write error");
             exit(1);
           }

          
         printf("due date\n");
         close(fd2);
          
         }
         else{
         printf("not a due date\n");
         }
     
    }
  
   fclose(f1);
 return ctrl2;  
}
static void alarmHandler(int signo){
    printf("due date  reminders have been mailed.\n");
}

int main(){
int flag=0;
   int ch;
printf("enter 1 to take a book from library others to send reminder ");
scanf("%d",&ch);
switch(ch){
case 1 :  input();
          break;

default:printf("records are being checked\n");
}


flag=putin_remainder_or_not();
 signal(SIGALRM, alarmHandler);
    alarm(1);
     if(flag>0){sleep(1);}

return 0;
}
