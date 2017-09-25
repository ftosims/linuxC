#include<stdio.h>
#include<string.h>
#include<algorithm>
#include<stdlib.h>


using namespace std;

char ans[1005];
int anslen;

void plusthem(char s1[],char s2[])
{
    int len1=strlen(s1);
    int len2=strlen(s2);

    for(int i=0;i<len1;i++)
        s1[i]-='0';
    for(int i=0;i<len2;i++)
      s2[i]-='0';

    int mid1=len1>>1;
    int mid2=len2>>1;

    for(int i=0;i<mid1;i++)
        swap(s1[i],s1[len1-1-i]);
    for(int i=0;i<mid2;i++)
        swap(s2[i],s2[len2-1-i]);
    anslen=max(len1,len2);
    int s=0,sum;
    for(int i=0;i<=anslen;i++){
        sum=(s1[i]+s2[i]+s);
        ans[i]=sum%10;
        s=sum/10;
    }
}
void printans(char ans[],int anslen)
{
    while(anslen>0&&(ans[anslen]==0))anslen--;
    if(anslen==0){
        printf("%d\n",ans[0]);
        return;
    }
    for(int i=anslen;i>-1;i--){
        printf("%d",ans[i]);
    }
    printf("\n");
}

int main()
{
    int cas;
    char s1[1005],s2[1005];
    scanf("%d",&cas);
    for(int i=1;i<=cas;i++){
            memset(s1,0,sizeof(s1));
            memset(s2,0,sizeof(s2));
            scanf("%s%s",s1,s2);
            printf("Case %d:\n",i);
            printf("%s + %s = ",s1,s2);
            plusthem(s1,s2);
            printans(ans,anslen);
            if(i!=cas)printf("\n");
    }
    return 0;
}
