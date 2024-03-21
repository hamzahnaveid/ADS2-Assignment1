#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define bool int
#define true 1
#define false (!true)

#define MAX 5 //max size of showroom

//Function prototypes
void addCar();  //add a car to the showroom
void sellCar(char *); //sell a car from the showroom
void carReserve(char *); //reserve/unreserve a car from the showroom
void viewAllCars(); //view all cars in the showroom
void viewUnreserved(); //view all unreserved cars in the showroom
void viewMake(char *); //view all cars of a certain make in the showroom
void viewColour(char *); //view all cars of a specific colour in the showroom
void viewSpecificCar(char *); //view a specific car in the showroom
void updateCar(); //update the details for a specific car in the showroom
bool isEmpty(); //return true if there are no cars in the showroom
bool isUnique(char *); //return true if there are no other cars in the showroom that have the given registration number
bool verifyReg(char *); //return true if the given registratrion number follows the correct format 'yyDxnnnn'
void displaySubMenu(); //display menu for options viewing cars in the showroom
void displayMenu(); //display user menu
void saveToFile(FILE *); //write all cars in the showroom to a binary file
void readFromFile(FILE *); //import all cars stored in a binary file into the showroom


//Structure template for a car
struct Car {
    char reg[10]; //car registration
    char make[20]; //car make
    char colour[15]; //car colour
    int prevOwners; //number of previous owners
    bool reserved; //true if reserved, false if not
    float reserveAmt; //deposit paid if car is reserved
};


//Stucture template for one node
struct LinearNode {
    struct Car car;
	struct LinearNode *next;
};


//Global Variables
FILE *fp; //file pointer
struct LinearNode *front = NULL; //front of the list
struct LinearNode *last = NULL; //end of the list
int currentSize = 0; //how many cars are currently in the showroom

void main() {
    int input;
    char regInput[8];
    char viewInput[20];
    bool exit = false;

    //import cars from file if it exists
    if ((fp = fopen("car.dat", "r+b")) != NULL) {
        readFromFile(fp);
    }

    do {
        displayMenu();
        scanf("%d", &input);
        switch (input) {
        case 1 :
            system("cls");
            addCar();
            break;
        case 2 :
            system("cls");
            printf("Please enter the registration number of the car you wish to sell (NOTE: The car must be reserved to be able to sell it) : ");
            scanf("%s", regInput);
            sellCar(regInput);
            break;
        case 3 :
            system("cls");
            printf("Please enter the registration number of the car you wish to reserve/unreserve : ");
            scanf("%s", regInput);
            carReserve(regInput);
            break;
        case 4 :
            system("cls");
            puts("Please select a menu option corresponding to its number (1-4)\n");
            displaySubMenu();
            scanf("%d", &input);
            system("cls");
            //nested switch-case block for submenu input
            switch (input) {
            case 1 :
                viewAllCars();
                break;
            case 2 :
                viewUnreserved();
                break;
            case 3 :
                printf("Please enter the make of the cars you wish to view : ");
                fflush(stdin);
                gets(viewInput);
                fflush(stdin);
                viewMake(viewInput);
                break;
            case 4 :
                printf("Please enter the colour of the cars you wish to view : ");
                fflush(stdin);
                gets(viewInput);
                fflush(stdin);
                viewColour(viewInput);
                break;
            }//end switch-case
            break;
        case 5 :
            system("cls");
            printf("Please enter the registration number of the car you wish to view : ");
            scanf("%s", regInput);
            viewSpecificCar(regInput);
            break;
        case 6 :
            system("cls");
            printf("Please enter the registration number of the car whose details you wish to update : ");
            scanf("%s", regInput);
            updateCar(regInput);
            break;
        case 7 :
            system("cls");
            exit = true;
            if (currentSize >= 0) {
                saveToFile(fp);
            }
            printf("Goodbye!");
            break;
        }//end switch-case
    }while (!exit);
}//end main

