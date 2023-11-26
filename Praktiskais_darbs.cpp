#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <limits>

using namespace std;

struct InventoryItem
{ // structure for items
    char name[50];
    double price;
    int quantity;
    int sold_quantity;
};

enum multChoice
{
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
    MOSTAVAILABLE = 11,
    MOSTUNAVAILABLE = 12,
    ASSORTED = 13,
    STOP = 14
};

class InventoryManager
{
private:
    vector<InventoryItem> inventory;

public:
    void addItem()
    { // endijs
        ifstream inFile("produkti.dat", ios::in | ios::binary);
        InventoryItem item;

        inventory.clear();
        if (inFile.is_open())
        {
            while (inFile.read(reinterpret_cast<char *>(&item), sizeof(InventoryItem)))
            {
                inventory.push_back(item);
            }
            inFile.close();
        }

        cout << "\n--- Datu ievade ---\n";

        cout << "Ievadiet produkta nosaukumu: ";
        cin.ignore();
        cin.getline(item.name, 50);

        cout << "\nIevadiet produkta cenu: ";
        cin >> item.price;

        cout << "\nIevadiet produkta daudzumu: ";
        cin >> item.quantity;

        item.sold_quantity = 0;

        bool itemExist = false;

        for (auto &product : inventory)
        {
            if (strcmp(product.name, item.name) == 0)
            {
                product.quantity += item.quantity;
                itemExist = true;
            }
        }

        if (!itemExist)
        { // ja tads neeksiste
            inventory.push_back(item);
        }

        ofstream outFile("produkti.dat", ios::out | ios::binary);

        if (outFile.is_open())
        {
            for (auto &product : inventory)
            {
                outFile.write(reinterpret_cast<const char *>(&product), sizeof(InventoryItem));
            }

            outFile.close();

            if (!itemExist)
            {
                cout << "Produkts ir veiksmigi saglabats binaraja faila!" << endl;
            }
            else
            {
                cout << "Sads produkts jau pastav, tadel tam tika papildinats jusu ievaditais skaits" << endl;
            }
        }
        else
        {
            cout << "Datus neizdevas saglabat!" << endl;
            return;
        }
    }

    void displayItems()
    { // endijs
        ifstream productData("produkti.dat", ios::in | ios::binary);
        InventoryItem item_read;

        if (productData.is_open())
        {
            cout << "Produkti noliktava: \n";
            cout << endl;
            cout << "Nosaukums\t\tCena\t\tPieejamais daudzums\t\tPardotais daudzums\n";
            cout << "-----------------------------------------------------------------------------------------\n";

            while (productData.read(reinterpret_cast<char *>(&item_read), sizeof(InventoryItem)))
            {
                cout << item_read.name << "\t\t\t" << item_read.price << "\t\t\t" << item_read.quantity << "\t\t\t" << item_read.sold_quantity << endl;
            }

            cout << "-----------------------------------------------------------------------------------------\n";
            productData.close();
        }
        else
        {
            cout << "Dati nav pieejami!" << endl;
        }
    }

    void sellItem()
    {
        ifstream productData("produkti.dat", ios::binary | ios::in);
        vector<InventoryItem> updatedItems;
        InventoryItem item;
        char productName[50];
        int amount;
        bool found = false;

        if (!productData.is_open())
        {
            cout << "\nDati nav pieejami!" << endl;
            return;
        }

        cout << "\nIevadiet produkta nosaukumu no produkta, kuru gribat pardot" << endl;
        cin.ignore();
        cin.getline(productName, 50);

        cout << "\nIevadiet produkta daudzumu:" << endl;
        cin >> amount;

        while (productData.read(reinterpret_cast<char *>(&item), sizeof(InventoryItem)))
        {
            if (strncmp(productName, item.name, sizeof(item.name)) == 0)
            {
                found = true;

                if (amount <= item.quantity)
                {
                    item.quantity -= amount;
                    item.sold_quantity += amount;
                }
                else
                {
                    cout << "\nNav pietiekosu produktu!" << endl;
                }
            }
        }

        productData.close();
        if (found = false)
        {
            cout << "\nSis produkts netika atrasts" << endl;
            return;
        }

        ofstream outFile("produkti.dat", ios::out | ios::binary);

        if (outFile.is_open())
        {
            for (size_t i = 0; i < updatedItems.size(); ++i)
            {
                outFile.write(reinterpret_cast<const char *>(&updatedItems[i]), sizeof(InventoryItem));
            }

            outFile.close();
            cout << "\nDati ir veiksmīgi atjaunoti!" << endl;
        }
        else
        {
            cout << "\nNeizdevas atjaunot datus!" << endl;
        }
    }

