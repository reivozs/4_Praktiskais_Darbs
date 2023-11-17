#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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
    vector<InventoryItem> inventory; //place to save items 

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

        item.sold_quantity = 0;

        saveInventory(item.name, item.quantity);
    }

    void displayItems() { //endijs
        loadInventory();

        if(!inventory.empty()){
            cout << "Produkti noliktava: \n";
            cout << endl;
            cout << "Nosaukums\t\tCena\t\tPieejamais daudzums\t\tPardotais daudzums\n";
            cout << "---------------------------------------------------------------------\n";

            for(const auto& item: inventory){
                cout << item.name << "\t\t" << item.price << "\t\t" << item.quantity << "\t\t" << item.sold_quantity << "\n"; 
            }

            cout << "---------------------------------------------------------------------\n";
        }else{
            cout << "Nav pieejams neviens produkts" << endl;
        }
    }

    void saveInventory(const string& name, int newQuantity) { //endijs
        ofstream outFile("myfile.txt", ios::out | ios::binary)

        if(outFile.is_open()){
            if(item.name == name){ //ja tads jau eksiste
                item.quantity += newQuantity;
                cout << "Sads produkts jau pastav, tadel tam tika papildinats jusu ievaditais daudzums!" << endl;
            }else{
                int nameLength = item.name.length();
                outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(int));
                outFile.write(item.name.c_str(), nameLength);
                outFile.write(reinterpret_cast<const char*>(&item.price), sizeof(double));
                outFile.write(reinterpret_cast<const char*>(&item.quantity), sizeof(int));
                outFile.write(reinterpret_cast<const char*>(&item.sold_quantity), sizeof(int));
                cout << "Produkts ir veiksmigi saglabats binaraja faila!" << endl;
            }
        }else{
            cout << "Radas problemas ar faila atversanu!" << endl;
            return;
        }

        outFile.close();
    } 

    void loadInventory() {
        ifstream inFile("myfile.txt", ios::in | ios::binary);

        if(inFile.is_open()){
            while(true){
                int numLength;
                inFile.read(reinterpret_cast<char*>(&numLength), sizeof(int));

                if (infile.eof()){
                    break;
                }

                string name(nameLength, \0);
                inFile.read(&name[0], numLength);

                double price;
                inFile.read(reinterpret_cast<char*>(&price), sizeof(double));

                int quantity;
                inFile.read(reinterpret_cast<char*>(&quantity), sizeof(int));

                int sold_quantity;
                inFile.read(reinterpret_cast<char*>(&sold_quantity), sizeof(int));

                InventoryItem item;
                item.name = name;
                item.price = price;
                item.quantity = quantity;
                item.sold_quantity = sold_quantity;

                inventory.push_back(item);
            }
        }else{
            cout << "Radas problemas ar faila atversanu!" << endl;
            return;
        }

        inFile.close();
    }

    void sellItem() { //roberts
        string name;
        int saleNum;

        cout << "Ievadiet nosaukumu no produkta, kuru Jums gribas pardot:" << endl;
        getline(cin, name);

        if (find(inventory.begin(), inventory.end(), name) != inventory.end()) {
            cout << "Ievadiet cik produktus gribat pardot" << endl;
            cin >> saleNum;
            if (saleNum <= inventory[find(inventory.begin(), inventory.end(), name)])
        }
        else {
            cout << "Sis produkts vel neeksiste" << endl;
        }
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