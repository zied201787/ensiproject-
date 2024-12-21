#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <windows.h>
const char configpath[100]="path to----->/config files/";

typedef struct {
    int deliveryId;
    char customerName[100];
    double departureX;
    double departureY;
    double arrivalX;
    double arrivalY;
    char receiverPhoneNumber[20];

    char currentTime[20];
    char deliveryTime[20]; // Added for current time char deliveryTime[20];
} Delivery;

//file manipulation
typedef struct Node {
    Delivery delivery;
    struct Node* next;
} Node;
typedef struct Queue {
    Node* front;
    Node* rear;
} queuu;
queuu* createQueue() {
    queuu* queue = (queuu*)malloc(sizeof(queuu));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}
Delivery dequeue(queuu *queue);
void enqueue(queuu* queue, Delivery delivery);

// Function prototypes
void shopping(queuu* queue);
void writeDeliveryToFile(FILE* filname,Delivery delivery);
void getStoreCoordinates(int index, double* storeX, double* storeY);
void printHeader(const char *title);
void printline(char c,int times);
void add_delivery(queuu* queue,int index, int num);
void main_menu() ;

//client interface
void req_delivery(char userid[255]);
void deliverystate(char userid[255]);

//food menu funcs
void sushiSpotMenu(queuu* deliveryQueue);
void mcdonaldMenu(queuu* deliveryQueue);
void Pizzahut(queuu* deliveryQueue);
void shawarmaHouseMenu(queuu* deliveryQueue);
void display_electronics(queuu* queue) ;
void display_drinks(queuu* queue);
void display_snacks(queuu* queue);
//client -> req deliv
void delivery_cli_cli(queuu* basket);
void fast_food(queuu *deliveryQueue);


//time manip funcs
void getCurrentTime(char *buffer) {// conev str ---> char  delivery.current time
    time_t rawTime;
    time(&rawTime);// machine time in unix
    struct tm *timeInfo = localtime(&rawTime);
    strftime(buffer, 20, "%Y-%m-%d_%H:%M:%S", timeInfo);
}//assign requst time

double calculateDistance(double x1, double y1, double x2, double y2) { return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); }

static time_t previousArrivalTime = 0;

void calculateDeliveryTime(double departureX, double departureY,double arrivalX, double arrivalY, char *buffer) {
    double distance = calculateDistance(departureX, departureY, arrivalX, arrivalY);
    double speed = 30.0;  // Assume an average speed of 30 units per hour
    double timeInHours = distance / speed;


    time_t startTime;// unix time
    time_t ref;// current time
    time(&ref);
    if (previousArrivalTime < ref) {//i5er arrival time enrigister fil fichier comparing decimal : note thta unix time give  decimal number  form 1990-1
        time(&startTime);// i4a wsil
    } else {
        startTime = previousArrivalTime;// i4a mawselch
    }

    // Calculate the new arrival time
    time_t newArrivalTime = startTime + (time_t)(timeInHours * 3600);  // Convert hours to seconds

    // Update the static variable
    previousArrivalTime = newArrivalTime;

    // Convert the new arrival time to local time
    struct tm *timeInfo = localtime(&newArrivalTime);
    strftime(buffer, 20, "%Y-%m-%d_%H:%M:%S", timeInfo);
}

