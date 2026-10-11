// GAME2001 - Assignment 1
// Name: Kemal Aras Yildirim
// Student ID: 101534224


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>



constexpr  int HIT_THRESHOLD = 50;
constexpr  int REPAIR_PERCENT = 5;




//   
class Fighter
{
private:
    std::string name;
    int damage;
    int structStrength;
    int maxStructStrength;

public:
    Fighter(const std::string & fighterName, int maxHealth, int attackDamage)
    {
        name = fighterName;
        damage = attackDamage;
        maxStructStrength = maxHealth;
        structStrength = maxHealth;
    }


    //  damage,  not go below zero
    void reduceStructure(int amount)
    {
        if (amount <= 0)
        {
            return;
        }

        structStrength = structStrength - amount;
        if (structStrength < 0)
        {
            structStrength = 0;
        }
    }
    // recover health, but do not go higher than max health
    void repair(int amount)
    {
        if (amount <= 0)
        {
            return;
        }

        structStrength = structStrength + amount;
        if (structStrength > maxStructStrength)
        {
            structStrength = maxStructStrength;
        }
    }



    bool isDestroyed() const
    {
        return structStrength <= 0;
    }

    const std::string& getName() const
    {
        return name;
    }

    int getDamage() const
    {
        return damage;
    }

    int getStructStrength() const
    {
        return structStrength;
    }

    int getMaxStructStrength() const
    {
        return maxStructStrength;
    }






    // Return Fighter information as a string
    std::string getInfo() const
    {
        std::ostringstream output;

        output << name
            << "  Structure: " << structStrength
            << "/" << maxStructStrength
            << "  Damage: " << damage;

        return output.str();
    }



};













class Carrier
{
private:
    std::string name;
    Fighter** bayList; // 
    int maxBays;
    int numFighters;



  
    void mergeSortWorker(int lower, int upper)
    {
        if (lower < upper)
        {
            int mid = (lower + upper) / 2;

            mergeSortWorker(lower, mid);
            //recursion
            mergeSortWorker(mid + 1, upper);

            merge(lower, mid, upper);
        }
    }

    // merge two sorted parts alphabetically //Lab 3
    void merge(int lo, int mid, int hi)
    {
        int size = hi - lo + 1;
        Fighter** temp = new Fighter * [size];

        int i = lo;
        int j = mid + 1;
        int k = 0;

        // Compare both parts
        while (i <= mid && j <= hi)
        {
            if (bayList[i]->getName() <= bayList[j]->getName())
            {
                temp[k] = bayList[i];
                i++;
            }
            else
            {
                temp[k] = bayList[j];
                j++;
            }

            k++;
        }





        // Copy remaining Fighters from left part
        while (i <= mid)
        {
            temp[k] = bayList[i];
            i++;
            k++;
        }

        // Copy remaining Fighters from right part
        while (j <= hi)
        {
            temp[k] = bayList[j];
            j++;
            k++;
        }

        // Copy sorted pointers back into bayList
        for (int x = 0; x < size; x++)
        {
            bayList[lo + x] = temp[x];
        }

        delete[] temp;
    }






public:



    // Sort Fighters alphabetically by name
    void mergeSortByName()
    {
        if (numFighters <= 1)
        {
            return;
        }

        mergeSortWorker(0, numFighters - 1);
    }



    bool hasFighters() const
    {
        return numFighters > 0;
    }




   
    Carrier(const std::string& carrierName, int capacity)
    {
        name = carrierName;
        maxBays = capacity;
        numFighters = 0;

        bayList = new Fighter * [maxBays](); // pointer slot as the capasity 
    }




    // Prevent copying ////
    Carrier(const Carrier&) = delete;
    Carrier& operator=(const Carrier&) = delete;



    // Destructor
    ~Carrier()
    {
        for (int i = 0; i < numFighters; i++)
        {
            delete bayList[i];
        }

        delete[] bayList;
    }





    // add a Fighter to the end of the array
    bool loadFighter(Fighter* fighter)
    {
        if (fighter == nullptr)
        {
            return false;
        }

        if (numFighters < maxBays)
        {
            bayList[numFighters] = fighter;
            numFighters++;
            return true;
        }

        return false;
    }



    void repairAllFighters()
    {
        for (int i = 0; i < numFighters; i++)
        {
            int amount = bayList[i]->getMaxStructStrength()
                * REPAIR_PERCENT / 100;

            bayList[i]->repair(amount);
        }
    }



