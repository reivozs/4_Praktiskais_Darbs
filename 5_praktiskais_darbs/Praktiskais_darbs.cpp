#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

using namespace std;

struct InventoryItem { //structure for items
    string name;
    double price;
    int quantity;
    int sold_quantity;
    bool saved;
};

enum multChoice {
    ADD = 1,
    ALLITEMS = 2,
    SELLITEM = 3,
    SPECIFICITEM = 4,
    MAXSOLD = 5,
    MINSOLD = 6,
    MAXINCOME = 7,
    MININCOME = 8,
    MAXPRICE = 9,
    MINPRICE = 10,
    STOP = 11
};

class InventoryManager {
private:
    vector<InventoryItem> inventory; 

public:
    void addItem() { //endijs
        InventoryItem item;

        cout << "\n--- Datu ievade ---\n";

        cout << "Ievadiet produkta nosaukumu: "; //uztaisit parbaudi
        cin.ignore();
        getline(cin, item.name);

        cout << "\nIevadiet produkta cenu: ";
        cin >> item.price;

        cout << "\nIevadiet produkta daudzumu: ";
        cin >> item.quantity;

        
        ofstream productFile("produkti.dat", ios::binary | ios::app);
        if (productFile.is_open()) {
            productFile.write(reinterpret_cast<char*>(&item), sizeof(InventoryItem));
            productFile.close();

            cout << "\nProdukts ir veiksmigi saglabats binaraja faila!" << endl;
        } else {
            cerr << "Kluda, nevar atvert failu" << endl;
        }
        
        //saveInventory(item.name, item.quantity, item.price);
    }

    void displayItems() { //endijs
        ifstream productData("produkti.dat", ios::binary);

        if(productData.is_open()) {
            InventoryItem item;

            cout << "Produkti noliktava: \n";
            cout << endl;
            cout << "Nosaukums\t\tCena\t\tPieejamais daudzums\t\tPardotais daudzums\n";
            cout << "-----------------------------------------------------------------------------------------\n";

            while (productData.read(reinterpret_cast<char*>(&item), sizeof(InventoryItem))) {
                cout << item.name << "\t\t\t" << item.price << "\t\t\t" << item.quantity << "\t\t\t" << item.sold_quantity << endl;
            }

            cout << "-----------------------------------------------------------------------------------------\n";

            productData.close();
        }
        else{
            cout << "Kluda, fails neatveras" << endl;
        }

        productData.close();
    }
    
    void sellItem() {
    InventoryItem item;

    ifstream productData("produkti.dat", ios::binary | ios::in);
    ofstream newProductData("jaunieProdukti.dat", ios::binary | ios::out | ios::app);

    if (productData.is_open() && newProductData.is_open()) {
        string productName;
        int amount;
        bool found = false;

        cout << "\nIevadiet produkta nosaukumu no produkta, kuru gribat pardot" << endl;
        cin.ignore();
        getline(cin, productName);

        cout << "\nIevadiet produkta daudzumu:" << endl;
        cin >> amount;

        while (productData.read(reinterpret_cast<char*>(&item), sizeof(InventoryItem))) {
            if (productName == item.name) {
                found = true;

                if (amount <= item.quantity) {
                    item.quantity -= amount;
                    item.sold_quantity += amount;
                } else {
                    cout << "\nNav pietiekosu produktu!" << endl;
                }
            }
            newProductData.write(reinterpret_cast<char*>(&item), sizeof(InventoryItem));
        }

        productData.close();
        newProductData.close();

        remove("produkti.dat");
        rename("jaunieProdukti.dat", "produkti.dat");
    } else {
        cout << "\nKluda, fails neatveras" << endl;
    }
}

    void displaySpecificItem() {
        InventoryItem item;

        ifstream productData("produkti.dat", ios::binary | ios::in);

        if (productData.is_open()) {
            string productName;
            bool found = false;

            cout << "\nIevadiet produkta nosaukumu, par kuru gribat uzzinat vairak:" << endl;
            cin.ignore();
            getline(cin, productName);

            while (productData.read(reinterpret_cast<char*>(&item), sizeof(InventoryItem))) {
                if (productName == item.name) {
                    found = true;

                    cout << "\nProdukts noliktava: \n";
                    cout << endl;
                    cout << "Nosaukums\t\tCena\t\tPieejamais daudzums\t\tPardotais daudzums\n";
                    cout << "-----------------------------------------------------------------------------------------\n";

                    cout << item.name << "\t\t\t" << item.price << "\t\t\t" << item.quantity << "\t\t\t" << item.sold_quantity << endl;

                    cout << "-----------------------------------------------------------------------------------------\n";
                }
            }

            if (found == false) {
                cout << "\nSis produkts netika atrasts" << endl;
            }
            productData.close();
        } else {
            cout << "\nKluda, fails neatveras" << endl;
        }
    }

