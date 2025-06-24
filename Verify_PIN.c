#include"header.h"
double extract_dou(char *buf)
{
	int end,start;
	char out[10];
	end=strlen(buf)-1;
	while(end>=0&&!(buf[end]>=48&&buf[end]<=57))
		end--;
	if(end<0)
	{
		return 0;
	}
	start=end;
	while(start>=0&&((buf[start]>=48)&&(buf[start]<=57)))
			start--;
	strncpy(out,buf+start+1,end-start);
	out[end-start]='\0';
	return atof(out);
}
char * extract_int(char *buf)
{
	int end,start;
	static char out[10];
	end=strlen(buf)-1;
	while(end>=0&&!(buf[end]>=48&&buf[end]<=57))
		end--;
	if(end<0)
	{
		return 0;
	}
	start=end;
	while(start>=0&&((buf[start]>=48)&&(buf[start]<=57)))
			start--;
	strncpy(out,buf+start+1,end-start);
	out[end-start]='\0';
	return out;
}
void Verify_PIN(char *buff,int fd)
{
	SLL *temp=rfid;
	char buf[20];
	printf("in verify pin");
	printf("%s\n",buff);
	strcpy(buf,extract_int(buff));
	printf("after extraction\n");
	printf("%s\n",buf);
	printf("%s\n",pin);

	if(strcmp(pin,buf)!=0)
	{
		printf("INVLAID PIN\n");
#ifdef DEBUG
		write(1,"@ERR#INVLD$",sizeof("@ERR#INVLD$"));
#else
		write(fd,"@ERR#INVLD$",sizeof("@ERR#INVLD$"));
#endif
	}
	else
	{
#ifdef DEBUG
		write(1,"@OK$",sizeof("@OK$"));
#else
		puts("in @ok$ sending");
		write(fd,"@OK$",sizeof("@OK$"));
#endif
		
	}
	
}
void BAL_ENQ(int fd)		
{
	char sendbuff[100]={0},floatbuff[20]={0};
	SLL *temp=rfid;
	/*
#ifdef DEBUG
			puts("bread");
			read_str(0,buf,30);
			puts("aread");
#else
			read_str(fd,buf,30);
#endif
			printf("service:%s",buf);
			if(strcmp(buf,"#REQ$")==0)*/
			
				puts("BALANCE REQ");
				sprintf(floatbuff,"%.1lf",temp->Account_Balance);
				strcpy(sendbuff,"@BAL:");
				strcat(sendbuff,floatbuff);
				strcat(sendbuff,"$");
				printf("balance:%s\n",sendbuff);
#ifdef DEBUG
				write(1,sendbuff,strlen(sendbuff));
#else
				write(fd,sendbuff,strlen(sendbuff));
#endif
			
}
void DEP_BAL(char *buf,int fd)
// if(strncmp(buf,"#DEP:",5)==0)
{
	char sendbuff[100],floatbuff[30];	
	SLL *temp=rfid;
	puts("IN DEPOSIT");
	size_t now;
	struct tm *current_time;
	int i,max=27835438,min=26857429;
	double amount;
	time(&now);
        current_time=localtime(&now);
				amount=extract_dou(buf);
				printf("%lf\n",amount);
				temp->Account_Balance=(temp->Account_Balance+amount);
				sprintf(floatbuff,"%.1lf",temp->Account_Balance);
				strcpy(sendbuff,"@BAL:");
				//sprintf(floatbuff,"%.1lf",temp->Account_Balance);
				strcat(sendbuff,floatbuff);
				strcat(sendbuff,"$");
				if(temp->number>=5)
                		{
                        		temp->number=0;
                        		temp->flag1=1;
                		}
                		memmove(&temp->history[1],&temp->history[0],4*sizeof(Trsnc));
				strcpy(temp->history[0].type,"DEPOSIT");
				temp->history[0].amount=amount;
				temp->history[0].Acc_Num=temp->Account_Number;
				temp->history[0].year=current_time->tm_year+1900;
				temp->history[0].month=current_time->tm_mon+1;
				temp->history[0].day=current_time->tm_mday;
				temp->history[0].hour=current_time->tm_hour;
				temp->history[0].min=current_time->tm_min;
				temp->history[0].trsncid=(rand()%(max-min+1))+min;
                		temp->number++;
				temp->Trasnction_Count++;
				Save_To_File();
#ifdef DEBUG
				write(1,sendbuff,strlen(sendbuff));
#else
				write(fd,sendbuff,strlen(sendbuff));
#endif

}
void WITH_BAL(char *buf,int fd)
// if(strncmp(buf,"#WTHD:",6)==0)
{
	char sendbuff[100],floatbuff[30];
	SLL *temp=rfid;
	size_t now;
	struct tm *current_time;
	int i,max=27835438,min=26857429;
	double amount;
	time(&now);
        current_time=localtime(&now);
	printf("WITHDRAW\n");
	amount=extract_dou(buf);
	printf("%f\n",amount);
				if(amount>temp->Account_Balance)
				{
					printf("INSUFFICIENT BALANCE\n");
#ifdef DEBUG
					write(1,"@ERR$",strlen("@ERR$"));
#else
					write(fd,"@ERR$",strlen("@ERR$"));
#endif
				}
				else
				{
					printf("SUCCESS WITHDRAW\n");
					temp->Account_Balance=(temp->Account_Balance-amount);
					strcpy(sendbuff,"@BAL:");
					sprintf(floatbuff,"%.1lf",temp->Account_Balance);
					strcat(sendbuff,floatbuff);
					strcat(sendbuff,"$");
					if(temp->number>=5)
                        		{
                		                temp->number=0;
					      	temp->flag1=1;
                        		}
                       	 		
   			                
                        		memmove(&temp->history[1],&temp->history[0],4*sizeof(Trsnc));
                 		       	strcpy(temp->history[0].type,"WITHDRAW");
                     			temp->history[0].amount=amount;
                        		temp->history[0].Acc_Num=temp->Account_Number;
                        		temp->number++;
                        		temp->history[0].year=current_time->tm_year+1900;
                    		    	temp->history[0].month=current_time->tm_mon+1;
                        		temp->history[0].day=current_time->tm_mday;
                        		temp->history[0].hour=current_time->tm_hour;
                        		temp->history[0].min=current_time->tm_min;
                        		temp->history[0].trsncid=(rand()%(max-min+1))+min;
					temp->Trasnction_Count++;
					Save_To_File();
#ifdef DEBUG
					write(1,sendbuff,strlen(sendbuff));
#else
					write(fd,sendbuff,strlen(sendbuff));
#endif
				}
}
void PIN_CHANG(char *buf,int fd)
//	 if(strncmp(buf,"#PINCHG:",8)==0)
{
	SLL *temp=rfid;
				printf("PINCHANGE NEWPIN\n");
				strcpy(temp->pin,extract_int(buf));
				strcpy(pin,temp->pin);
				Save_To_File();
#ifdef DEBUG
				write(1,"@CGD$",strlen("@CGD$"));
#else
				write(fd,"@CGD$",strlen("@CGD$"));
#endif
}
void BLK_CARD(int fd)
//if(strcmp(buf,"#BLK$")==0)
{
	SLL *temp=rfid;
				printf("PIN CHANGE BLOCK\n");
				temp->block=1;
				Save_To_File();
#ifdef DEBUG
				write(1,"@BLK$",strlen("@BLK$"));
#else	
				write(fd,"@BLK$",strlen("@BLK$"));
#endif
}
void MINI_STMT(int fd)
{
//if(strcmp(buf,"#MINI#REQ$")==0)
	char sendbuff[100],buf[20];
	int i,j;
	SLL *temp=rfid;
				printf("IN MINISTMT\n");
				strcpy(sendbuff,"@OK#STMT:");
				j=temp->Trasnction_Count;
				if(j>=3)
				{
					for(i=0;i<3;i++)
					{
						strcat(sendbuff,temp->history[i].type);
						sprintf(buf,"%.2lf",temp->history[i].amount);
						strcat(sendbuff,"-");
						strcat(sendbuff,buf);
						strcat(sendbuff,"|");
					}
				}
				else if(j==2)
				{
					for(i=0;i<2;i++)
					{
						strcat(sendbuff,temp->history[i].type);
						sprintf(buf,"%.2lf",temp->history[i].amount);
						strcat(sendbuff,"-");
						strcat(sendbuff,buf);
						strcat(sendbuff,"|");
					}
				}
				else if(j==1)
				{
					for(i=0;i<1;i++)
					{
						strcat(sendbuff,temp->history[i].type);
						sprintf(buf,"%.2lf",temp->history[i].amount);
						strcat(sendbuff,"-");
						strcat(sendbuff,buf);
						strcat(sendbuff,"|");
					}
				}
				strcat(sendbuff,"$");
#ifdef DEBUG
				write(1,sendbuff,strlen(sendbuff));
#else
				write(fd,sendbuff,strlen(sendbuff));
#endif
}
/*
			//if(strcmp(buf,"#OK:CNTD$")==0)
			{
				printf("CONNECTED\n");
#ifdef DEBUG
				write(1,"@OK:CNTD$",strlen("@OK:CNTD$"));
#else
				write(fd,"@OK:CNTD$",strlen("@OK:CNTD$"));
#endif			
			}
			else if(strcmp(buf,"#EXIT$")==0)
			{
				printf("IN EXIT\n");
				break;
			}
			else if(strcmp(buf,"#TIMEOUT$")==0)
			{
				printf("TIME OUT\n");
				break;
			}
		}
	}
}*/
