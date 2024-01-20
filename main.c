#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct bin_struct {
	unsigned char b[8];
} BIN_T;

//Still not good at coming up with good function names. Oh well.
void utf8_read(const char * str);
BIN_T char2bin(const unsigned char input);
void bin_display(const BIN_T bin);
unsigned char bin_chartype(const BIN_T bin);
unsigned char bin_validext(const BIN_T bin);
unsigned int bin_int(const char * b, const unsigned int len);

int main()
{
	char * test_1 = "\x4D\xC3\xAC\x6E\x68\x20\x6E\xC3\xB3\x69\x20\x74\x69\xE1\xBA\xBF\x6E\x67\x20\x56\x69\xE1\xBB\x87\x74";
	char * test_2 = "\xF0\xA8\x89\x9F\xE5\x91\x90\xE3\x97\x82\xE8\xB6\x8A\x21\x21";
	printf("Hi there.\nThis cannot be displayed: %s\nBut that's actually what I wanted to do.\n\nNor shall this: %s\n\n",test_1, test_2);
	printf("---Test #1---\n");
	utf8_read(test_1);
	printf("\n---Test #2---\n");
	utf8_read(test_2);
	return 0;
}

void utf8_read(const char * str)
{
	unsigned int i,j,z,k,type,len,b_len,n;
	BIN_T b[4];
	unsigned char rb[32];
	len=0;
	b_len=0;
	for (i=0;i<strlen(str);i++)
	{
		printf("%d: %c",i,str[i]);
		if ((unsigned char) str[i]>0x7F)
		{
			printf(" <SPECIAL>");
			printf("\n");
			b[0]=char2bin((unsigned char) str[i]);
			type=bin_chartype(b[0]);
			//bin_display(s);
			printf("Type: %d\n",type);
			switch(type)
			{
			default:
				n=(unsigned char) str[i];
				len++;
				break;
			case 0x2:
				if (i+1<strlen(str))
				{
					b[1]=char2bin((unsigned char) str[i+1]);
					if (bin_validext(b[1]))
					{
						b_len=11;
						j=0;
						z=0+3;
						k=0;
						while (j<b_len)
						{
							rb[j]=b[k].b[z];
							z++;
							if (z>7)
							{
								z=2;
								k++;
							}
							j++;
						}
						n=bin_int((char *)rb,b_len);
						len++;
					}
				}
				i+=1;
				
				break;
			case 0x3:
				if (i+2<strlen(str))
				{
					b[1]=char2bin((unsigned char) str[i+1]);
					b[2]=char2bin((unsigned char) str[i+2]);
					if (bin_validext(b[1]) && bin_validext(b[2]))
					{
						b_len=16;
						j=0;
						z=0+4;
						k=0;
						while (j<b_len)
						{
							rb[j]=b[k].b[z];
							z++;
							if (z>7)
							{
								z=2;
								k++;
							}
							j++;
						}
						n=bin_int((char *)rb,b_len);
						len++;
					}
				}
				i+=2;
				
				break;
			case 0x4:
				if (i+3<strlen(str))
				{
					b[1]=char2bin((unsigned char) str[i+1]);
					b[2]=char2bin((unsigned char) str[i+2]);
					b[3]=char2bin((unsigned char) str[i+3]);
					if (bin_validext(b[1]) && bin_validext(b[2]) && bin_validext(b[3]))
					{
						b_len=21;
						j=0;
						z=0+5;
						k=0;
						while (j<b_len)
						{
							rb[j]=b[k].b[z];
							z++;
							if (z>7)
							{
								z=2;
								k++;
							}
							j++;
						}
						n=bin_int((char *)rb,b_len);
						len++;
					}
				}
				i+=3;
				break;
			}
		}
		else
		{
			n=(unsigned char) str[i];
			len++;
			printf("\n");
		}
		if (n>127)
			printf("[UTF-8: %X]\n\n",n);
		else
			printf("[%X]\n\n",n);
	}
	printf("\nLength: %d\n",len);
}

BIN_T char2bin(const unsigned char input)
{
	BIN_T res_bin;
	div_t res;
	unsigned char s[8];
	unsigned int n,quo,i,j,rem[8];
	for (i=0;i<8;i++)
	{
		s[i]=0;
		rem[i]=0;
		res_bin.b[i]=0;
	}
	quo=input;
	if (quo==0)
	{
		return res_bin;
	}
	j=7;
	while (quo>0)
	{
		n=quo;
		res=div(n,2);
		quo=res.quot;
		rem[j]=res.rem;
		j--;
	}
	for (i=0;i<8;i++)
	{
		res_bin.b[i]=(unsigned char) rem[i];
	}
	return res_bin;
}

void bin_display(const BIN_T bin)
{
	unsigned int i;
	for (i=0;i<8;i++)
	{
		printf("%d",bin.b[i]);
	}
	printf("\n");
}

unsigned char bin_chartype(const BIN_T bin)
{
	if (bin.b[0]==0x01 && bin.b[1]==0x01 && bin.b[2]==0x01 && bin.b[3]==0x01 && bin.b[4]==0x00) //11110 = 4
		return 0x04;
	if (bin.b[0]==0x01 && bin.b[1]==0x01 && bin.b[2]==0x01 && bin.b[3]==0x00) //1110 = 3
		return 0x03;
	if (bin.b[0]==0x01 && bin.b[1]==0x01 && bin.b[2]==0x00) //110 = 2
		return 0x02;
	return 0x01;
}

unsigned char bin_validext(const BIN_T bin)
{
	if (bin.b[0]==0x01 && bin.b[1]==0x00)
		return 0x01;
	return 0x00;
}

unsigned int bin_int(const char * b, const unsigned int len)
{
	unsigned int i,n,power;
	n=0;
	for(i=0;i<len;i++)
	{
		power=(int) pow(2,(double) (len-1)-i);
		n+=power*b[i];
	}
	return n;
}