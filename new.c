#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct Node
{
	char place[40];
	int hour;
    int minute;
	struct Node *next;
}Node;
typedef struct List
{
	Node *head;
	Node *tail;
}List;
Node *makenew(char *place,int hour, int minute)
{
	Node *N=(Node*)malloc(sizeof(Node));
	if(N==NULL)
	{
		printf("Ko du bo nho.\n\n");
		exit(1);
	}
	N->next=NULL;
	strcpy(N->place,place);
	N->hour=hour;
    N->minute=minute;
	return N;
}
List Init(List list)
{
	list.head=list.tail=NULL;
	return list;
}
List addHead(List list,char *place,int hour, int minute)
{
	Node *N=makenew(place,hour,minute);
	if(list.head==NULL) list.head=list.tail=N;
	else
	{
		N->next=list.head;
		list.head=N;
	}
	return list;
}
List addTail(List list,char *place,int hour, int minute)
{
	if (list.head==NULL) list=addHead(list,place,hour,minute);
	else
	{
		Node *N=makenew(place,hour,minute);
		list.tail->next=N;
		list.tail=N;
	}
	return list;
}
List importfromfile(List list,char *filename)
{
    char place[40];
	int hour;
    int minute;
    FILE *f=fopen(filename,"r");
    if(!f)
    {
        printf("Ko tim thay file tu dien.");
        exit(1);
    }
    while(fscanf(f,"%s %d %d",place,&hour,&minute)==3)
    {
        list=addTail(list,place,hour,minute);
    }
    fclose(f);
    return list;
}
void output(List list){
    printf("Lich su di chuyen:\n");
	while(list.head!=NULL){
		printf("%50s%10d%10d\n", list.head->place, list.head->hour, list.head->minute);
		list.head=list.head->next;
	}
	printf("\n");
}
Node *hadplace(List list,char *place)
{
	Node *tmp=list.head;
	while(tmp)
	{
		if(strcmp(tmp->place,place)==0) return tmp;
		tmp=tmp->next;
	}
    return NULL;
}
Node *hadtime(List list,int hour,int minute)
{
	Node *tmp=list.head;
	while(tmp)
	{
		if(tmp->hour==hour&&tmp->minute==minute) return tmp;
		tmp=tmp->next;
	}
    return NULL;
}
Node *checkalltime(List list,char *place,int hour,int minute)
{
    if(!hadplace(list,place)) return NULL;
    Node *tmp=list.head;
	while(tmp)
	{
		if(strcmp(tmp->place,place)==0)
        {
            if(tmp->hour>hour) return tmp;
            if(tmp->hour==hour&&tmp->minute>=minute) return tmp;
        } 
		tmp=tmp->next;
	}
    return NULL;
}
void searchplace(List list)
{
    char place[40];
    printf("Nhap dia diem:");
    fflush(stdin);
    gets(place);
    if(!hadplace(list,place))
    {
        printf("Ban chua toi dia diem do.");
        return;
    }
	Node *tmp=list.head;
	while(tmp)
	{
		if(strcmp(tmp->place,place)==0)
        {
            printf("%d:%d",tmp->hour,tmp->minute);
            break;
        }
		tmp=tmp->next;
	}
    tmp=tmp->next;
    while(tmp)
    {
        if(strcmp(tmp->place,place)==0) printf(", %d:%d",tmp->hour,tmp->minute);
		tmp=tmp->next;
    }
    printf("\n");
}
void searchtime(List list)
{
    int hour,minute;
    printf("Nhap gio va phut:");
    scanf("%d %d",&hour,&minute);
    while(hour>=24||hour<0||minute>=60||minute<0)
    {
        printf("Gio hoac phut ko hop le. Nhap lai:");
        scanf("%d %d",&hour,&minute);
    }
    if(!hadtime(list,hour,minute)) printf("Khong tim thay lich su di chuyen!\n");
	Node *tmp=list.head;
	while(tmp!=NULL)
	{
		if(tmp->hour==hour&&tmp->minute==minute) printf("%s\n",tmp->place);
		tmp=tmp->next;
	}
    return;
}
void checkforcovidrisk(List list)
{
    char s[100];
    char string[3][100];
    char place[40];
    int hour,minute,i,j,k;
    for(j=0;j<3;j++)
    {
        strncpy(string[j],"\0",100);
    }
    j=0;
    printf("Nhap dia diem, thoi gian co benh nhan:");
    fflush(stdin);
    gets(s);
    fflush(stdin);
    for(i=0;i<strlen(s);i++)
    {
        if(s[i]!=' '&&s[i]!='\0') 
        {
            string[j][k]=s[i];
            k++;
        }
        else 
        {
            j++;
            k=0;
        }
    }
    strcpy(place,string[0]);
    hour=atoi(string[1]);
	minute=atoi(string[2]);
    if(!hadplace(list,place))
    {
        printf("Lich su di chuyen cua ban OK.\n");
        return;
    }
    if(checkalltime(list,place,hour,minute))
    {
        printf("Ban co kha nang bi lay Covid, can khai bao y te ngay lap tuc!\n");
        return;
    }
    printf("Lich su di chuyen cua ban OK.\n");
    return;
}
int main()
{
    int checkc1;
    char c[]="log.txt";
	List list;
	list=Init(list);
	int menu;
	while (1){
        printf("\n=============================\n");
        printf("CHUONG TRINH TRUY VET COVID19\n");
		printf("1. Nap file lich su di chuyen\n");
		printf("2. In ra lich su di chuyen\n");
		printf("3. Tim kiem lich su theo dia diem\n");
		printf("4. Tim kiem lich su theo thoi gian\n");
		printf("5. Kiem tra truy vet moi nhat\n");
        printf("6. Thoat\n");
		printf("Hay chon menu: ");
		scanf("%d", &menu);
		switch (menu){
			case 1:
            {
				list=importfromfile(list,c);
                checkc1++;
				break;
            }
			case 2:
                if(checkc1==0)
                {
                    printf("Chua nap file log.\n");
                    break;
                }
				if(list.head==NULL)
				{
					printf("Lich su trong.\n\n");
					break;
				}
				output(list);
				break;
			case 3:
			    if(checkc1==0)
                {
                    printf("Chua nap file log.\n");
                    break;
                }
				if(list.head==NULL)
				{
					printf("Lich su trong.\n\n");
					break;
				}
				searchplace(list);
			break;
			case 4:
            if(checkc1==0)
                {
                    printf("Chua nap file log.\n");
                    break;
                }
                if(list.head==NULL)
				{
					printf("Lich su trong.\n\n");
					break;
				}
			searchtime(list);
            break;
            case 5:
            if(checkc1==0)
                {
                    printf("Chua nap file log.\n");
                    break;
                }
                if(list.head==NULL)
				{
					printf("Lich su trong.\n\n");
					break;
				}
            checkforcovidrisk(list);
            break;
            case 6:
            {
				Node *del = list.head;
				while (del != NULL){
					list.head = list.head->next;
					free(del);
					del = list.head;
				}
				exit(0);
			}
				
			}
		}
}