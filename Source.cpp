#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>  // for std::this_thread::sleep_for

bool PlayerBlock;
int run = 10;

using namespace std;

class Character {
public:
    std::string name;
    int intel;
    int speed;
    int health;
    int attack;

    Character(const std::string& n, int h, int a, int s, int i) : name(n), health(h), attack(a), speed(s), intel(i) {}

    bool isAlive() const {
        return health > 0;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) {
            health = 0;
        }
    }

    int dealDamage() const {
        return rand() % (attack / 2) + (attack / 2);
    }

    int dealHeal() const {
        return rand() % (attack / 2) + (attack / 2);
    }
};

void displayHPBars(Character& player, Character& enemy) {
    std::cout << "Player HP: " << player.health << " / 100\t";
    std::cout << "Enemy HP: " << enemy.health << " / 80" << std::endl;
}

void displayMenu() {
    std::cout << "1. Attack\n";
    std::cout << "2. Defend\n";
    std::cout << "3. Run\n";
    cout << "4. Heal\n";
}

int getPlayerChoice() {
    int choice;
    std::cout << "Choose your action: ";
    std::cin >> choice;
    return choice;
}

void playerTurn(Character& player, Character& enemy) {
    displayMenu();
    int choice = getPlayerChoice();

    switch (choice) {
    case 1: {
        // Attack
        int playerDamage = player.dealDamage();
        std::cout << player.name << " deals " << playerDamage << " damage to " << enemy.name << std::endl;
        enemy.takeDamage(playerDamage);
        break;
    }
    case 2:
        PlayerBlock = true;
        std::cout << player.name << " defends!" << std::endl;
        break;
    case 3:
        std::cout << "You attempt to run away!" << std::endl;
        if (player.speed > enemy.speed) {
            cout << player.name << " successfully runs away." << endl;
            run = 1;
        }
        else if (player.speed < enemy.speed) {
            cout << "Escape is unsuccessful";
            run = 0;
        }
        break;
    case 4: {
        player.health = player.health + 30;
        cout << "Player Heals!";
        if (player.health > 100)
        {
            player.health = 100;
        }
        cout << "PLAYER HP: " << player.health;
        break;
    }
    default:
        std::cout << "Invalid choice. Try again." << std::endl;
        playerTurn(player, enemy);
    }
}

void enemyTurn(Character& player, Character& enemy) {
    // Simple enemy AI: always attack
    int originalEnemyDamage = enemy.dealDamage();
    int reducedEnemyDamage = originalEnemyDamage / 2;

    if (PlayerBlock == true) {
        std::cout << enemy.name << " deals " << reducedEnemyDamage << " damage to " << player.name << std::endl;
        player.takeDamage(reducedEnemyDamage);
    }
    else {
        std::cout << enemy.name << " deals " << originalEnemyDamage << " damage to " << player.name << std::endl;
        player.takeDamage(originalEnemyDamage);
    }
    PlayerBlock = false;
}

void battle(Character& player, Character& enemy) {
    std::cout << "Battle Start! " << player.name << " vs " << enemy.name << std::endl;

    while (player.isAlive() && enemy.isAlive()) {
        // Display updated information
        displayHPBars(player, enemy);

        // Player's turn
        playerTurn(player, enemy);

        // Wait for 2 seconds
        std::this_thread::sleep_for(std::chrono::seconds(2));

        if (!enemy.isAlive()) {
            std::cout << enemy.name << " has been defeated!" << std::endl;
            break;
        }

        if (run == 1) {
            cout << player.name << " has escaped!" << endl;
            break;
        }
        if (run == 0) {
            cout << player.name << " hasn't escaped!" << endl;
        }

        // Enemy's turn
        enemyTurn(player, enemy);

        // Display updated information
        displayHPBars(player, enemy);

        

        if (!player.isAlive()) {
            std::cout << player.name << " has been defeated!" << std::endl;
            break;
        }
        // Wait for 2 seconds
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Clear the console
        system("CLS");
    }

    std::cout << "Battle End!" << std::endl;
}

int main() {
    std::srand(std::time(0));

    Character player("Player", 100, 20, 39, 30);
    Character enemy("Enemy", 80, 10, 10, 10);

    battle(player, enemy);

    return 0;
}