time_t convertToTimeT(const char* dateTimeStr) {
    struct tm timeInfo;// struct predefined
    int year, month, day, hour, minute, second;
    sscanf(dateTimeStr, "%d-%d-%d_%d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    timeInfo.tm_year = year - 1900;// conversion  to unix time
    timeInfo.tm_mon = month - 1;
    timeInfo.tm_mday = day;
    timeInfo.tm_hour = hour;
    timeInfo.tm_min = minute;
    timeInfo.tm_sec = second;
    timeInfo.tm_isdst = -1;
    return mktime(&timeInfo);
}

double calculateElapsedPercentage(Delivery delivery, time_t currentTime) {
    time_t startTime = convertToTimeT(delivery.currentTime);
    time_t endTime = convertToTimeT(delivery.deliveryTime);
    double totalDuration = difftime(endTime, startTime);
    double elapsedTime = difftime(currentTime, startTime);
    double percentageElapsed = (elapsedTime / totalDuration) * 100;
    if (percentageElapsed > 100) { percentageElapsed = 100; }
    return percentageElapsed;  }

//--------------------------------------------------------------------------------------------------------------------


void enqueue(queuu* queue, Delivery delivery) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->delivery = delivery;
    newNode->next = NULL;
    if (queue->front == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}//done

Delivery dequeue(queuu *queue) {
    if (queue->front != NULL) {
        Node* temp = queue->front;
        queue->front = queue->front->next;
        Delivery res=temp->delivery;
        free(temp);
        return res;

    }
}//done

void printHeader(const char* title) {
    printf("====================================\n");
    printf("|| %-30s ||\n", title);
    printf("====================================\n");
}//done

void printline(const char c,int times) {
    for (int i = 0; i < times; i++){
        printf("%c",c);
    }
    printf("\n");
}

void getStoreCoordinates(int index, double* storeX, double* storeY) {
    if (index==0) {//mcdonnalds
        *storeX = 10.0;
        *storeY = 20.0;
    } else if (index== 1) {//pizzahutt
        *storeX = 30.0;
        *storeY = 40.0;
    }else if (index==2) {//sushispot
        *storeX = 50.0;
        *storeY = 60.0;


    }else if (index==3) {//chawarmahouse
        *storeX = 100.0;
        *storeY = 200.0;
    }
    else if(index==4) {
        *storeX = 300.0;
        *storeY = 400.0;
    }
    else if(index==5) {
        *storeX = 500.0;
        *storeY = 600.0;
    }
}//done

void displayMenu(const char *filename) {
    char buffer[255];
    strcpy(buffer, configpath);
    strcat(buffer, "storesdata/");
    strcat(buffer,filename);
    FILE *file = fopen(buffer, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }
    char line[255];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}//done
static int deliveryId;
void add_delivery(queuu* queue,int index, int num) {
    Delivery delivery;



    delivery.deliveryId = deliveryId++;
    char x[255];
    if (num==0) {
        printf("enter you choices seperated by comma") ;
        scanf("%255s", x);
    }
    // Get coordinates for Client A (the store) using get store Coords function
    getStoreCoordinates(index, &delivery.departureX, &delivery.departureY);


    // Get coordinates from the user for Client B
    printf("Enter Client B coordinates (arrivalX arrivalY): ");
    scanf("%lf %lf", &delivery.arrivalX, &delivery.arrivalY);

    // Get other delivery details
    printf("Enter Customer Name: ");
    scanf("%s", delivery.customerName);


    printf("Enter Receiver Phone Number: ");
    scanf("%s", delivery.receiverPhoneNumber);

    // Get current time
    getCurrentTime(delivery.currentTime);

    // Add the delivery to the queue
    enqueue(queue, delivery);


    /*writeDeliveryToFile(delivery);*/

    printf("Items added to the delivery queue.\n");
}//done


void sushiSpotMenu(queuu* deliveryQueue) {
    int choice;


    displayMenu("sushispot.txt");
    char ch[100];
    printf("\nWhat would you like to do next?\n");
    printf("1. Go back to fast food chains menu\n");
    printf("2. Proceed with delivery\n");
    do {
        printf("Enter your choice: ");
        scanf("%d", &choice);


        switch (choice) {
            case 1:
            break;
            case 2:
                add_delivery(deliveryQueue, 2,0);
            break;
            default:
                printf("Invalid option. Try again.\n");

        }
    }while (choice != 2&&choice != 1);
}//done

void mcdonaldMenu(queuu* deliveryQueue) {
    int choice;
    displayMenu("mcdonalds.txt");

    printf("\nWhat would you like to do next?\n");
    printf("1. Go back to fast food chains menu\n");
    printf("2. Proceed with delivery\n");
    do {
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
            break;
            case 2:
                add_delivery(deliveryQueue, 0,0);
            break;
            default:
                printf("Invalid option. Try again.\n");
        }
    } while (choice != 2&&choice != 1);
}//done

void Pizzahut(queuu* deliveryQueue) {
    int choice;
    displayMenu("pizzahut.txt");

    printf("\nWhat would you like to do next?\n");
    printf("1. Go back to fast food chains menu\n");
    printf("2. Proceed with delivery\n");
    do {
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
            break;
            case 2:
                add_delivery(deliveryQueue, 1,0);
            break;
            default:
                printf("Invalid option. Try again.\n");

        }
    } while (choice != 2 && choice != 1);
}//done

