#include <string.h>
#include <stdio.h>

// TODO : make encoder
// TODO : make trie_node
// TODO : apply json
// TODO : deal with redis
// TODO : enforce find algorithm

// 자음 , 모음, 자+모 를.. 다른 글자들에 맞춰주자

class char_codec
{
public:
  static void url2utf8(char *str)
  {
    int len = strlen(str), cnt = 0;
    for(int i=0;i<len;i++)
    {
      if(str[i]=='%')
      {
        int tmp = 16;
        str[cnt] = 0;
        for(int j=i+1;j<i+3;j++)
        {
          if('0' <= str[j] && str[j] <= '9') str[cnt] += (str[j]-'0') * tmp;
          else str[cnt] += (str[j]-'A'+10) * tmp;
          tmp/=16;
        }
        cnt++;
        i+=2;
      }
      else str[cnt++]=str[i];
    }
    str[cnt]=0;
  }
  static void encode(char *str)
  {
    int len = strlen(str), cnt = 0;

    for(int i=0;i<len;i++)
    {
      if((unsigned char)str[i]>122)
      {
        int l = (unsigned char)str[i], m = (unsigned char)str[i+1], r = (unsigned char)str[i+2];
        if(l == 227 && m == 132 && r == 177) str[cnt++]=1; 
        else if(l == 227 && m == 132 && r == 178) str[cnt++]=2;
        else if(l == 227 && m == 132 && r == 180) str[cnt++]=3;
        else if(l == 227 && m == 132 && r == 183) str[cnt++]=4;
        else if(l == 227 && m == 132 && r == 184) str[cnt++]=5;
        else if(l == 227 && m == 132 && r == 185) str[cnt++]=6;
        else if(l == 227 && m == 133 && r == 129) str[cnt++]=7;
        else if(l == 227 && m == 133 && r == 130) str[cnt++]=8;
        else if(l == 227 && m == 133 && r == 131) str[cnt++]=9;
        else if(l == 227 && m == 133 && r == 133) str[cnt++]=10;
        else if(l == 227 && m == 133 && r == 134) str[cnt++]=11;
        else if(l == 227 && m == 133 && r == 135) str[cnt++]=12;
        else if(l == 227 && m == 133 && r == 136) str[cnt++]=13;
        else if(l == 227 && m == 133 && r == 137) str[cnt++]=14;
        else if(l == 227 && m == 133 && r == 138) str[cnt++]=15;
        else if(l == 227 && m == 133 && r == 139) str[cnt++]=16;
        else if(l == 227 && m == 133 && r == 140) str[cnt++]=17;
        else if(l == 227 && m == 133 && r == 141) str[cnt++]=18;
        else if(l == 227 && m == 133 && r == 142) str[cnt++]=19;
        else if(l == 227 && m == 133 && 143 <= r && r <= 163)
        {
          str[cnt++] = 1;
          str[cnt++] = r - 143 + 20;
        }
        else
        {
          l-=234, m-=128, r-=128;
          int idx = r + m * 64 + l * 64 * 64 - 3072;
          idx += (idx/588) + 1;
          str[cnt++] = idx/589 + 1;
          if(idx%589!=0)
          {
            idx = idx%589-1;
            str[cnt++] = idx/28 + 20;
            if(idx%28!=0) str[cnt++] = idx%28 + 40;
          }
        }
        i+=2;
      }
      else
      {
        str[cnt++]=str[i]+66;
      }
    }
    str[cnt]=0;
  }

  // No need to decode
};
