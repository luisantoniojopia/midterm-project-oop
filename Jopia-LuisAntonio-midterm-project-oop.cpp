#include <iostream>
#include <iomanip>
#include <cctype>
#include <string>
#include <limits>
#include <vector>
using namespace std;

// Abstract Base Class
class Item {
	protected:
		string itemID;	// Encapsulation
		string itemName;
		int itemQuantity;
		double itemPrice;

	public:
		// Virtual destructor to delete from the base and derived classes
		virtual ~Item() = default;

		// Constructor parameters are assigned to corresponding class attributes
		Item(string id, string name, int quantity, double price)
			: itemID(id), itemName(name), itemQuantity(quantity), itemPrice(price) {} // Initializers match the constructor's parameters

		// Function to display item details
		void displayItemDetails() const;

		// Pure virtual function to display item category
		virtual void displayItemCategory() const = 0;

		// Setters
		void setQuantity(int newQuantity) {
			itemQuantity = newQuantity;    // Considering adding edit name
		}
		void setPrice(double newPrice) {
			itemPrice = newPrice;
		}

		// Getters return protected attributes
		string getItemID() const {
			string lowerID = itemID;
			for (char& c : lowerID) {
				c = tolower(c);
			}
			return lowerID;
		}
		string getItemName() const {
			return itemName;
		}
		int getItemQuantity() const {
			return itemQuantity;
		}
		double getItemPrice() const {
			return itemPrice;
		}
};

void Item::displayItemDetails() const {
	cout << "\t\tID: " << getItemID() << endl;
	cout << "\t\tName: " << getItemName() << endl;
	cout << "\t\tQuantity: " << getItemQuantity() << endl;
	cout << "\t\tPrice: " << fixed << setprecision(2) << getItemPrice() << endl;
}

// Derived Classes
class ClothingItem : public Item {
	public:
		// Constructor initializes the base class attributes using an initializer list
		ClothingItem(const string& id, const string& name, int quantity, double price)
			: Item(id, name, quantity, price) {} // Call to base class constructor

		void displayItemDetails();
		void displayItemCategory() const override {
			cout << "\t\tCategory: Clothing";
		};
};

class ElectronicsItem : public Item {
	public:
		ElectronicsItem(const string& id, const string& name, int quantity, double price)
			: Item(id, name, quantity, price) {}

		void displayItemDetails();
		void displayItemCategory() const override {
			cout << "\t\tCategory: Electronics";
		};
};

class EntertainmentItem : public Item {
	public:
		EntertainmentItem(string id, string name, int quantity, double price)
			: Item(id, name, quantity, price) {}

		void displayItemDetails();
		void displayItemCategory() const override {
			cout << "\t\tCategory: Entertainment";
		};
};

// Class Manager
class Inventory {
	private:
		vector<Item*> itemStorage; // Store pointers (all 3 categories of items) to Item Base Class

	public:
		static bool isValidID(const string& id);
		bool isIDTaken(const string& fullID);
		static bool validateChar(char input); // Forward declaration
		static bool validateString(const string &input);
		static string capitalizeFirstLetter(const string& input);
		static bool validateInt(int input);
		static bool validateDouble(const string& input);
		static bool isString(const string& input);
		static bool isAllDigits(const string& input);
		static char validateYesNo(const string& prompt);

		void addItem();
		void updateItem();
		void removeItem();
		void displayByCategory();
		void displayAllItems();
		static void displayItemDetails(const Item* item, const string& category);
		void searchItem();
		void sortItems();
		void displayLowStock();
		static string getCategory(Item* item);

		~Inventory() {
			// Destructor to clean allocated memory
			for(Item* item : itemStorage) {
				delete item; // Free each allocated memory
			}
		}
};

// Validations
bool Inventory::isValidID(const string& id) {
	if (id.empty()) {
		return false; // Empty string is invalid
	}

	for (char c : id) {
		if (!isalnum(c) || isspace(c)) {
			return false; // Return false if any character is not alphanumeric
		}
	}
	return true;
}