void shawarmaHouseMenu(queuu* deliveryQueue) {
    int choice=0;
    displayMenu("shawarmahouse.txt");

    printf("\nWhat would you like to do next?\n");
    printf("1. Go back to fast food chains menu\n");
    printf("2. Proceed with delivery\n");
    printf("Enter your choice: ");
    do {
        scanf("%d", &choice);

        switch (choice) {
            case 1:
            break;
            case 2:
                add_delivery(deliveryQueue, 3,0);
            break;
            default:
                printf("Invalid option. Try again.\n");
        }
    } while ((choice != 2 )&&(choice != 1));
}//done

void fast_food(queuu* deliveryQueue) {
    int choice = 0;
    int choice2 = 0;

    do {
        system("cls");
        printHeader("Choose a Fast Food Chain");
        printf("1. McDonald's\n");
        printf("2. Pizzahut\n");
        printf("3. Shawarma House\n");
        printf("4. Sushi Spot\n");
        printf("5. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("All products here are boycotted. Are you sure you want to buy them?\n");
                printf("You can buy from Shawarma House or Sushi Spot instead.\n");
                printf("1: Continue\n");
                printf("2: Quit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice2);

                if (choice2 == 1) {
                    mcdonaldMenu(deliveryQueue);
                }
                break;
            case 2:
                printf("All products here are boycotted. Are you sure you want to buy them?\n");
                printf("You can buy from Shawarma House or Sushi Spot instead.\n");
                printf("1: Continue\n");
                printf("2: Quit\n");
                printf("Enter your choice: ");
                scanf("%d", &choice2);

                if (choice2 == 1) {
                    Pizzahut(deliveryQueue);
                }
                break;
            case 3:
                shawarmaHouseMenu(deliveryQueue);
                break;
            case 4:
                sushiSpotMenu(deliveryQueue);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n press enter to retry");
                break;
        }
    } while (choice != 5);
}//done

void shopping(queuu* queue) {
    int choice;
    do {system("cls");
        printHeader("Choose a Category");
        printf("1. Electronics\n");
        printf("2. Snacks\n");
        printf("3. Drinks\n");
        printf("4. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                display_electronics(queue);

            break;
            case 2:
                display_snacks(queue);
            break;
            case 3:
                display_drinks(queue);
            break;
            case 4:
                break;
            default:
                printf("Invalid choice. Try again.\n");
            break;
        }
    } while (choice != 4);
}

void delivery_cli_cli(queuu* basket) {
    static int deliveryId = 1;  // Static variable to keep track of the ID and increment it automatically
    Delivery delivery;

    // Assign the auto-generated ID to the delivery
    delivery.deliveryId = deliveryId++;
    system("cls");

    // Get coordinates from the user for Client A and Client B
    printf("Enter Client A coordinates (departureX departureY): ");
    scanf("%lf %lf", &delivery.departureX, &delivery.departureY);

    printf("Enter Client B coordinates (arrivalX arrivalY): ");
    scanf("%lf %lf", &delivery.arrivalX, &delivery.arrivalY);

    // Get other delivery details
    printf("Enter Customer Name: ");
    scanf("%s", delivery.customerName);

    printf("Enter Receiver Phone Number: ");
    scanf("%s", delivery.receiverPhoneNumber);

    // Get current time
    getCurrentTime(delivery.currentTime);

    // Add the delivery to the queue
    enqueue(basket, delivery);

    printf("Client-to-client delivery added to the basket.\n");
    _sleep(250);
}

void writeDeliveryToFile(FILE *filename, Delivery delivery) {
    calculateDeliveryTime(delivery.departureX, delivery.departureY, delivery.arrivalX, delivery.arrivalY, delivery.deliveryTime);
    fprintf(filename, "%d %s %.2f %.2f %s %.2f %.2f %s %s\n",
            delivery.deliveryId,
            delivery.customerName,
            delivery.departureX,
            delivery.departureY,
            delivery.receiverPhoneNumber,
            delivery.arrivalX,
            delivery.arrivalY,
            delivery.currentTime,
            delivery.deliveryTime);
}
//done

void feedback(char userid[255]) {
    char buffer[255], line[255];
    strcpy(buffer, configpath);
    strcat(buffer, "userdata/");
    strcat(buffer, userid);
    strcat(buffer, ".txt");

    FILE* file = fopen(buffer, "r");
    if (!file) {
        printf("Error opening user data file.\n");
        return;
    }
    printline('=',50);
    printf("               Deliveries for user %s:\n", userid);
    printline('=',50);
    int deliveryId;
    while (fgets(line, sizeof(line), file)) {
        Delivery delivery;
        sscanf(line, "%d %s %lf %lf %s %lf %lf %s %s", &delivery.deliveryId, delivery.customerName,
               &delivery.departureX, &delivery.departureY, delivery.receiverPhoneNumber, &delivery.arrivalX,
               &delivery.arrivalY, delivery.currentTime, delivery.deliveryTime);
        printf("%d: %s to (%.2f, %.2f) at %s\n", delivery.deliveryId, delivery.customerName, delivery.arrivalX, delivery.arrivalY, delivery.deliveryTime);
    }
    fclose(file);
    printline('=',50);
    printf("Enter the ID of the delivery you want to leave feedback for: ");
    scanf("%d", &deliveryId);

    printf("Enter your feedback: ");
    char feedback[255];
    scanf(" %[^\n]s", feedback);

    // Save the feedback to a feedback file
    strcpy(buffer, configpath);
    strcat(buffer, "userdata/feedbacks.txt");

    FILE* feedbackFile = fopen(buffer, "a");
    if (!feedbackFile) {
        printf("Error opening feedback file.\n");
        return;
    }

    fprintf(feedbackFile, "User: %s, Delivery ID: %d, Feedback: %s\n", userid, deliveryId, feedback);
    fclose(feedbackFile);

    printf("Thank you for your feedback!\n press enter");
    printline('=',50);
    getchar();
}
//client UI
void client_interface(char userid[255]) {
    int choice=0;
    while (choice != 4) {
        system("cls");
        char buffer2[255]="Welcome, User ID:";
        strcat(buffer2,userid);
        printHeader(buffer2);
        printHeader("Client Interface");
        printf("|      1. Start Delivery           |\n");
        printf("|      2. Check Current Delivery   |\n");
        printf("|      3. Review Last Delivery     |\n");
        printf("|      4. Logout                   |\n");
        printf("====================================\n");

        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                req_delivery(userid);
            break;
            case 2:
                deliverystate(userid);
            _sleep(2000);
            printf("press enter to continu");
            int c; while ((c = getchar()) != '\n' && c != EOF) { }
            getchar();
            break;
            case 3:
                feedback(userid);

            break;
            case 4:

                printf("Logging out...\n");
            return;
            default:
                printf("Invalid choice.\n press enter to retry");
        }
    }
    /*free(file);
    free(temp);
    fclose(fich);*/
}//to check

