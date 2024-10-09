#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

using namespace std;

struct Character
{
    int id;
    string name;
    int level;
    int maxHealth;
    int strength;
};

const string FILENAME = "characters.txt";
const string ID_FILENAME = "last_id.txt";

int get_int(const string& prompt);
int generate_unique_id();
void saveCharacterToFile(const Character& character);
void loadCharactersFromFile(vector<Character>& characters, map<int, Character>& characterMap);
void updateCharacter(vector<Character>& characters, map<int, Character>& characterMap);
void deleteCharacter(vector<Character>& characters, map<int, Character>& characterMap);
void createCharacter(vector<Character>& characters, map<int, Character>& characterMap);
void displayCharacterById(const map<int, Character>& characterMap, int id);
void displayAllCharacters(const vector<Character>& characters);

int main()
{
    vector<Character> characters;
    map<int, Character> characterMap;

    loadCharactersFromFile(characters, characterMap);

    int choice;

    do
    {
        cout << "Fantasy Character Management System" << '\n';
        cout << "-----------------------------------" << '\n';
        cout << "1. Create Character" << '\n';
        cout << "2. Search for a Character" << '\n';
        cout << "3. Display all Characters" << '\n';
        cout << "4. Delete a character" << '\n';
        cout << "5. Exit" << '\n';

        choice = get_int("\n>> ");

        switch (choice)
        {
            case 1:
                createCharacter(characters, characterMap);
                break;
            case 2:
            {
                int id = get_int("Enter ID of Character: ");
                displayCharacterById(characterMap, id);
            }
            break;
            case 3:
                displayAllCharacters(characters);
                break;
            case 4:
                deleteCharacter(characters, characterMap);
                break;
            case 5:
                cout << "Exiting the program.\n";
                return 0;
            default:
                cout << "Invalid input. Please try again.\n";
        }
        cout << "Enter any key to continue...";
        cin.get();
        system("CLS");

    } while (choice != 5);

    return 0;
}

int get_int(const string& prompt)
{
    regex integer_regex("^-?[0-9]+$");
    string input;

    while (true)
    {
        cout << prompt;
        getline(cin, input);

        if (regex_match(input, integer_regex))
        {
            try
            {
                return stoi(input);
            }
            catch (out_of_range&)
            {
                cout << "Error: Number out of range. Please try again.\n";
            }
        }
        else
        {
            cout << "Invalid input. Please enter a valid integer.\n";
        }
    }
}

int generate_unique_id()
{
    int last_id = 0;

    ifstream idFile(ID_FILENAME);
    if (idFile)
    {
        idFile >> last_id;
        idFile.close();
    }

    last_id++;

    ofstream outFile(ID_FILENAME);
    if (outFile)
    {
        outFile << last_id;
        outFile.close();
    }
    return last_id;
}

void saveCharacterToFile(const Character& character)
{
    ofstream outFile(FILENAME, ios::app);
    if (outFile)
    {
        outFile << "id: " << character.id << '\n'
                << "name: " << character.name << '\n'
                << "level: " << character.level << '\n'
                << "maxHealth: " << character.maxHealth << '\n'
                << "strength: " << character.strength << '\n'
                << "---\n";
        outFile.close();
    }
    else
    {
        cout << "Error opening file for writing.\n";
    }
}

void loadCharactersFromFile(vector<Character>& characters, map<int, Character>& characterMap)
{
    ifstream inFile(FILENAME);
    if (inFile)
    {
        string line;
        Character character;
        while (getline(inFile, line))
        {
            if (line.empty())
            {
                continue;
            }

            if (line == "---")
            {
                characters.push_back(character);
                characterMap[character.id] = character;
                continue;
            }

            size_t pos = line.find(": ");
            if (pos != string::npos)
            {
                string key = line.substr(0, pos);
                string value = line.substr(pos + 2);

                if (key == "id")
                {
                    character.id = stoi(value);
                }
                else if (key == "name")
                {
                    character.name = value;
                }
                else if (key == "level")
                {
                    character.level = stoi(value);
                }
                else if (key == "maxHealth")
                {
                    character.maxHealth = stoi(value);
                }
                else if (key == "strength")
                {
                    character.strength = stoi(value);
                }
            }
        }

        if (character.id != 0)
        {
            characters.push_back(character);
            characterMap[character.id] = character;
        }
        inFile.close();
    }
    else
    {
        cout << "Error opening file for reading.\n";
    }
}