void addCar() {
    struct LinearNode *aNode;
	struct Car aCar;

    //no more than 5 cars to be added into the showroom
	if (currentSize == MAX) {
        printf("ERROR - Showroom has reached maximum capacity. No space for the new car\n");
        return;
	}

    printf("Entering new car into the showroom...\n");

    do {
    printf("Enter the registration number in the format 'yyDxnnnn' (Example : 15D21234) : \n");
    scanf("%s", aCar.reg);
    fflush(stdin);
    }while (isUnique(aCar.reg) == false || verifyReg(aCar.reg) == false);//end do-while

    printf("Enter the make and model : \n");
    gets(aCar.make);
    fflush(stdin);

    printf("Enter the colour : \n");
    gets(aCar.colour);
    fflush(stdin);

    do {
    printf("Enter the number of previous owners (0 if this is a brand new car) : \n");
    scanf("%d", &(aCar.prevOwners));
    if (aCar.prevOwners < 0 || aCar.prevOwners > 3) {
        printf("\nERROR - Invalid input. No more than 3 previous owners are allowed for a car to be listed in the showroom\n");
    }
    }while (aCar.prevOwners < 0 || aCar.prevOwners > 3);//end do-while

    printf("Enter '1' if this car is reserved OR '0' if this car is not reserved : \n");
    scanf("%d", &(aCar.reserved));
    if (aCar.reserved == 1) {
        printf("Enter the reserve amount for this car : \n");
        scanf("%f", &(aCar.reserveAmt));
    }
    else {
        aCar.reserveAmt = 0;
    }//end else

    //create space in memory for a LinearNode struct
    aNode = (struct LinearNode *)malloc(sizeof(struct LinearNode));

    //add data part to the node
    aNode->next = NULL;
    aNode->car = aCar;
    currentSize++; //keeps track of number of cars in showroom

    //add node to front of the list
    if (isEmpty())
    {
        front = aNode;
        last = aNode;
    }
    else {
        last->next = aNode;
        last = aNode;
    }//end else

    system("cls");
    printf("\nCar successfully added to showroom!\n");
}//end addCar

void sellCar(char *regNum) {
    struct LinearNode *current, *previous;
	bool found = false;

	if (isEmpty()) {
		printf("\nERROR - there are no cars in the showroom\n");
		return;
	}

    printf("\nSearching the showroom for car with registration number %s...\n", regNum);
    current = previous = front;

    //searching for a reserved car in the showroom with given registration number
    while (!found && current != NULL) {
        if (strcmp(regNum, current->car.reg) == 0 && current->car.reserved == true)
            found = true;
        else {
            previous = current;
            current = current->next;
        }//end else
    }//end while

    if (!found) {
        printf("\nERROR - there is no such car in the showroom with the registration number %s that is reserved\n", regNum);
        return;
    }

    //removing car from showroom
    if (current == front) {
        front = front->next;
        free(current);
    }
    else if (current == last){
        last = previous;
        last->next=NULL;
        free(current);
    }
    else {
        previous->next = current->next;
        free(current);
    }//end else

    currentSize--;
    printf("\nCar with registration number %s has been successfully sold\n", regNum);
    printf("\nPress the ENTER key to return to the main menu\n");
    getchar();
    getchar();
    system("cls");
}//end sellCar

void carReserve(char *regNum) {
    struct LinearNode *current;
    struct Car *aCar;
    bool found = false;

    if (isEmpty()) {
        printf("\nERROR - there are no cars in the showroom\n");
        return;
    }

    printf("\nSearching the showroom for car with registration number %s...\n", regNum);
    current = front;

    //searching for car in the showroom with given registration number
    while (!found && current != NULL) {
        if (strcmp(regNum, current->car.reg) == 0) {
            aCar = &(current->car);
            printf("Car found!\n");
            found = true;
        }
        else {
            current = current->next;
        }//end else
    }//end while

    if (!found) {
        printf("\nERROR - there is no such car in the showroom with the registration number %s\n", regNum);
        return;
    }

    //checking car's initial reserve status
    if (aCar->reserved == false) {
        printf("This car is currently not reserved.\n");
        do {
        printf("Enter the deposit amount to reserve this car (deposit must be between €500.00 and €1500.00) : \n");
        scanf("%f", &aCar->reserveAmt);
        }while (!(aCar->reserveAmt >= 500.00 && aCar->reserveAmt <= 1500.00));//end do-while
        aCar->reserved = true;
    }
    else {
        printf("This car is currently reserved.\n");
        printf("Enter '0' to unreserve this car OR enter '1' to keep it reserved : \n");
        scanf("%d", &aCar->reserved);
    }//end else

    //check car's 'final' reserve status
    if (aCar->reserved == true) {
        printf("Car with registration number %s is currently reserved for €%.2f\n", aCar->reg, aCar->reserveAmt);
    }
    else {
        printf("Car with registration number %s is currently unreserved\n", aCar->reg);
    }//end else
    printf("\nPress the ENTER key to return to the main menu\n");
    getchar();
    getchar();
    system("cls");
}//end carReserve

