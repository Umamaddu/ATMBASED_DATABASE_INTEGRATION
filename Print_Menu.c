#include "header.h"
void Print_Menu(void)
{
	printf("\033[34m\n");
	printf("         !__________________________BANK MENU__________________________!\n");
	printf("\033[0m\n");
	printf("\033[33m\n");

	printf(	"\t-->c/C---Create Account\t\t\t"
		"Mini Statement---h/H<--\n"
		"\t-->w/W-------Withdrawal\t\t\t"
		"Deposit----------d/D<--\n"
		"\t-->b/B--Balance Enquiry\t\t\t"
		"Transfer---------t/T<--\n"
		"\t-->e/E----Show Accounts\t\t\t"
		"Save To File-----s/S<--\n"
		"\t-->f/F-----Find Account\t\t\t"
		"Update Account---u/U<--\n"
		"\t-->l/L-----Change Status\t\t"
		"Exit-------------q/Q<--\n");
	
	printf("Select A Transaction->");
	printf("\033[0m\n");
//	printf("\03m\n");

}
