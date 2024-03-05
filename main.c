#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int
#define true 1
#define false (!true)

#define SIZE 5 //max size of showroom

//Stucture template for one node
struct LinearNode {
    struct Car *car;
	struct LinearNode *next;
};


// Function prototypes
void addCar();  //add a car to the showroom
void sellCar(char *); //sell a car from the showroom
void carReserve(char *); //reserve/unreserve a car from the showroom
void viewAllCars(); //view all cars in the showroom
void viewSpecificCar(char *); //view a specific car in the showroom
void functionTBD(); //extra feature to be decided
bool isEmpty(); //returns true if there are no cars in the showroom
void displayMenu(); //display user menu


// Global Variables
struct LinearNode *front = NULL; //front of the list
struct LinearNode *last = NULL;


//Structure template for a car
struct Car {
    char reg[8]; //car registration
    char make[20]; //car make
    char colour[15]; //car colour
    int prevOwners; //number of previous owners
    bool reserved; //true if reserved, false if not
    float reserveAmt; //deposit paid if car is reserved
};

void main()
{
    int input;
    char regInput[8];
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
            printf("Please enter the registration number of the car you wish to sell : ");
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
            viewAllCars();
            break;
        case 5 :
            system("cls");
            printf("Please enter the registration number of the car you wish to view : ");
            scanf("%s", regInput);
            viewSpecificCar(regInput);
            break;
        case 6 :
            system("cls");
            //function TBD
            break;
        case 7 :
            exit = true;
            break;
        }//end switch
    }while (!exit);
}

void addCar() {

} //end addCar

void sellCar(char *regNum) {

}// end sellCar

void carReserve(char *regNum) {

}//end carReserve

void viewAllCars() {

} //end viewAllCars

void viewSpecificCar(char *regNum) {

}//end viewSpecificCar

void functionTBD() {

}//end functionTBD

bool isEmpty() {
	if (front == NULL)
		return true;
	else
		return false;
}//end isEmpty

void displayMenu() {
    puts("1.    Add a new car to the showroom");
    puts("2.    Sell a car from the showroom");
    puts("3.    Reserve/Unreserve a car in the showroom");
    puts("4.    View all cars in the showroom");
    puts("5.    View a specific car in the showroom");
    puts("6.    Function TBD");
    puts("7.    Exit the system");
}//end displayMenu
