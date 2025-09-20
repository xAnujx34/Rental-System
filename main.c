#include <stdio.h>
#include "common.h"

Vehicle *vehicleHead = NULL; 
User *userHead = NULL;

int main()
{
    loadUsersFromFile();
    int choice;
    while (1)
    {
        clearScreen();

        printf("      WHEELGO:TWO-WHEELER RENTAL MANAGEMENT SYSTEM\n");
        printf("==============================================\n");
        printf("           Developed by: *H@SH CR@CKERS*\n");
        printf("           Version     : 1.0\n");
        printf("           Language    : C\n");
        printf("==============================================\n\n");
        printf("**For fast booking register yourself.\nDon't you have an account?=>Register\n\n");
        printf("|1. Register|      |2. Login|    |3. viewFacilities|        |4. Exit|\n");
        printf("=========================================================================\n\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            registerUser();
            break;
        case 2:
            loginUser();
            break;
        case 3:
            viewFacilities();
            break;
        case 4:
            printf("Thank you for using the system. Goodbye!\n");
            return 0;
        default:
            printf("Invalid choice! Please try again.\n");
            getchar();
        }
    }
    return 0;
}