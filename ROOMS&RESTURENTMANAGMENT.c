#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>


#define MAX_TABLES 20
#define MAX_NAME_LENGTH 50
#define MAX_MENU_ITEMS 50
#define MAX_ITEMS 10
#define MAX_ORDERS 100
#define MAX_ROOMS 20
#define USER_DATA_FILE "user_data.txt"
#define ROOM_DATA_FILE "room_data.txt"
#define FEEDBACK_FILE "feedback.txt"


typedef struct {
    int roomNumber;
    char name[MAX_NAME_LENGTH];
    char checkInDate[11];
    int bookingID;
    int numberOfDays;
    int isBooked;
    int numberOfAdults;
    int numberOfChildren;
    int isACRoom;
    float roomRate;
} RoomBooking;

typedef struct {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
} UserData;

typedef struct {
    int id;
    char name[50];
    float price;
    char category[20];
} MenuItem;

typedef struct {
    int menuID;
    int quantity;
} OrderedItem;

typedef struct {
    int orderID;
    int tableNo;
    char name[MAX_NAME_LENGTH];
    char date[11];
    int slot;
    float totalBill;
    OrderedItem items[MAX_ITEMS];
    int itemCount;
} Order;

MenuItem menu[MAX_MENU_ITEMS];
Order orders[MAX_ORDERS];
int menuSize = 0;
int orderCount = 0;
int opt;
int currentOrderID = 1000;
RoomBooking rooms[MAX_ROOMS];
UserData users[100];
int userCount = 0;


void initializeRooms();
int generateBookingID();
void displayHelpline();
void feedback();
void signUp();
int login();
void mainMenu();
void loadUserData();
void saveUserData();
void bookRoom();
void modifyRoom();
void cancelRoom();
void loadRoomData();
void saveRoomData();
int isValidDate(const char* date);
void showBill(RoomBooking room);
void initializeMenu();
void saveOrdersToFile();
void loadOrdersFromFile();
void displayMenu(const char* category);
void bookTable();
void modifyOrder();
void cancelOrder();
void viewBookings();
void printBill(Order order);
int validateTableNumber(int tableNo);
int validateMenuID(int menuID, int menuSize);
int generateOrderID();
float calculatePrice(int category, int menuID, int quantity);
void addItemToOrder(Order* order, int menuID, int category, int quantity, float price);
void removeItemFromOrder(Order* order, int removeID);
float calculateTotalBill(Order* order);
void restarentManagement();

int validateMenuID(int menuID, int menuSize) {
    if (menuID < 1 || menuID > menuSize) {
        return 0;
    }
    return 1;
}

