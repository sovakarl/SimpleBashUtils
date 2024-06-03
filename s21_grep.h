#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>


typedef struct regex{
    char *regular;
    int status;
    struct regex *next;
} Regex;

void add(char *regular,Regex **now);

void fflag(char *filename,Regex **now);
int pars(int *flags,int argc,char *argv[],Regex **now);

int pars(int *flags,int argc,char *argv[],Regex **now){
    int flag;
    while((flag=getopt(argc,argv,"e:ivclnhsf:o"))!=-1){
        switch (flag)
        {
        case 'e':
            flags[0]=1;
            add(optarg,now);
            continue;
        case 'i':
            flags[1]=1;
            continue;
        case 'v':
            flags[2]=1;
            continue;
        case 'c':
            flags[3]=1;
            continue;
        case 'l':
            flags[4]=1;
            continue;
        case 'n':
            flags[5]=1;
            continue;
        case 'h':
            flags[6]=1;
            continue;
        case 's':
            flags[7]=1;
            continue;
        case 'f':
            flags[8]=1;
            fflag(optarg,now);
            continue;
        case 'o':
            flags[9]=1;
            continue;
        default:
            exit(1);
        }
    }
    return optind;
}

void add(char *regular,Regex **now){
    Regex *pointer=(Regex*)malloc(sizeof(Regex));
    pointer->status=0;
    pointer->next=NULL;
    (*now)->status=1;
    (*now)->regular=regular;
    (*now)->next=pointer;
    *now=pointer;
}

void fflag(char *filename,Regex **now){
    FILE *file=fopen(filename,"r");
    char data[1000];
    fgets(data,1000,file);
    add(data,now);
}

void freelist(Regex **head){
    Regex *current=*head;
    Regex *next; 
    while(current!=NULL){
    next=current->next;
    free((current));
    current=next;
    }
    *head=NULL;
}
