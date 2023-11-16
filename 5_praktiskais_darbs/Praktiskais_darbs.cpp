#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct InventoryItem { //structure for items
    string name;
    double price;
    int quantity;
    int sold_quantity;
    bool saved;
};

class InventoryManager {
private:
    verctor<InventoryItem> inventory; //place to save items 

public:
    void addItem() { //endijs
        InventoryItem item;

        cout << "--- Datu ievade ---\n";

        cout << "Ievadiet produkta nosaukumu: "; //uztaisit parbaudi
        cin.ignore();
        getline(cin, item.name);

        cout << "\nIevadiet produkta cenu: ";
        cin >> item.price;

        cout << "\nIevadiet produkta daudzumu: ";
        cin >> item.quantity;

        //ja tads jau ir tad pieskaita skaitu
        item.sold_quantity = 0;

        saveInventory(item.name, item.quantity);
        inventory.push_back(item);
    
    }

    void displayItems() { //endijs
        cout << "Produkti noliktava: \n";
        cout << endl;
        cout << "Nosaukums\t\tCena\t\tPieejamais daudzums\t\tPardotais daudzums\n";
        cout << "---------------------------------------------------------------------\n";

        for(const auto& item: inventory){
            cout << item.name << "\t\t" << item.price << "\t\t" << item.quantity << "\t\t" << item.sold_quantity << "\n"; 
        }

        cout << "---------------------------------------------------------------------\n";

    }

    void saveInventory(const string& name, int newQuantity) { //endijs
        ofstream outFile("mytfile.txt", ios::out | ios::binary)

        if(outFile.is_open()){
            if(item.name == name){
                item.quantity += newQuantity;
            }else{
                int nameLength = item.name.length();
                outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(int));
                outFile.write(item.name.c_str(), nameLength);
                outFile.write(reinterpret_cast<const char*>(&item.price), sizeof(double));
                outFile.write(reinterpret_cast<const char*>(&item.quantity), sizeof(int));
                outFile.write(reinterpret_cast<const char*>(&item.sold_quantity), sizeof(int));
            }
        }else{
            cout << "Radas problemas ar faila atversanu!" << endl;
            return;
        }

        outFile.close();
        cout << "Produkts ir veiksmigi saglabats binaraja faila!" << endl;

    } 

    void sellItem() { //roberts

    }

    void searchItem() { //roberts

    }

}

int main() {
    InventoryManager inventorymanager;
    int choice;

    while(true) { // izvelne Roberts (switch)
        


    }
    return 0;
}