    void top3MinSold() {
        ifstream productData("produkti.dat", ios::binary);
        vector<InventoryItem> items; //??
        InventoryItem item;
        
        if (productData.is_open()) {
            
            while (productData.read(reinterpret_cast<char*>(&item), sizeof(InventoryItem))) {
                items.push_back(item);
        }

        sort(items.begin(), items.end(), [](const InventoryItem& a, const InventoryItem& b) {
                return a.sold_quantity < b.sold_quantity;
            });
        
        cout << "\nVis mazak pardotie 3 produkti ir:" << endl;
        cout << "nosaukums \t\t pardotais skaits" << endl;

        int minNum = min(static_cast<int>(items.size()), 3);
            for (int i = 0; i < minNum; ++i) {
                cout << items[i].name << "\t\t" << items[i].sold_quantity << endl;
            }

        productData.close();
        }
        else {
            cout << "\nKluda, fails neatveras" << endl;
        }
    }
    
    void top3MaxIncome() {
        ifstream productData("produkti.dat", ios::binary);
        vector<InventoryItem> items;
        InventoryItem item;

        if (productData.is_open()) {
            while (productData.read(reinterpret_cast<char*>(&item), sizeof(InventoryItem))) {
                items.push_back(item);
            }

            sort(items.begin(), items.end(), [](const InventoryItem& a, const InventoryItem& b) {
                return (a.price * a.sold_quantity) > (b.price * b.sold_quantity);
            });

            cout << "\nTop 3 augstakie ienakumi ir:" << endl;
            cout << "Nosaukums \t Ienakumi" << endl;

            int maxNum = min(static_cast<int>(items.size()), 3);
            for (int i = 0; i < maxNum; ++i) {
                cout << items[i].name << "\t\t" << (items[i].price * items[i].sold_quantity) << endl;
            }

            productData.close();
        } else {
            cout << "\nKluda, fails neatveras" << endl;
        }
    }

    void top3MinIncome() {
        ifstream productData("produkti.dat", ios::binary);
        vector<InventoryItem> items;
        InventoryItem item;

        if (productData.is_open()) {
            while (productData.read(reinterpret_cast<char*>(&item), sizeof(InventoryItem))) {
                items.push_back(item);
            }

            sort(items.begin(), items.end(), [](const InventoryItem& a, const InventoryItem& b) {
                return (a.price * a.sold_quantity) < (b.price * b.sold_quantity);
            });

            cout << "\nTop 3 mazakie ienakumi ir:" << endl;
            cout << "Nosaukums \t Ienakumi" << endl;

            int minNum = min(static_cast<int>(items.size()), 3);
            for (int i = 0; i < minNum; ++i) {
                cout << items[i].name << "\t\t" << (items[i].price * items[i].sold_quantity) << endl;
            }

            productData.close();
        } else {
            cout << "\nKluda, fails neatveras" << endl;
        }
    }



    void top3MinPrice() {
    ifstream productData("produkti.dat", ios::binary);
    vector<InventoryItem> items;
    InventoryItem item;

    if (productData.is_open()) {
        while (productData.read(reinterpret_cast<char*>(&item), sizeof(InventoryItem))) {
            items.push_back(item);
        }

        sort(items.begin(), items.end(), [](const InventoryItem& a, const InventoryItem& b) {
            return a.price < b.price;
        });

        cout << "\nVis letakie 3 produkti ir:" << endl;
        cout << "Nosaukums \t Cena" << endl;

        int minNum = min(static_cast<int>(items.size()), 3);
        for (int i = 0; i < minNum; ++i) {
            cout << items[i].name << "\t\t" << items[i].price << endl;
        }

        productData.close();
    } else {
        cout << "\nKluda, fails neatveras" << endl;
    }
    }