void initializeMenu() {
    menuSize = 0;

    //Starters
    menu[menuSize++] = (MenuItem){1, "Samosa", 11.50, "Starters"};
    menu[menuSize++] = (MenuItem){2, "Paneer Tikka", 18.00, "Starters"};
    menu[menuSize++] = (MenuItem){3, "Veg Cutlet", 20.00, "Starters"};
    menu[menuSize++] = (MenuItem){4, "Pav Bhaji", 65.00, "Starters"};
    menu[menuSize++] = (MenuItem){5, "Mushroom 65", 165.00, "Starters"};
    menu[menuSize++] = (MenuItem){6, "Gobi Manchuria", 95.00, "Starters"};
    menu[menuSize++] = (MenuItem){7, "Egg Manchuria", 65.00, "Starters"};
    menu[menuSize++] = (MenuItem){8, "Chicken Tandoori", 265.00, "Starters"};
    menu[menuSize++] = (MenuItem){9, "Sheer Kebab", 265.00, "Starters"};
    menu[menuSize++] = (MenuItem){10, "VEG starters combo", 465.00, "Starters"};
    menu[menuSize++] = (MenuItem){11, "Non Veg starters combo", 565.00, "Starters"};
    menu[menuSize++] = (MenuItem){12, "S.P.S  special starters platter", 865.00, "Starters"};

    // Main Course
    menu[menuSize++] = (MenuItem){13, "Paneer Butter Masala", 200.00, "Main Course"};
    menu[menuSize++] = (MenuItem){14, "Kaju Butter Masala", 220.00, "Main Course"};
    menu[menuSize++] = (MenuItem){15, "Mushroom Masala", 260.00, "Main Course"};
    menu[menuSize++] = (MenuItem){16, "Dal makhani", 200.00, "Main Course"};
    menu[menuSize++] = (MenuItem){17, "Butter Chicken", 200.00, "Main Course"};
    menu[menuSize++] = (MenuItem){18, "Biryani(veg)", 180.00, "Main Course"};
    menu[menuSize++] = (MenuItem){19, "Hyderabad dum Biryani", 200.00, "Main Course"};
    menu[menuSize++] = (MenuItem){20, "S.P.S  special Biryani", 400.00, "Main Course"};
    menu[menuSize++] = (MenuItem){21, "Nalli ka Gosh Biryani", 500.00, "Main Course"};
    menu[menuSize++] = (MenuItem){22, "Mutton Biryani", 300.00, "Main Course"};

    // Desserts
    menu[menuSize++] = (MenuItem){23, "Gulab Jamun", 30.00, "Desserts"};
    menu[menuSize++] = (MenuItem){24, "Rasgulla", 25.50, "Desserts"};
    menu[menuSize++] = (MenuItem){25, "Ras Malai", 35.50, "Desserts"};
    menu[menuSize++] = (MenuItem){26, "Double ka Meeta", 65.50, "Desserts"};
    menu[menuSize++] = (MenuItem){27, "Matka Kulfi", 45.50, "Desserts"};
    menu[menuSize++] = (MenuItem){28, "S.P.S  SP Kheer", 75.50, "Desserts"};

    // Drinks
    menu[menuSize++] = (MenuItem){29, "Badam Kheer", 75.50, "Drinks"};
    menu[menuSize++] = (MenuItem){30, "Rose Milk", 65.50, "Drinks"};
    menu[menuSize++] = (MenuItem){31, "Sweet Lassi", 55.50, "Drinks"};
    menu[menuSize++] = (MenuItem){32, "Lemonade", 45.50, "Drinks"};
    menu[menuSize++] = (MenuItem){33, "Body Cooler", 55.50, "Drinks"};
    menu[menuSize++] = (MenuItem){34, "Tender Coconut Water", 55.50, "Drinks"};
    menu[menuSize++] = (MenuItem){35, "Fresh Sugarcane Juice", 55.50, "Drinks"};
}

void loadUserData() {
    FILE *file = fopen(USER_DATA_FILE, "r");
    if (file) {
        while (fscanf(file, "%s %s", users[userCount].username, users[userCount].password) != EOF) {
            userCount++;
        }
        fclose(file);
    }
}

void saveUserData() {
    FILE *file = fopen(USER_DATA_FILE, "w");
    if (file) {
        for (int i = 0; i < userCount; i++) {
            fprintf(file, "%s %s\n", users[i].username, users[i].password);
        }
        fclose(file);
    }
}

void signUp() {
    if (userCount >= 100) {
        printf("User limit reached. Cannot sign up.\n");
        return;
    }

    printf("Enter username: ");
    fgets(users[userCount].username, sizeof(users[userCount].username), stdin);
    users[userCount].username[strcspn(users[userCount].username, "\n")] = 0;

    printf("Enter password: ");
    fgets(users[userCount].password, sizeof(users[userCount].password), stdin);
    users[userCount].password[strcspn(users[userCount].password, "\n")] = 0;

    userCount++;
}

int login() {
    char username[MAX_NAME_LENGTH], password[MAX_NAME_LENGTH];
    printf("Enter username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Enter password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return 1; // Login successful
        }
    }
    return 0; // Login failed
}

void saveOrdersToFile() {
    FILE *file = fopen("orders.dat", "wb");
    if (!file) {
        printf("Error saving orders.\n");
        return;
    }
    fwrite(&orderCount, sizeof(int), 1, file);
    fwrite(orders, sizeof(Order), orderCount, file);
    fclose(file);
}

void loadOrdersFromFile() {
    FILE *file = fopen("orders.dat", "rb");
    if (!file) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }
    fread(&orderCount, sizeof(int), 1, file);
    fread(orders, sizeof(Order), orderCount, file);
    fclose(file);
}

