/**
 *  Header converter for pdp8 AYT.
 *  Decodes:
 *
 *  __Header lbl name flags vector
 *  __Colon MUL2 2*
 *  __Immed DOTQUO ."
 *  __Const US_TIB TIB
 **/
 
#include <stdio.h>
#include <string.h>

FILE *gSrc, *gDst;

#define kMaxLen 128
#define kMaxSymLen 32

char gSrcLine[kMaxLen];
char gHeaderTxt[kMaxSymLen], gLabel[kMaxSymLen], gName[kMaxSymLen],gVector[kMaxSymLen];
int gFlags;

void Usage(void)
{
	printf(" AytHead src");
}

void Header(char *header, char *label, char *name, int flags, char *vector)
{
	char delim='\\';
	printf("\tTEXT %c%c%s%c\n",delim,(int)strlen(name)+' '+flags,name,delim);
	printf("%s,\n\t%s\n",label,vector);
}

int main(int argc, char *argv[])
{
	char *src;
	if(argc<2)
		Usage();
	if((gSrc=fopen(argv[1],"r"))==NULL)
		Usage();
	while(!feof(gSrc)) {
		fgets(gSrcLine,kMaxLen,gSrc);
		if(strncmp(gSrcLine,"__Header",8)==0) {
			sscanf(gSrcLine,"%s %s %s %d %s",gHeaderTxt,gLabel,gName,&gFlags,gVector);
			Header(gHeaderTxt,gLabel,gName,gFlags,gVector);
		}
		else if(strncmp(gSrcLine,"__Colon",7)==0) {
			sscanf(gSrcLine,"%s %s %s",gHeaderTxt,gLabel,gName);
			Header(gHeaderTxt,gLabel,gName,0,"ENTER");
		}
		else if(strncmp(gSrcLine,"__Immed",7)==0) {
			sscanf(gSrcLine,"%s %s %s",gHeaderTxt,gLabel,gName);
			Header(gHeaderTxt,gLabel,gName,040,"ENTER");
		}
		else if(strncmp(gSrcLine,"__Const",7)==0) {
			sscanf(gSrcLine,"%s %s %s",gHeaderTxt,gLabel,gName);
			Header(gHeaderTxt,gLabel,gName,0,"CONSTD");
		}
		else {
			printf("%s\n",gSrcLine);
		}
	}
	fclose(gSrc);
}