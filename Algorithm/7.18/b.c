#include<stdio.h>
int main()
{
    int n,a[101];
    int i,j,t;
    double sum;
    while(scanf("%d", &n)!=EOF)
        {
            for(i=0;i<n;i++)
                scanf("%d", &a[i]);
            for(i=0;i<n-1;i++)
                for(j=i+1;j<n;j++)
                    {
                        if(a[i]>a[j])
                            {t=a[i];a[i]=a[j];a[j]=t;}
                    }
            for(i=1,sum=0;i<n-1;i++)
                sum+=a[i];
            printf("%.2f\n", sum/(n-2));
        }
    return 0;
}
