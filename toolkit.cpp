#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;

struct myStuff
{
				//struct that just holds our individual token
char token[80];



};

struct pipeStuff		//this struct stores our command and its arguments, this is used for piping/I/O redirection portion of the assignment
{
char command[25];
char *arguments[15];
char fullThing[80];
int argumentCount;
};


void pipeFunction(myStuff * myList,int tokenCount,char * what[]);
void redirectFunction(myStuff * myList,int x,char y[]);		//function prototypes for our functions that handle piping and redirection


int main()
{
char s[256];
char * s2[256];
char pipeInput[256];
char * pipeTokens[256];
string inputString;
printf("$ ");
int jayjay=0;
while(getline(cin,inputString))
{
myStuff myTokens[10];

int j=0;
int tokenCount=0;
int pipeCount=0;
int redirectCount=0;
	
	strcpy(s,"");

	strcpy(s,inputString.c_str());
	
	int h=0; 
	char* token = strtok(s, " ");
        s2[j]=token;

	j+=1;
	h+=1;
        while (token) 
        {
	token = strtok(NULL, " ");
        s2[j]=token;
                j+=1;
        }
tokenCount=j;
char temp[80];
strcpy(temp,"");
int counter=0;
for(int lu=0;lu<tokenCount-1;lu++)
{                                               //NOTE: tokenCount also counts the null character at the end, so it must always be tokenCount-1;

        if(strcmp(s2[lu],"|")==0)
        {                                       //here we tokenize all our tokens for each command by the | in order for us to be able to handle piping


                myStuff temp2;
                strcpy(temp2.token,temp);
                strcpy(temp,"");
                myTokens[pipeCount]=temp2;
                counter+=1;
                pipeCount+=1;
        }                                      
        else
        {
                if(strcmp(temp,"")==0)
                {						
                        strcpy(temp,s2[lu]);
                }
                else					//gather up our full command, including all arguments to be included as part of our token
                {
                        strcat(temp," ");
                        strcat(temp,s2[lu]);
                }
        }
}

if(pipeCount>0)
{
myStuff lastOne;
strcpy(lastOne.token,temp);
myTokens[pipeCount]=lastOne;		//get the last token/command we will need for piping
counter+=1;
pipeCount+=1;
}					
strcpy(temp,"");
int counterV2=0;
redirectCount=0;
char x[20];

for(int lu=0;lu<tokenCount-1;lu++)
{                                               //NOTE: tokenCount also counts the null character at the end, so it must always be tokenCount-1;

        if(strcmp(s2[lu],">")==0 || strcmp(s2[lu],"<")==0)
        {                                       


                myStuff temp2;
                strcpy(temp2.token,temp);
                strcpy(temp,"");
		if(strcmp(s2[lu],">")==0)
		{
		x[redirectCount]='>';		//store our redirect characters that we will be using
		}
		else
		{
		x[redirectCount]='<';
		}
                myTokens[redirectCount]=temp2;
                counterV2+=1;
                redirectCount+=1;
        }                                       
        else
        {
                if(strcmp(temp,"")==0)
                {
                        strcpy(temp,s2[lu]);
                }
                else
                {
                        strcat(temp," ");
                        strcat(temp,s2[lu]);
                }
        }
}

if(redirectCount>0)
{					//gets the last command that doesnt have a redirect symbol after it
myStuff lastOne;
strcpy(lastOne.token,temp);
myTokens[redirectCount]=lastOne;
counterV2+=1;
redirectCount+=1;
}

 if(strcmp(s2[0],"myexit")==0)
   {					//MYEXIT functionality is here. Simply if our command we type, with one argument is "myexit", end our shell program
		exit(1);
    }


else if(redirectCount>0)				//if we have found a redirect symbol, call our redirect function to take care of it for us
{
		redirectFunction(myTokens,counterV2,x);
		printf("$ ");

}

else if(pipeCount>0)
	{					//if we encountered a pipe, then call the pipe function to handle it for us

		pipeFunction(myTokens,counter,s2);
		printf("$ ");
	
	}

   else
   {

 	  pid_t pid =fork(); //create a new process
    	if (pid == 0) { //Child process
		if(strcmp(s2[0],"mypwd")==0)
		{				//MYPWD functionality is here. We compare if our input is mypwd or not, if it is call the pwd command. 
		  execvp("pwd",s2);
		}
               else if(strcmp(s2[0],"mycd")==0)
		     {

			chdir(s2[1]);		//change directory, allows for any type of path changes

	
		     }
    	    else if (execvp(s2[0], s2) == -1) {
        	    printf("Command not found\n");
           	 exit(1);
        	}
		else
		{
			execvp(s2[0],s2);
		}
    }
    else { 
	int status;				//inside the parent process, wait for the child to finish up
        waitpid(pid, &status, 0);
        //printf("Child process completed!\n");
    }


printf("$ ");
}


}
    return 0;
}