bool Inventory::isIDTaken(const string& fullID) {
	for (Item* item : itemStorage) {
		if (item->getItemID() == fullID) {
			return true; // ID already exists
		}
	}
	return false; // ID is available
}

bool Inventory::validateChar(char input) {
	return isalpha(input);
}

bool Inventory::isString(const string& input) {
	for (char c : input) {
		if (!isalpha(c) && !isdigit(c) && c != ' ') {
			return false;
		}
	}
	return true;
}

bool Inventory::validateString(const string &input) {
	return !input.empty();
}

string Inventory::capitalizeFirstLetter(const string& input) {
	string result;
	bool capitalizeNext = true;

	for (char c : input) {
		if (isspace(c)) {
			capitalizeNext = true; 
			result += c; // Add space as it is
		} else {
			if (capitalizeNext) {
				result += toupper(c); // Capitalize the character
				capitalizeNext = false; // Reset flag
			} else {
				result += tolower(c); // Lowercase the character
			}
		}
	}
	return result;
}

bool Inventory::validateInt(int input) {
	return input >= 0;
}

bool Inventory::validateDouble(const string& input) { 
	bool decimalPoint = false;
	if (input.empty()) return false; // Empty string is invalid

	for (char c : input) {
		if(!isdigit(c)) {
			if (c == '.' && !decimalPoint) {
				decimalPoint = true; 
			} else if (!isspace(c)) {
				return false;
			} else {
				return false;
			}
		}
	}
	return true;
}

bool Inventory::isAllDigits(const string& input) {
	for (char c : input) {
		if (!isdigit(c) || isspace(c)) {
			return false;
		}
	}
	return true;
}

char Inventory::validateYesNo(const string& prompt) {
	char choice;
	bool validInput;
	
	do {
		cout << prompt << " [Y/N]: ";
		cin >> choice;
		cout << endl;
		
		// Check if input is a single character
        if (cin.fail() || cin.peek() != '\n') {
            cin.clear(); // Clear the fail state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining input
            validInput = false; // Invalid input
            cout << "\tInvalid input! Please enter only 1 letter (Y or N)." << endl << endl;
            continue; // Ask for input again
        }
        
		choice = toupper(choice);
		
		if (choice == 'Y' || choice == 'N') {
			validInput = true;
			cin.ignore();
		} else {
			validInput = false;
			cout << "\tInvalid choice! Please enter Y or N." << endl << endl;
		}
	} while (!validInput);
	return choice;
}

void toLowerCase(string& str) {
	for (char&  c : str) {
		if (c >= 'A' && c <= 'Z') {
			c += 32;
		}
	}
}

bool validateMenuChoice(int& choice, int min, int max) {
    string menuChoice;
    bool validInput;
	
    do {
        validInput = true;
        cout << "Select Action: ";
        getline(cin, menuChoice);

        for (char c : menuChoice) {
            if (!isdigit(c) || isspace(c)) {
                validInput = false;
                break;
            }
        }

        if (validInput) {
        	try {
            	choice = stoi(menuChoice);
            	
	            if (choice < min || choice > max) {
	                validInput = false;
	                cout << "\tInvalid choice! Please select a number between " << min << " and " << max << "." << endl << endl;
	            }
			} catch (invalid_argument&) {
        		cout << "\tInvalid input. Please enter a numeric value and/or avoid space." << endl << endl;
        		validInput = false;
			}
        } else {
            cout << "\tInvalid input! Please enter a numeric value and/or avoid space." << endl << endl;
        }

    } while (!validInput);
    return true;
}