void deliverystate(char userid[255]) {
    time_t now ;
    time(&now);
    char buffer[100],line[255];
    strcpy(buffer,configpath);
    strcat(buffer,"userdata/");
    strcat(buffer,userid);
    strcat(buffer,".txt");
    FILE* file = fopen(buffer, "r");
    if (!file) { perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    system("cls");
    printf("File opened successfully.\n");
    _sleep(500);
    system("cls");
    // Get the current time in Unix format
    time_t currentTime = time(NULL);
    Delivery delivery;

    while(fscanf(file, "%d %s %lf %lf %s %lf %lf %s %s", &delivery.deliveryId, delivery.customerName,
               &delivery.departureX, &delivery.departureY, delivery.receiverPhoneNumber, &delivery.arrivalX,
               &delivery.arrivalY, delivery.currentTime, delivery.deliveryTime) == 9) {


        double percentageElapsed = calculateElapsedPercentage(delivery, currentTime);
        printf("%d %s %.2f %.2f %s %.2f %.2f %s %s - Completion: %.2f%%\n",delivery.deliveryId,
            delivery.customerName,
            delivery.departureX,
            delivery.departureY,
            delivery.receiverPhoneNumber,
            delivery.arrivalX,
            delivery.arrivalY,
            delivery.currentTime,
            delivery.deliveryTime,
            percentageElapsed);
               }

    fclose(file);
}

void commit(char * userid, queuu * queuu) {
    char delivsfile[255],userdelivs[255];

    strcpy(delivsfile, configpath);
    strcat(delivsfile,"delivs.txt");

    strcpy(userdelivs,configpath);
    strcat(userdelivs,"userdata/");
    strcat(userdelivs,userid);
    strcat(userdelivs,".txt");

    FILE *user_data=fopen(userdelivs, "a");
    FILE *delivslist = fopen(delivsfile, "a");
    Delivery temp;
    while(queuu->front != NULL) {
        temp=dequeue(queuu);
        writeDeliveryToFile(delivslist, temp);
        writeDeliveryToFile(user_data, temp);

    }
    fclose(user_data);
    fclose(delivslist);
}

void propose_replacements(const char* mainFile, const char* replacementFile, const char* productId) {
    char buffer[255], buffer2[255];

    snprintf(buffer, sizeof(buffer), "%sstoresdata/%s", configpath, mainFile);
    snprintf(buffer2, sizeof(buffer2), "%sstoresdata/%s", configpath, replacementFile);

    FILE* main = fopen(buffer, "r");
    FILE* replacement = fopen(buffer2, "r");

    if (!main || !replacement) {
        printf("Error opening files.\n");
        if (main) fclose(main);
        if (replacement) fclose(replacement);
        return;
    }
    char mainLine[255], replLine[255];
    char proposedProducts[1024] = "";
    int found = 0;
    while (fgets(mainLine, sizeof(mainLine), main)) {
        char lineCopy[255];
        strncpy(lineCopy, mainLine, sizeof(lineCopy));
        lineCopy[sizeof(lineCopy) - 1] = '\0';
        char *mainProductId = strtok(lineCopy, ",");
        if (strcmp(mainProductId, productId) == 0) {
            found = 1;
            char *productName = strtok(NULL, ",");
            char *productType = strtok(NULL, ",");
            printf("*************************************************************\n");
            printf("Main Product: %s %s %s", mainProductId, productName, productType);
            // Propose replacements if the product is boycutted
            fseek(replacement, 0, SEEK_SET); // Move to the start of the replacement file
            while (fgets(replLine, sizeof(replLine), replacement)) {
                char replLineCopy[255];
                strncpy(replLineCopy, replLine, sizeof(replLineCopy));
                replLineCopy[sizeof(replLineCopy) - 1] = '\0';
                char *replProductId = strtok(replLineCopy, ",");
                char *replProductName = strtok(NULL, ",");
                char *replProductType = strtok(NULL, ",");
                if (!strcmp(replProductType, productType)) {
                    strcat(proposedProducts, replLine);
                }
            }
            break;
        }
    }
    fclose(main);
    fclose(replacement);
    if (strlen(proposedProducts) > 0) {
        printf("*************************************************************\n");
        printf("Here are some replacement options for your chosen products:\n%s", proposedProducts);
        printf("*************************************************************\n");
    } else if (found) { printf("No replacement options found for your chosen product.\n"); } else {
        printf("Product not boycutted .\n");
    }
}

void display_electronics(queuu* queue) {
    int choice = 1;
    int checkedin = 0;
    char product[255];
    char buffer[150];
    strcpy(buffer, configpath);
    strcat(buffer, "storesdata/elect.txt");

    do {system("cls");
        printf("Electronics Products:\n");
        displayMenu("elect.txt");
        displayMenu("replaceelec.txt");
        printf("\n");
        printline('=',36);
        printf("What would you like to do next?\n");
        printf("1) Add product\n");
        printf("2) Checkout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter id product: ");
                scanf("%s", product);
                int found = 0;

                // Check if product is in the main list
                FILE* main = fopen(buffer, "r");
                if (!main) {
                    printf("Error opening main file.\n");
                    break;
                }
                char mainLine[255];
            while (fgets(mainLine, sizeof(mainLine), main)) {
                    char lineCopy[255];
                    strncpy(lineCopy, mainLine, sizeof(lineCopy));
                    lineCopy[sizeof(lineCopy) - 1] = '\0';
                    char *mainProductId = strtok(lineCopy, ",");
                    if (strcmp(mainProductId, product) == 0) {
                        found = 1;
                        system("cls");
                        printline('*',36);
                        printf("Product is boycutted.\n");
                        printline('*',36);
                        break;
                    }
                }
                fclose(main);
                if (found) {
                    propose_replacements("elect.txt", "replaceelec.txt", product);
                    printf("Do you want to change the chosen product to a replacement? (y/n): ");
                    char response;
                    scanf(" %c", &response);
                    if (response == 'y' || response == 'Y') {
                        printf("Enter id of the replacement product: ");
                        scanf("%s", product);
                    }
                } //removed else
                checkedin = 1;
                break;
            case 2: if (checkedin != 0) {
                    add_delivery(queue, 1, 1);
            } break;
            default: printf("Invalid choice.\n press enter to retry");getchar();
            break;
        }
    } while (choice ==1);  }
