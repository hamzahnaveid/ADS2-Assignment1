#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define true 1
#define false (!true)

#define MAX 5 //max size of showroom

//Stucture template for one node
struct LinearNode {
    struct Car *car;
	struct LinearNode *next;
};


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
bool isEmpty(); //returns true if there are no cars in the showroom
void displaySubMenu(); //display menu for options viewing cars in the showroom
void displayMenu(); //display user menu


//Global Variables
struct LinearNode *front = NULL; //front of the list
struct LinearNode *last = NULL; //end of the list
int currentSize = 0; //how many cars are currently in the showroom


//Structure template for a car
struct Car {
    char reg[10]; //car registration
    char make[20]; //car make
    char colour[15]; //car colour
    int prevOwners; //number of previous owners
    bool reserved; //true if reserved, false if not
    float reserveAmt; //deposit paid if car is reserved
};

void main() {
    int input;
    char regInput[8];
    char viewInput[20];
    bool exit = false;

    do {
        displayMenu();
        printf("\nPlease select a menu option corresponding to its number (1-7)\n");
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
            exit = true;
            break;
        }//end switch-case
    }while (!exit);
}

void addCar() {
    struct LinearNode *aNode;
    struct LinearNode *current;
	struct Car *aCar;
	bool found;

    //no more than 5 cars to be added into the showroom
	if (currentSize == MAX) {
        printf("Error - Showroom has reached maximum capacity. No space for the new car\n");
        return;
	}

    printf("Entering new car into the showroom...\n");

    //create space in memotry for a Car struct
    aCar = (struct Car *)malloc(sizeof(struct Car));

    do {
    printf("Enter the registration number : \n");
    scanf("%s", aCar->reg);
    fflush(stdin);
    //TO-DO: Error-checking code here
    found = false;
    current = front;

    //search showroom for a car that already exists with the registration number given by the user
    while (!found && current != NULL) {
        if (strcmp(aCar->reg, current->car->reg) == 0) {;
            found = true;
            printf("\nError - The car with this registration already exists in the showroom! Registration number must be unique.\n");
        }
        else {
            current = current->next;
        }//end else
    }//end while
    }while (strlen(aCar->reg) < 8 || strlen(aCar->reg) > 8 || found == 1);//end do-while

    printf("Enter the make and model : \n");
    gets(aCar->make);
    fflush(stdin);

    printf("Enter the colour : \n");
    gets(aCar->colour);
    fflush(stdin);

    do {
    printf("Enter the number of previous owners (0 if this is a brand new car) : \n");
    scanf("%d", &(aCar->prevOwners));
    if (aCar->prevOwners < 0 || aCar->prevOwners > 3) {
        printf("\nError - Invalid input. No more than 3 previous owners are allowed for a car to be listed in the showroom\n");
    }
    }while (aCar->prevOwners < 0 || aCar->prevOwners > 3);//end do-while

    printf("Enter '1' if this car is reserved OR '0' if this car is not reserved : \n");
    scanf("%d", &(aCar->reserved));
    if (aCar->reserved == 1) {
        printf("Enter the reserve amount for this car : \n");
        scanf("%f", &(aCar->reserveAmt));
    }
    else {
        aCar->reserveAmt = 0;
    }//end else

    //create space in memory for a LinearNode struct
    aNode = (struct LinearNode *)malloc(sizeof(struct LinearNode));

    // add data part to the node
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
		printf("\nError - there are no cars in the showroom\n");
		return;
	}

    printf("\nSearching the showroom for car with registration number %s...\n", regNum);
    current = previous = front;

    //searching for a reserved car in the showroom with given registration number
    while (!found && current != NULL) {
        if (strcmp(regNum, current->car->reg) == 0 && current->car->reserved == true)
            found = true;
        else {
            previous = current;
            current = current->next;
        }//end else
    }//end while

    if (!found) {
        printf("\nError - there is no such car in the showroom with the registration number %s that is reserved\n", regNum);
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
}//end sellCar

