/*
Name:Darren Ytsma
student number:301171125
SFU username:dytsma
Lecture section: CPMT 300 D100
instructor: Brian Booth
TA: Scott Kristjanson
*/

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
char* getTime() //return out the current time in weekday Month Day time year format, without the newline character
{
        time_t TheTime; //time
        time(&TheTime); //time
        char* TIME;
        TIME=ctime(&TheTime);
        int length=strlen(TIME);
        TIME[length-1]='\0';
        return TIME;
}


void inputs(int inputs) //checks for the appropate number of inputs
{
    if (inputs != 3) 
    {
        fprintf(stderr,"usage: lyrebird.server config_file.txt log_file.txt\n");//change this line to match what is required
        exit(0);
    }
}

void OpenReadFile(char * filename ) //only need a read one since, will create file for writing
{
	FILE * file;
	if(file=fopen(filename,"r"))
    {

    }
    else
    {	
    	 printf("The file %s does not exits, now exiting... \n",filename );
        exit(0);
    }
}

void fixchar(char* loc, char* fix)//gets rid of extra unwanted letters in char array by adding '\n' after .txt so we can strtok it
{
	int i=0;
	int m=0;
	int length=strlen(loc);
	for(i=0; i<length; i=i+1)
	{
		if(loc[i]=='.' && loc[i+1]=='t' && loc[i+2]=='x' && loc[i+3]=='t')
		{	
			m=i+4;
			loc[m]='\n';
			break;
		}
	}
}

void convertToNum(char* six, int* temp,int change ) // for changing the letters into the numbers
{
	for(change=0; change<6; change=change+1)
	{
		if(six[change]==' '){temp[change]=0;}
		if(six[change]=='a'){temp[change]=1;}
		if(six[change]=='b'){temp[change]=2;}
		if(six[change]=='c'){temp[change]=3;}
		if(six[change]=='d'){temp[change]=4;}
		if(six[change]=='e'){temp[change]=5;}
		if(six[change]=='f'){temp[change]=6;}
		if(six[change]=='g'){temp[change]=7;}
		if(six[change]=='h'){temp[change]=8;}
		if(six[change]=='i'){temp[change]=9;}
		if(six[change]=='j'){temp[change]=10;}
		if(six[change]=='k'){temp[change]=11;}
		if(six[change]=='l'){temp[change]=12;}
		if(six[change]=='m'){temp[change]=13;}
		if(six[change]=='n'){temp[change]=14;}
		if(six[change]=='o'){temp[change]=15;}
		if(six[change]=='p'){temp[change]=16;}
		if(six[change]=='q'){temp[change]=17;}
		if(six[change]=='r'){temp[change]=18;}
		if(six[change]=='s'){temp[change]=19;}
		if(six[change]=='t'){temp[change]=20;}
		if(six[change]=='u'){temp[change]=21;}
		if(six[change]=='v'){temp[change]=22;}
		if(six[change]=='w'){temp[change]=23;}
		if(six[change]=='x'){temp[change]=24;}
		if(six[change]=='y'){temp[change]=25;}
		if(six[change]=='z'){temp[change]=26;}
		if(six[change]=='#'){temp[change]=27;}
		if(six[change]=='.'){temp[change]=28;}
		if(six[change]==','){temp[change]=29;}
		if(six[change]=='\''){temp[change]=30;}
		if(six[change]=='!'){temp[change]=31;}
		if(six[change]=='?'){temp[change]=32;}
		if(six[change]=='('){temp[change]=33;}
		if(six[change]==')'){temp[change]=34;}
		if(six[change]=='-'){temp[change]=35;}
		if(six[change]==':'){temp[change]=36;}
		if(six[change]=='$'){temp[change]=37;}
		if(six[change]=='/'){temp[change]=38;}
		if(six[change]=='&'){temp[change]=39;}
		if(six[change]=='\\'){temp[change]=40;}
	}
}


void convertTochar(char* encrypted_string, int* temp,int change,int group_start )// for changing the numbers into the letters
{
	for(change=0; change<=5; change=change+1) //change back into characters
	{
		if(temp[change]==0){encrypted_string[change+group_start]=' ';}
		if(temp[change]==1){encrypted_string[change+group_start]='a';}
		if(temp[change]==2){encrypted_string[change+group_start]='b';}
		if(temp[change]==3){encrypted_string[change+group_start]='c';}
		if(temp[change]==4){encrypted_string[change+group_start]='d';}
		if(temp[change]==5){encrypted_string[change+group_start]='e';}
		if(temp[change]==6){encrypted_string[change+group_start]='f';}
		if(temp[change]==7){encrypted_string[change+group_start]='g';}
		if(temp[change]==8){encrypted_string[change+group_start]='h';}
		if(temp[change]==9){encrypted_string[change+group_start]='i';}
		if(temp[change]==10){encrypted_string[change+group_start]='j';}
		if(temp[change]==11){encrypted_string[change+group_start]='k';}
		if(temp[change]==12){encrypted_string[change+group_start]='l';}
		if(temp[change]==13){encrypted_string[change+group_start]='m';}
		if(temp[change]==14){encrypted_string[change+group_start]='n';}
		if(temp[change]==15){encrypted_string[change+group_start]='o';}
		if(temp[change]==16){encrypted_string[change+group_start]='p';}
		if(temp[change]==17){encrypted_string[change+group_start]='q';}
		if(temp[change]==18){encrypted_string[change+group_start]='r';}
		if(temp[change]==19){encrypted_string[change+group_start]='s';}
		if(temp[change]==20){encrypted_string[change+group_start]='t';}
		if(temp[change]==21){encrypted_string[change+group_start]='u';}
		if(temp[change]==22){encrypted_string[change+group_start]='v';}
		if(temp[change]==23){encrypted_string[change+group_start]='w';}
		if(temp[change]==24){encrypted_string[change+group_start]='x';}
		if(temp[change]==25){encrypted_string[change+group_start]='y';}
		if(temp[change]==26){encrypted_string[change+group_start]='z';}
		if(temp[change]==27){encrypted_string[change+group_start]='#';}
		if(temp[change]==28){encrypted_string[change+group_start]='.';}
		if(temp[change]==29){encrypted_string[change+group_start]=',';}
		if(temp[change]==30){encrypted_string[change+group_start]='\'';}
		if(temp[change]==31){encrypted_string[change+group_start]='!';}
		if(temp[change]==32){encrypted_string[change+group_start]='?';}
		if(temp[change]==33){encrypted_string[change+group_start]='(';}
		if(temp[change]==34){encrypted_string[change+group_start]=')';}
		if(temp[change]==35){encrypted_string[change+group_start]='-';}
		if(temp[change]==36){encrypted_string[change+group_start]=':';}
		if(temp[change]==37){encrypted_string[change+group_start]='$';}
		if(temp[change]==38){encrypted_string[change+group_start]='/';}
		if(temp[change]==39){encrypted_string[change+group_start]='&';}
		if(temp[change]==40){encrypted_string[change+group_start]='\\';}
	}
}