void displayMenu(const char* category) {
    printf("\n=== %s ===\n", category);
    for (int i = 0; i < menuSize; i++) {
        if (strcmp(menu[i].category, category) == 0) {
            printf("ID: %d, Name: %s, Price: %.2f\n", menu[i].id, menu[i].name, menu[i].price);
        }
    }
}

void bookTable() {
    char name[50], date[11];
    int slot, tableNumber, category, menuID, quantity;
    float totalBill = 0.0;
    Order order;

    printf("Enter your name: ");
    scanf(" %[^\n]", name);
    printf("Enter date (YYYY-MM-DD): ");
    scanf(" %s", date);

    printf("Available time slots:\n");
    printf("1. 6-7\n2. 7-8\n3. 8-9\n4. 9-10\n");
    printf("Select a slot: ");
    scanf("%d", &slot);

    printf("Enter Table Number (1-20): ");
    scanf("%d", &tableNumber);

    // Check for double booking
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].tableNo == tableNumber && orders[i].slot == slot && strcmp(orders[i].date, date) == 0) {
            printf("Error: Table %d is already booked for %s at the selected time slot.\n", tableNumber, date);
            printf("Press B/b to View Contact Details : ");
            fflush(stdin);
            opt=getch();
            if(opt=='B'||opt=='b'){
            return;
            } // Exit function
        }
    }

    // Proceed with booking if no conflict
    order.orderID = generateOrderID();
    order.tableNo = tableNumber;
    strcpy(order.name, name);
    strcpy(order.date, date);
    order.slot = slot;
    order.itemCount = 0;
    order.totalBill = 0.0;

    do {
        printf("\nSelect a category: 1. Starters, 2. Main Course, 3. Desserts, 4. Drinks: ");
        scanf("%d", &category);

        displayMenu(category == 1 ? "Starters" : category == 2 ? "Main Course" : category == 3 ? "Desserts" : "Drinks");

        printf("Enter Menu ID: ");
        scanf("%d", &menuID);
        printf("Enter quantity: ");
        scanf("%d", &quantity);

        float price = calculatePrice(category, menuID, quantity);
        addItemToOrder(&order, menuID, category, quantity, price);

        printf("Add more items? (1 for Yes, 0 for No): ");
        scanf("%d", &category);
    } while (category == 1);

    // Save booking details
    orders[orderCount++] = order;
    printf("\nTable booked successfully!\n");
    printBill(order);
    printf("Press B/b to View Contact Details : ");
    fflush(stdin);
    opt=getch();
    if(opt=='Y'||opt=='y'){
    return;
    }
}

