#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

// Product class
class Product {
public:
    string id;
    string name;
    double price;

    Product() {}
    Product(string id, string name, double price) : id(id), name(name), price(price) {}

    void display() const {
        cout << setw(15) << left << id
             << setw(15) << left << name
             << setw(10) << left << price << endl;
    }
};

// ShoppingCart class
class ShoppingCart {
public:
    struct CartItem {  
        Product product;
        int quantity;
    };

private:
    CartItem* cart;
    int cartSize;

public:
    ShoppingCart() : cart(nullptr), cartSize(0) {}

    ~ShoppingCart() { 
        delete[] cart; 
        cart = nullptr;
    }

    void addProduct(const Product& product) {
        for (int i = 0; i < cartSize; i++) {
            if (cart[i].product.id == product.id) {
                cart[i].quantity++;
                cout << "Product added successfully!\n";
                return;
            }
        }

        CartItem* newCart = new CartItem[cartSize + 1];
        for (int i = 0; i < cartSize; i++) {
            newCart[i] = cart[i];
        }
        newCart[cartSize] = {product, 1};

        delete[] cart;
        cart = newCart;
        cartSize++;
        cout << "Product added successfully!\n";
    }

    void viewCart() const {
        if (cartSize == 0) {
            cout << "Your shopping cart is empty.\n";
            return;
        }
        cout << "\nProduct ID      Name           Price     Quantity\n";
        cout << "------------------------------------------------\n";
        for (int i = 0; i < cartSize; i++) {
            cout << setw(15) << left << cart[i].product.id
                 << setw(15) << left << cart[i].product.name
                 << setw(10) << left << cart[i].product.price
                 << setw(10) << left << cart[i].quantity << endl;
        }
    }

    bool isEmpty() const { return cartSize == 0; }
    CartItem* getCartItems() { return cart; }
    int getCartSize() const { return cartSize; }

    void clearCart() { 
        delete[] cart; 
        cart = nullptr;
        cartSize = 0; 
    }
};


// Order class
class Order {
private:
    static int orderCounter;
    int orderId;
    double totalAmount;
    ShoppingCart::CartItem orderDetails[20];
    int orderSize;

public:
    Order() : orderId(0), totalAmount(0), orderSize(0) {}

    void createOrder(ShoppingCart::CartItem* cartItems, int cartSize) {
        orderId = ++orderCounter;
        totalAmount = 0;
        orderSize = cartSize;
        for (int i = 0; i < cartSize; i++) {
            orderDetails[i] = cartItems[i];
            totalAmount += cartItems[i].product.price * cartItems[i].quantity;
        }
    }

    void displayOrder() const {
        cout << "\nOrder ID: " << orderId << "\nTotal Amount: Php " << totalAmount << endl;
        cout << "Order Details:\n";
        cout << "Product ID      Name           Price     Quantity\n";
        cout << "------------------------------------------------\n";
        for (int i = 0; i < orderSize; i++) {
            cout << setw(15) << left << orderDetails[i].product.id
                 << setw(15) << left << orderDetails[i].product.name
                 << setw(10) << left << orderDetails[i].product.price
                 << setw(10) << left << orderDetails[i].quantity << endl;
        }
    }
};

int Order::orderCounter = 0;

// Input validation
string getValidatedStringInput() {
    string input;
    cin.ignore();
    getline(cin, input);
    return input;
}

char getYesOrNo() {
    char choice;
    while (true) {
        cout << "Enter 'Y' or 'N': ";
        cin >> choice;
        choice = toupper(choice);
        if (choice == 'Y' || choice == 'N') return choice;
        cout << "Invalid input! Please enter 'Y' or 'N'.\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

int getValidIntegerInput(int min, int max) {
    int number;
    while (true) {
        if (cin >> number && number >= min && number <= max) {
            return number;
        }
        cout << "Invalid input! Please enter a number between " << min << " and " << max << ": ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// Main function
int main() {
    //list ng products
    Product productList[] = {
        {"ABC", "Paper", 50.00},
        {"CDE", "Pencil", 15.00},
        {"QWE", "Notebook", 100.00},
        {"TRE", "Eraser", 10.00},
        {"ASD", "Marker", 50.00},
        {"ZXC", "Crayons", 150.00}
    };
    int productListSize = 6;
    ShoppingCart cart;
    Order orders[10];
    int orderCount = 0;
    int choice;
    bool continueRunning = true;

    while (continueRunning) {
        cout << "\n===== Store Menu =====\n";
        cout << "1. View Products\n2. View Shopping Cart\n3. View Orders\n4. Exit\nEnter your choice: ";
        choice = getValidIntegerInput(1, 4);

        switch (choice) {
            case 1:
                for (int i = 0; i < productListSize; i++) productList[i].display();
                cout << "\nEnter the ID of the product you want to add to the cart: ";
                {
                    string productId = getValidatedStringInput();
                    bool found = false;
                    for (int i = 0; i < productListSize; i++) {
                        if (productList[i].id == productId) {
                            cart.addProduct(productList[i]);
                            found = true;
                            break;
                        }
                    }
                    if (!found) cout << "Invalid Product ID!\n";
                }
                break;
            case 2:
                cart.viewCart();
                if (!cart.isEmpty() && orderCount < 10) {
                    cout << "\nDo you want to check out? (Y/N): ";
                    if (getYesOrNo() == 'Y') {
                        orders[orderCount].createOrder(cart.getCartItems(), cart.getCartSize());
                        cout << "Checked out successfully!\n";
                        orderCount++;
                        cart.clearCart();
                    }
                }
                break;
            case 3:
                if (orderCount == 0) cout << "No orders placed yet.\n";
                else for (int i = 0; i < orderCount; i++) orders[i].displayOrder();
                break;
            case 4:
                continueRunning = false;
                break;
        }
    }
    return 0;
}