#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define HISTORY_SIZE 5
int used_space = 0;

//function prototypes
void displayLEDs(uint8_t* LEDs);
void turnOnAllLEDs(uint8_t* LEDS);
void turnOffAllLEDs(uint8_t* LEDS);
void toggleLED(uint8_t* LEDs, int led_num);
void turnOnLED(uint8_t* LEDs, int led_num);
void turnOffLED(uint8_t* LEDs, int led_num);
void saveState(uint8_t* LEDs, uint8_t history[], int* history_index);
void displayHistory(uint8_t* LEDs, uint8_t history[], int* history_index);

int main(){
    uint8_t history[HISTORY_SIZE];
    uint8_t LEDs = 0;
    int history_index = 0;
    int led_num = 0;
    int choice = 0;

    while (1) {
        printf("\n-------------------------");
        printf("\n8-bit LED Controller Menu\n");
        printf("1. Toggle LED\n");
        printf("2. Turn ON LED\n");
        printf("3. Turn OFF LED\n");
        printf("4. Turn ON All LEDs\n");
        printf("5. Turn OFF All LEDs\n");
        printf("6. Display LED State\n");
        printf("7. Save LED state\n");
        printf("8. Show LED State History\n");
        printf("9. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        printf("-------------------------\n");

        switch (choice) {
            case 1:
                printf("Enter LED number (0-7): ");
                scanf("%d", &led_num);
                toggleLED(&LEDs, led_num);
                break;
            case 2:
                printf("Enter LED number (0-7): ");
                scanf("%d", &led_num);
                turnOnLED(&LEDs, led_num);
                break;
            case 3:
                printf("Enter LED number (0-7): ");
                scanf("%d", &led_num);
                turnOffLED(&LEDs, led_num);
                break;
            case 4:
                turnOnAllLEDs(&LEDs);
                break;
            case 5:
                turnOffAllLEDs(&LEDs);
                break;
            case 6:
                displayLEDs(&LEDs);
                break;
            case 7:
                saveState(&LEDs, history, &history_index);
                break;
            case 8:
                displayHistory(&LEDs, history, &history_index);
                break;
            case 9:
                printf("Exiting Program\n");
                return 0;
            default:
                printf("Invalid choice, try again.\n");
        }
    }
    return 0;
}


void displayLEDs(uint8_t* LEDs){
    uint8_t mask = 1;
    uint8_t number = 0;
    mask = mask << 7;
    
    for(int i = 0; i < 8; i++){//iterate through all LED bits

        if(i == 4){//add a space after displaying four bits
            printf(" ");
        }

        if((*LEDs & mask) == mask){//check is there is a 1 at mask position (or is same value as mask)
            printf("%i", 1);//print a 1 if a 1 is found at same mask position
        }else{
            printf("%i", 0);//print 0 if a 1 is not found at same mask position
        }
        mask = mask >> 1;//shift mask to the right by one bit
    }
}

void turnOnAllLEDs(uint8_t* LEDs){
    *LEDs = 0xFF;//turn on all LEDs
}

void turnOffAllLEDs(uint8_t* LEDs){
    *LEDs = 0x00;//turn off all LEDs
}

void toggleLED(uint8_t* LEDs, int led_num){
    uint8_t mask = 1;
    mask = mask << led_num;//left shift the masking bit to the desired amount
    *LEDs = *LEDs ^ mask;//perform bit wise XOR which does toggling
}

void turnOnLED(uint8_t* LEDs, int led_num){
    uint8_t mask = 1;
    mask = mask << led_num;//left shift to desired position
    *LEDs = *LEDs | mask;//bitwise OR to set the corresponding bit value high
}

void turnOffLED(uint8_t* LEDs, int led_num){
    uint8_t mask = 0xFE;//preseve the lsb bit as a 0
    for(int i = 1; i <= led_num; i++){
        if(led_num == 0){
            break;
        }
        mask = mask << 1;
        mask = mask | 1;//make sure all ones are present within the mask except for the 0
    }
    *LEDs = *LEDs & mask;//bitwise AND operation to set bit position value to low
}

void saveState(uint8_t* LEDs, uint8_t history[], int* history_index){
    //fifo data structure
    if(!(used_space == HISTORY_SIZE)){
        used_space++;
    }
    
    if(*history_index == HISTORY_SIZE){
        *history_index = 0;//reset index to 0 to overwrite the next stored values
    }

    //last recently added number is removed
    //this is essentially a fifo data structure
    history[*history_index] = *LEDs;
    *history_index = *history_index + 1;
    printf("LED State Saved\n");
}

void displayHistory(uint8_t* LEDs, uint8_t history[], int* history_index){
    printf("Previous 5 states\n");
    printf("-----Oldest-----\n");
    if(used_space == HISTORY_SIZE){//print from current position in the array until the end
        for(int i = *history_index; i < used_space; i++){
            uint8_t value = history[i];
            displayLEDs(&value);
            printf("\n");
        }
    
        for(int i = 0; i < *history_index; i++){//print from the 0th position to the current position in the array
            uint8_t value = history[i];
            displayLEDs(&value);
            printf("\n");
        }
    }else{
        for(int i = 0; i < *history_index; i++){//print values from 0 to current position
            uint8_t value = history[i];
            displayLEDs(&value);
            printf("\n");
        }
        for(int i = *history_index; i < HISTORY_SIZE; i++){//print empty space
            printf("empty space\n");
        }
    }
    printf("-----Newest-----");
    return;
}