void updateCharacter(vector<Character>& characters, map<int, Character>& characterMap)
{
    int id = get_int("Enter the ID of the character to update: ");
    auto it = characterMap.find(id);

    if (it != characterMap.end())
    {
        Character& character = it->second;
        cout << "Updating character: \n";
        cout << "Current Name: " << character.name << "\n";
        cout << "Current Level: " << character.level << "\n";
        cout << "Current Max Health: " << character.maxHealth << "\n";
        cout << "Current Strength: " << character.strength << "\n";

        string newName;
        cout << "Enter new name (leave blank to keep current): ";
        getline(cin >> ws, newName);
        if (!newName.empty())
        {
            character.name = newName;
        }

        character.level = get_int("Enter new level (keep current if 0): ");
        if (character.level <= 0)
        {
            character.level = character.level; // No change
        }

        character.maxHealth = get_int("Enter new max health (keep current if 0): ");
        if (character.maxHealth <= 0)
        {
            character.maxHealth = character.maxHealth;
        }

        character.strength = get_int("Enter new strength (keep current if 0): ");
        if (character.strength <= 0)
        {
            character.strength = character.strength;
        }

        ofstream outFile(FILENAME);
        if (outFile)
        {
            for (const auto& chara : characters)
            {
                outFile << "id: " << chara.id << '\n'
                        << "name: " << chara.name << '\n'
                        << "level: " << chara.level << '\n'
                        << "maxHealth: " << chara.maxHealth << '\n'
                        << "strength: " << chara.strength << '\n'
                        << "---\n";
            }
            outFile.close();
            cout << "Character updated successfully.\n";
        }
        else
        {
            cout << "Error opening file for writing.\n";
        }
    }
    else
    {
        cout << "Character with ID " << id << " not found.\n";
    }
}

void deleteCharacter(vector<Character>& characters, map<int, Character>& characterMap)
{
    int id = get_int("Enter the ID of the character to delete: ");
    auto it = characterMap.find(id);

    if (it != characterMap.end())
    {
        characters.erase(remove_if(characters.begin(), characters.end(),
                                     [id](const Character& character) { return character.id == id; }),
                          characters.end());

        characterMap.erase(it);

        ofstream outFile(FILENAME);
        if (outFile)
        {
            for (const auto& chara : characters)
            {
                outFile << "id: " << chara.id << '\n'
                        << "name: " << chara.name << '\n'
                        << "level: " << chara.level << '\n'
                        << "maxHealth: " << chara.maxHealth << '\n'
                        << "strength: " << chara.strength << '\n'
                        << "---\n";
            }
            outFile.close();
            cout << "Character with ID " << id << " deleted successfully.\n";
        }
        else
        {
            cout << "Error opening file for writing.\n";
        }
    }
    else
    {
        cout << "Character with ID " << id << " not found.\n";
    }
}

void createCharacter(vector<Character>& characters, map<int, Character>& characterMap)
{
    Character newCharacter;

    cout << "Enter character name: ";
    getline(cin >> ws, newCharacter.name);

    newCharacter.level = get_int("Enter base level: ");
    newCharacter.maxHealth = get_int("Enter max health: ");
    newCharacter.strength = get_int("Enter base strength: ");
    newCharacter.id = generate_unique_id();

    saveCharacterToFile(newCharacter);

    characters.push_back(newCharacter);
    characterMap[newCharacter.id] = newCharacter;

    cout << "Character created successfully with ID: " << newCharacter.id << '\n';
}

void displayCharacterById(const map<int, Character>& characterMap, int id)
{
    auto it = characterMap.find(id);
    if (it != characterMap.end())
    {
        const Character& character = it->second;
        cout << "Character Details: \n"
             << "ID: " << character.id << "\n"
             << "Name: " << character.name << "\n"
             << "Level: " << character.level << "\n"
             << "Max Health: " << character.maxHealth << "\n"
             << "Strength: " << character.strength << "\n";
    }
    else
    {
        cout << "Character with ID " << id << " not found.\n";
    }
}

void displayAllCharacters(const vector<Character>& characters)
{
    if (characters.empty())
    {
        cout << "No characters to display.\n";
        return;
    }

    cout << "Characters:\n";
    for (const auto& character : characters)
    {
        cout << "ID: " << character.id
             << ", Name: " << character.name
             << ", Level: " << character.level
             << ", Max Health: " << character.maxHealth
             << ", Strength: " << character.strength << '\n';
    }
}
