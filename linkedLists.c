/*****************************************************************************************
Prologue

UCSC Extension: Advanced C Programming

Instructor: Rajainder A. Yeldandi

Author: Kevin Geiszler

Assignment Number: 11, Linked Lists

Topics: Linked Lists

File Name: assignment11_KevinGeiszler.c

Date: 3/3/2017

Objective: This program reads information from an input file and places the information
		   into a linked list.
		   
Comments:  The linked list is displayed in its normal form, along with its reversed form.
		   The user is prompted to remove items from the list if they choose to do so. The
		   list is then printed again after items have been removed.

*****************************************************************************************/

//Preprocessor directives

#include <stdio.h>
#include <stdlib.h>

struct listnode {
	int data;
	struct listnode *nextptr;
};

typedef struct listnode LISTNODE;
typedef LISTNODE *LISTNODEPTR;

/****************************************************************************************/

//Function prototypes
void scanInputFile(FILE *, FILE *, LISTNODEPTR *);
void displayListInfo(FILE *, LISTNODEPTR *, int);
void promptToDelete(FILE *, LISTNODEPTR *);
void insert(LISTNODEPTR *, int);
char delete(LISTNODEPTR *, int);
int isempty(LISTNODEPTR);
void printlist(FILE *, LISTNODEPTR);
void instructions(void);
void reverselist(LISTNODEPTR *);

/****************************************************************************************/

//The main function
int main(int argc, char *argv[])
{
	LISTNODEPTR startptr = NULL;
	int choice, item, number;
	FILE *pInFile, *pOutFile;
	
	//check command line arguments
	if (argc != 3)
	{
		printf("Error, need the following file usage: %s inputFile outputFile\n", argv[0]);
		exit(1);
	}
	
	//open input file
	if ((pInFile = fopen(argv[1], "r")) == NULL)
	{
		printf("Error opening input file %s.\n", argv[1]);
		exit(1);
	}
	
	//open output file
	if ((pOutFile = fopen(argv[2], "w")) == NULL)
	{
		printf("Error opening output file %s.\n", argv[2]);
		exit(1);
	}
	
	rewind(pInFile);
	
	printf("Reading information from input file...\n\n");
	
	//scan input file and add items to the linked list
	scanInputFile(pInFile, pOutFile, &startptr);
	
	fprintf(pOutFile, "\n");
	
	//input file is no longer needed
	fclose(pInFile);
	
	//display list into in format form 1
	displayListInfo(pOutFile, &startptr, 1);
		
	printf("Enter a number to be deleted from the list (blank line to stop): ");
	
	//let the user choose which items to delete from the list
	promptToDelete(pOutFile, &startptr);
	
	fprintf(pOutFile, "The user has chosen to stop removing items from the list.\n\n", number);
	
	//display list info in format form 2
	displayListInfo(pOutFile, &startptr, 2);

	printf("End of run.\n");
	fprintf(pOutFile, "End of run.\n");

	//output file no is no longer needed
	fclose(pOutFile);

	return 0;
}

/****************************************************************************************/

//Check if the list is empty.

int isempty(LISTNODEPTR sptr)
{
	return(sptr == NULL);
}

/****************************************************************************************/

//Insert the provided value into the list.

void insert(LISTNODEPTR *sptr, int value)
{
	LISTNODEPTR newptr, prevptr, currptr;
	newptr = (LISTNODEPTR)malloc(sizeof(LISTNODE));
	
	if (newptr != NULL)
	{
		newptr->data    = value;
		newptr->nextptr = NULL;
		prevptr         = NULL;
		currptr         = *sptr;

		while((currptr != NULL))
		{
			prevptr = currptr;
			currptr = currptr->nextptr;
		}
		
		if (prevptr == NULL)
		{
			newptr->nextptr =  *sptr;
			*sptr			= newptr;
		}
		else
		{
			prevptr->nextptr =  newptr;
			 newptr->nextptr = currptr;
		}
	}
	else
		printf("%d not inserted. No memory is available.\n", value);
}

/****************************************************************************************/

//Delete the provided value from the list.

char delete(LISTNODEPTR *sptr, int value)
{
	LISTNODEPTR prevptr, currptr, tempptr;
	
	if (value == (*sptr)->data)
	{
		tempptr = *sptr;
		*sptr   = (*sptr)->nextptr;
		free(tempptr);
		
		return(value);
	}
	else
	{
		prevptr = *sptr;
		currptr = (*sptr)->nextptr;
		
		while((currptr != NULL) && (currptr->data != value))
		{
			prevptr = currptr;
			currptr = currptr->nextptr;
		}
		
		if (currptr != NULL)
		{
			tempptr = currptr;
			prevptr->nextptr = currptr->nextptr;
			currptr = currptr->nextptr;
			free(tempptr);
			return(value);
		} //end inner if
	} //end outer else
	
	return('\0');
}