void display_snacks(queuu* queue){
    int choice = 1;
    int checkedin = 0;
    char product[255];
    char buffer[150];
    strcpy(buffer, configpath);
    strcat(buffer, "storesdata/snacks.txt");
    do {system("cls");
        printf("snacks Products:\n");
        displayMenu("snacks.txt");
        displayMenu("snacksrep.txt");
        printf("\n");
        printline('=',36);
        printf("What would you like to do next?\n");
        printf("1) Add product\n");
        printf("2) Checkout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter id product: ");
                scanf("%s", product);
                int found = 0;

                // Check if product is in the main list
                FILE* main = fopen(buffer, "r");
                if (!main) {
                    printf("Error opening main file.\n");
                    break;
                }
                char mainLine[255];
            while (fgets(mainLine, sizeof(mainLine), main)) {
                    char lineCopy[255];
                    strncpy(lineCopy, mainLine, sizeof(lineCopy));
                    lineCopy[sizeof(lineCopy) - 1] = '\0';
                    char *mainProductId = strtok(lineCopy, ",");
                    if (strcmp(mainProductId, product) == 0) {
                        found = 1;
                        printf("Product is available in the main list.\n");
                        break;
                    }
                }
                fclose(main);
                if (found) {
                    system("cls");
                    propose_replacements("snacks.txt", "snacksrep.txt", product);
                    printf("Do you want to change the chosen product to a replacement? (y/n): ");
                    char response;
                    scanf(" %c", &response);
                    if (response == 'y' || response == 'Y') {
                        printf("Enter id of the replacement product: ");
                        scanf("%s", product);
                    }
                }
                checkedin = 1;
                break;
            case 2: if (checkedin != 0) {
                    add_delivery(queue, 1, 1);
            } break;
            default: printf("Invalid choice.\n press enter to retry");getchar();
            break;
        }
    } while (choice ==1);  }