    // shift others left ////////////////////////
    Fighter* launchNextFighter()
    {
        if (numFighters == 0)
        {
            return nullptr;
        }

        Fighter* first = bayList[0];

        for (int i = 1; i < numFighters; i++)
        {
            bayList[i - 1] = bayList[i];
        }

        numFighters--;
        bayList[numFighters] = nullptr;

        return first;
    }




    //  Fighters from strongest to weakest
    void insertionSortByStrength() ///// big O
    {
        int prev;
        Fighter* temp;

        for (int start = 1; start < numFighters; start++)
        {
            prev = start - 1;
            temp = bayList[start];

            while (prev >= 0 &&
                bayList[prev]->getStructStrength() < temp->getStructStrength())
            {
                bayList[prev + 1] = bayList[prev];
                prev--;
            }

            bayList[prev + 1] = temp;
        }
    }





    // Return Carrier and Fighter information
    std::string getInfo() const
    {
        std::ostringstream output;

        output << "Carrier: " << name
            << " (" << numFighters
            << "/" << maxBays
            << " bays in use)\n";

        if (numFighters == 0)
        {
            output << "  (no fighters)\n";
        }
        else
        {
            for (int i = 0; i < numFighters; i++)
            {
                output << "  " << i << ". "
                    << bayList[i]->getInfo()
                    << "\n";
            }
        }

        return output.str();
    }



    //////////



    int getNumFighters() const
    {
        return numFighters;
    }

    int getCapacity() const
    {
        return maxBays;
    }

    const std::string& getName() const
    {
        return name;
    }
};




/// <summary>
/// 
/// </summary>
/// <param name="name"></param>
void cleanName(std::string& name)
{
    while (!name.empty())
    {
        char last = name[name.size() - 1];

        if (last == ' ' || last == '\t' ||
            last == '\r' || last == '\n')
        {
            name.erase(name.size() - 1);
        }
        else
        {
            break;
        }
    }
}






Carrier* readCarrier(std::istream& input)
{
    std::string carrierName;

    if (!std::getline(input >> std::ws, carrierName))
    {
        return nullptr;
    }

    cleanName(carrierName);

    if (carrierName.empty())
    {
        return nullptr;
    }

    int capacity;
    int fighterCount;

    if (!(input >> capacity >> fighterCount))
    {
        return nullptr;
    }

    if (capacity < 0 || fighterCount < 0)
    {
        return nullptr;
    }

    Carrier* carrier = new Carrier(carrierName, capacity);

    for (int i = 0; i < fighterCount; i++)
    {
        std::string fighterName;
        int maxHealth;
        int damage;

        if (!std::getline(input >> std::ws, fighterName))
        {
            delete carrier;
            return nullptr;
        }

        cleanName(fighterName);

        if (fighterName.empty() ||
            !(input >> maxHealth >> damage))
        {
            delete carrier;
            return nullptr;
        }

        if (maxHealth <= 0 || damage <= 0)
        {
            delete carrier;
            return nullptr;
        }

        Fighter* fighter =
            new Fighter(fighterName, maxHealth, damage);

        if (!carrier->loadFighter(fighter))
        {
            std::cout << "Warning: " << fighterName
                << " could not be loaded onto "
                << carrier->getName()
                << " (no free bays).\n";

            delete fighter;
        }
    }

    return carrier;
}






//  random number from 1 to sides
int rollDie(std::mt19937& rng, int sides)
{
    std::uniform_int_distribution<int> dist(1, sides);

    return dist(rng);
}




// true if the defender is destroyed
bool attack(Fighter& attacker, Fighter& defender,
    int round, std::mt19937& rng)
{
    int hitRoll = rollDie(rng, 100);

    // Check if the attack misses
    if (hitRoll < HIT_THRESHOLD)
    {
        std::cout << "[R" << round << "] "
            << attacker.getName()
            << " misses "
            << defender.getName() << "\n";

        return false;
    }




    int damage = rollDie(rng, attacker.getDamage());

    defender.reduceStructure(damage);



    // Print the attack result
    std::cout << "[R" << round << "] "
        << attacker.getName()
        << " hits "
        << defender.getName()
        << " for " << damage
        << " (" << defender.getName() << ": "
        << defender.getStructStrength() << "/"
        << defender.getMaxStructStrength() << ")\n";

    return defender.isDestroyed();
}