// Menu Options
void Inventory::addItem() {
	string categoryChoice, name, alphaNumericIDInput, quantityInput, priceInput;
	int quantity = 0;
	double price = 0;

	do {
		cout << "Enter the new item information." << endl << endl;
		cout << "\tCL - Clothing\n\tEL - Electronics\n\tEN - Entertainment" << endl;
		
		// Input category
		do {
		    cout << "\tCategory: ";
		    getline(cin, categoryChoice);
		    
		    if (categoryChoice.length() > 2) {
		        cout << "\tInvalid input! Please enter exactly two letters (CL, EL, or EN)." << endl;
		    } else {
		        for (char& c : categoryChoice) {
		            c = tolower(c);
        		}		

		        if (categoryChoice != "cl" && categoryChoice != "el" && categoryChoice != "en") {
		            cout << "\tCategory " << categoryChoice << " does not exist! Please enter CL, EL, or EN." << endl;
		        }
		    }
		    cout << endl;
		} while (categoryChoice.length() != 2 || (categoryChoice != "cl" && categoryChoice != "el" && categoryChoice != "en"));

		// Input id
		do {
			cout << "\tID: ";
			getline(cin, alphaNumericIDInput); 
			
			if (!isValidID(alphaNumericIDInput)) {
				cout << "\tInvalid input. Please enter alphanumeric characters and avoid space." << endl << endl;
			} else if (isIDTaken(categoryChoice + alphaNumericIDInput)) {
				cout << "\tThis ID is already taken. Please choose another." << endl;
			}
		} while (!isValidID(alphaNumericIDInput) || isIDTaken(categoryChoice + alphaNumericIDInput));
		
		string id = categoryChoice + alphaNumericIDInput;
		cout << "\tOfficial ID: " << id << endl;
		
		// Input name
		do {
			cout << "\tName: ";
			getline(cin, name);
			
			if (name.empty()) {
				cout << "\tInvalid input. Avoid space and enter a valid name." << endl << endl;
			} else {
				name = capitalizeFirstLetter(name);
			}
		} while (name.empty());

		// Input quantity
		do {
			cout << "\tQuantity: ";
			getline(cin, quantityInput);
			
			if (quantityInput.empty() || !isAllDigits(quantityInput)) {
		        cout << "\tInvalid input. Please enter a numeric value and/or avoid space." << endl << endl;
		        continue;
		    } else {
				try { // Handle the exceptions due to stoi
					quantity = stoi(quantityInput);
					if (quantity <= 0) {
						cout << "\tInvalid input. Please enter a positive quantity" << endl << endl;
					} else {
						break;
					}
				} catch (invalid_argument&) { // Handle invalid numeric conversion
	        		cout << "\tInvalid input. Please enter a numeric value and/or avoid space." << endl << endl;
				} catch (out_of_range&) { // Handle very large number
					cout << "\tInput is out of range. Please enter a smaller number." << endl << endl;
 				}
			}
		} while (quantityInput.empty() || !isAllDigits(quantityInput) || quantity <= 0);

		// Input price
		do {
			cout << "\tPrice: ";
			getline(cin, priceInput);
			if (priceInput.empty() || !validateDouble(priceInput)) {
				cout << "\tInvalid input. Please enter a numeric value and/or avoid space." << endl << endl;
				continue;
			}
			
			try {
				price = stod(priceInput);
				if (price <= 0) {
					cout << "\tInvalid input. Please enter a positive price" << endl << endl;
					continue;
				} 
				break;
			} catch (invalid_argument&) {
				cout << "\tInvalid input. Please enter a numeric value and/or avoid space." << endl << endl;
			} catch (out_of_range&) {
				cout << "Input is out of range. Please enter a smaller number." << endl;
			}
		} while (priceInput.empty() || !validateDouble(priceInput) || price <= 0);

		// Create the item and add it to storage after gathering all inputs
		if( categoryChoice == "CL" || categoryChoice == "cl") {
			itemStorage.push_back(new ClothingItem(id, name, quantity, price));
		} else if (categoryChoice == "EL" || categoryChoice == "el") {
			itemStorage.push_back(new ElectronicsItem(id, name, quantity, price));
		} else if (categoryChoice == "EN" || categoryChoice == "en") {
			itemStorage.push_back(new EntertainmentItem(id, name, quantity, price));
		}

		cout << "\tItem added successfully!" << endl << endl;
	} while (validateYesNo("Add Another Item") == 'Y');
	system("pause");
}