    void displaySpecificItem()
    { // roberts
        InventoryItem item;

        ifstream productData("produkti.dat", ios::binary | ios::in);

        if (productData.is_open())
        {
            char productName[50];
            bool found = false;

            cout << "\nIevadiet produkta nosaukumu, par kuru gribat uzzinat vairak:" << endl;
            cin.ignore();
            cin >> productName;

            while (productData.read(reinterpret_cast<char *>(&item), sizeof(InventoryItem)))
            {
                if (productName == item.name)
                {
                    found = true;

                    cout << "\nProdukts noliktava: \n";
                    cout << endl;
                    cout << "Nosaukums\t\tCena\t\tPieejamais daudzums\t\tPardotais daudzums\n";
                    cout << "-----------------------------------------------------------------------------------------\n";

                    cout << item.name << "\t\t\t" << item.price << "\t\t\t" << item.quantity << "\t\t\t" << item.sold_quantity << endl;

                    cout << "-----------------------------------------------------------------------------------------\n";
                }
            }

            if (found == false)
            {
                cout << "\nSis produkts netika atrasts" << endl;
            }
            productData.close();
        }
        else
        {
            cout << "\nDati nav pieejami!" << endl;
        }
    }

    void top3MaxSold()
    { // endijs
        ifstream productData("produkti.dat", ios::binary);
        vector<InventoryItem> items;
        InventoryItem item;

        if (productData.is_open())
        {
            while (productData.read(reinterpret_cast<char *>(&item), sizeof(InventoryItem)))
            {
                items.push_back(item);
            }

            sort(items.begin(), items.end(), [](const InventoryItem &a, const InventoryItem &b)
                 { return a.sold_quantity > b.sold_quantity; });

            cout << "\nVisvairak pardotie 3 produkti ir:\n"
                 << endl;
            cout << "Nosaukums \t Pardotais skaits" << endl;
            cout << "-------------------------------" << endl;

            for (int i = 0; i < 3; ++i)
            {
                cout << items[i].name << "\t\t" << items[i].sold_quantity << endl;
            }

            productData.close();
        }
        else
        {
            cout << "\nDati nav pieejami!" << endl;
        }
    }

    void top3MinSold()
    { // roberts
        ifstream productData("produkti.dat", ios::binary);
        vector<InventoryItem> items;
        InventoryItem item;

        if (productData.is_open())
        {
            while (productData.read(reinterpret_cast<char *>(&item), sizeof(InventoryItem)))
            {
                items.push_back(item);
            }

            sort(items.begin(), items.end(), [](const InventoryItem &a, const InventoryItem &b)
                 { return a.sold_quantity < b.sold_quantity; });

            cout << "\nVis mazak pardotie 3 produkti ir:" << endl;
            cout << "nosaukums \t\t pardotais skaits" << endl;

            for (int i = 0; i < 3; ++i)
            {
                cout << items[i].name << "\t\t" << items[i].sold_quantity << endl;
            }

            productData.close();
        }
        else
        {
            cout << "\nDati nav pieejami!" << endl;
        }
    }

    void top3MaxIncome()
    { // roberts
        ifstream productData("produkti.dat", ios::binary);
        vector<InventoryItem> items;
        InventoryItem item;

        if (productData.is_open())
        {
            while (productData.read(reinterpret_cast<char *>(&item), sizeof(InventoryItem)))
            {
                items.push_back(item);
            }

            sort(items.begin(), items.end(), [](const InventoryItem &a, const InventoryItem &b)
                 { return (a.price * a.sold_quantity) > (b.price * b.sold_quantity); });

            cout << "\nTop 3 augstakie ienakumi ir:" << endl;
            cout << "Nosaukums \t Ienakumi" << endl;

            for (int i = 0; i < 3; ++i)
            {
                cout << items[i].name << "\t\t" << (items[i].price * items[i].sold_quantity) << endl;
            }

            productData.close();
        }
        else
        {
            cout << "\nDati nav pieejami!" << endl;
        }
    }

