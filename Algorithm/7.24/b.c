#include <stdio.h>
#include <stdlib.h>
long long gcd(long long a,long long b);

int
main()
{
    long long a,b,c,d;
    int n;
    scanf("%d", &n );
    while (n--){
        scanf("%lld/%lld %lld/%lld", &a, &b, &c, &d);
        a = a * d;
        c = c * b;
        a = (a/gcd(a, c)) * c;
        b *= d ;
        d = gcd(a,b);
        a = a / d;
        b = b / d;
        printf("%lld", a);
        if (b == 1){
            printf("\n");
        } else {
            printf("/%lld\n", b);
        }
    }

}

long long
gcd(long long a,long long b)
{
    return ((a%b==0?b:gcd(b,a%b)));
}