void carReserve(char *regNum) {
    struct LinearNode *current;
    struct Car *aCar;
    bool found = false;

    if (isEmpty()) {
        printf("\nError - there are no cars in the showroom\n");
        return;
    }

    printf("\nSearching the showroom for car with registration number %s...\n", regNum);
    current = front;

    //searching for car in the showroom with given registration number
    while (!found && current != NULL) {
        if (strcmp(regNum, current->car->reg) == 0) {
            aCar = current->car;
            printf("Car found!\n");
            found = true;
        }
        else {
            current = current->next;
        }//end else
    }//end while

    if (!found) {
        printf("\nError - there is no such car in the showroom with the registration number %s\n", regNum);
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
}//end carReserve

void viewAllCars() {
    struct LinearNode *current;

	if (isEmpty()) {
		printf("\nError - there are no cars in the showroom\n");
		return;
	}

    printf("Displaying all cars currently in the showroom...\n");
    current = front;
    while (current != NULL) {
        printf("\nCar registration number is %s\n", (current->car->reg));
        printf("Car make is %s\n", current->car->make);
        printf("Car colour is %s\n", current->car->colour);
        printf("Number of previous owners for this car is %d\n", current->car->prevOwners);
        //check current car's reserve status
        if (current->car->reserved == true) {
            printf("This car is reserved and it's reserve amount is €%.2f\n", current->car->reserveAmt);
        }
        else {
            printf("This car is not reserved\n");
        }//end else
        //move on to next car in the showroom
        current = current->next;
    }//end while
}//end viewAllCars

void viewUnreserved() {
    struct LinearNode *current;

	if (isEmpty()) {
		printf("\nError - there are no cars in the showroom\n");
		return;
	}

    printf("Displaying all unreserved cars currently in the showroom...\n");
    current = front;
    while (current != NULL) {
        if (current->car->reserved == false) {
        printf("\nCar registration number is %s\n", (current->car->reg));
        printf("Car make is %s\n", current->car->make);
        printf("Car colour is %s\n", current->car->colour);
        printf("Number of previous owners for this car is %d\n", current->car->prevOwners);
        }
        //move on to next car in the showroom
        current = current->next;
    }//end while
}//end viewUnreserved

void viewMake(char *carMake) {
    struct LinearNode *current;

	if (isEmpty()) {
		printf("\nError - there are no cars in the showroom\n");
		return;
	}

    printf("Displaying all cars of the make %s currently in the showroom...\n", carMake);
    current = front;
    while (current != NULL) {
        if (strcmp(carMake, current->car->make) == 0) {
        printf("\nCar registration number is %s\n", (current->car->reg));
        printf("Car make is %s\n", current->car->make);
        printf("Car colour is %s\n", current->car->colour);
        printf("Number of previous owners for this car is %d\n", current->car->prevOwners);
            //check current car's reserve status
            if (current->car->reserved == true) {
                printf("This car is reserved and it's reserve amount is €%.2f\n", current->car->reserveAmt);
            }
            else {
                printf("This car is not reserved\n");
            }//end else
        }
        //move on to next car in the showroom
        current = current->next;
    }//end while
}//end viewMake

void viewColour(char *carColour) {
    struct LinearNode *current;

	if (isEmpty()) {
		printf("\nError - there are no cars in the showroom\n");
		return;
	}

    printf("Displaying all cars of the colour %s currently in the showroom...\n", carColour);
    current = front;
    while (current != NULL) {
        if (strcmp(carColour, current->car->colour) == 0) {
        printf("\nCar registration number is %s\n", (current->car->reg));
        printf("Car make is %s\n", current->car->make);
        printf("Car colour is %s\n", current->car->colour);
        printf("Number of previous owners for this car is %d\n", current->car->prevOwners);
        //check current car's reserve status
            if (current->car->reserved == true) {
                printf("This car is reserved and it's reserve amount is €%.2f\n", current->car->reserveAmt);
            }
            else {
                printf("This car is not reserved\n");
            }//end else
        }
        //move on to next car in the showroom
        current = current->next;
    }//end while
}//end viewColour

void viewSpecificCar(char *regNum) {
    struct LinearNode *current;
    struct Car *aCar;
	bool found = false;

	if (isEmpty()) {
        printf("\nError - there are no cars in the showroom\n");
        return;
    }

    printf("\nSearching the showroom for car with registration number %s...\n", regNum);
    current = front;

    //searching for car in the showroom with given registration number
    while (!found && current != NULL) {
        if (strcmp(regNum, current->car->reg) == 0) {
            aCar = current->car;
            printf("Car found!\n");
            found = true;
        }
        else {
            current = current->next;
        }//end else
    }//end while

    if (!found) {
        printf("\nError - there is no such car in the showroom with the registration number %s\n", regNum);
        return;
    }

    //print out the desired car's details
    printf("\nCar registration number is %s\n", (aCar->reg));
    printf("Car make is %s\n", aCar->make);
    printf("Car colour is %s\n", aCar->colour);
    printf("Number of previous owners for this car is %d\n", aCar->prevOwners);
    //check current car's reserve status
    if (aCar->reserved == true) {
        printf("This car is reserved and it's reserve amount is €%.2f\n", aCar->reserveAmt);
    }
    else {
        printf("This car is not reserved\n");
    }//end else
}//end viewSpecificCar

void updateCar(char *regNum) {
     struct LinearNode *current;
     struct Car *aCar;
     bool found;

	if (isEmpty()) {
		printf("\nError - there are no cars in the showroom\n");
		return;
	}

	printf("\nRetrieving the details for the car with registration number %s...\n", regNum);
    current = front;

    //searching for car in the showroom with the given registration number
    while (!found && current != NULL) {
        if (strcmp(regNum, current->car->reg) == 0) {
            aCar = current->car;
            printf("Car found!\n");
            found = true;
        }
        else {
            current = current->next;
        }//end else
    }//end while

    if (!found) {
        printf("\nError - there is no such car in the showroom with the registration number %s\n", regNum);
        return;
    }

    printf("This car's current make is %s. Enter the new make : \n", aCar->make);
    fflush(stdin);
    gets(aCar->make);
    fflush(stdin);

    printf("This car's current colour is %s. Enter the new colour : \n", aCar->colour);
    fflush(stdin);
    gets(aCar->colour);
    fflush(stdin);

    do {
    printf("This car has %d previous owners. Enter the new number of previous owners (0 if this is a brand new car) : \n", aCar->prevOwners);
    scanf("%d", &(aCar->prevOwners));
    if (aCar->prevOwners < 0 || aCar->prevOwners > 3) {
        printf("\nError - Invalid input. No more than 3 previous owners are allowed for a car to be listed in the showroom\n");
    }
    }while (aCar->prevOwners < 0 || aCar->prevOwners > 3);//end do-while

    if (aCar->reserved == true) {
        do {
        printf("This car currently has a reserve amount of €%.2f. Enter the new reserve amount (must be between €500.00 and €1500.00) : \n", aCar->reserveAmt);
        scanf("%f", &aCar->reserveAmt);
        }while (!(aCar->reserveAmt >= 500.00 && aCar->reserveAmt <= 1500.00));//end do-while
    }
}//end updateCar

bool isEmpty() {
	if (front == NULL)
		return true;
	else
		return false;
}//end isEmpty

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
    puts("1.    Add a new car to the showroom");
    puts("2.    Sell a car from the showroom");
    puts("3.    Reserve/Unreserve a car in the showroom");
    puts("4.    View all cars in the showroom");
    puts("5.    View a specific car in the showroom");
    puts("6.    Update the details for a specific car in the showroom");
    puts("7.    Exit the system");
}//end displayMenu