void Inventory::updateItem() {
	string id, updateChoice, quantityInput, priceInput;
	char updateChar = 0;
	int newQuantity = 0;
	double newPrice = 0;
	bool itemFound = false;

	if (itemStorage.empty()) {
		cout << "\tNo items in the inventory. Nothing to update." << endl << endl;
	    system("pause");
	    return;
	}

	do {
		itemFound = false;
		cout << "Enter the ID, what to update and its new value." << endl << endl;
		
		do {
			cout << "\tID: ";
			getline(cin, id); 
			cout << endl;
			
			if (!isValidID(id)) {
				cout << "\tInvalid input. Please enter alphanumeric characters and/or avoid space." << endl << endl;
			}
		} while (!isValidID(id));
		
		toLowerCase(id);

		// Loop through itemStorage vector to find the item
		for (Item* item : itemStorage) {
			if (item->getItemID() == id) {
				itemFound = true;

				cout << "\tCurrent Details of the Item" << endl;
				item->displayItemDetails();
				item->displayItemCategory();
				cout << endl << endl;

				// Ask what to update
				cout << "\tQ - Quantity\n\tP - Price" << endl;
				do {
					cout << "\tWhat to update: ";
					getline(cin, updateChoice);
					
					if (updateChoice.length() > 1) {
						cout << "\tInvalid input! Please enter only 1 letter (Q or P)." << endl << endl;
					} else {
						updateChoice[0] = toupper(updateChoice[0]);
						updateChar = updateChoice[0];
						
						if (updateChoice != "Q" && updateChoice != "P") {
						cout << "\tInvalid choice! Please enter Q for Quantity or P for Price." << endl << endl;
						}
					}
				} while (updateChoice.length() != 1 || updateChoice != "Q" && updateChoice != "P");
				
				switch(updateChar) {
					case 'Q': {
						const int oldQuantity = item->getItemQuantity(); // Getter
						
						do {
							cout << "\tNew Quantity: ";
							getline(cin, quantityInput);
							
							if (quantityInput.empty() || !isAllDigits(quantityInput)) {
								cout << "\tInvalid input. Please enter a positive whole number and/or avoid space." << endl << endl;
							} else {
								try {
									newQuantity = stoi(quantityInput);
									
									if (newQuantity == oldQuantity) {
										cout << "\tYou entered the same amount. Please enter a different value." << endl << endl;
									} else {
										item->setQuantity(newQuantity); // Setter
										cout << "\tQuantity of Item " << item->getItemName() << " is updated from " << oldQuantity << " to " << newQuantity << endl << endl;
										break;
									} 
								} catch (invalid_argument&) {
						            cout << "\tInvalid input. Please enter a numeric value and/or avoid space." << endl << endl;
						        } catch (out_of_range&) {
						            cout << "\tInput is out of range. Please enter a smaller number." << endl << endl;
						        }
							}
						} while (quantityInput.empty() || !isAllDigits(quantityInput) || newQuantity == oldQuantity); 
						break;
					}
					case 'P': {
						const double oldPrice = item->getItemPrice();
						
						do {
							cout << "\tNew Price: ";
							getline(cin, priceInput);
							if (priceInput.empty() || !validateDouble(priceInput)) {
								cout << "\tInvalid input. Please enter a positive whole number and/or avoid space." << endl << endl;
							} else {
								try {
								 	newPrice = stoi(priceInput);
									
									if (newPrice == oldPrice) {
										cout << "\tYou entered the same amount. Please enter a different value." << endl << endl;
									} else {
										item->setPrice(newPrice);
										cout << "\tPrice of Item " << item->getItemName() << " is updated from " << oldPrice << " to " << newPrice << endl << endl;
										break;
									}
								} catch (invalid_argument&) {
						            cout << "\tInvalid input. Please enter a numeric value and/or avoid space." << endl << endl;
						        } catch (out_of_range&) {
						            cout << "\tInput is out of range. Please enter a smaller number." << endl << endl;
						        }
							}
						} while (priceInput.empty() || !validateDouble(priceInput) || newPrice == oldPrice);
						break;
					}
					default:
						cout << "\tInvalid choice!" << endl;
						break;
				}
				break;
			}
		}
		if (!itemFound) {
			cout << "\tItem not found!" << endl << endl;
		}
	} while (validateYesNo("Update Another Item") == 'Y');
	system("pause");
}

