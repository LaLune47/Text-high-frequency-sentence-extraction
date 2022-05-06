//词频统计综合大作业 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define MAX 1000000  //句子个数最大值的估计值 
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

int N;  //最后需要的句子数量
int getWord(FILE *fp,char *w){   //读单词

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

unsigned int hash(char *str) {//哈希函数
    unsigned int hash=0;
    
	while(*str){
		hash=(*str++)+(hash<<6)+(hash<<16)-hash;
	}
         
    return   hash%400009;
}

struct dot {//单个单词节点的定义 
	char word[30];
	int type;  //标识停用词和非stop词语，0为停用，1为非停用 
	struct dot* next;
	int number;
}; 
struct dot* tap[400200];


struct dot* create(struct dot* head,char w[],int kind){//向单词表中增加新单词     
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
struct dot* find(char w[]){//寻找单词在哈希表中的位置 ,找不到就创建   //逻辑混乱，加的时候重复了 
	struct dot* q=NULL;
	int h;
	h=hash(w);

	for(q=tap[h];q!=NULL&&strcmp(q->word,w)!=0;q=q->next);
	
	if(q!=NULL){
	  return q;	
	}
	else   return NULL;
}


struct sen{//存储句子的结构体
	char sentence[5000];
	int sum;
	int order;
}; 
struct sen lines[MAX];
int k;//句子个数 

int cmp(const void*p1,const void*p2)
{
	struct sen *a=(struct sen*)p1;
	struct sen *b=(struct sen*)p2;
	if(a->sum!=b->sum)	return b->sum-a->sum; 
	else return a->order-b->order ; 
}

void addSentence(int k){
    int i=0;
    int j=0;  //第一句是肯定存在的，不需要判断为不为零，因为是直接录入到第一句里面的 
	struct sen   tmp; 
	
	tmp=lines[k];
	for(i=0;i!=k&&i<=N-1&&lines[i].sum>=lines[k].sum;i++);//找到了插入位置为i 
	
	if(i==k||i>N-1) 
	    return;     //说明已经是最小排在最后，不用再更改位置
	if(k>=N-1) k=N-1;
	for(j=k;j>=i;j--)
	    lines[j+1]=lines[j];
	lines[i]=tmp;
    return;
}

int iscut (char c){   //句子结束的判断 
	return (c=='!'||c=='.'||c=='?');
}
int getLine(FILE *fp,int k){   //读句子 
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
	lines[k].sentence[i]='\0';//此时已经读到了句子末尾
    
    do{                                  //原因竟然在这里！！！结尾有空格西八！果然需要改变数据集，从里面找方法。 
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
