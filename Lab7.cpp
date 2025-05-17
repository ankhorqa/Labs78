#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <iostream>
#include <string>
#include <cstdlib>


class Entity
{
protected:
    std::string name;
    int health;
    int attack;
    int defense;
public:
    Entity(const std::string& name, int health, int attack, int defense)
        : name(name), health(health), attack(attack), defense(defense) {}

    std::string getName() const { return name; }

    int getHealth() const { return health; }

    int getAttack() const { return attack; }

    int getDefense() const { return defense; }

    virtual void displayInfo() const
    {
        std::cout << "Имя: " << name << ", HP: " << health
            << ", Атака: " << attack << ", Защита: " << defense << std::endl;
    }

    virtual void heal(int amount) { }

    virtual void takeDamage(int amount)
    {
        if (amount < 0)
        {
            std::cout << "Без урона!" << std::endl;
        }
        else if (health - amount <= 0)
        {
            health = 0;
            std::cout << name << " получил " << amount
                << " урона и умер!" << std::endl;
        }
        else
        {
            health -= amount;
            std::cout << name << " получил " << amount
                << " урона, текущее ХП: " << health << std::endl;
        }
    }

    virtual void attackEnemy(Entity& target)
    {
        std::cout << name << " атакует " << target.name << " >> ";
        int damage = attack - target.defense;
        target.takeDamage(damage);
    }

    virtual ~Entity() {}
};

class Character : public Entity
{
public:
    Character(const std::string& name, int health, int attack, int defense)
        : Entity(name, health, attack, defense) {}

    void heal(int amount) override
    {
        if (health + amount > 100)
        {
            health = 100;
            std::cout << name << " ХП восстановлено до максимума."
                << health << std::endl;
        }
        else
        {
            health += amount;
            std::cout << name << " восстановлено " << amount
                << " ХП, текущее ХП: " << health << std::endl;
        }
    }

    void attackEnemy(Entity& target) override
    {
        if (target.getHealth() == 0)
            return;

        int damage = attack - target.getDefense();

        bool isCriticalHit = (rand() % 100) < 20;
        if (isCriticalHit)
            std::cout << "\033[35m" << name << " атакует " << target.getName() << " с крит. уроном! >> ";
        else
            std::cout << "\033[32m" << name << " атакует " << target.getName() << " >> ";
        target.takeDamage(isCriticalHit ? damage * 2 : damage);
        std::cout << "\033[0m";
    }

    void displayInfo() const override
    {
        std::cout << "Персонаж: " << name << ", ХП: " << health
            << ", Атака: " << attack << ", Защита: " << defense << std::endl;
    }
};

class Monster : public Entity
{
public:
    Monster(const std::string& name, int health, int attack, int defense)
        : Entity(name, health, attack, defense) {}

    void attackEnemy(Entity& target) override
    {
        int damage = attack - target.getDefense();

        bool hasExtraDamage = rand() % 100 < 30;
        if (hasExtraDamage)
            std::cout << "\033[35m" << name << " атакует " << target.getName() << " с крит. уроном +5 >> ";
        else
            std::cout << "\033[36m" << name << " атакует " << target.getName() << " >> ";

        target.takeDamage(hasExtraDamage ? damage + 5 : damage);
        std::cout << "\033[0m";
    }

    void displayInfo() const override
    {
        std::cout << "Монстр: " << name << ", ХП: " << health
            << ", Атака: " << attack << ", Защита: " << defense << std::endl;
    }
};


bool heroAlive = true;
std::vector<Monster> monsters;
std::mutex monstersMutex;

void generateMonsters() {
    while (heroAlive) {
        std::this_thread::sleep_for(std::chrono::seconds(3)); 
        std::lock_guard<std::mutex> lock(monstersMutex);
        monsters.push_back(Monster("Goblin", 50, 15, 5));
        std::cout << "Появился новый монстр!\n";
    }
}


void fight(Character& hero) {
    while (heroAlive)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        std::lock_guard<std::mutex> lock(monstersMutex);
        for (auto& monster : monsters)
        {
            if (rand() % 100 < 50 ? false : true)
                std::cout << "\033[33m" << "Герой не попадает!" << "\033[0m" << std::endl;
            else
                hero.attackEnemy(monster);
            
            std::this_thread::sleep_for(std::chrono::seconds(1));

            if (rand() % 100 < 50 ? false : true)
                std::cout << "\033[33m" << "Монстр не попадает!" << "\033[0m" << std::endl;
            else
                monster.attackEnemy(hero);
            

            if (hero.getHealth() <= 0)
            {
                std::cout << "\033[31m" << hero.getName() << " убит!" << "\033[0m" << std::endl;
                heroAlive = false;
                break;
            }
        }
    }
}


int main() {
    std::thread monsterGenerator(generateMonsters);
    monsterGenerator.detach();

    Character hero("Hero", 100, 20, 10);

    std::thread fightThread(fight, std::ref(hero));
    fightThread.detach();

    return 0;
}
