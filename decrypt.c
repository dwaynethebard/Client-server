/*
Name:Darren Ytsma
student number:301171125
SFU username:dytsma
Lecture section: CPMT 300 D100
instructor: Brian Booth
TA: Scott Kristjanson
*/
#include "decrypt.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "memwatch.h"
#include "FUNctions.h" 
void decode(char* file_input, char* file_output)
{
 
  FILE * encryptweet; //pointer for file
  encryptweet=fopen(file_input,"r"); //opens ecrypted file

  FILE * dencryptweet; //pointer for file
  dencryptweet=fopen(file_output,"w"); //opens blank file decrypted output and allows writing

	char *str; //dynamic memory for the strings

  if(encryptweet!=NULL)// makes sure the files is there
  {

    while(1) //go until break condition
    {
      str = (char *) malloc(200); //make memory for 150 bytes    
      fgets(str,200,encryptweet);
      if( feof(encryptweet) )//if end of file
      { 
        break ;
      }  
      decrypt(str);
      fputs(str,dencryptweet);
      free(str); //frees the memory

    }
    free(str); //frees the memory
    fclose(encryptweet); //closes the file
    fclose(dencryptweet); //closes the file
  } 
}

void decrypt(char* encrypted_string) 
	{
		int i=0; // counter for a variety of loops
		int length=strlen(encrypted_string);//length of the string


		int times=length;// how many spaces that will be removed
		times=times/8; //how many white spaces will be removed
	  
		int position=7;//where we will remove characters
	  
		if(length>8) //remove every 8th letter 
		{  
			while(position<length-1)//while not at end 
			{
				encrypted_string[position]=' ';
				i=i+1;
				position=7+i*8;
			}  
		}

	  
		int ahead;//counter for removing spaces
		int current=1;//counter
		int space=1;//counter for how many on the spaces are removed
	  
		if(length>7) //checks to see if it is long enough for spaces
		{ 
			while(space<=times)
			{
				current=7*space;
				while(current<length)
				{
					ahead=current+1;
					if(encrypted_string[current]==' ')
					{ 
						encrypted_string[current]=encrypted_string[ahead];
						encrypted_string[ahead]=' ';
					}
					current=current+1;
				}
			space=space+1;
			}
		}


	    length=strlen(encrypted_string);//length of string

		int six_group; //how many group of six characters that need to be decoded
		six_group=length/6;

		i=0;//counter for the legth of 6 strings
		while(i<six_group) //changes the string of 6 into number then back to char
		{ 
			int group_start=6*i;//jumps to start of every group of six characters
			char six[6]={encrypted_string[group_start],encrypted_string[group_start+1],encrypted_string[group_start+2],encrypted_string[group_start+3],encrypted_string[group_start+4],encrypted_string[group_start+5]};
			int temp[6];//holds the numbers before changing from base 41 to base 10
			int change=0;
			convertToNum(six,temp,change);

			unsigned long number=0UL; //the number in base 41
			

			number=temp[0]*pow(41,5)+temp[1]*pow(41,4)+temp[2]*pow(41,3)+temp[3]*pow(41,2)+temp[4]*pow(41,1)+temp[5]*pow(41,0);

			unsigned long expo=0UL; //raised to this power
			unsigned long mod=0UL; //what we are moding by
			

			unsigned long Base=0UL; // 
			unsigned long ans=0UL; // the number raised and moded
			
			

			mod=4294434817;
			expo=1921821779;
			ans=1;
				Base=number%mod;
				while(expo!=0)
				{
					if(expo%2==1)
					{
						ans=(ans*Base)%mod;

					}
					expo=expo/2;
					Base=(Base*Base)%mod;

				}

	  

			unsigned long base41[6];
			base41[0]=pow(41,5);
			base41[1]=pow(41,4);
			base41[2]=pow(41,3);
			base41[3]=pow(41,2);
			base41[4]=pow(41,1);
			base41[5]=pow(41,0);

			unsigned long quentent;
			int loop;
			unsigned long into;
			into=ans;
			quentent=0;
			for(loop=0; loop<=5; loop=loop+1)//change back to base 41
			{
				if(into>=base41[loop])
				{
					quentent=into/base41[loop];
					into=into-quentent*base41[loop];

					temp[loop]=quentent;
				}
				else
				{
					temp[loop]=0;
				}
			}

			
				convertTochar(encrypted_string,temp, change, group_start );
			
			i=i+1;
		}
	} 
