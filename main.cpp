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

int get_int(string prompt)
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
    static int current_id = 0;
    return ++current_id;
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

    characters.push_back(newCharacter);
    characterMap[newCharacter.id] = newCharacter;

    cout << "Character created with ID: " << newCharacter.id << '\n';

}

int main()
{
    vector<Character> characters;
    map<int, Character> characterMap;

    createCharacter(characters, characterMap);
}