void Inventory::removeItem() {
	string id;

	if (itemStorage.empty()) {
		cout << "\tNo items in the inventory. Nothing to remove." << endl << endl;
	    system("pause");
	    return;
	}

	do {
		cout << "Enter the ID of the item to remove." << endl << endl;
		do {
			cout << "\tID: ";
			getline(cin, id); 
			cout << endl;
			
			if (!isValidID(id)) {
				cout << "\tInvalid input. Please enter alphanumeric characters and/or avoid space." << endl << endl;
			}
		} while (!isValidID(id));

		toLowerCase(id);

		// Loop to search and remove an item
		for (auto it = itemStorage.begin(); it != itemStorage.end();) {
			if ((*it)->getItemID() == id) {
				delete *it; // Free memory
				itemStorage.erase(it); // Remove item from storage
				cout << "\tItem " << id << " has been removed from the inventory." << endl << endl;
				system("pause");
				return;
			} else {
				++it; // Only increment if not erased
			}
		}
		cout << "\tItem not found!" << endl << endl;

	} while (validateYesNo("Remove Another Item") == 'Y');
}

void Inventory::displayByCategory() {
	string categoryChoice, cleanedCategory, category;

	if (itemStorage.empty()) {
		cout << "\tNo items in the inventory. Nothing to display." << endl << endl;
	    system("pause");
	    return;
	}

	do {
		cout << "Enter the category code to display." << endl << endl; 
		cout << "\tCL - Clothing" << endl;
		cout << "\tEL - Electronics" << endl;
		cout << "\tEN - Entertainment" << endl;
	
		// Input category
		do {
		    cout << "\tCategory: ";
		    getline(cin, categoryChoice);
		    if (categoryChoice.length() > 2) {
		        cout << "\tInvalid input! Please enter exactly two letters (CL, EL, or EN)." << endl;
		    } else {
		        for (char& c : categoryChoice) {
		            c = tolower(c);
        		}		

		        if (categoryChoice != "cl" && categoryChoice != "el" && categoryChoice != "en") {
		            cout << "\tCategory " << categoryChoice << " does not exist! Please enter CL, EL, or EN." << endl;
		        }
		    }
		    cout << endl;
		} while (categoryChoice.length() != 2 || (categoryChoice != "cl" && categoryChoice != "el" && categoryChoice != "en"));

		// Determine the category based on the code
		if (categoryChoice == "cl") {
			category = "Clothing";
		} else if (categoryChoice == "el") {
			category = "Electronics";
		} else if (categoryChoice == "en") {
			category = "Entertainment";
		} else {
			cout << "\tCategory " << categoryChoice << " does not exist." << endl << endl;
			return;
		}

		bool found = false; // Track if any items were found

		// Print header
		cout << "\t" << left << setw(15) << "ID"
		     << setw(15) << "Name"	
		     << setw(15) << "Quantity"
		     << setw(15) << "Price"	
		     << setw(15) << "Category" << endl;

		// Loop through all items and display only those that match the category
		for (Item* item : itemStorage) { 
				if ((category == "Clothing" && dynamic_cast<ClothingItem*>(item)) ||
			        (category == "Electronics" && dynamic_cast<ElectronicsItem*>(item)) ||
			        (category == "Entertainment" && dynamic_cast<EntertainmentItem*>(item))) {
				
				string itemName = item->getItemName();
				if (itemName.length() > 18 - 3) {
					itemName = itemName.substr(0, 15) + "...";
				}
				
				// Display item details in a table row
				cout << "\t" << left << setw(15) << item->getItemID()
				     << setw(15) << itemName	
				     << setw(15) << item->getItemQuantity()
				     << setw(15) << fixed << setprecision(2) << item->getItemPrice() 
				     << setw(15) << category << endl;
				found = true; // Set flag to true when an item is found
			}
		}
		cout << endl;
		if (!found) {
			cout << "\tNo items found in the " << category << " Category." << endl << endl; 
		}

	} while (validateYesNo("Display Another Category") == 'Y');
	system("pause");
}

