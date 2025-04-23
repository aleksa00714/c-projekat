#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_IME 20+1
#define GRANDSLAM 4

typedef struct teniser_st{
  unsigned score;
  char ime[MAX_IME];
  char prezime[MAX_IME];
  char grand_slam[GRANDSLAM][GRANDSLAM+1];
}TENISER;

typedef struct list_st{
  TENISER teniser;
  struct list_st* pnext;
}LIST;

void initialization(LIST** head){
  *head=NULL;
}
LIST* create_item(TENISER tenisplayer){
  LIST* new=malloc(sizeof(LIST));
  if(new == NULL){
    printf("Download more RAM!\n");
    exit(1);
  }
  new->teniser=tenisplayer;

  return new;
}
void add_to_list(LIST** head,TENISER tenisplayer){

  LIST* new=create_item(tenisplayer);

    new->pnext=*head;
    *head=new;

}
void destroy_list(LIST* head){

  LIST* curr=head;
  while(curr != NULL){
    head=head->pnext;
    free(curr);
    curr=head;
  }
  initialization(&head);
}

void read_from_file(FILE *pf,LIST** head){

  TENISER temp;
  while(fscanf(pf,"%s %s %s %s %s %s",temp.ime,temp.prezime,temp.grand_slam[0],temp.grand_slam[1],temp.grand_slam[2],temp.grand_slam[3]) != EOF){
    add_to_list(head,temp);
  }
}
void table_of_points(TENISER* tenisplayer){

  tenisplayer->score=0;
  int i=0;
  for(;i<GRANDSLAM;i++){
    if(!strcmp(tenisplayer->grand_slam[i],"W")){
      tenisplayer->score+=2000;
    } else if(!strcmp(tenisplayer->grand_slam[i],"F")){
      tenisplayer->score+=1200;
    } else if(!strcmp(tenisplayer->grand_slam[i],"SF")){
      tenisplayer->score+=720;
    } else if(!strcmp(tenisplayer->grand_slam[i],"QF")){
      tenisplayer->score+=360;
    } else if(!strcmp(tenisplayer->grand_slam[i],"R16")){
      tenisplayer->score+=180;
    } else if(!strcmp(tenisplayer->grand_slam[i],"R32")){
      tenisplayer->score+=90;
    } else if(!strcmp(tenisplayer->grand_slam[i],"R64")){
      tenisplayer->score+=45;
    } else if(!strcmp(tenisplayer->grand_slam[i],"R128")){
      tenisplayer->score+=10;
    }
  }
}
void calculate_points(LIST** head){

  if(*head != NULL){
    table_of_points(&((*head)->teniser));
    calculate_points(&((*head)->pnext));
  }
}
void sort_list(LIST** head){

  LIST *temp=*head;
  LIST *iter=*head;
  TENISER pomocnik;
  while(temp != NULL){
    while(iter != NULL){
      if(iter->teniser.score > temp->teniser.score){
        pomocnik=iter->teniser;
        iter->teniser=temp->teniser;
        temp->teniser=pomocnik;
      }
      iter=iter->pnext;
  }
    temp=temp->pnext;
    iter=temp;
 }
}
void write_to_file(FILE* pf,LIST* head,int broj_tenisera,int counter){

  if(head != NULL && counter < broj_tenisera){
    fprintf(pf,"%s %s %u\n",head->teniser.ime,head->teniser.prezime,head->teniser.score);
    counter++;
    write_to_file(pf,head->pnext,broj_tenisera,counter);
  }
}

FILE* safe_open(char* filename,char* mood){

  FILE* pf=fopen(filename,mood);
  if(pf == NULL){
    printf("I can't open '%s'.\nPlease try again!\n",filename);
    exit(1);
  }
  return pf;
}
void check_arguments(int argv){
  if(argv != 4){
    printf("Please call program properly!\nLike this'./tenis ulaz.txt izlaz.txt broj'\n");
    exit(0xDED5EC);
  }
}
void print_list(LIST *head){

  if(head != NULL){
    printf("%s %u\n",head->teniser.ime,head->teniser.score);
    print_list(head->pnext);
  }
}
int main(int argv, char* arg[]){

  LIST* head;
  initialization(&head);
  check_arguments(argv);

    FILE* filein=safe_open(arg[1],"r");
    read_from_file(filein,&head);
    calculate_points(&head);

    sort_list(&head);
    FILE* fileout=(safe_open(arg[2],"w"));
    int counter=0;
    write_to_file(fileout,head,atoi(arg[3]),counter);

      fclose(fileout);
      fclose(filein);

      destroy_list(head);

  return 0;
}