    void top3MaxPrice() {
    ifstream productData("produkti.dat", ios::binary);
    vector<InventoryItem> items;
    InventoryItem item;

    if (productData.is_open()) {
        while (productData.read(reinterpret_cast<char*>(&item), sizeof(InventoryItem))) {
            items.push_back(item);
        }

        sort(items.begin(), items.end(), [](const InventoryItem& a, const InventoryItem& b) {
            return a.price > b.price;
        });

        cout << "\n3 dargakie produkti ir:" << endl;
        cout << "Nosaukums \t Cena" << endl;

        int maxNum = min(static_cast<int>(items.size()), 3);
        for (int i = 0; i < maxNum; ++i) {
            cout << items[i].name << "\t\t" << items[i].price << endl;
        }

        productData.close();
    } else {
        cout << "\nKluda, fails neatveras" << endl;
    }
    }
};

int main() {
    InventoryManager inventoryManager;
    bool works = true;
    int choice;

    while(works) {
        cout << "\n\nJusu izveles ir:\n"
            "1: Produkta ievade\n" 
            "2: Visu produktu datu izvade\n"
            "3: Pardot produktu\n"
            "4: Konkreti dati par vienu produktu\n"
            "5: Top3 visvairak iztirgotie produkti\n"
            "6: Top3 vismazak iztirgotie produkti\n"
            "7: Top3 produkti par kuriem ir visvairak nopelnits\n"
            "8: Top3 produkti par kuriem ir vismazak nopelnits\n"
            "9: Top3 visdargakie produkti\n"
            "10: Top3 visletakie produkti\n"
            "11: Beigt darbibu\n";

        do {
            cout << "\nLudzu ievadiet savu izveli (1-11): ";
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (choice < 1 || choice >11);
    

    switch (choice) {
        case ADD:
            inventoryManager.addItem();
            break;
        case ALLITEMS:
            inventoryManager.displayItems();
            break;
        case SELLITEM:
            inventoryManager.sellItem();
            break;
        case SPECIFICITEM:
            inventoryManager.displaySpecificItem();
            break;
        case MAXSOLD:
            inventoryManager.top3MaxIncome();
            break;
        case MINSOLD:
            inventoryManager.top3MinSold();
            break;
        case MAXINCOME:
            inventoryManager.top3MaxIncome();
            break;
        case MININCOME:
            inventoryManager.top3MinIncome();
            break;
        case MAXPRICE:
            inventoryManager.top3MaxPrice();
            break;
        case MINPRICE:
            inventoryManager.top3MinPrice();
            break;
        case STOP:
            works = false;
            break;
    }
    }

    return 0;
}

    /*
    void saveInventory(const string& name, int quantity, double price) { //endijs
        ofstream outFile("produkti.dat", ios::out | ios::app | ios::binary);

        if(outFile.is_open()){
            
            bool itemExist = false;

            for (auto& item: inventory){
                if (item.name == name and item.price == price){
                    item.quantity += quantity;
                    itemExist = true;
                    break;
                }
            }

            if(!itemExist){ //ja tads jau eksiste
                InventoryItem newItem;
                newItem.name = name;
                newItem.price = price;
                newItem.quantity = quantity;
                newItem.sold_quantity = 0;
            
                int nameLength = newItem.name.length();
                outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(int));
                outFile.write(newItem.name.c_str(), nameLength);
                outFile.write(reinterpret_cast<const char*>(&newItem.price), sizeof(double));
                outFile.write(reinterpret_cast<const char*>(&newItem.quantity), sizeof(int));
                outFile.write(reinterpret_cast<const char*>(&newItem.sold_quantity), sizeof(int));

                inventory.push_back(newItem);

                cout << "Produkts ir veiksmigi saglabats binaraja faila!" << endl;
            }else{
                cout << "Sads produkts jau pastav, tadel tam tika papildinats jusu ievaditais skaits" << endl;
                return;
            
                outFile.close();
            }
        }else{
            cout << "Radas problemas ar faila atversanu!" << endl;
            return;
        }
    } 
    
    void loadInventory() {
        ifstream inFile("produkti.dat", ios::in | ios::binary);

        if(inFile.is_open()){
            while(true){
                int nameLength;
                inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(int));

                if (inFile.eof()){
                    break;
                }

                string name(nameLength, '\0');
                inFile.read(&name[0], nameLength);

                double price;
                inFile.read(reinterpret_cast<char*>(&price), sizeof(double));

                int quantity;
                inFile.read(reinterpret_cast<char*>(&quantity), sizeof(int));

                int sold_quantity;
                inFile.read(reinterpret_cast<char*>(&sold_quantity), sizeof(int));

                cout << name << "\t\t\t" << price << "\t\t\t" << quantity << "\t\t\t" << sold_quantity << "\n";
            }
        }else{
            cout << "Radas problemas ar faila atversanu!" << endl;
            return;
        }

        inFile.close();
    }
    */