/****************************************************************************************/

//Print the list in its current form.

void printlist(FILE *pOut, LISTNODEPTR currptr)
{
	if (currptr == NULL)
	{
		printf("The list is empty.\n");
		fprintf(pOut, "The list is empty.\n");
	}
	else
	{
		printf("Here is the current list:\n\n");
		fprintf(pOut, "Here is the current list:\n\n");
		
		while (currptr != NULL)
		{
			printf("%d --> ", currptr->data);
			fprintf(pOut, "%d --> ", currptr->data);
			currptr = currptr->nextptr;
		} //end of while loop
		
		printf("NULL\n\n");
		fprintf(pOut, "NULL\n\n");
	} //end of else
}

/****************************************************************************************/

//Reverse the list, but leave NULL at the end of the list.

void reverselist(LISTNODEPTR *sptr)
{
    LISTNODEPTR prevptr    = NULL;
    
    //recall that LISTNODEPTR is a pointer, so LISTNODEPTR * is a pointer to a pointer
    //this means *sptr holds an address, so currptr == the addredd sptr is holding
    LISTNODEPTR currentptr = *sptr;
    
    LISTNODEPTR nextptr;
    
    //printf("*sptr = %p\n", *sptr);
    
    while (currentptr != NULL)
    {
    	//assign nextptr to the pointer in currentptr
        nextptr  = currentptr->nextptr;
        
        //assign the nextptr found in currentptr to the value of prevptr
        //the next pointer is now the previous pointer
        currentptr->nextptr = prevptr; 
        
        //prevptr is now the current pointer  
        prevptr = currentptr;
        
        //the current pointer now has the value of the next pointer, which has the value
        //of the previous pointer
        currentptr = nextptr;
    }
    
    //make the starting pointer point to the new front of the list
    *sptr = prevptr;
}

/****************************************************************************************/

//Scan the input file and add items to the linked list.

void scanInputFile(FILE *pIn, FILE *pOut, LISTNODEPTR *sptr)
{
	int num;

	while(fscanf(pIn, "%d,", &num) == 1)
	{
		insert(sptr, num);
		fprintf(pOut, "%-3d has been read from the input file and added to the end of the list.\n", num);
	}
}

/****************************************************************************************/

//Display the list's information. The information formatting can be presented in two
//forms.

void displayListInfo(FILE *pOut, LISTNODEPTR *sptr, int choice)
{
	switch (choice)
	{
	 //If the list if not empty, then display the list, reverse it, and display it again.
	 //Afterwards, the list is placed back into its original order.
	 //Otherwise, tell the user the list is empty and exit the program.
	 case 1:
			if (!isempty(*sptr))
			{
				printlist(pOut, *sptr);
				printf("The list has been reversed. ");
				fprintf(pOut, "The list has been reversed. ");
				reverselist(sptr);
				printlist(pOut, *sptr);
				reverselist(sptr);
				printf("The list is now back in its original order.\n\n");
				fprintf(pOut, "The list is now back in its original order.\n\n");
			}
			else
			{
				printf("The list is empty. Exiting the program now.\n");
				exit(1);
			}
			break;
	 
	 //After some items in the list have been deleted, display the revered list, put it
	 //back in its original order, then display it again.
	 case 2:
			printlist(pOut, *sptr);
			break;
			
	default:
			printf("Error, invalid case choice in displayListInfo().\n");
			fprintf(pOut, "Error, invalid case choice in displayListInfo().\n");
			break;
	}
}

/****************************************************************************************/

//Prompts the user to enter an item to be deleted from the list. An error message is
//displayed if the value is not in the list. The user can choose to stop deleting items
//from the list by entering a blank line.

void promptToDelete(FILE *pOut, LISTNODEPTR *sptr)
{
	char string[20];
	int number;

	while((fgets(string, sizeof(string), stdin) != NULL) && (string[0] != '\n'))
	{
		sscanf(string, "%d", &number);
		
		if (delete(sptr, number))
		{
			printf("\n%d has been removed from the list.\n\n", number);
			fprintf(pOut, "The user chose to remove %d from the list.\n\n", number);
			printlist(pOut, *sptr);
		}
		else
		{
			printf("\n%d not found.\n\n", number);
			fprintf(pOut, "The user tried to remove %d from the list, but it is not in the list.\n\n", number);
		}
				
		printf("Enter a number to be deleted from the list (blank line to stop): ");
	}
	
	printf("\n");
}

/****************************************************************************************/







































