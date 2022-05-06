#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#pragma GCC optimize(3,"Ofast","inline")
struct sen{
	int sum,l,r,order;
}a[200000];
typedef struct te{
	int num;
	struct te *next[26];
}w;
w *root,*p,words[63000];
char article[20000005];
char stop[10005];
int *a_words[2000005],a_cnt;
int r[200005],r_cnt=1,n,words_cnt;
void k_th(int l,int r){
	int i=l,j=r,tem=a[l].sum;
	struct sen t;
	while(i!=j){
		while((a[j].sum<tem||(a[j].sum==tem&&a[j].order>=a[l].order))&&i<j) j--;
		while((a[i].sum>tem||(a[i].sum==tem&&a[i].order<=a[l].order))&&i<j) i++;
		if(i!=j) t=a[i],a[i]=a[j],a[j]=t;
	}
	t=a[l],a[l]=a[i],a[i]=t;
	if(i>n-1)
	k_th(l,i-1);
	else if(i<n-1)
	k_th(i+1,r);
}
int fx(const void *p1,const void *p2){
	if(((struct sen*)p2)->sum!=((struct sen*)p1)->sum)
	return ((struct sen*)p2)->sum-((struct sen*)p1)->sum;
	return ((struct sen*)p1)->order-((struct sen*)p2)->order;
}
int main(){
	register int i,j,k,ping=0,cnt=0;
	register char ch;
	root=&words[words_cnt++];
	FILE *in=fopen("article.txt","r"),*sto=fopen("stopwords.txt","r"),*out=fopen("results.txt","w");
	scanf("%d",&n);
	fread(article,20000000,1,in);
	k=0,p=root;
	while(article[k]){
		ch=article[k++];
		if(ch>='a'&&ch<='z'){
			if(!p->next[ch-'a'])
			p->next[ch-'a']=&words[words_cnt++];
			p=p->next[ch-'a'];
		}
		else if(ch>='A'&&ch<='Z'){
			ch+=32;
			if(!p->next[ch-'a'])
			p->next[ch-'a']=&words[words_cnt++];
			p=p->next[ch-'a'];
		}
		else{
			if(p!=root)
			++p->num,a_words[a_cnt++]=&p->num,p=root;
			if(ch=='.'||ch=='?'||ch=='!')
			r[r_cnt++]=k-1,++a_cnt;
		}
	}
	fclose(in);
	fread(stop,10005,1,sto);
	k=0;
	while(stop[k]){
		ch=stop[k++];
		if(ch>='a'&&ch<='z'){
			if(p)
			p=p->next[ch-'a'];
		}
		else{
			if(p) p->num=0;
			p=root;
		}
	}
	fclose(sto);
	j=k=0,r_cnt=1;
	while(j<a_cnt){
		while(a_words[j])
		ping+=*a_words[j++];
		if(ping)
		a[cnt].sum=ping,a[cnt].l=r[r_cnt-1]+1,a[cnt].r=r[r_cnt],a[cnt++].order=cnt;
		ping=0,++j,++r_cnt;
	}
	k_th(0,cnt-1);
	qsort(a,n,sizeof(struct sen),fx);
	for(i=0;i<=4;++i){
		printf("%d ",a[i].sum); 
		j=a[i].l;
		while(article[j]==' ') ++j;
		fwrite(article+j,1,a[i].r-j+1,stdout);
		printf("\n");
	}
	for(i=0;i<=n-1;++i){
		fprintf(out,"%d ",a[i].sum);
		j=a[i].l;
		while(article[j]==' ') ++j;
		fwrite(article+j,1,a[i].r-j+1,out);
		fprintf(out,"\n");
	}
	fclose(out);
}