void redirectFunction(myStuff * myList,int tokenCount,char x[])
{
pipeStuff commandList[10];
int counter=0;
for(int s=0;s<tokenCount;s++)
{

        pipeStuff myStuff;
        myStuff.argumentCount=0;
  char* token = strtok(myList[s].token, " ");
        int j=0;
        strcpy(myStuff.command,token);			//this loop of code further tokenizes our tokens
        myStuff.arguments[j]=token;
        j+=1;						//what this means is that it allows us to get the arguments from the commands of our tokens
        while(token)
        {
        token=strtok(NULL," ");
        myStuff.arguments[j]=token;			//these are all stored as special structs, defined at beginning of the program
        if(token != NULL)
        {
        myStuff.argumentCount+=1;
        }
        j+=1;
        }
        commandList[counter]=myStuff;
        counter+=1;
}



if(tokenCount==2 && x[0]=='>')		//this handles the case for when we have command1 > output1.txt
{
pid_t pid;
pid=fork();
if(pid==0)
	{
		int f1 =open(commandList[1].command, O_WRONLY | O_CREAT, 0644);	//open the file we want to write to
										
		int f2=dup2(f1,STDOUT_FILENO);			//duplicate our file descriptor to take over standard output and make sure we close original
		close(f1);					
		execvp(commandList[0].command,commandList[0].arguments);	//execute our command
	}
	else
	{
		waitpid(pid,NULL,0);			//wait in parent process for child process to be completed. 
	}
}
else if(tokenCount==2 && x[0]=='<')
{

pid_t pid2;
pid2=fork();

if(pid2==0)
{
	int fd0 = open(commandList[1].command, O_RDONLY);	//open up the file we are reading from
        dup2(fd0, STDIN_FILENO);				
        close(fd0);						//duplication stdin and close the original
	execvp(commandList[0].command,commandList[0].arguments);
}
else
{
	waitpid(pid2,NULL,0);		//wait for child to be done
}

}

else
{

if(tokenCount==3 && x[0]=='<' && x[1]=='>')
{
pid_t myPid;
int p[2];
int z=0;
int myInput=0;

      pipe(p);
        myPid=fork();
      if (myPid== -1)
        {
		perror("Failed to fork");
        }
      else if (myPid == 0)
        {

		close(0);					//close our stdin
		 int fd0 = open(commandList[1].command, O_RDONLY);;	//open up a new file which will replace stdin
		close(1);					//close stdout			
		open(commandList[2].command, O_WRONLY | O_CREAT, 0644);
		execvp(commandList[0].command,commandList[0].arguments);	//open up a new file which will replace stdout

        }
      else
        {
          waitpid(myPid,NULL,0);

        }
			

}
	else
	{
		printf("Error! Following formats are accepted: cmd1 < file1.txt;   cmd1 > file1.txt ;  cmd 1 < file1.txt > file2.txt\n");
	}

}



}
void pipeFunction(myStuff * myList,int tokenCount, char * s2[])
{
	//printf("INSIDE PIPING FUNCTION");
	//printf("\n");
	int i=0;


pipeStuff commandList[10];
int counter=0;
for(int s=0;s<tokenCount;s++)
{

	pipeStuff myStuff;
	strcpy(myStuff.fullThing,"");
	myStuff.argumentCount=0;
  char* token = strtok(myList[s].token, " ");
	strcat(myStuff.fullThing,token);
        int j=0;
        char * green[80];
        strcpy(myStuff.command,token);
	myStuff.arguments[j]=token;
	j+=1;
	while(token)					//retokenize again in order to achieve all our arguments for each command we want to pipe
        {
	token=strtok(NULL," ");

       	myStuff.arguments[j]=token; 
	if(token != NULL)
	{
	myStuff.argumentCount+=1;
	}
	j+=1;
	}
	commandList[counter]=myStuff;
	counter+=1;
}

	if(tokenCount==2)
	{

		pid_t pid;
		int pipefd[2];


		pipe(pipefd);
		for(int z=0;z<tokenCount-1;z++)
		{					//this case is for a single pipe and was done this way to achieve practice with double forking


        		 pid =fork(); //create a new process
        		if (pid == 0) 
			{ //Child process
			 close(pipefd[0]);
     			 dup2(pipefd[1], 1);
      			close(pipefd[1]);
	 		if(strcmp(commandList[z].command,"mypwd")==0)
                	{                               //MYPWD functionality is here. We compare if our input is mypwd or not, if it is call the pwd command. 
                  	execvp("pwd",s2);
                	}
              		 else if(strcmp(commandList[z].command,"mycd")==0)
                     	{

                       	 chdir(commandList[z].arguments[1]);
                     	}
           		 else if (execvp(commandList[z].command, commandList[z].arguments) == -1) {
                    		printf("Command not found\n");
                 		
                	}
                	else
                	{
                        execvp(commandList[z].command,commandList[z].arguments);
                	}
		}
    
		else
		{
			pid_t mychildPID = fork();
  	    	if (mychildPID == 0) 
		{
													
		close(pipefd[1]);
       		 dup2(pipefd[0], 0);				//close and dup2 as necessary
        	close(pipefd[0]);

		if(strcmp(commandList[z+1].command,"mypwd")==0)
                {                               //MYPWD functionality is here. We compare if our input is mypwd or not, if it is call the pwd command. 
                  execvp("pwd",s2);
                }
               else if(strcmp(commandList[z+1].command,"mycd")==0)
                     {
			chdir(commandList[z].command);
			}									//perform special commands as needed
            else if (execvp(commandList[z+1].command, commandList[z+1].arguments) == -1) {
                    printf("Command not found\n");
                 exit(1);
                }
                else
                {
                        execvp(commandList[z+1].command,commandList[z+1].arguments);
                }
        }
	else
	{
		int status;	
		close(pipefd[0]);
        close(pipefd[1]);			//wait for child process to be complete
	waitpid(mychildPID,&status,0);
		//printf("Child process done");
	
	}
}
}

 }  
if(tokenCount>=3)
{

int z=0;
int myFD[2];
pid_t myPid;
int myInput=0;
for(z=0;z<tokenCount;z++)
{
      pipe(myFD);			
	myPid=fork();	//for each command, open up a pipe and fork
      if (myPid== -1)
        {

        }
      else if (myPid == 0)
        {
          dup2(myInput, 0); //change the input according to the old one 
          if (z!=tokenCount-1)
            {dup2(myFD[1], 1);}			//redirect file output
          close(myFD[0]);	//close off input
		 if(strcmp(commandList[z].command,"mypwd")==0)
                        {                               //MYPWD functionality is here. We compare if our input is mypwd or not, if it is call the pwd command. 
                        execvp("pwd",commandList[z].arguments);
                        }
                         else if(strcmp(commandList[z].command,"mycd")==0)
                        {
				chdir(commandList[z].command);
                        }
                         else if (execvp(commandList[z].command, commandList[z].arguments) == -1) {
                                printf("Command not found\n");

                        }
                        else						//execute our command if found
                        {
                        execvp(commandList[z].command,commandList[z].arguments);
                        }


        }
      else
        {
          waitpid(myPid,NULL,0);		//wait for child process to end
          close(myFD[1]);
          myInput = myFD[0]; //save the input for the next command

        }
    

}


}


}	

