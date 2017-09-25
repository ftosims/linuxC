#include <stdio.h>

int gcd(int m, int n)
{
    while(m>0)
        {
            int c = n % m;
            n = m;
            m = c;
        }
    return n;
}

int main(void)
{
    int linenum, a, b, c;
    scanf("%d", &linenum);
    while(linenum--) {
        scanf("%d%d", &a, &b);
        c = b + b;
        while(gcd(a, c) != b)
            c += b;
        printf("%d\n", c);
    }
    return 0;
}

