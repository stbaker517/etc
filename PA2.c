#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

//================================================================
#define INPUTSIZE 201
#define COMANDSIZE 100
#define PIDSIZE 100
char input[INPUTSIZE];


char *comands[COMANDSIZE];
int ComIDX = 0;

int fdsPipe[2*COMANDSIZE];
int pids[COMANDSIZE];
//pid_t pids[PIDSIZE];
//int pidIDX=0;

//=================================================================

//[control inputs]==================
void getStdInput();
void getComandsFromInput();
//==================================

//[main excution]===================
void excuteMainFunc(int n); //excute n-th comand line.
//==================================

//[comand implemented]==============
int isComandValid(char *);
void evalCMD(char *,int);
//==================================

int main(int argc, char **argv)
{

    while (1)
    {
        getStdInput();
        for(int i=1;i<ComIDX;i++){
            excuteMainFunc(i);
        }
        excuteMainFunc(0);
        
        for(int i=0;i<ComIDX;i++)
        {
            close(fdsPipe[2*i]);
            close(fdsPipe[2*i+1]);
        }
    }
}

void getStdInput()
{
    //memset(input, 0, INPUTSIZE);
    //read(STDIN_FILENO, input, INPUTSIZE);
    
    size_t size;
    char* tmp=NULL;
    
    getline(&tmp,&size,stdin);
    tmp[strlen(tmp)-1]=0;
    strcpy(input,tmp);

    free(tmp);
    

    
    getComandsFromInput();

    return;
}

void getComandsFromInput()
{
    ComIDX = 0;
    for (int i = 0; i < COMANDSIZE; i++)
    {
        comands[i] = NULL;
    }

    char *temp = strtok(input, "|"); //파이프 기준으로 문자열 자르기

    while (temp != NULL)
    { //널이 아닐때까지 반복
        comands[ComIDX] = temp;
        //printf("strtok TEST: %s\n",comands[ComIDX]);

        temp = strtok(NULL, "|"); //널문자를 기준으로 다시 자르기
        //printf("%s\n\n",comands[ComIDX]);
        ComIDX++;
    }
    printf("COMIDX:%d\n", ComIDX);

    for(int i=0;i<ComIDX;i++){
        pipe(fdsPipe+2*i);
    }
}

void excuteMainFunc(int numOfComand)
{ //excute n-th comand line.


    //if(pipe(fd)<0)eixt(1);
    //printf("%d excution\n",numOfComand);
    if (numOfComand == ComIDX)
        return;
    if ((pids[numOfComand] = fork()) == 0)
    {
        if(numOfComand!=0)//not start
        {
            dup2(fdsPipe[2*(numOfComand-1)],STDIN_FILENO);
        }
        if(numOfComand!=ComIDX-1){
            dup2(fdsPipe[2*(numOfComand)+1],STDOUT_FILENO);
        }

        //char path[200];
        //memset(path, 0, 200);
        //sprintf(path, "/bin/%s", comands[numOfComand]);
        //execve("/bin/ls", comands[numOfComand], NULL);
        //excuteMainFunc(numOfComand + 1);
        evalCMD(comands[numOfComand],numOfComand);
        //printf("TEST [%d]:%s\n", numOfComand, path);

        
        exit(0);
    }

}