void Inventory::displayAllItems() {
	string category;
	bool hasClothingItems = false;
	bool hasElectronicsItems = false;
	bool hasEntertainmentItems = false;

	if (itemStorage.empty()) {
		cout << "\tNo items in the inventory. Nothing to display." << endl << endl;
	    system("pause");
	    return;
	}

	// Print header
	cout << "\t" << left << setw(15) << "ID"
	     << setw(15) << "Name"	
	     << setw(15) << "Quantity"
	     << setw(15) << "Price"	
	     << setw(15) << "Category" << endl;

	// Separate sections for each category
	for (Item* item : itemStorage) {
		if (dynamic_cast<ClothingItem*>(item)) {
			if (!hasClothingItems) {
				hasClothingItems = true;
			}
			displayItemDetails(item, "Clothing");
		}
	}

	for (Item* item : itemStorage) {
		if (dynamic_cast<ElectronicsItem*>(item)) {
			if (!hasElectronicsItems) {
				hasElectronicsItems = true;
			}
			displayItemDetails(item, "Electronics");
		}
	}

	for (Item* item : itemStorage) {
		if (dynamic_cast<EntertainmentItem*>(item)) {
			if (!hasEntertainmentItems) {
				hasEntertainmentItems = true;
			}
			displayItemDetails(item, "Entertainment");
		}
	}

	cout << endl;
	if (!hasClothingItems && !hasElectronicsItems&& !hasEntertainmentItems) {
		cout << "\tNo items found in any category." << endl << endl;
	}
	system("pause");
}

void Inventory::displayItemDetails(const Item* item, const string& category) {
	string itemName = item->getItemName();
	if (itemName.length() > 18 - 3) {
		itemName = "";
		string currentItemName = item->getItemName();
		for (int i = 0; i < 18 - 3;i++) {
			itemName += currentItemName[i];
		}
		itemName += "..."; // Replace long item name
	}
	
	cout << "\t" << left << setw(15) << item->getItemID()
	     << setw(15) << itemName	
	     << setw(15) << item->getItemQuantity()
	     << setw(15) << fixed << setprecision(2) << item->getItemPrice() 
	     << setw(15) << category << endl;
}

void Inventory::searchItem() {
	string searchTerm;

	if (itemStorage.empty()) {
		cout << "\tNo items in the inventory. Nothing to search." << endl << endl;
	    system("pause");
	    return;
	}

	do {
		cout << "Enter the ID to search." << endl << endl;
		do {
			cout << "\tID: ";
			getline(cin, searchTerm); 
			cout << endl;
			
			if (!isValidID(searchTerm)) {
				cout << "\tInvalid input. Please enter alphanumeric characters and/or avoid space." << endl << endl;
			}
		} while (!isValidID(searchTerm));

		toLowerCase(searchTerm);
		bool found = false;

		for (Item* item : itemStorage) {
			if (item->getItemID() == searchTerm) {
				found = true;
				cout << "\tCurrent Details of the Item" << endl;
				item->displayItemDetails();
				item->displayItemCategory();
				cout << endl << endl;
			}
		}
		if (!found) {
			cout << "\tItem not found!" << endl << endl;
		}

	} while (validateYesNo("Search Another Item") == 'Y');
	system("pause");
}
				
