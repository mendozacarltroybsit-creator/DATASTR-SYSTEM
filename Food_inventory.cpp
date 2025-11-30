#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

enum Category {
    MEAT,
    FISH,
    VEGGIE
};

class FoodItem {
public:
    string id;
    string name;
    Category cat;
    int quantity;
    double price;
    string expiry;
    
    FoodItem(string i, string n, Category c, int q, double p, string e) 
        : id(i), name(n), cat(c), quantity(q), price(p), expiry(e) {}
};

class Inventory {
private:
    vector<FoodItem> items;
    int nextId = 1;
    
    string makeId(Category c) {
        string prefix;
        switch(c) {
            case MEAT: prefix = "M"; break;
            case FISH: prefix = "F"; break;
            case VEGGIE: prefix = "V"; break;
        }
        return prefix + to_string(nextId++);
    }
    
    FoodItem* findItem(string id) {
        for(auto& item : items) {
            if(item.id == id) return &item;
        }
        return nullptr;
    }

public:
    Inventory() {
        // Quick test data - some items might have issues
        items.push_back(FoodItem("M1", "Beef", MEAT, 10, 15.99, "2024-02-28"));
        items.push_back(FoodItem("F1", "Salmon", FISH, 3, 22.50, "2024-01-15")); // probably expired
        items.push_back(FoodItem("V1", "Carrots", VEGGIE, 25, 3.49, "2024-03-10"));
    }
    
    void addItem() {
        string name, exp;
        int type, qty;
        double cost;
        
        cout << "Add Item\nName: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Type (1=Meat, 2=Fish, 3=Veg): ";
        cin >> type;
        
        cout << "Quantity: ";
        cin >> qty;
        
        cout << "Price: ";
        cin >> cost;
        
        cout << "Expiry (yyyy-mm-dd): ";
        cin.ignore();
        getline(cin, exp);
        
        Category cat;
        if(type == 1) cat = MEAT;
        else if(type == 2) cat = FISH;
        else cat = VEGGIE;
        
        string id = makeId(cat);
        items.push_back(FoodItem(id, name, cat, qty, cost, exp));
        
        cout << "Added " << name << " (" << id << ")\n";
    }
    
    void updateStock() {
        string id;
        int newQty;
        
        cout << "Update Stock\nItem ID: ";
        cin >> id;
        
        FoodItem* item = findItem(id);
        if(item) {
            cout << item->name << " current: " << item->quantity << "\nNew quantity: ";
            cin >> newQty;
            item->quantity = newQty;
        } else {
            cout << "Item not found\n";
        }
    }
    
    void sell() {
        string id;
        int amount;
        
        cout << "Sell Item\nID: ";
        cin >> id;
        
        FoodItem* item = findItem(id);
        if(!item) {
            cout << "Not found\n";
            return;
        }
        
        cout << "Sell how many " << item->name << "? Have " << item->quantity << ": ";
        cin >> amount;
        
        if(amount > item->quantity) {
            cout << "Not enough stock\n";
            return;
        }
        
        item->quantity -= amount;
        double total = amount * item->price;
        cout << "Sold " << amount << " for $" << total << endl;
    }
    
    void listAll() {
        cout << "\n--- Inventory ---\n";
        for(auto& item : items) {
            cout << item.id << " " << item.name << " " << item.quantity << " @$" << item->price;
            if(item.quantity < 5) cout << " [LOW]";
            cout << endl;
        }
    }
    
    void showByCategory() {
        int type;
        cout << "Category (1=Meat, 2=Fish, 3=Veg): ";
        cin >> type;
        
        Category cat;
        if(type == 1) cat = MEAT;
        else if(type == 2) cat = FISH;
        else cat = VEGGIE;
        
        cout << "\n--- ";
        switch(cat) {
            case MEAT: cout << "Meat"; break;
            case FISH: cout << "Fish"; break;
            case VEGGIE: cout << "Vegetables"; break;
        }
        cout << " ---\n";
        
        bool found = false;
        for(auto& item : items) {
            if(item.cat == cat) {
                cout << item.id << " " << item.name << " " << item.quantity << endl;
                found = true;
            }
        }
        
        if(!found) cout << "No items\n";
    }
    
    // TODO: This should actually check dates properly
    void checkExpired() {
        cout << "\nExpired items (manual check needed):\n";
        bool any = false;
        for(auto& item : items) {
            // Just flag anything with old-looking dates
            if(item.expiry < "2024-01-01") {
                cout << item.name << " expired " << item.expiry << endl;
                any = true;
            }
        }
        if(!any) cout << "None found\n";
    }
    
    void lowStockReport() {
        cout << "\nLow stock:\n";
        bool any = false;
        for(auto& item : items) {
            int threshold = 5;
            if(item.cat == FISH) threshold = 2; // Fish needs more frequent restocking
            
            if(item.quantity <= threshold) {
                cout << item.name << " only " << item.quantity << " left\n";
                any = true;
            }
        }
        if(!any) cout << "All good\n";
    }
    
    // Quick search - could be better
    void search() {
        string term;
        cout << "Search: ";
        cin.ignore();
        getline(cin, term);
        
        bool found = false;
        for(auto& item : items) {
            if(item.name.find(term) != string::npos) {
                cout << item.id << " " << item.name << " " << item.quantity << endl;
                found = true;
            }
        }
        
        if(!found) cout << "No matches\n";
    }
    
    void generateReport() {
        int totalItems = 0;
        double totalValue = 0.0;
        int meatCount = 0, fishCount = 0, vegCount = 0;
        
        for(auto& item : items) {
            totalItems += item.quantity;
            totalValue += item.quantity * item.price;
            
            switch(item.cat) {
                case MEAT: meatCount++; break;
                case FISH: fishCount++; break;
                case VEGGIE: vegCount++; break;
            }
        }
        
        cout << "\n=== Inventory Report ===\n";
        cout << "Total items: " << totalItems << endl;
        cout << "Total value: $" << totalValue << endl;
        cout << "Meat items: " << meatCount << endl;
        cout << "Fish items: " << fishCount << endl;
        cout << "Vegetables: " << vegCount << endl;
        
        // Quick low stock count
        int lowCount = 0;
        for(auto& item : items) {
            if(item.quantity < 5) lowCount++;
        }
        cout << "Items needing restock: " << lowCount << endl;
    }
};

int main() {
    Inventory inv;
    int choice;
    
    cout << "Food Inventory System\n";
    
    while(true) {
        cout << "\n1. Add item\n2. Update stock\n3. Sell\n4. List all\n5. By category\n";
        cout << "6. Check expired\n7. Low stock\n8. Search\n9. Report\n0. Exit\n";
        cout << "Choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1: inv.addItem(); break;
            case 2: inv.updateStock(); break;
            case 3: inv.sell(); break;
            case 4: inv.listAll(); break;
            case 5: inv.showByCategory(); break;
            case 6: inv.checkExpired(); break;
            case 7: inv.lowStockReport(); break;
            case 8: inv.search(); break;
            case 9: inv.generateReport(); break;
            case 0: return 0;
            default: cout << "Invalid\n";
        }
    }
    
    return 0;
}