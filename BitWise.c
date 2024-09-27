#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include <sys/stat.h>
#include <fcntl.h>



int main(int argc, char *argv[])
{
	int sel;
	char word[1024];
	unsigned char reg = 0;
	while(1)
	{
		printf("1)\tAuthor info\n2)\tcheck status\n3)\tclear status\n4)\tsave status\n5)\tload status\n6)\tSet user value\n7)\tset LED color\n8)\ttoggle reset\n9)\tturn on\n10)\tturn off\n11)\tShift\n12)\tSet value\n0)\tExit\n");

        scanf("%d", &sel);  //User input

        //------------------------------------User Input Handling---------------------------------
        switch(sel){    //The switch statement will go to the case that is selectd by the user
            case 0:
            printf("Goodbye!"); //Program exits the loop and quits if 0 is selceted
            break;
            
            case 1:
            printf("Noe Cifuentes\n"); //My name is printed if 1 is selcted
            break;

            case 2:
            if((reg & 0x01) == 0){    //If 2 is selected by the use, we will first check if the power bit (first bit) is off, where we will print off and end that sequence
                printf("OFF\n");
                break;
            }
            else{   //If the bit isn't off, we will go on to print the status
                printf("Color: ");
                switch((reg >> 5) & 0b00000011){    //shifting to only focus on the color bits, using AND to determine which color gets printed
                    case 0b00000000:
                    printf("OFF\n");
                    break;
                    case 0b00000001:
                    printf("RED\n");
                    break;
                    case 0b00000010:
                    printf("GREEN\n");
                    break;
                    case 0b00000011:
                    printf("BLUE\n");
                    break;
                }
                if((reg & 0b10000000) == 0b10000000){    //Now checking the reset pin, if on, we will print high, if off, print low
                    printf("reset pin: High\n");
                }
                else{
                    printf("reset pin: Low\n");
                }
                if((reg & 0b00000001)){     //Checking the power pin, hypotheically it should always print on, it wouldn't get to this part of the code if it wasn't
                    printf("power pin: ON\n");
                }
                else{
                    printf("power pin: OFF\n");
                }
                printf("user value: %d\n", (reg >> 3) & 0b00000011);  //for printing the user value. We are first isolating the user bits, and printing them using a using a %d formatting
            }
            break;  //The break for the entire original user selected 2

            case 3:     //User input 3 that sets all bits to 0
            reg = 0;
            break;

            //Source: GeekforGeeks, https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/
            case 4:     //C Descriptor, saving the current reg value to a binary file
            printf("Type in file name: \n");    //User types in the file into the terminal
            while (getchar() != '\n');
            scanf("%s", word);
            sel = open(word, O_WRONLY | O_CREAT, 0644);     //We will open, or create a binary file if it doesnt exist
            if (sel != -1){
                write(sel, &reg, sizeof(reg));  //Writing reg into the fil
                close(sel);  //Closing file after we are done
            }
            break;

            //Source: GeekforGeeks, https://www.geeksforgeeks.org/input-output-system-calls-c-create-open-close-read-write/
            case 5:     //Loading whatever is in the binary file to the temrinal
            printf("Type in file name: \n");
            scanf("%s", word);  //User types in file name inside the terminal
    
            sel = open(word, O_RDONLY);     //Opening the file, in read only mode, so we dont accidently break something in accident lol
            if (sel != -1) {
            if (read(sel, &reg, sizeof(reg)) == sizeof(reg)) {
            printf("Value: %u\n", reg);  //Print the value
            } else {
            printf("Couldn't read the file\n");   //Error handeling just in case
            }
            close(sel);  //Close the file
            } else {
            printf("Error.\n"); //Error handeling
            }
            break;

            case 6: //User will enter 2 bits, first one will replace bit 4 and second one will replace 
            printf("Enter 2 bits: \n");     //Enter one bit, press enter key, enter second bit, press enter key
            while (getchar() != '\n');  //this statement clears the buffer for any /n that were left over. Pretty much leaving a cleaner terminal
            scanf("%c", &word[0]);  //holds new bit 4
            while (getchar() != '\n');
            scanf("%c", &word[1]);  //holds new bit 3
            if(word[0] == '1'){
                reg |= 0b00010000;  //turns on bit 4
            }
            else{
                reg &= 0b11101111;  //turns off bit 4
            }
            if(word[1] == '1'){
                reg |= 0b00001000;  //turns on bit 3
            }
            else{
                reg &= 0b11110111;  //turns off bit 3
            }
            break;

            case 7:     //User chooses what LED color is wanted, changing bits 6 and 5
            printf("Choose a # 1-4 to set the LED color\n");
            printf("1) 00: OFF\n2) 01: RED\n3) 10: Green\n4) 11: Blue\n");
            while (getchar() != '\n');
            scanf("%c", &word[2]);
            switch(word[2]){
                case '1':
                reg &= 0b10011111;  //turns off bits 5 and 6, setting LED to OFF
                break;
                case '2':
                reg &= 0b10011111;  //turns off bits 5 and 6 first (for easier copy and paste)
                reg |= 0b00100000;  //turns on bit 5 to set LED to RED
                break;
                case '3':
                reg &= 0b10011111;  //turns off bits 5 and 6 first
                reg |= 0b01000000;  //turns on bit 6 to set LED to GREEN
                break;
                case '4':
                reg &= 0b10011111;  //turns off bits 5 and 6 first
                reg |= 0b01100000;  //turns on bit 6 and 5 to set LED to BLUE
                break;
                default:
                printf("A number 1-4 was not chosen\n");
                break;
            }
            break;


            case 8:     //Toggling bit 7, the reset bit
            reg ^= 0b10000000;  //using the XOR bitwise operator to invert bit 7
            break;

            case 9:     //Setting bit 0 to 1, to turn on the whole thing
            reg |= 0b00000001;  //Using OR bitwise operator, the 1 bit will force bit 0 to be 1
            break;

            case 10:    //Setting bit 0 to 0, to turn off the whole thing
            reg &= 0b11111110;  //Using AND bitwise operator, the 0 bit will force bit 0 to be 0
            break;

            case 11:    //Shift all bits to the right, user specifies how many bits to shift
            printf("Enter the amount of bits you would like to shift: \n");
            while (getchar() != '\n');  
            scanf("%c", &word[3]);
            reg >>= (word[3] - '0'); //Since we stored our input in a char array, we have to subtract that char type with ASCII value '0' to convert to the appropiate integer
            break;

            case 12:    //Setting reg to the user specified value
            printf("Enter new reg value: ");
            while (getchar() != '\n');  
            scanf("%hhu", &reg);    //The %hhu tells scanf to read an unsigned character from the input, and then assign that value to reg
            break;
        }
        //------------------------------------User Input Handling---------------------------------


        if (sel == 0){  //Quits the program
            break;
        }

	}


	return(0);
}