void viewAllCars() {
    struct LinearNode *current;

	if (isEmpty()) {
		printf("\nERROR - there are no cars in the showroom\n");
		return;
	}

    printf("Displaying all cars currently in the showroom...\n");
    current = front;
    while (current != NULL) {
        //display current car's details
        printf("\nCar registration number is %s\n", (current->car.reg));
        printf("Car make is %s\n", current->car.make);
        printf("Car colour is %s\n", current->car.colour);
        printf("Number of previous owners for this car is %d\n", current->car.prevOwners);
        //check current car's reserve status
        if (current->car.reserved == true) {
            printf("This car is reserved and it's reserve amount is €%.2f\n", current->car.reserveAmt);
        }
        else {
            printf("This car is not reserved\n");
        }//end else
        //move on to next car in the showroom
        current = current->next;
    }//end while
    printf("\nPress the ENTER key to return to the main menu\n");
    getchar();
    getchar();
    system("cls");
}//end viewAllCars

void viewUnreserved() {
    struct LinearNode *current;

	if (isEmpty()) {
		printf("\nERROR - there are no cars in the showroom\n");
		return;
	}

    printf("Displaying all unreserved cars currently in the showroom...\n");
    current = front;
    while (current != NULL) {
        //search for cars in the showroom that are unreserved
        if (current->car.reserved == false) {
        //display unreserved car's details
        printf("\nCar registration number is %s\n", (current->car.reg));
        printf("Car make is %s\n", current->car.make);
        printf("Car colour is %s\n", current->car.colour);
        printf("Number of previous owners for this car is %d\n", current->car.prevOwners);
        }
        //move on to next car in the showroom
        current = current->next;
    }//end while
    printf("\nPress the ENTER key to return to the main menu\n");
    getchar();
    getchar();
    system("cls");
}//end viewUnreserved

void viewMake(char *carMake) {
    struct LinearNode *current;

	if (isEmpty()) {
		printf("\nERROR - there are no cars in the showroom\n");
		return;
	}

    printf("Displaying all cars of the make %s currently in the showroom...\n", carMake);
    current = front;
    while (current != NULL) {
        //search for cars in the showroom with the given make
        if (strcmp(carMake, current->car.make) == 0) {
        //display details of a car with the given make
        printf("\nCar registration number is %s\n", (current->car.reg));
        printf("Car make is %s\n", current->car.make);
        printf("Car colour is %s\n", current->car.colour);
        printf("Number of previous owners for this car is %d\n", current->car.prevOwners);
            //check current car's reserve status
            if (current->car.reserved == true) {
                printf("This car is reserved and it's reserve amount is €%.2f\n", current->car.reserveAmt);
            }
            else {
                printf("This car is not reserved\n");
            }//end else
        }
        //move on to next car in the showroom
        current = current->next;
    }//end while
    printf("\nPress the ENTER key to return to the main menu\n");
    getchar();
    getchar();
    system("cls");
}//end viewMake

