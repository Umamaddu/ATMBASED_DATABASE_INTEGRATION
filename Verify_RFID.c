#include"header.h"
void Verify_RFID(char *buff,int fd)
{

	SLL *temp;
	char buf[30];
	temp=Account_Details;
	strcpy(buf,extract_int(buff));
	printf("...%s...\n",buf);
	while(temp)
	{
		if(strcmp(temp->RFID,buf)==0)
		{
			rfid=temp;
			strcpy(pin,temp->pin);
			printf("%s\n",pin);
			break;
		}
		temp=temp->next;
	}
	if(temp)
	{
		if(!temp->block)
		{
			printf("ACTIVE\n");
#ifdef DEBUG
			write(1,"@OK#ACTV$",strlen("@OK#ACTV$"));
#else
			puts("in sending");
			write(fd,"@OK#ACTV$",strlen("@OK#ACTV$"));
#endif
		}
		else
		{
			printf("BLOCKED\n");
#ifdef DEBUG
			write(1,"@BLKD$",strlen("@BLKD$"));
#else 
			write(fd,"@BLKD$",strlen("@BLKD$"));
#endif
		}
	}
	else
	{
		printf("INVALID CARD\n");
#ifdef DEBUG
		write(1,"@ERR#CARD$",strlen("@ERR#CARD$"));
#else 
		write(fd,"@ERR#CARD$",strlen("@ERR#CARD$"));
#endif
	}
}
