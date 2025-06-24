#include <stdio.h>
#include <string.h>

#define MAX_TRANSACTIONS 5

// Structure to store transaction details
typedef struct {
     	char type[20];       // e.g., "Deposit" or "Withdrawal"
        double amount;       // Transaction amount
	int transactionID;   // Unique transaction ID
} Transaction;
int main() {
	Transaction transactions[MAX_TRANSACTIONS];
        int current = 0; // Points to the next position to overwrite
        int totalTransactions = 0;

        printf("Enter transaction details (type, amount, ID):\n");
        while (1) 
	{
                                         // Input transaction details from the user
             printf("Transaction %d:\n", totalTransactions + 1);
             printf("Type (e.g., Deposit/Withdrawal): ");
             scanf("%s", transactions[current].type); // Read transaction type
             printf("Amount: ");
             scanf("%lf", &transactions[current].amount); // Read transaction amount
             printf("Transaction ID: ");
             scanf("%d", &transactions[current].transactionID); // Read transaction ID
                      // Increment counters
            current = (current + 1) % MAX_TRANSACTIONS; // Move to next position
             totalTransactions++;
                       // Ask user if they want to add another transaction
            char choice;
            printf("Do you want to add another transaction? (y/n): ");
            scanf(" %c", &choice); // Note the space before %c to consume leftover newline character
            if (choice == 'n' || choice == 'N') 
	    {
              break;
            }
                                                                                                                                                                                  }
                                                                                                                                                                                            // Display the last 5 transactions
                                                                                                                                                                                   printf("\nLast %d Transactions:\n", MAX_TRANSACTIONS);
                                                                                                                                                                                   int start = totalTransactions >= MAX_TRANSACTIONS ? current : 0; // Determine start position
                                                                                                                                                                                   for (int i = 0; i < (totalTransactions < MAX_TRANSACTIONS ? totalTransactions : MAX_TRANSACTIONS); i++) 
																						   {
                                                                                                                                                                                    int index = (start + i) % MAX_TRANSACTIONS;
                                                                                                                                                                                     printf("Transaction Type: %s, Amount: %.2f, Transaction ID: %d\n",
                                                                                                                                                                                  transactions[index].type, transactions[index].amount, transactions[index].transactionID);
                                                                                                                                                                                   }
                                                                                                                                                                                  return 0;                                                                                                                                                                     }
                                                                                       
