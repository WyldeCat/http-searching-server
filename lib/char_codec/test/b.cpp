#include "../include/char_codec.hpp"
#include <string.h>
#include <stdio.h>

int main( )
{
  char test1[]="황인규";
  char test2[]="ㅎㅇㄱ";
  char test3[]="각갂갃간난닫닿";

  printf("%s\n",test1);
  char_codec::encode(test1);
  for(int i=0;i<strlen(test1);i++)
    printf("%d ",(unsigned char)test1[i]);
  printf("\n");

  printf("%s\n",test2);
  char_codec::encode(test2);
  for(int i=0;i<strlen(test2);i++)
    printf("%d ",(unsigned char)test2[i]);
  printf("\n");

  printf("%s\n",test3);
  char_codec::encode(test3);
  for(int i=0;i<strlen(test3);i++)
    printf("%d ",(unsigned char)test3[i]);
  printf("\n");
}