void display_drinks(queuu* queue){
    int choice = 1;
    int checkedin = 0;
    char product[255];
    char buffer[150];
    strcpy(buffer, configpath);
    strcat(buffer, "storesdata/drinksnck.txt");

    do {//system("cls");
        printHeader("drinks Products:");
        displayMenu("drink.txt");
        displayMenu("drinkrep.txt");
        printf("\n");
        printline('=',36);
        printf("What would you like to do next?\n");
        printf("1) Add product\n");
        printf("2) Checkout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter id product: ");
                scanf("%s", product);
                int found = 0;

                // Check if product is in the main list
                FILE* main = fopen(buffer, "r");
                if (!main) {
                    printf("Error opening main file.\n");
                    break;
                }
                char mainLine[255];
            while (fgets(mainLine, sizeof(mainLine), main)) {
                    char lineCopy[255];
                    strncpy(lineCopy, mainLine, sizeof(lineCopy));
                    lineCopy[sizeof(lineCopy) - 1] = '\0';
                    char *mainProductId = strtok(lineCopy, ",");
                    if (strcmp(mainProductId, product) == 0) {
                        found = 1;
                        printf("Product is available in the main list.\n");
                        break;
                    }
                }
                fclose(main);
                if (found) {
                    //system("cls");
                    propose_replacements("drink.txt", "drinkrep.txt", product);
                    printf("Do you want to change the chosen product to a replacement? (y/n): ");
                    char response;
                    scanf(" %c", &response);
                    if (response == 'y' || response == 'Y') {
                        printf("Enter id of the replacement product: ");
                        scanf("%s", product);
                    }
                }
                checkedin = 1;
                break;
            case 2: if (checkedin != 0) {
                    add_delivery(queue, 1, 1);
                } break;
            default: printf("Invalid choice.\n press enter to retry");getchar();
                break;
        }
    } while (choice ==1); }