void Inventory::sortItems() {
	string sortChoice, orderChoice;

	if (itemStorage.empty()) {
		cout << "\tNo items in the inventory. Nothing to sort" << endl << endl;
	    system("pause");
	    return;
	}
	
	do {
		// Sort by quantity or price
		cout << "Enter the letter to sort the list accordingly." << endl << endl;
		cout << "\tQ - Quantity\n\tP - Price" << endl;
		do {
			cout << "\tSort By: ";
			getline(cin, sortChoice);
			cout << endl;
			
			if (sortChoice.length() > 1) {
				cout << "\tInvalid input! Please enter only 1 letter (Q or P)." << endl << endl;
			} else {
				sortChoice[0] = toupper(sortChoice[0]);

				if(sortChoice != "Q" && sortChoice != "P") {
					cout << "\tInvalid choice! Please enter Q for Quantity or P for Price." << endl << endl;
				}
			}
		} while (sortChoice.length() != 1 || sortChoice != "Q" && sortChoice != "P");
		
		// Sort by ascending or descending
		cout << "Select order." << endl << endl;
		cout << "\tA - Ascending\n\tD - Descending" << endl;
		do {
			cout << "\tArranged By: ";
			getline(cin, orderChoice);
			cout << endl;
			
			if (orderChoice.length() > 1) {
				cout << "\tInvalid input! Please enter only 1 letter (A or D)." << endl << endl;
			} else {
				orderChoice[0] = toupper(orderChoice[0]);

				if(orderChoice != "A" && orderChoice != "D") {
					cout << "\tInvalid choice! Please enter A for Ascending or D for Descending." << endl << endl;
				}
			}
		} while (orderChoice.length() != 1 || orderChoice != "A" && orderChoice != "D");
		
		// Determine sorting order
		bool ascending = (orderChoice == "A" || orderChoice == "a");
		int n;
		n = itemStorage.size();

		// Bubble Sort Algorithm
		for (int i = 0; i < n - 1; i++) {
			for (int j = 0; j < n - i - 1; j++) {
				bool condition = false;
				
				if (sortChoice == "Q" || sortChoice == "q") { // Check if sorting by quantity
					condition = ascending
						? itemStorage[j]->getItemQuantity() > itemStorage[j + 1]->getItemQuantity() // Checks if qty is > than the next to do the swap for ascending
						: itemStorage[j]->getItemQuantity() < itemStorage[j + 1]->getItemQuantity(); // Checks if qty is < than the next to do the swap for descending
				} else if (sortChoice == "P" || sortChoice == "p") { // Check if sorting by price
					condition = ascending
						? itemStorage[j]->getItemPrice() > itemStorage[j + 1]->getItemPrice()
						: itemStorage[j]->getItemPrice() < itemStorage[j + 1]->getItemPrice();
				}
				
				// Swap elements if condition is true
				if (condition) {
					Item* temp = itemStorage[j];
					itemStorage[j] = itemStorage[j + 1];
					itemStorage[j + 1] = temp;
				}
			} 
		}

		// Display table header
		cout << "\t" << left << setw(15) << "ID"
		     << setw(15) << "Name"    
		     << setw(15) << "Quantity"
		     << setw(15) << "Price"    
		     << setw(15) << "Category" << endl;

		for (Item* item : itemStorage) {
			string itemName = item->getItemName();
			if (itemName.length() > 18 - 3) {
				itemName = itemName.substr(0, 15) + "...";
			}
			
			cout << "\t" << left << setw(15) << item->getItemID()   
			     << setw(15) << itemName               
			     << setw(15) << item->getItemQuantity()               
			     << setw(15) << fixed << setprecision(2) << item->getItemPrice() 
			     << setw(15) << getCategory(item) << endl;              
		}
		cout << endl;
	} while (validateYesNo("Sort Again") == 'Y');
	system("pause");
}

