#include <string>
#include <cstring>
#include <cstdio>

int main( )
{
  char text1[] = "ㄱ";
  char text2[] = "가";
  char text3[] = "겨";
  char text4[] = "각";
  char text5[] = "나";
  char text6[] = "낙";
  char text7[] = "까";
  char text8[] = "깕";
  char text9[] = "낧";
  char text10[] = "ㄲ";
  char text11[] = "ㅏ";

  printf("%s ",text1);
  for(int i=0;i<strlen(text1);i++) printf("%x ",(unsigned char)text1[i]);
  printf("\n");

  printf("%s ",text2);
  for(int i=0;i<strlen(text2);i++) printf("%x ",(unsigned char)text2[i]);
  printf("\n");

  printf("%s ",text3);
  for(int i=0;i<strlen(text3);i++) printf("%x ",(unsigned char)text3[i]);
  printf("\n");

  printf("%s ",text4);
  for(int i=0;i<strlen(text4);i++) printf("%x ",(unsigned char)text4[i]);
  printf("\n");

  printf("%s ",text5);
  for(int i=0;i<strlen(text5);i++) printf("%x ",(unsigned char)text5[i]);
  printf("\n");

  printf("%s ",text6);
  for(int i=0;i<strlen(text6);i++) printf("%x ",(unsigned char)text6[i]);
  printf("\n");

  printf("%s ",text7);
  for(int i=0;i<strlen(text7);i++) printf("%x ",(unsigned char)text7[i]);
  printf("\n");

  printf("%s ",text8);
  for(int i=0;i<strlen(text8);i++) printf("%x ",(unsigned char)text8[i]);
  printf("\n");

  printf("%s ",text9);
  for(int i=0;i<strlen(text9);i++) printf("%x ",(unsigned char)text9[i]);
  printf("\n");

  printf("%s ",text10);
  for(int i=0;i<strlen(text10);i++) printf("%x ",(unsigned char)text10[i]);
  printf("\n");

  printf("%s ",text11);
  for(int i=0;i<strlen(text11);i++) printf("%x ",(unsigned char)text11[i]);
  printf("\n");
  return 0;
}