    void top3MinIncome()
    { // endijs
        ifstream productData("produkti.dat", ios::binary);
        vector<InventoryItem> items;
        InventoryItem item;

        if (productData.is_open())
        {
            while (productData.read(reinterpret_cast<char *>(&item), sizeof(InventoryItem)))
            {
                items.push_back(item);
            }

            sort(items.begin(), items.end(), [](const InventoryItem &a, const InventoryItem &b)
                 { return (a.price * a.sold_quantity) < (b.price * b.sold_quantity); });

            cout << "\nTop 3 mazakie ienakumi ir:\n"
                 << endl;
            cout << "Nosaukums \t Ienakumi" << endl;
            cout << "--------------------------" << endl;

            for (int i = 0; i < 3; ++i)
            {
                cout << items[i].name << "\t\t" << (items[i].price * items[i].sold_quantity) << endl;
            }

            productData.close();
        }
        else
        {
            cout << "\nDati nav pieejami" << endl;
        }
    }

    void top3MinPrice()
    { // roberts
        ifstream productData("produkti.dat", ios::binary);
        vector<InventoryItem> items;
        InventoryItem item;

        if (productData.is_open())
        {
            while (productData.read(reinterpret_cast<char *>(&item), sizeof(InventoryItem)))
            {
                items.push_back(item);
            }

            sort(items.begin(), items.end(), [](const InventoryItem &a, const InventoryItem &b)
                 { return a.price < b.price; });

            cout << "\nVis letakie 3 produkti ir:" << endl;
            cout << "Nosaukums \t Cena" << endl;

            for (int i = 0; i < 3; ++i)
            {
                cout << items[i].name << "\t\t" << items[i].price << endl;
            }

            productData.close();
        }
        else
        {
            cout << "\nDati nav pieejami!" << endl;
        }
    }

    void top3MaxPrice()
    { // endijs
        ifstream productData("produkti.dat", ios::binary);
        vector<InventoryItem> items;
        InventoryItem item;

        if (productData.is_open())
        {
            while (productData.read(reinterpret_cast<char *>(&item), sizeof(InventoryItem)))
            {
                items.push_back(item);
            }

            sort(items.begin(), items.end(), [](const InventoryItem &a, const InventoryItem &b)
                 { return a.price > b.price; });

            cout << "\n3 dargakie produkti ir:\n"
                 << endl;
            cout << "Nosaukums \t Cena" << endl;
            cout << "----------------------" << endl;

            for (int i = 0; i < 3; ++i)
            {
                cout << items[i].name << "\t\t" << items[i].price << endl;
            }

            productData.close();
        }
        else
        {
            cout << "\nDati nav pieejami!" << endl;
        }
    }

    void assorted()
    {
        ifstream productData("produkti.dat", ios::binary | ios::in);
        vector<InventoryItem> startingItems;
        vector<InventoryItem> updatedItems;

        InventoryItem item;

        if (productData.is_open())
        {
            double amountOfMoney;

            cout << "Ievadiet naudas summu: ";
            cin >> amountOfMoney;
            bool productsSkipped = true;
            // push all objects into a vector
            while (productData.read(reinterpret_cast<char *>(&item), sizeof(InventoryItem)))
            {
                startingItems.push_back(item);
            }
            updatedItems = startingItems;

            productData.close();

            while (true)
            {
                productsSkipped = true;
                // go through vector and change values, if neccessary
                for (InventoryItem &item : updatedItems)
                {
                    if (item.quantity > 0 && amountOfMoney >= item.price)
                    {
                        amountOfMoney -= item.price;
                        item.quantity -= 1;
                        item.sold_quantity += 1;
                        productsSkipped = false;
                    }
                }
                // if couldnt afford any product
                if (productsSkipped)
                {

                    cout << "\nTika nopirkti sadi produkti:\n"
                         << endl;
                    cout << "Nosaukums \t Daudzums \t Kopsumma" << endl;
                    cout << "---------------------------------" << endl;

                    for (size_t i = 0; i < startingItems.size(); ++i)
                    {
                        if (startingItems[i].quantity != updatedItems[i].quantity)
                        {
                            int quantity = startingItems[i].quantity - updatedItems[i].quantity;
                            cout << updatedItems[i].name << "\t\t" << quantity << "\t\t" << (quantity * updatedItems[i].price) << endl;
                        }
                    }
                    cout << "Pari palika " << amountOfMoney << " naudas." << endl;
                    saveData(updatedItems);
                    return;
                }
            }
        }
        else
        {
            cout << "Dati nav pieejami!" << endl;
        }
    }