Fighter* duel(Fighter* f1, Fighter* f2, std::mt19937& rng)
{
    int round = 1;

    while (!f1->isDestroyed() && !f2->isDestroyed())
    {
        Fighter* first;
        Fighter* second;

        // Randomly choose who attacks first
        int coinFlip = rollDie(rng, 2);

        if (coinFlip == 1)
        {
            first = f1;
            second = f2;
        }
        else
        {
            first = f2;
            second = f1;
        }

        // First attack
        if (attack(*first, *second, round, rng))
        {
            std::cout << "BOOOOOMMMM "
                << second->getName()
                << " destroyed!!!!!\n";

            delete second;
            return first;
        }

        // Counterattack
        if (attack(*second, *first, round, rng))
        {
            std::cout << "BOOOOOMMMM "
                << first->getName()
                << " destroyed!!!!!\n";

            delete first;
            return second;
        }

        round++;
    }

    return nullptr;
}








//   battle between two Carriers
void battle(Carrier& c1, Carrier& c2, std::mt19937& rng)
{
    int duelCount = 0;

    std::cout << "\n BATTLE \n";

    while (c1.hasFighters() && c2.hasFighters())
    {
        // Sort by current strength
        c1.insertionSortByStrength();
        c2.insertionSortByStrength();

        // Launch strongest Fighters
        Fighter* f1 = c1.launchNextFighter();
        Fighter* f2 = c2.launchNextFighter();

        duelCount++;

        std::cout << "\n--- Duel " << duelCount
            << ": " << f1->getName()
            << " (" << c1.getName() << ") vs "
            << f2->getName()
            << " (" << c2.getName() << ") ---\n";

        // Fight and get the survivor
        Fighter* winner = duel(f1, f2, rng);

        // Determine the winner's Carrier
        Carrier* home;

        if (winner == f1)
        {
            home = &c1;
        }
        else
        {
            home = &c2;
        }

        // Return survivor to its Carrier
        std::cout << winner->getName()
            << " returns to " << home->getName()
            << " with "
            << winner->getStructStrength()
            << "/"
            << winner->getMaxStructStrength()
            << " structure.\n";

        if (!home->loadFighter(winner))
        {
            std::cout << "Error: Cannot reload Fighter.\n";
            delete winner;
            return;
        }

        // Repair both Carriers after every duel
        c1.repairAllFighters();
        c2.repairAllFighters();
    }



    // the winning Carrier
    if (c1.hasFighters())
    {
        std::cout << "\n*** " << c1.getName()
            << " wins the battle after "
            << duelCount << " duels with "
            << c1.getNumFighters()
            << " fighter(s) remaining. ***\n";
    }
    else if (c2.hasFighters())
    {
        std::cout << "\n*** " << c2.getName()
            << " wins the battle after "
            << duelCount << " duels with "
            << c2.getNumFighters()
            << " fighter(s) remaining. ***\n";
    }
    else
    {
        std::cout << "\n*** No winner. ***\n";
    }
}









int main()
{

    std::random_device seed;
    std::mt19937 rng(seed());


    std::ifstream input("shipData.txt");

    if (!input)
    {
        std::cout << "Error: Cannot open shipData.txt\n";
        return 1;
    }

  
    Carrier* carrier1 = readCarrier(input);

    if (carrier1 == nullptr)
    {
        std::cout << "Error reading first Carrier.\n";
        return 1;
    }

    Carrier* carrier2 = readCarrier(input);

    if (carrier2 == nullptr)
    {
        std::cout << "Error reading second Carrier.\n";
        delete carrier1;
        return 1;
    }

    input.close();

    
    carrier1->mergeSortByName();
    carrier2->mergeSortByName();

    std::cout << "\n AFTER LAUNCH\n";

    std::cout << carrier1->getInfo() << "\n";
    std::cout << carrier2->getInfo() << "\n";

    // Simulate the battle
    battle(*carrier1, *carrier2, rng);

    // Final reports alphabetically 
    carrier1->mergeSortByName();
    carrier2->mergeSortByName();

    std::cout << "\n FINAL STATE \n";

    std::cout << carrier1->getInfo() << "\n";
    std::cout << carrier2->getInfo() << "\n";

  
    delete carrier1;
    delete carrier2;

    return 0;
}
