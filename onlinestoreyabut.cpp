#include <iostream>
#include <iomanip>

using namespace std;

// product class
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

// shoppingcart class
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
    ~ShoppingCart() { delete[] cart; }

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
    void clearCart() { delete[] cart; cart = nullptr; cartSize = 0; }
};

// order class
class Order {
private:
    static int orderCounter;
    int orderId;
    double totalAmount;
    ShoppingCart::CartItem* orderDetails;
    int orderSize;

public:
    Order() : orderId(0), totalAmount(0), orderDetails(nullptr), orderSize(0) {}
    ~Order() { delete[] orderDetails; }

    void createOrder(ShoppingCart::CartItem* cartItems, int cartSize) {
        orderId = ++orderCounter;
        totalAmount = 0;
        orderSize = cartSize;
        orderDetails = new ShoppingCart::CartItem[cartSize];
        for (int i = 0; i < cartSize; i++) {
            orderDetails[i] = cartItems[i];
            totalAmount += cartItems[i].product.price * cartItems[i].quantity;
        }
    }

    void displayOrder() const {
        cout << "\nOrder ID: " << orderId << "\nTotal Amount: Php: " << totalAmount << endl;
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

// input validation func
string getValidatedStringInput() {
    string input;
    while (!(cin >> input)) {
        cout << "Invalid input! Please enter a valid string: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return input;
}

char getYesOrNo() {
    char choice;
    do {
        cin >> choice;
        choice = toupper(choice);
        if (choice == 'Y' || choice == 'N') {
            return choice;
        }
        cout << "Invalid input! Please enter 'Y' or 'N': ";
        cin.clear();
        cin.ignore(1000, '\n');
    } while (choice != 'Y' && choice != 'N'); 

    return choice;
}

int getValidIntegerInput() {
    int number;
    while (!(cin >> number)) {
        cout << "Invalid input! Please enter a valid number: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return number;
}

// main func
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
    Order* orders = nullptr;
    int orderCount = 0;
    int choice;
    bool continueRunning = true;

    while (continueRunning) {
        cout << "\n===== Store Menu =====\n";
        cout << "1. View Products\n2. View Shopping Cart\n3. View Orders\n4. Exit\nEnter your choice: ";
        choice = getValidIntegerInput();

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
                if (!cart.isEmpty()) {
                    cout << "\nDo you want to check out? (Y/N): ";
                    if (getYesOrNo() == 'Y') {
                        Order* newOrders = new Order[orderCount + 1];
                        for (int i = 0; i < orderCount; i++) newOrders[i] = orders[i];
                        newOrders[orderCount].createOrder(cart.getCartItems(), cart.getCartSize());
                        delete[] orders;
                        orders = newOrders;
                        cout << "Checked out successfully!\n";
                        orderCount++;
                        cart.clearCart();
                    }
                }
                break;
            case 3:
                for (int i = 0; i < orderCount; i++) orders[i].displayOrder();
                break;
            case 4:
                cout << "Exiting. Thank you!\n";
                continueRunning = false;
                break;
            default:
                cout << "Invalid choice!\n";
        }
    }
    delete[] orders;
    return 0;
}
