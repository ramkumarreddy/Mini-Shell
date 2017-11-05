#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>

#define Delimiters " \t\r\n\a"

char *read_line()
{
	char *line = NULL;
	ssize_t buffersize = 0;
	getline(&line ,&buffersize , stdin);
//	printf("line=%s\n",line);
	return line;
}

char **split_line(char *line)
{
	int buffersize = 256;
	int position = 0;
	char **tokens = malloc(buffersize * sizeof(char *));
	char *token;

	token = strtok(line , Delimiters);
	while(token != NULL)
	{
		tokens[position] = token;
		token = strtok(NULL , Delimiters);
		position++;
	}
//	printf("%s\n",tokens[1]);
	tokens[position] = NULL;
	return tokens;
}

char execute_command(char **args)
{
	pid_t pid;
	pid = fork();
	if(pid==0)
	{
//		execl("/bin/bash","/bin/bash","-c",*args,NULL);
		execvp(*args,args);
	}
}
char Line[10000];


int run_command(char **args,char *hostname,char *homedire,char *currdire,char *line1)
{
	int p,q,r,s;
	p = strcmp(args[0] , "cd" );
	q = strcmp(args[0] , "pwd" );
	r = strcmp(args[0] , "exit" );
	s = strcmp(args[0] , "echo" );
	if(r==0)
		return 2;
	if(p==0 || q==0 || s==0)
	{
		if(p==0)
		{
			if(strcmp(args[1],".")==0)
			{
				fputs(hostname, stdout);
				fputs(currdire, stdout);
				printf(">");
				return 3;
			}
			else if(strcmp(args[1],"~")==0)
			{
				chdir(homedire);
			}
			else
			{
				chdir(args[1]);
			}
		}
		if(q==0)
		{
//			getcwd(currdire ,sizeof(currdire));
			printf("%s\n",currdire);
		}
		if(s==0)
		{
			int n=5;
			int flag1=0,flag2=0,flag3=0;
			while(line1[n+1]!='\0')
			{
				if(flag1==0)
				{
					if(line1[n]==' ')
					{
						n++;
						continue;
					}
					else
						flag1==1;
				}
				if(line1[n]=='"' && flag3==0)
				{
					if(flag2==0)
						flag2==1;
					else
						flag2==0;
					n++;
					continue;
				}
				if(line1[n]=='\'' && flag2==0)
				{
					if(flag3==0)
						flag3==1;
					else
						flag3==0;
					n++;
					continue;
				}
				printf("%c",line1[n]);
				n++;
			}
			printf("\n");
		}
	}
	else
	{
		execute_command(args);
	}
	return 1;
}

int main()
{
	int status;
	char shell_name[3];
	char hostname[1024],hostname1[1024];
	char *line,line1[1024];
	char **args;
	gethostname(hostname, 1024);
	strcat(hostname,":");
	int c=1;
	line=(char*)malloc(1024*sizeof(char));
	
	char currdire[1024];
	char homedire[1024];
	getcwd(homedire , sizeof(homedire));
	strcpy(hostname1,hostname);
	strcat(hostname1,homedire);
	fputs(hostname1 , stdout);
	printf(">");
	while(c)
	{
//		int qq=0,flag=1;		
		int ii=0;
		getcwd(currdire ,sizeof(currdire));
		char *Line = NULL;
		ssize_t buffersize = 0;
		getline(&Line ,&buffersize , stdin);
		//fgets(Line,10000,stdin);
		int jj=strlen(Line)-1;
		while(ii<jj)
		{
			int kk=0;
			while(ii<jj && Line[ii]!=';')	
				{
					line[kk]=Line[ii];
					ii++;
					kk++;
				}	
			ii++;
			line[kk]='\0';
		//line = read_line();
		strcpy(line1,line);
//		while(line[qq]!='\0')
//		{
//			if(line[qq]==';')
//			{
//				flag=1;
		args = split_line(line);
		int ss;
		ss = run_command(args,hostname,homedire,currdire,line1);
		if(ss==2)
			break;
		if(ss==3)
			continue;
		//free(line);
		free(args);
		wait(3);
		}
		getcwd(currdire ,sizeof(currdire));
		fputs(hostname, stdout);
		fputs(currdire , stdout);
		printf(">");
	}
	return 0;
}