void req_delivery(char userid[255]) {
    int choice=0;
    queuu *basket = createQueue();
    while (choice != 4) {
        system("cls");
    printHeader("Your Request?");
    printf("|1. Shopping                       |\n");
    printf("|2. Fast Food                      |\n");
    printf("|3. Delivery from Client to Client |\n");
    printf("|4. Back to Client Interface       |\n");
    printline('=',36);

    printf("Choose an option: ");
    scanf("%d", &choice);

        switch (choice) {
            case 1:
                shopping(basket);
            break;
            case 2:
                fast_food(basket);
            break;
            case 3:
                delivery_cli_cli(basket);
            break;
            case 4:printf("commiting delivery requests...\n");
            commit(userid,basket);
            break;
            default:
                printf("Invalid choice.\n press enter to retry ");getchar();
        }
    }
}//done
static int currentId;
void signup_menu() {
    char userId[100];
    char inputUsername[100];
    char inputPassword[100];
    char buffer[255];
    strcpy(buffer, configpath);
    strcat(buffer, "logindata/users.txt");
    currentId ++;
    system("cls");
    // Convert currentId to string and assign it to userId
    snprintf(userId, sizeof(userId), "%d", currentId);
    printHeader("signup menu");
    printf("Enter username: ");
    scanf("%s", inputUsername);
    printf("Enter password: ");
    scanf("%s", inputPassword);

    // Open the file to append new user information
    FILE *userFile = fopen(buffer, "a");
    if (userFile == NULL) {
        perror("Error opening user file");
        return;
    }

    // Write user information to the file
    fprintf(userFile, "%s %s %s\n", userId, inputUsername, inputPassword);
    fclose(userFile);

    printf("\n\n\nsigned up with ID: %s\n press enter to continu", userId);
    int c; while ((c = getchar()) != '\n' && c != EOF) { }
    getchar();
}

char* login() {
    static char userId[100]="-1";
    char buffer[255];
    strcpy(buffer, configpath);
    strcat(buffer,"logindata/users.txt");
    FILE *file = fopen(buffer, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return "-1";
    }

    char inputUsername[100], inputPassword[100];
    char username[100], password[100];
    system("cls");
    printline('=',33);
    printf("|          login menu           |\n");
    printline('=',33);
    printf("Entrez le nom d'utilisateur : ");
    scanf("%s", inputUsername);

    printf("Entrez le mot de passe : ");
    scanf("%s", inputPassword);

    while (fscanf(file, "%s %s %s", userId, username, password) == 3) {
        if (strcmp(inputUsername, username) == 0 && strcmp(inputPassword, password) == 0) {
            fclose(file);
            return userId; // Return the userId if credentials match
        }
    }

    fclose(file);
    return "-1";

}

void main_menu() {
    char filnam[100];
    strcpy(filnam,configpath);
    strcat(filnam,"temp file.txt");// partie mte3 config
    FILE *file = fopen(filnam,"r");
    fscanf(file, "%lld %d %d", &previousArrivalTime,&deliveryId,&currentId);
    fclose(file);
    int choice;
    char*loggedinid;
    do {
        //printf("%lld %d %d\n",previousArrivalTime,deliveryId,currentId);
        system("cls");
        printline('=',33);
        printf("|Welcome to the Delivery System!|\n");
        printline('=',33);
        printf("1. login\n");
        printf("2. signup\n");
        printf("3. Exit\n");
        printline('=',33);
        printf("Enter your choice: ");
        scanf("%d", &choice);



        switch (choice) {
            case 1:
                loggedinid = login();
                if (strcmp(loggedinid, "-1") != 0) {

                    client_interface(loggedinid);
            } else { printf("Invalid username or password.\n press enter to continu");getchar(); }
            break;
            case 2:
                signup_menu();
            break;
            case 3:
                printf("Exiting...\n");
            break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 3);
    FILE*f=fopen(filnam, "w");
    fprintf(f,"%lld %d %d",previousArrivalTime,deliveryId,currentId);
    fclose(f);
}

int main(){
    main_menu();

    return 0;
}



