void modifyOrder() {
    int orderID, found = 0, choice;
    char newDate[11];
    int newSlot, newTable, category, menuID, quantity, removeID;

    printf("Enter Order ID to modify: ");
    scanf("%d", &orderID);

    for (int i = 0; i < orderCount; i++) {
        if (orders[i].orderID == orderID) {
            found = 1;
            Order* order = &orders[i];

            // Display current booking details
            printf("\nCurrent Booking Details:\n");
            printf("Order ID: %d, Name: %s, Table: %d, Date: %s, Time Slot: %d\n", order->orderID, order->name, order->tableNo, order->date, order->slot);
            printf("Items Ordered:\n");
            for (int j = 0; j < order->itemCount; j++) {
                printf("  ID: %d, Name: %s, Quantity: %d, Price: %.2f\n", menu[order->items[j].menuID - 1].id, menu[order->items[j].menuID - 1].name, order->items[j].quantity, menu[order->items[j].menuID - 1].price);
            }
            printf("Total Bill: %.2f\n", order->totalBill);

            printf("\nWhat would you like to modify?\n");
            printf("1. Change Date, Time, or Table\n");
            printf("2. Add or Remove Items\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            if (choice == 1) {
                printf("Enter new date (YYYY-MM-DD): ");
                scanf(" %s", newDate);
                printf("Available time slots:\n");
                printf("1. 6-7\n2. 7-8\n3. 8-9\n4. 9-10\n");
                printf("Select a new slot: ");
                scanf("%d", &newSlot);
                printf("Enter new Table Number (1-20): ");
                scanf("%d", &newTable);

                // Check for double booking
                for (int j = 0; j < orderCount; j++) {
                    if (orders[j].tableNo == newTable && orders[j].slot == newSlot && strcmp(orders[j].date, newDate) == 0) {
                        printf("Error: Table %d is already booked for %s at the selected time slot.\n", newTable, newDate);
                        printf("Press B/b to View Contact Details : ");
                        fflush(stdin);
                        opt=getch();
                        if(opt=='B'||opt=='b'){
                        return;
                        }// Exit function
                    }
                }

                strcpy(order->date, newDate);
                order->slot = newSlot;
                order->tableNo = newTable;
                printf("Booking modified successfully!\n");
            }else if (choice == 2) {
    int subChoice;
    do {
        printf("\n--- Modify Order: Add or Remove Items ---\n");
        printf("1. Add Items\n");
        printf("2. Remove Items\n");
        printf("3. Finish Modifications\n");
        printf("Enter your choice: ");
        scanf("%d", &subChoice);

        switch (subChoice) {
            case 1: {
                // Add Items
                int category, menuID, quantity;
                char addMore;

                do {
                    // Select Category
                    printf("\nSelect a category to view items:\n");
                    printf("1. Starters\n2. Main Course\n3. Desserts\n4. Drinks\n");
                    printf("Enter your choice: ");
                    scanf("%d", &category);

                    // Display items in the selected category
                    if (category < 1 || category > 4) {
                        printf("Invalid category choice. Please try again.\n");
                        continue;
                    }
                    displayMenu(category == 1 ? "Starters" : category == 2 ? "Main Course" :
                                category == 3 ? "Desserts" : "Drinks");

                    // Select Item
                    printf("Enter Menu ID to add: ");
                    scanf("%d", &menuID);

                    // Validate Menu ID
                    if (!validateMenuID(menuID, menuSize)) {
                        printf("Invalid Menu ID. Please try again.\n");
                        continue;
                    }

                    // Check if item already exists
                    int alreadyExists = 0;
                    for (int j = 0; j < order->itemCount; j++) {
                        if (order->items[j].menuID == menuID) {
                            alreadyExists = 1;
                            break;
                        }
                    }
                    if (alreadyExists) {
                        printf("Item already exists in the order. Consider modifying the quantity instead.\n");
                        continue;
                    }

                    // Enter Quantity
                    printf("Enter quantity: ");
                    scanf("%d", &quantity);
                    if (quantity <= 0) {
                        printf("Quantity must be greater than 0. Please try again.\n");
                        continue;
                    }

                    // Add Item
                    float price = calculatePrice(category, menuID, quantity);
                    addItemToOrder(order, menuID, category, quantity, price);
                    printf("Item added successfully! Current Total Bill: %.2f\n", order->totalBill);

                    // Ask if the user wants to add another item
                    printf("Add another item? (y/n): ");
                    scanf(" %c", &addMore);
                } while (addMore == 'y' || addMore == 'Y');
                break;
            }
            case 2: {
                // Remove Items
                int removeID;
                char removeMore;

                do {
                    // Display Current Items
                    printf("\n--- Current Items in Order ---\n");
                    for (int j = 0; j < order->itemCount; j++) {
                        printf("Menu ID: %d, Name: %s, Quantity: %d, Price: %.2f\n",
                               menu[order->items[j].menuID - 1].id,
                               menu[order->items[j].menuID - 1].name,
                               order->items[j].quantity,
                               menu[order->items[j].menuID - 1].price);
                    }
                    printf("Total Bill: %.2f\n", order->totalBill);

                    // Prompt to Remove Item
                    printf("\nEnter Menu ID of the item to remove: ");
                    scanf("%d", &removeID);

                    // Validate Item ID
                    int found = 0;
                    for (int j = 0; j < order->itemCount; j++) {
                        if (order->items[j].menuID == removeID) {
                            found = 1;
                            break;
                        }
                    }

                    if (!found) {
                        printf("Invalid Menu ID or Item not found in the order. Please try again.\n");
                        continue;
                    }

                    // Remove Item
                    removeItemFromOrder(order, removeID);
                    printf("Item removed successfully! Updated Total Bill: %.2f\n", order->totalBill);

                    // Prompt to Remove Another Item
                    printf("Remove another item? (y/n): ");
                    scanf(" %c", &removeMore);

                } while (removeMore == 'y' || removeMore == 'Y');
                break;
            }
            case 3:
                // Finish Modifications
                printf("Finished modifying the order. Updated Total Bill: %.2f\n", order->totalBill);
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (subChoice != 3);
}

        }
    }
    if (!found) {
        printf("Order not found.\n");
    }
}

void cancelOrder() {
    int orderID, found = 0;

    printf("Enter Order ID to cancel: ");
    scanf("%d", &orderID);

    for (int i = 0; i < orderCount; i++) {
        if (orders[i].orderID == orderID) {
            found = 1;
            for (int j = i; j < orderCount - 1; j++) {
                orders[j] = orders[j + 1];
            }
            orderCount--;
            printf("Order %d canceled successfully!\n", orderID);
            printf("Press B/b to View Contact Details : ");
            fflush(stdin);
            opt=getch();
            if(opt=='B'||opt=='b'){
            break;
            }
        }
    }
    if (!found) {
        printf("Order not found.\n");
        printf("Press B/b to View Contact Details : ");
        fflush(stdin);
        opt=getch();
        if(opt=='B'||opt=='b'){
       return;
        }
    }
}

void viewBookings() {
    printf("\n--- View Bookings ---\n");
    for (int i = 0; i < orderCount; i++) {
        printf("Order ID: %d, Name: %s, Table: %d, Date: %s, Slot: %d, Total Bill: %.2f\n", orders[i].orderID, orders[i].name, orders[i].tableNo, orders[i].date, orders[i].slot, orders[i].totalBill);
    }
}

void printBill(Order order) {
    printf("\n--- Bill ---\n");
    printf("Order ID: %d\n", order.orderID);
    printf("Name: %s\n", order.name);
    printf("Table: %d\n", order.tableNo);
    printf("Date: %s\n", order.date);
    printf("Slot: %d\n", order.slot);
    printf("Items Ordered:\n");
    for (int i = 0; i < order.itemCount; i++) {
        printf("  Name: %s, Quantity: %d, Price: %.2f\n", menu[order.items[i].menuID - 1].name, order.items[i].quantity, menu[order.items[i].menuID - 1].price);
    }
    printf("Total Bill: %.2f\n", order.totalBill);
}

int generateOrderID() {
    return currentOrderID++;
}

void addItemToOrder(Order* order, int menuID, int category, int quantity, float price) {
    if (order->itemCount < MAX_ITEMS) {
        order->items[order->itemCount].menuID = menuID;
        order->items[order->itemCount].quantity = quantity;
        order->totalBill += price;
        order->itemCount++;
    }
}

void removeItemFromOrder(Order* order, int removeID) {
    for (int i = 0; i < order->itemCount; i++) {
        if (order->items[i].menuID == removeID) {
            // Subtract the price of the removed item from the total bill
            float price = menu[order->items[i].menuID - 1].price; // Use menuID to get the price
            order->totalBill -= price * order->items[i].quantity; // Subtract the total cost of the item
            // Shift items after the removed one
            for (int j = i; j < order->itemCount - 1; j++) {
                order->items[j] = order->items[j + 1];
            }
            order->itemCount--; // Reduce the item count
            printf("Item removed successfully!\n");
            return;
        }
    }
    printf("Item not found in order.\n");
}

float calculatePrice(int category, int menuID, int quantity) {
    return menu[menuID - 1].price * quantity;
}

void restarentManagement() {
    int choice;
    loadOrdersFromFile();
    initializeMenu();

    do {
        printf("\nRestaurant Management System\n");
        printf("1. Book Table\n");
        printf("2. Modify Order\n");
        printf("3. Cancel Order\n");
        printf("4. View Bookings\n");
        printf("5. Exit\n");
        printf("Select an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: bookTable(); break;
            case 2: modifyOrder(); break;
            case 3: cancelOrder(); break;
            case 4: viewBookings(); break;
            case 5: printf("Exiting...\n"); saveOrdersToFile(); return;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);
}

void initializeRooms() {
    for (int i = 0; i < MAX_ROOMS; i++) {
        rooms[i].roomNumber = i + 1;
        rooms[i].isBooked = 0;
        rooms[i].roomRate = (i < 10) ? 1800 : 1200;  // Rooms 1-10 Non-AC, 11-20 AC
    }
    loadRoomData();
}

int generateBookingID() {
    return rand() % 9000 + 1000;
}

void displayHelpline() {

    printf("Helpline Numbers:\n");
    printf("Manager: 1234567890\n");
    printf("Assistant Manager: 0987654321\n");
    printf("Press any key to go back: ");
    getchar();
}

void feedback() {
    int rating;
    char feedbackText[256];


    printf("Rate us from 1 to 5: ");
    scanf("%d", &rating);
    getchar();

    printf("Write your feedback: ");
    fgets(feedbackText, sizeof(feedbackText), stdin);
    feedbackText[strcspn(feedbackText, "\n")] = 0;

    FILE *file = fopen(FEEDBACK_FILE, "a");
    if (file) {
        fprintf(file, "Rating: %d\nFeedback: %s\n---\n", rating, feedbackText);
        fclose(file);
        printf("Thank you for your feedback!\n");
    } else {
        printf("Error saving feedback. Please try again later.\n");
    }
    printf("Press any key to go back: ");
    getchar();
}

void roomManagement() {
    int choice;
    do {

        printf("1. Book a room\n");
        printf("2. Modify a room\n");
        printf("3. Cancel a room\n");
        printf("4. Feedback\n");
        printf("5. Help\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Consume newline character

        switch (choice) {
            case 1: bookRoom(); break;
            case 2: modifyRoom(); break;
            case 3: cancelRoom(); break;
            case 4: feedback(); break;
            case 5: displayHelpline(); break;
            case 6: printf("Goodbye! Exiting...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while (choice != 6);
}

void loadRoomData() {
    FILE *file = fopen(ROOM_DATA_FILE, "r");
    if (file) {
        int i = 0;
        while (fscanf(file, "%d %s %s %d %d %d %d %d %f\n", &rooms[i].roomNumber, rooms[i].name, rooms[i].checkInDate,
                      &rooms[i].bookingID, &rooms[i].numberOfDays, &rooms[i].isBooked,
                      &rooms[i].numberOfAdults, &rooms[i].numberOfChildren, &rooms[i].isACRoom, &rooms[i].roomRate) != EOF) {
            i++;
        }
        fclose(file);
    }
}

void saveRoomData() {
    FILE *file = fopen(ROOM_DATA_FILE, "w");
    if (file) {
        for (int i = 0; i < MAX_ROOMS; i++) {
            if (rooms[i].isBooked) {
                fprintf(file, "%d %s %s %d %d %d %d %d %.2f\n", rooms[i].roomNumber, rooms[i].name, rooms[i].checkInDate,
                        rooms[i].bookingID, rooms[i].numberOfDays, rooms[i].isBooked, rooms[i].numberOfAdults, rooms[i].numberOfChildren, rooms[i].isACRoom, rooms[i].roomRate);
            }
        }
        fclose(file);
    }
}

int isValidDate(const char* date) {
    int day, month, year;
    if (sscanf(date, "%2d/%2d/%4d", &day, &month, &year) != 3) {
        return 0;
    }
    if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 31) {
        return 0;
    }
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return 0;
    if (month == 2) {
        int leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (day > 28 + leap) return 0;
    }
    return 1;
}

void showBill(RoomBooking room) {

    printf("Bill Details:\n");
    printf("Booking ID: %d\n", room.bookingID);
    printf("Room Number: %d\n", room.roomNumber);
    printf("Number of Days: %d\n", room.numberOfDays);
    printf("Room Rate: %.2f\n", room.roomRate);
    printf("Total Bill: %.2f\n", room.numberOfDays * room.roomRate);
    printf("Press any key to go back: ");
    getchar();
}

void bookRoom() {
    int roomNumber, numberOfDays, numberOfAdults, numberOfChildren, isACRoom;
    char name[MAX_NAME_LENGTH], checkInDate[11];


    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter check-in date (DD/MM/YYYY): ");
    fgets(checkInDate, sizeof(checkInDate), stdin);
    checkInDate[strcspn(checkInDate, "\n")] = 0;

    if (!isValidDate(checkInDate)) {
        printf("Invalid date format.\n");
        return;
    }

    printf("Enter number of adults (0-2): ");
    scanf("%d", &numberOfAdults);
    printf("Enter number of children (0-2): ");
    scanf("%d", &numberOfChildren);
    printf("Enter number of days (0-30): ");
    scanf("%d", &numberOfDays);

    if (numberOfAdults < 0 || numberOfAdults > 2 || numberOfChildren < 0 || numberOfChildren > 2 || numberOfDays < 0 || numberOfDays > 30) {
        printf("Invalid input for persons or days.\n");
        return;
    }

    for (int i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i].isBooked == 0) {
            roomNumber = rooms[i].roomNumber;
            isACRoom = (roomNumber > 10);  // AC rooms are 11-20
            rooms[i].isBooked = 1;
            rooms[i].bookingID = generateBookingID();
            strcpy(rooms[i].name, name);
            strcpy(rooms[i].checkInDate, checkInDate);
            rooms[i].numberOfDays = numberOfDays;
            rooms[i].numberOfAdults = numberOfAdults;
            rooms[i].numberOfChildren = numberOfChildren;
            rooms[i].isACRoom = isACRoom;
            rooms[i].roomRate = isACRoom ? 1200 : 1800;
            showBill(rooms[i]);
            saveRoomData();
            return;
        }
    }

    printf("No rooms available.\n");
}

void modifyRoom() {
    int bookingID;
    printf("Enter your booking ID to modify: ");
    scanf("%d", &bookingID);
    getchar();

    for (int i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i].isBooked && rooms[i].bookingID == bookingID) {
            printf("Booking found for %s. Enter new details:\n", rooms[i].name);

            printf("Enter number of days (0-30): ");
            scanf("%d", &rooms[i].numberOfDays);

            printf("Enter number of adults (0-2): ");
            scanf("%d", &rooms[i].numberOfAdults);

            printf("Enter number of children (0-2): ");
            scanf("%d", &rooms[i].numberOfChildren);

            printf("Enter room type (1 for AC, 0 for Non-AC): ");
            scanf("%d", &rooms[i].isACRoom);

            rooms[i].roomRate = rooms[i].isACRoom ? 1200 : 1800;
            saveRoomData();
            showBill(rooms[i]);
            return;
        }
    }
    printf("Booking not found.\n");
}

void cancelRoom() {
    int bookingID;
    printf("Enter your booking ID to cancel: ");
    scanf("%d", &bookingID);
    getchar();

    for (int i = 0; i < MAX_ROOMS; i++) {
        if (rooms[i].isBooked && rooms[i].bookingID == bookingID) {
            rooms[i].isBooked = 0;
            printf("Booking cancelled successfully!\n");
            saveRoomData();
            return;
        }
    }
    printf("Booking ID not found.\n");
}

void mainMenu() {
    int mainChoice;

    do {

        printf("\n--- After Login ---\n");
        printf("1. Room Booking\n");
        printf("2. Restaurant Booking\n");
        printf("3. Logout\n");
        printf("4. Feedback\n");
        printf("5. Helpline\n");
        printf("Select an option: ");
        scanf("%d", &mainChoice);
        getchar();

        switch (mainChoice) {
            case 1:
                roomManagement();
                break;
            case 2:
                restarentManagement();
                break;
            case 3:
                printf("Logged out successfully.\n");

                return;
            case 4:
                feedback();
                break;
            case 5:
                displayHelpline();
                break;
            default:
                printf("Invalid option! Please try again.\n");

        }
    } while (mainChoice != 3);
}

int main() {
    srand(time(NULL));
    initializeRooms();
    loadUserData();

    int choice;
    do {
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Consume newline character

        switch (choice) {
            case 1: signUp(); saveUserData(); break;
            case 2:
                if (login()) {
                    mainMenu();
                } else {
                    printf("Login failed. Try again.\n");
                }
                break;
            case 3: printf("Exiting. Goodbye!\n"); break;
            default: printf("Invalid choice. Try again.\n");
        }
    } while (choice != 3);

    return 0;
}
