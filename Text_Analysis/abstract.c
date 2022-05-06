//��Ƶͳ���ۺϴ���ҵ 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define MAX 1000000  //���Ӹ������ֵ�Ĺ���ֵ 
int isalpha(char c){
	if((c<='Z'&&c>='A')||(c<='z'&&c>='a'))
	    return 1;
	else   return 0;
}

char tolower(char c){
	if(c<='Z'&&c>='A'){
		c=c-'A'+'a';
	}
	return c;
}

int isspace(char c){
    if(c=='\n'||c=='\t'||c==' '||c=='\v'||c=='\f'||c=='\r')	
        return 1;
    else return 0;
}

int N;  //�����Ҫ�ľ�������
int getWord(FILE *fp,char *w){   //������

	int c;
	
	while(isalpha(c=fgetc(fp))==0){
		if(c==EOF)  return c;
		else continue;
	}
	
	do{
		*w++=tolower(c);
	}while(isalpha(c=fgetc(fp)));

	*w='\0';
	return 1;  
}

unsigned int hash(char *str) {//��ϣ����
    unsigned int hash=0;
    
	while(*str){
		hash=(*str++)+(hash<<6)+(hash<<16)-hash;
	}
         
    return   hash%400009;
}

struct dot {//�������ʽڵ�Ķ��� 
	char word[30];
	int type;  //��ʶͣ�ôʺͷ�stop���0Ϊͣ�ã�1Ϊ��ͣ�� 
	struct dot* next;
	int number;
}; 
struct dot* tap[400200];


struct dot* create(struct dot* head,char w[],int kind){//�򵥴ʱ��������µ���     
	struct dot* p=NULL,*q=NULL,*r=NULL;
	
	p=(struct dot*)malloc(sizeof(struct dot));
	strcpy(p->word,w);
	p->next=NULL;
	p->number=0;
	
	if(kind==0)
	   p->type=0;
	else if(kind==1){
		p->type=1;
		p->number=1;
	}
	   
	if(head==NULL){
		head=p;
		return head;
	}
	   
	else{
		for(q=head;q!=NULL;r=q,q=q->next);
		  r->next=p;
		return head;
	}
}
struct dot* find(char w[]){//Ѱ�ҵ����ڹ�ϣ���е�λ�� ,�Ҳ����ʹ���   //�߼����ң��ӵ�ʱ���ظ��� 
	struct dot* q=NULL;
	int h;
	h=hash(w);

	for(q=tap[h];q!=NULL&&strcmp(q->word,w)!=0;q=q->next);
	
	if(q!=NULL){
	  return q;	
	}
	else   return NULL;
}


struct sen{//�洢���ӵĽṹ��
	char sentence[5000];
	int sum;
	int order;
}; 
struct sen lines[MAX];
int k;//���Ӹ��� 

int cmp(const void*p1,const void*p2)
{
	struct sen *a=(struct sen*)p1;
	struct sen *b=(struct sen*)p2;
	if(a->sum!=b->sum)	return b->sum-a->sum; 
	else return a->order-b->order ; 
}

void addSentence(int k){
    int i=0;
    int j=0;  //��һ���ǿ϶����ڵģ�����Ҫ�ж�Ϊ��Ϊ�㣬��Ϊ��ֱ��¼�뵽��һ������� 
	struct sen   tmp; 
	
	tmp=lines[k];
	for(i=0;i!=k&&i<=N-1&&lines[i].sum>=lines[k].sum;i++);//�ҵ��˲���λ��Ϊi 
	
	if(i==k||i>N-1) 
	    return;     //˵���Ѿ�����С������󣬲����ٸ���λ��
	if(k>=N-1) k=N-1;
	for(j=k;j>=i;j--)
	    lines[j+1]=lines[j];
	lines[i]=tmp;
    return;
}

int iscut (char c){   //���ӽ������ж� 
	return (c=='!'||c=='.'||c=='?');
}
int getLine(FILE *fp,int k){   //������ 
	char c;
	int i=0,j=0;
	char w[100];
	
	lines[k].order = k;
	lines[k].sum =0; 

		do{
			c=fgetc(fp);
		}while(isspace(c));
		ungetc(c,fp);
	
	i=0;
	while(1){
		
		while(!isalpha(c=fgetc(fp))){
			 lines[k].sentence[i++]=c;
		if(iscut(c))  
		    break;
		else   
		    continue;
	    }
	        if(iscut(c))  
		       break;
	    j=0;
	    do{
		  w[j++]=tolower(c);
		  lines[k].sentence[i++]=c;
	    }while(isalpha(c=fgetc(fp)));
	    ungetc(c,fp);
    	w[j]='\0';
    	if(find(w)->type==1){
	        	lines[k].sum +=find(w)->number ;
			}
	}
	lines[k].sentence[i]='\0';//��ʱ�Ѿ������˾���ĩβ
    
    do{                                  //ԭ��Ȼ�����������β�пո����ˣ���Ȼ��Ҫ�ı����ݼ����������ҷ����� 
			c=fgetc(fp);
		}while(isspace(c));
	ungetc(c,fp);
    
	if((c=fgetc(fp))!=EOF)
	    {
	    	ungetc(c,fp);
	    	return 1;
		}
	else   return EOF;
}

int main(){
	char string[30]={'\0'};
	FILE *fp=fopen("article.txt","r");
	FILE *stop=fopen("stopwords.txt","r");
	int h;
	char c;
	int i;
	struct dot* pos=NULL;
	
   scanf("%d",&N);
	
	while(fscanf(stop,"%s",string)!=EOF){
		h=hash(string);
		tap[h]=create(tap[h],string,0);
	} 
	fclose(stop);
	
	while(getWord(fp,string) !=EOF){
		pos=find(string);
		if(pos==NULL) {
            h=hash(string);
		    tap[h]=create(tap[h],string,1);
        }
        else if(pos->type==1)   
		   pos->number ++;
	}
        
	fseek(fp,0,SEEK_SET);
	k=0;
	
	while(getLine(fp,k)!=EOF){
	//	addSentence(k);
		k++;
	}
	qsort(lines,k,sizeof(struct sen),cmp);
	fclose(fp);

	for(i=0;i<5;i++)
	   printf("%d %s\n",lines[i].sum,lines[i].sentence);
	FILE *out=fopen("results.txt","w");
	for(i=0;i<N;i++)
	   fprintf(out,"%d %s\n",lines[i].sum,lines[i].sentence);
	fclose(out);
	
	return 0;
} 
