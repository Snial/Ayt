/**
 * AytMod.c
 **/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define kLineLen 127
#define kTokLen 32
char aLine[kLineLen+1];
char param0[kTokLen+1];
char param1[kTokLen+1];
char param2[kTokLen+1];
int gLnk=0;

int makeTEXT(char *name, int flags)
{
	char delim='.';
	char *sPos=name;
	int len=strlen(name+1),ix;
	name[0]=64+len+flags;	// ASCII "@" to "Z" map to 0..25.
	while(*sPos!='\0' && delim>' ') {
		if(*sPos==delim) {
			sPos=name;	// reset the sPos pointer.
			delim--;
		}
		else
			sPos++;
	}
	return delim;
}

void ShowHeader(char *name, int flags, char *lbl, char *cfa)
{
	int len=strlen(name+1),ix;
	printf("LK%d,\n\tLK%d\n\t",gLnk+1,gLnk);
	name[0]=len+flags;
	for(ix=0;ix<=len;ix+=2) {
		printf("%o; ",(name[ix]&077)|((name[ix+1]&077)<<6));
	}
	printf("\t/ %s\n%s,\n\t%s\n",name+1,lbl,cfa);
	gLnk++;
}

// __Header BRA (BRANCH) 0 BRA+1
// Internal format is: Link, [Len+Opt]Text, CFA, Label
void ProcessHeader(char *aLine)
{
	int flags;
	sscanf(aLine+9,"%s %s %d %s",param0,param1+1,&flags,param2);
	ShowHeader(param1,flags,param0,param2);
}

// __Colon MUL2 2*
void ProcessColon(char *aLine,int flags)
{
	sscanf(aLine+8,"%s %s",param0,param1+1);
	ShowHeader(param1,flags,param0,"ENTER");
}

// __Const US_TIB TIB
void ProcessConst(char *aLine)
{
	sscanf(aLine+8,"%s %s",param0,param1+1);
	ShowHeader(param1,0,param0,"CONSTD");
}

int main(int argc, char *argv[])
{
	FILE *src;
	if(argc<2) {
		printf("Usage: AytMod link0 src ");
		return 0;
	}
	gLnk=atoi(argv[1]);
	if((src=fopen(argv[2],"r"))==NULL) {
		printf("Can't open %s",argv[1]);
		return 0;
	}
	while(!feof(src)){
		fgets(aLine,kLineLen,src);
		if(strncmp(aLine,"__Header ",9)==0)
			ProcessHeader(aLine);
		else if(strncmp(aLine,"__Colon ",8)==0)
			ProcessColon(aLine,0);
		else if(strncmp(aLine,"__Const ",8)==0)
			ProcessConst(aLine);
		else if(strncmp(aLine,"__Immed ",8)==0)
			ProcessColon(aLine,0x10);
		else if(strncmp(aLine,"__",2)==0)
			printf("*** Unknown Header: %s ***\n",aLine);
		else if(!feof(src))
			printf("%s",aLine);
	}
	fclose(src);
}