    void saveData(vector<InventoryItem> updatedItems)
    {
        ofstream outFile("produkti.dat", ios::out | ios::binary);

        if (outFile.is_open())
        {
            for (size_t i = 0; i < updatedItems.size(); ++i)
            {
                outFile.write(reinterpret_cast<const char *>(&updatedItems[i]), sizeof(InventoryItem));
            }

            outFile.close();
        }
        else
        {
            cout << "Dati nav pieejami!" << endl;
        }
    }

    void top3MostAvailable()
    {
        ifstream productData("produkti.dat", ios::binary);
        vector<InventoryItem> items;
        InventoryItem item;

        if (productData.is_open())
        {
            while (productData.read(reinterpret_cast<char *>(&item), sizeof(InventoryItem)))
            {
                items.push_back(item);
            }

            productData.close();

            sort(items.begin(), items.end(), [](const InventoryItem &a, const InventoryItem &b)
                 { return a.quantity > b.quantity; });

            cout << "\n3 Pieejamakie produkti ir:\n"
                 << endl;
            cout << "Nosaukums \t Cena" << endl;
            cout << "----------------------" << endl;

            for (int i = 0; i < 3; ++i)
            {
                cout << items[i].name << "\t\t" << items[i].quantity << endl;
            }
        }
        else
        {
            cout << "\nDati nav pieejami!" << endl;
        }
    }

    void top3MostUnavailable()
    {
        ifstream productData("produkti.dat", ios::binary);
        vector<InventoryItem> items;
        InventoryItem item;

        if (productData.is_open())
        {
            while (productData.read(reinterpret_cast<char *>(&item), sizeof(InventoryItem)))
            {
                items.push_back(item);
            }

            productData.close();

            sort(items.begin(), items.end(), [](const InventoryItem &a, const InventoryItem &b)
                 { return a.quantity < b.quantity; });

            cout << "\n3 Pieejamakie produkti ir:\n"
                 << endl;
            cout << "Nosaukums \t Cena" << endl;
            cout << "----------------------" << endl;

            for (int i = 0; i < 3; ++i)
            {
                cout << items[i].name << "\t\t" << items[i].quantity << endl;
            }
        }
        else
        {
            cout << "\nDati nav pieejami!" << endl;
        }
    }
};

int main()
{ // roberts
    InventoryManager inventoryManager;
    bool works = true;
    int choice;

    while (works)
    {
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
                "11: Top3 pieejamakie produkti\n"
                "12: Top3 nepieejamakie produkti\n"
                "13: Produktu asorti\n"
                "14: Beigt darbibu\n";
        do
        {
            cout << "\nLudzu ievadiet savu izveli (1-14): ";
            cin >> choice;

            if (cin.fail())
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (choice < 1 || choice > 14);

        switch (choice)
        {
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
            inventoryManager.top3MaxSold();
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
        case ASSORTED:
            inventoryManager.assorted();
            break;
        case MOSTAVAILABLE:
            inventoryManager.top3MostAvailable();
            break;
        case MOSTUNAVAILABLE:
            inventoryManager.top3MostUnavailable();
            break;
        case STOP:
            works = false;
            break;
        }
    }

    return 0;
}