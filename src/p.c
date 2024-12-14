#include<stdio.h>
int main(int argc, char* argv[])
{
 int a,b,c;
 int code;
 scanf("%d %d %d", &a,&b,&c);
 printf("---> %d\n",a);
 printf("---> %d\n",b);
 printf("---> %d\n",c);
 for (int i =0; i < a ; i++){
   scanf("%d", &code);
   printf("code %d \n",code);
 } 
}