void viewColour(char *carColour) {
    struct LinearNode *current;

	if (isEmpty()) {
		printf("\nERROR - there are no cars in the showroom\n");
		return;
	}

    printf("Displaying all cars of the colour %s currently in the showroom...\n", carColour);
    current = front;
    while (current != NULL) {
        //search for cars in the showroom with the given colour
        if (strcmp(carColour, current->car.colour) == 0) {
        //display details of a car with the given colour
        printf("\nCar registration number is %s\n", (current->car.reg));
        printf("Car make is %s\n", current->car.make);
        printf("Car colour is %s\n", current->car.colour);
        printf("Number of previous owners for this car is %d\n", current->car.prevOwners);
        //check current car's reserve status
            if (current->car.reserved == true) {
                printf("This car is reserved and it's reserve amount is €%.2f\n", current->car.reserveAmt);
            }
            else {
                printf("This car is not reserved\n");
            }//end else
        }
        //move on to next car in the showroom
        current = current->next;
    }//end while
    printf("\nPress the ENTER key to return to the main menu\n");
    getchar();
    getchar();
    system("cls");
}//end viewColour

void viewSpecificCar(char *regNum) {
    struct LinearNode *current;
    struct Car aCar;
	bool found = false;

	if (isEmpty()) {
        printf("\nERROR - there are no cars in the showroom\n");
        return;
    }

    printf("\nSearching the showroom for car with registration number %s...\n", regNum);
    current = front;

    //searching for car in the showroom with given registration number
    while (!found && current != NULL) {
        if (strcmp(regNum, current->car.reg) == 0) {
            aCar = current->car;
            printf("Car found!\n");
            found = true;
        }
        else {
            current = current->next;
        }//end else
    }//end while

    if (!found) {
        printf("\nERROR - there is no such car in the showroom with the registration number %s\n", regNum);
        return;
    }

    //print out the desired car's details
    printf("\nCar registration number is %s\n", (aCar.reg));
    printf("Car make is %s\n", aCar.make);
    printf("Car colour is %s\n", aCar.colour);
    printf("Number of previous owners for this car is %d\n", aCar.prevOwners);
    //check current car's reserve status
    if (aCar.reserved == true) {
        printf("This car is reserved and it's reserve amount is €%.2f\n", aCar.reserveAmt);
    }
    else {
        printf("This car is not reserved\n");
    }//end else
    printf("\nPress the ENTER key to return to the main menu\n");
    getchar();
    getchar();
    system("cls");
}//end viewSpecificCar

void updateCar(char *regNum) {
     struct LinearNode *current;
     struct Car *aCar;
     bool found;

	if (isEmpty()) {
		printf("\nERROR - there are no cars in the showroom\n");
		return;
	}

	printf("\nRetrieving the details for the car with registration number %s...\n", regNum);
    current = front;

    //searching for car in the showroom with the given registration number
    while (!found && current != NULL) {
        if (strcmp(regNum, current->car.reg) == 0) {
            aCar = &(current->car);
            printf("Car found!\n");
            found = true;
        }
        else {
            current = current->next;
        }//end else
    }//end while

    if (!found) {
        printf("\nERROR - there is no such car in the showroom with the registration number %s\n", regNum);
        return;
    }

    //update make
    printf("This car's current make is %s. Enter the new make : \n", aCar->make);
    fflush(stdin);
    gets(aCar->make);
    fflush(stdin);

    //update colour
    printf("This car's current colour is %s. Enter the new colour : \n", aCar->colour);
    fflush(stdin);
    gets(aCar->colour);
    fflush(stdin);

    //update number of previous owners
    do {
    printf("This car has %d previous owners. Enter the new number of previous owners (0 if this is a brand new car) : \n", aCar->prevOwners);
    scanf("%d", &(aCar->prevOwners));
    if (aCar->prevOwners < 0 || aCar->prevOwners > 3) {
        printf("\nError - Invalid input. No more than 3 previous owners are allowed for a car to be listed in the showroom\n");
    }
    }while (aCar->prevOwners < 0 || aCar->prevOwners > 3);//end do-while

    //if car is reserved, update reserve amount
    if (aCar->reserved == true) {
        do {
        printf("This car currently has a reserve amount of €%.2f. Enter the new reserve amount (must be between €500.00 and €1500.00) : \n", aCar->reserveAmt);
        scanf("%f", &aCar->reserveAmt);
        }while (!(aCar->reserveAmt >= 500.00 && aCar->reserveAmt <= 1500.00));//end do-while
    }
    printf("\nThe details for the car with registration number %s have been successfully updated\n", regNum);
    printf("\nPress the ENTER key to return to the main menu\n");
    getchar();
    getchar();
    system("cls");
}//end updateCar