void evalCMD(char *Comand,int num)
{
    /*
        cmd_type1 ::= { ls, man, grep, sort, awk, bc }
        path
            path ::= { pathname with leading "./" }
        cmd_type2 ::= { head, tail, cat }
        cmd_type3 ::= { mv, rm, cp, cd }
        cmd_type4 ::= { pwd, exit }

        l cmd_type1: Programs that need to be loaded (fork-exec *) and run
        l cmd_type2: Program to be implemented (with arguments)
        l cmd_type3: Program to be implemented (with arguments)
        l cmd_type4: Program to be implemented (without arguments)
    */

    if (Comand == NULL)
    {
        printf("[evalCMD]: argument is NULL -%d\n",num);
    }
    printf("[%s]\n",Comand);

    char *cmd[100];
    char *tmp = strtok(Comand," ");

    int cmdidx = 0;
    int redirec=0;
    int redirecIdx=0;
    char redirecfile[1024];
    int direcfd;
    while (tmp != NULL)
    {

        if (strncmp(tmp," ",1)!=0)
            cmd[cmdidx++] = tmp;
        if(strncmp(tmp,"<",1)==0)
        {  
            redirec=1;
            redirecIdx=cmdidx-1;

        }
        else if(strncmp(tmp,">>",2)==0)
        {  
            redirec=2;
            redirecIdx=cmdidx-1;
        }
        else if(strncmp(tmp,">",1)==0)
        {  
            redirec=3;
            redirecIdx=cmdidx-1;
        }
        tmp = strtok(NULL, " ");
    }
    char hlk[]={'a','b','s',0};

    cmd[cmdidx++] = tmp;
    cmd[cmdidx] = NULL;

    if(redirec){
        
        cmd[redirecIdx]=NULL;

        
        sprintf(redirecfile,"./%s",cmd[redirecIdx+1]);
        
        if(redirec==1){
            if((direcfd=open(redirecfile,O_RDONLY))<0){
                printf("open error!\n");
                exit(1);
            }
            dup2(direcfd,STDIN_FILENO);
        }
        else if(redirec==2){
            if((direcfd=open(redirecfile,O_APPEND))<0){
                printf("open error!\n");
                exit(1);
            }
            dup2(direcfd,STDOUT_FILENO);
            write(STDOUT_FILENO,hlk,4);
        }
        else{
            if((direcfd=open(redirecfile,O_WRONLY|O_CREAT,644))<0){
                printf("open error!\n");
                exit(1);
            }
            printf("direcfd:%d  \n",direcfd);
            dup2(direcfd,STDOUT_FILENO);
            printf("STDOUT: %d\n",STDOUT_FILENO);
            write(STDOUT_FILENO,hlk,4);
        }
        //close(direcfd);
    }
    
    
    char src1[100];
    char src2[100];
    
    char path[100];
    sprintf(path,"/bin/%s",cmd[0]);


    //cmd_type1
    if (strncmp("ls", cmd[0], 2) == 0)
    { //fork
        if(fork()==0){
            if(execv(path, cmd)<0){
                perror("why");
            }
            exit(1);
        }
        else wait(NULL);
    }
    else if (strncmp("echo", cmd[0], 4) == 0)
    { //fork
        if(fork()==0){
            if(execv(path, cmd)<0){
                perror("why");
            }
            exit(1);
        }
        else wait(NULL);
    }
    else if (strncmp("man", cmd[0], 3) == 0)
    { //fork
        if(fork()==0){
            if(execv(path, cmd)<0){
                perror("why");
            }
            exit(1);
        }
        else wait(NULL);
    }
    else if (strncmp("grep", cmd[0], 4) == 0)
    { //fork
        if(fork()==0){
            if(execv(path, cmd)<0){
                perror("why");
            }
            exit(1);
        }
        else wait(NULL);
    }
    else if (strncmp("sort", cmd[0], 4) == 0)
    { //fork
        if(fork()==0){
            if(execv(path, cmd)<0){
                perror("why");
            }
            exit(1);
        }
        else wait(NULL);
    }
    else if (strncmp("awk", cmd[0], 3) == 0)
    { //fork
        if(fork()==0){
            if(execv(path, cmd)<0){
                perror("why");
            }
            exit(1);
        }
        else wait(NULL);
    }
    else if (strncmp("bc", cmd[0], 2) == 0)
    { //fork
        if(fork()==0){
            if(execv(path, cmd)<0){
                perror("why");
            }
            exit(1);
        }
        else wait(NULL);
    }
    else if (strncmp("./", cmd[0], 2) == 0)
    { //fork
        if(fork()==0){
            if(execve("a.out", cmd, NULL)<0){
                perror("why");
            }
            exit(1);
        }
        else wait(NULL);
    }
    //cmd_type2
    else if (strncmp("head", cmd[0], 4) == 0)
    {
        char buf[1];
        sprintf(src1, "./%s", cmd[1]);

        int fd = open(src1, O_RDONLY);
        int lines = 0;
        int n;
        if (cmd[2] == NULL)
            n = 10;
        else
            n = cmd[2][0] - '0';
        while ((read(fd, buf, 1)) > 0)
        {
            write(STDOUT_FILENO, buf, 1);

            if (buf[0] == '\n')
                lines++;
            if (lines == n)
                break;
        }
        close(fd);
    }
    else if (strncmp("tail", cmd[0], 4) == 0)
    {
        char buf[1];
        sprintf(src1, "./%s", cmd[1]);

        int fd = open(src1, O_RDONLY);
        lseek(fd,-1,SEEK_END);
        int lines = 0;
        int n;
        if (cmd[2] == NULL)
            n = 10;
        else
            n = cmd[2][0] - '0';
        while(read(fd,buf,1)>0){
            if(buf[0]=='\n')lines++;
            if(lines==n)break;
            lseek(fd,-2,SEEK_CUR);
        }



        while ((read(fd, buf, 1)) > 0)
        {
            write(STDOUT_FILENO, buf, 1);
        }

        printf("\n");
        close(fd);
    }
    else if (strncmp("cat", cmd[0], 3) == 0)
    {
        char buf[1];
        sprintf(src1, "./%s", cmd[1]);
        int tmpfd=open(src1,O_RDONLY);
        
        while(read(tmpfd,buf,1)>0){
            write(STDOUT_FILENO,buf,1);
        }
        close(tmpfd);
    }
    //cmd_type3
    else if (strncmp("mv", cmd[0], 2) == 0)
    {
        sprintf(src1, "./%s", cmd[1]);
        sprintf(src2, "./%s", cmd[2]);
        rename(src1, src2);
    }
    else if (strncmp("rm", cmd[0], 2) == 0)
    {
        sprintf(src1, "./%s", cmd[1]);
        unlink(src1);
    }
    else if (strncmp("cp", cmd[0], 2) == 0)
    {
        char buf[1];
        sprintf(src1, "./%s", cmd[1]);
        sprintf(src2, "./%s", cmd[2]);
        int fd_from=open(src1, O_RDONLY);
        int fd_to = open(src2, O_WRONLY | O_CREAT, 0755);
        while(read(fd_from,buf,1)>0){
            write(fd_to,buf,1);
        }
        close(fd_from);
        close(fd_to);
    }
    else if (strncmp("cd", cmd[0], 2) == 0)
    {
        chdir(cmd[1]);
    }
    //cmd_type4
    else if (strncmp("pwd", cmd[0], 3) == 0)
    {
        char cwd[1024];
        getcwd(cwd,sizeof(cwd));
        printf("%s\n",cwd);
    }
    else if (strncmp("exit", cmd[0], 4) == 0)
    {
    }
    return;
}