void Inventory::displayLowStock() {
	bool foundLowStock = false;

	if (itemStorage.empty()) {
		cout << "\tNo items in the inventory. Nothing to display." << endl << endl;
	    system("pause");
	    return;
	}

	cout << "Items with low stock (5 or fewer)." << endl << endl;
	// Print header
	cout << "\t" << left << setw(15) << "ID"
	     << setw(15) << "Name"	
	     << setw(15) << "Quantity"
	     << setw(15) << "Price"	
	     << setw(15) << "Category" << endl;

	for (Item* item : itemStorage) {
		int lowStockLevel = 5;
		if (item->getItemQuantity() <= lowStockLevel) {
			foundLowStock = true;
			cout << "\t" << left << setw(15) << item->getItemID()
			     << setw(15) << item->getItemName()
			     << setw(15) << item->getItemQuantity()
			     << setw(15) << fixed << setprecision(2) << item->getItemPrice() 
			     << setw(15) << getCategory(item) << endl;
		}
	}
	if (!foundLowStock) {
		cout << "\n\tNo items with low stock." << endl; 
	}
	cout << endl;
	system("pause");
}

string Inventory::getCategory(Item* item) {
	if (dynamic_cast<ClothingItem*>(item)) {
		return "Clothing";
	} else if (dynamic_cast<ElectronicsItem*>(item)) {
		return "Electronics";
	} else if (dynamic_cast<EntertainmentItem*>(item)) {
		return "Entertainment";
	}
	return "Unknown";
}

// Menu
void displayMenu(Inventory& inventory) {
	int menuChoice;
	do {
		system("cls");
		cout << "============================= Inventory Management System =============================" << endl << endl;
		cout << "Menu" << endl;
		cout << "\t1 - Add Item" << endl;
		cout << "\t2 - Update Item" << endl;
		cout << "\t3 - Remove Item" << endl;
		cout << "\t4 - Display Items By Category" << endl;
		cout << "\t5 - Display All Items" << endl;
		cout << "\t6 - Search Item" << endl;
		cout << "\t7 - Sort Items" << endl;
		cout << "\t8 - Display Low Stock Items" << endl;
		cout << "\t9 - Exit" << endl;
		validateMenuChoice(menuChoice, 1, 9);
        cout << endl;

		switch (menuChoice) {
			case 1:
				cout << "------------------------------------- [1] Add Item ------------------------------------" << endl << endl;
				inventory.addItem();
				break;
			case 2:
				cout << "------------------------------------ [2] Update Item ----------------------------------" << endl << endl;
				inventory.updateItem();
				break;
			case 3:
				cout << "------------------------------------ [3] Remove Item ----------------------------------" << endl << endl;
				inventory.removeItem();
				break;
			case 4:
				cout << "----------------------------- [4] Display Items by Category ---------------------------" << endl << endl;
				inventory.displayByCategory();
				break;
			case 5:
				cout << "--------------------------------- [5] Display All Items -------------------------------" << endl << endl;
				inventory.displayAllItems();
				break;
			case 6:
				cout << "----------------------------------- [6] Search Item -----------------------------------" << endl << endl;
				inventory.searchItem();
				break;
			case 7:
				cout << "------------------------------------ [7] Sort Items -----------------------------------" << endl << endl;
				inventory.sortItems();
				break;
			case 8:
				cout << "------------------------------ [8] Display Low Stock Items ----------------------------" << endl << endl;
				inventory.displayLowStock();
				break;
			case 9:
				cout << "\t\tThank you for using the Inventory Management System!" << endl << endl;
				cout << "=======================================================================================" << endl << endl;
				cout << "Ooprog Midterm Examination" << endl;
				cout << "Created by: Luis Antonio M. Jopia" << endl;
				cout << "Section: C2B" << endl;
				break;
			default:
				cout << "Invalid action! Please try again." << endl << endl;
		}
	} while (menuChoice !=9);
}

int main() {
	Inventory inventory;
	displayMenu(inventory);
	return 0;
}