bool isEmpty() {
	if (front == NULL)
		return true;
	else
		return false;
}//end isEmpty

bool isUnique(char *regNum) {
    struct LinearNode *current = front;

    //search showroom for a car that already exists with the registration number given by the user
    while (current != NULL) {
        if (strcmp(regNum, current->car.reg) == 0) {;
            //car with the given registration number found
            printf("\nERROR - The car with this registration already exists in the showroom! Registration number must be unique.\n");
            return false;
        }
        else {
            current = current->next;
        }//end else
    }//end while
    return true;
}

bool verifyReg(char *regNum) {
    bool valid = true;

    //check if registration number is exactly 8 characters long
    if (strlen(regNum) < 8 || strlen(regNum) > 8) {
        valid = false;
    }

    //check if registration number has the correct year format
    if (regNum[0] != '1' && regNum[0] != '2') {
        valid = false;
    }

    //check if registration number has the correct year format
    if ((regNum[0] == '1' && (int)regNum[1] < 4) && (regNum[0] == '2' && (int)regNum[1] > 4)) {
        valid = false;
    }

    //check if registration number contains 'D' as the third character
    if (regNum[2] != 'D') {
        valid = false;
    }

    //check if registration number contains either '1' or '2' to indicate which half of the year it was registered
    if (regNum[3] != '1' && regNum[3] != '2') {
        valid = false;
    }

    //check if remaining characters in registration number are all digits
    for (int i = 4; i < 8; i++) {
        if (!isdigit(regNum[i])) {
            valid = false;
        }
    }//end for

    if (!valid) {
        puts("\nERROR - Invalid registration number.");
    }
    return valid;
}//end verifyReg

void displaySubMenu() {
    puts("1.    View all cars in the showroom");
    puts("2.    View all unreserved car the showroom");
    puts("3.    View all cars of the certain make in the showroom");
    puts("4.    View all cars of a specific colour in the showroom");
}//end displaySubMenu

void displayMenu() {
    //display when there are no cars in the showroom
    if (currentSize == 0) {
        puts("\n***There are currently no cars in the showroom. Add a car to the showroom by selecting option 1***\n");
    }
    printf("\nPlease select a menu option corresponding to its number (1-7)\n");
    puts("1.    Add a new car to the showroom");
    puts("2.    Sell a car from the showroom");
    puts("3.    Reserve/Unreserve a car in the showroom");
    puts("4.    View all cars in the showroom");
    puts("5.    View a specific car in the showroom");
    puts("6.    Update the details for a specific car in the showroom");
    puts("7.    Exit the system");
}//end displayMenu

void saveToFile(FILE *fp) {
    struct LinearNode *current;
    fp = fopen("car.dat", "w+b");

    current = front;
    while (current != NULL) {
        fwrite(&(current->car), sizeof(struct Car), 1, fp);
        current = current->next;
    }//end while
    printf("\nAll cars in the showroom have been successfully saved\n");
    fclose(fp);
}//end saveToFile

void readFromFile(FILE *fp) {
    struct Car aCar;
    struct LinearNode *aNode;

    printf("File with cars found! Importing cars from file into showroom...\n");
    while ((fread(&aCar, sizeof(struct Car), 1, fp)) != NULL) {
        //create space in memory for a LinearNode struct
        aNode = (struct LinearNode *)malloc(sizeof(struct LinearNode));

        //add data part to the node
        aNode->next = NULL;
        aNode->car = aCar;
        currentSize++; //keeps track of number of cars in showroom

        //add node to front of the list
        if (isEmpty()) {
            front = aNode;
            last = aNode;
        }
        else {
            last->next = aNode;
            last = aNode;
        }//end else
    }//end while
    printf("\nImported all cars from the file into the showroom successfully\n");
    fclose(fp);
}//end readFromFile
