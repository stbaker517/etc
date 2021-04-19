#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>



//setting polys
int setPoly(int[]);
void insert(int[],int,int);


//mult
int multPoly(int,int[],int,int[],int[]);
//div
int divPoly(int,int[],int, int[], int[]);

void printPoly(int, int[]);
int main() {
	int poly1[2001] = { 0 };
	int degofpoly1, degofpoly2, degofresult;
	int poly2[2001] = { 0 };
	int result[2001] = { 0 };
	int Q[2001] = { 0 };
	int R[2001] = { 0 };
	char op;

	scanf("%c", &op);
	degofpoly1 = setPoly(poly1);
	degofpoly2 = setPoly(poly2);

	if (op == '*')
	{
		degofresult=multPoly(degofpoly1, poly1, degofpoly2, poly2, result);
		printPoly(degofresult, result);
	}
	else
	{
		degofresult = divPoly(degofpoly1, poly1, degofpoly2, poly2, result);
		printf("q\n");
		printPoly(degofresult, result);
		printf("r\n");
		printPoly(degofpoly2, poly1);
	}



}

int setPoly(int poly[])
{
	int n, deg=0,coef,exp;
	scanf("%d", &n);
	for(int i=0;i<n;i++)
	{
		scanf("%d %d", &coef, &exp);
		insert(poly, coef, exp);
		if (!deg)deg = exp;
	}
	return deg;
}
void insert(int poly[],int coef,int exp)
{
	poly[exp] += coef;
}

int multPoly(int nP1, int poly1[], int nP2, int poly2[], int result[])
{
	for (int p1 = 0;p1 <= nP1;p1++)
	{
		if (!poly1[p1])continue;

		for (int p2 = 0;p2 <= nP2;p2++)
		{
			insert(result, poly1[p1] * poly2[p2], p1 + p2);
		}
	}

	return nP1 + nP2;
}

int divPoly(int nP1, int poly1[], int nP2, int poly2[], int Q[])
{
	for (int i = nP1;i >= nP2;i--)
	{
		if (poly1[i] == 0)continue;

		Q[i - nP2] = poly1[i] / poly2[nP2];
		for (int j = nP2;j >= 0;j--)
		{
			insert(poly1, -1*Q[i-nP2]*poly2[j], i +j-nP2);
		}
	}
	return nP1 - nP2;
}

void printPoly(int deg, int poly[])
{
    int flag=0;
		for (int i = deg;i >= 0;i--)
		{
			if (poly[i] != 0)
            {
                flag=1;
                printf("%d %d\n", poly[i], i);
            }
		}

        if(!flag)printf("0 0\n");

}
