#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <memory>
#include <random>
#include <stack>
#include <ctime>
#include <map>
#include <exception>

#define RED     "\033[31m"
#define RESET   "\033[0m"

using namespace std;

class MyException2 : public exception {
private:
    string message = "Error: ";
public:
    MyException2(const string& message = "") {
        this->message = string(RED) + message + RESET;
    }
    const char* what() {
        return (this->message).c_str();
    }
};

// Forward declarations for member subclasses
class Executioner;
class Healer;
class Leader;
class Trybe;
class Member;



class Member {
protected:
    const string name;
    const int age;
    int level = 1;
    static int memberCount;
    static inline map<string, int> killStats;
public:
    virtual void setLevel(int level) { this->level = level; }
    virtual int getLevel() { return this->level; }
    static int getMemberCount() { return memberCount; }
    const string getName() const { return this->name; }
    int getAge() const { return this->age; }

    static void recordKill(const string &killerName) {
        killStats[killerName]++;
    }

    static void printKillStats() {
        cout << "\n--- Kill Statistics ---\n";
        for (const auto& pair : killStats) {
            cout << pair.first << " has " << pair.second << " kills\n";
        }
    }

    virtual void action(Member &m) = 0;

    Member(const string _name, const int _age) : name(_name), age(_age) {
        memberCount++;
    }
    virtual ~Member() {
        memberCount--;
    }

    Member& operator=(const Member& m) {
        if (this != &m) {
            this->level = m.level;
        }
        cout << "Membrii implicati au acum levelul: " << this->level << "\n";
        return *this;
    }

    friend ostream& operator<<(ostream& os, const Member& member);
    friend bool operator==(const Member& lhs, const Member& rhs) {
        return lhs.level == rhs.level;
    }
    friend bool operator!=(const Member& lhs, const Member& rhs) {
        return !(lhs == rhs);
    }
    friend bool operator<(const Member& lhs, const Member& rhs) {
        return lhs.level < rhs.level;
    }
    friend bool operator<=(const Member& lhs, const Member& rhs) {
        return (lhs < rhs || lhs == rhs);
    }
    friend bool operator>(const Member& lhs, const Member& rhs) {
        return rhs < lhs;
    }
    friend bool operator>=(const Member& lhs, const Member& rhs) {
        return !(lhs < rhs);
    }

    static shared_ptr<Member> createMember(istream& is);
};

int Member::memberCount = 0;

shared_ptr<Member> Member::createMember(istream& is) {
    string name;
    int age, type;
    cout << "Enter member name: ";
    is.ignore();
    getline(is, name);
    cout << "Enter member age: ";
    is >> age;

    cout << "Select member type:" << endl;
    cout << "1. Executioner" << endl;
    cout << "2. Healer" << endl;
    cout << "3. Leader" << endl;
    cout << "Enter choice: ";
    is >> type;

    switch (type) {
        case 1:
            return static_pointer_cast<Member>(make_shared<Executioner>(name, age));
        case 2:
            return static_pointer_cast<Member>(make_shared<Healer>(name, age));
        case 3:
            return static_pointer_cast<Member>(make_shared<Leader>(name, age));
        default:
            cout << "Invalid member type." << endl;
            return nullptr;
    }
}

ostream& operator<<(ostream& os, const Member& member) {
    os << "Name: " << member.name << ", Age: " << member.age << ", Level: "
       << member.level << ", Type: " << typeid(member).name();
    return os;
}

istream& operator>>(istream& is, Member& member) {
    string name;
    int age, type;
    cout << "Enter member name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter member age: ";
    cin >> age;

    cout << "Select member type:" << endl;
    cout << "1. Executioner" << endl;
    cout << "2. Healer" << endl;
    cout << "3. Leader" << endl;
    cout << "Enter choice: ";
    cin >> type;

    return is;
}
class Item {
protected:
    int value;
    int power;
public:
    Item(int _value, int _power) : value(_value), power(_power) {}
    virtual void ability(vector<shared_ptr<Member>> &members) = 0;
    virtual ~Item() {}
    int getValue() const { return value; }
    int getPower() const { return power; }
};
template <typename T>
class Repository {
private:
    vector<T*> items;
    string repositoryName;
public:
    Repository(const string& name = "Default Repository") : repositoryName(name) {}
    ~Repository() {
        items.clear();
    }
    void add(T* item) {
        if (item != nullptr) {
            items.push_back(item);
        }
    }
    bool remove(T* item) {
        auto it = find(items.begin(), items.end(), item);
        if (it != items.end()) {
            items.erase(it);
            return true;
        }
        return false;
    }
    bool removeAt(size_t index) {
        if (index < items.size()) {
            items.erase(items.begin() + index);
            return true;
        }
        return false;
    }
    T* getAt(size_t index) {
        if (index < items.size()) {
            return items[index];
        }
        return nullptr;
    }
    const vector<T*>& getAll() const {
        return items;
    }
    string getName() const {
        return repositoryName;
    }
    void setName(const string& name) {
        repositoryName = name;
    }
    size_t count() const {
        return items.size();
    }
    void printAll() const {
        cout << "Repository: " << repositoryName << " (" << items.size() << " items)" << endl;
        int i = 0;
        for (auto& w : items) {
            cout << ++i << typeid(*w).name() << endl;
        }
    }
};

class Amulet : public Item {
public:
    Amulet(int _value, int _power) : Item(_value, _power) {}
    void ability(vector<shared_ptr<Member>> &members) override {
        cout << "Amulet does smth!";
    }
};

class Totem : public Item {
public:
    Totem(int _value, int _power) : Item(_value, _power) {}
    void ability(vector<shared_ptr<Member>> &members) override {
        cout << "Totem does smth!"<<endl;
            // vector<shared_ptr<Executioner>> execs;
            // for (auto& m : members) {
            //     shared_ptr<Executioner> ex = dynamic_pointer_cast<Executioner>(m);
            //     if (ex) execs.push_back(ex);
            // }
            // if (!execs.empty()) {
            //     srand(static_cast<unsigned>(time(nullptr)));
            //     int idx = rand() % execs.size();
            //     shared_ptr<Executioner> ex = execs[idx];
            //     int increaseAmount = this->getPower();
            //     int oldLevel = ex->getLevel();
            //     ex->setLevel(oldLevel + increaseAmount);
            //     cout << "The Totem glows, empowering Executioner " << ex->getName()
            //          << "! Their level increases from " << oldLevel << " to " << ex->getLevel() << "." << endl;
            // } else {
            //     cout << "No Executioner in the trybe to empower with the Totem." << endl;
            // }
    }
};

class Trybe {
private:
    int trybe_id = 0;
    string trybe_name;
    vector<shared_ptr<Member>> people;
    Repository<Item> repo;
public:
    Trybe() : trybe_name("Default"), repo("Default Trybe Repository") {
        trybe_id++;
    }
    Trybe(string name) : trybe_name(name), repo(name + "'s Item Repository") {
        trybe_id++;
    }
    Trybe(const Trybe& other) : trybe_name(other.trybe_name), repo(other.repo.getName()) {
        for (const auto& member : other.people) {
            people.push_back(member);
        }
        cout << "Trybe copy constructor called for " << trybe_name << endl;
    }
    Trybe& operator=(const Trybe& other) {
        if (this != &other) {
            trybe_id = other.trybe_id;
            trybe_name = other.trybe_name;
            people.clear();
            for (const auto& member : other.people) {
                people.push_back(member);
            }
        }
        cout << "Trybe assignment operator called for " << trybe_name << endl;
        return *this;
    }
    ~Trybe() {
        cout << "Trybe destructor called for " << trybe_name << endl;

        for (int i = repo.count() - 1; i >= 0; --i) {
            Item* item = repo.getAt(i);
            repo.removeAt(i);
            delete item;
        }
    }
    void addMember(shared_ptr<Member> m) {
        people.push_back(m);
    }
    void addItem(Item* item) {
        repo.add(item);
    }
    void removeItemAt(int index) {
        if (index >= 0 && index < (int)repo.count()) {
            Item* item = repo.getAt(index);
            if (item != nullptr) {
                repo.removeAt(index);
                delete item;
            }
        }
    }
    void printAllMembers() const {
        cout << "Trybe " << trybe_name << " has the following members:\n";
        for (const auto& member : people) {
            cout << " - " << *member << "\n";
        }
    }
    void printAllItems() const {
        cout << "Trybe " << trybe_name << " has the following items:\n";
        const auto& items = repo.getAll();
        if (items.empty()) {
            cout << " - No items\n";
        } else {
            for (Item* item : items) {
                string typeName;
                if (dynamic_cast<Amulet*>(item)) typeName = "Amulet";
                else if (dynamic_cast<Totem*>(item)) typeName = "Totem";
                else typeName = "Item";
                cout << " - " << typeName
                     << " (Value: " << item->getValue() << ", Power: " << item->getPower() << ")\n";
            }
        }
    }
    const vector<shared_ptr<Member>>& getMembers() const {
        return people;
    }
    int getItemCount() const {
        return repo.count();
    }
    Item* getItemAt(int index) {
        return repo.getAt(index);
    }
    void removeMemberAt(int index) {
        if (index >= 0 && index < (int)people.size()) {
            people.erase(people.begin() + index);
        }
    }
    int getTrybeId() const { return trybe_id; }
    string getTrybeName() const { return trybe_name; }

    friend ostream& operator<<(ostream& os, const Trybe& trybe);
    friend istream& operator>>(istream& is, Trybe& trybe);
    friend bool operator>(const Trybe& t1, const Trybe& t2) {
        return t1.getMembers().size() > t2.getMembers().size();
    }
    friend bool operator==(const Trybe& t1, const Trybe& t2) {
        return t1.getMembers().size() == t2.getMembers().size();
    }
    friend bool operator<(const Trybe& t1, const Trybe& t2) {
        return t1.getMembers().size() < t2.getMembers().size();
    }
    friend bool operator<=(const Trybe& t1, const Trybe& t2) {
        return t1.getMembers().size() <= t2.getMembers().size();
    }
    friend bool operator>=(const Trybe& t1, const Trybe& t2) {
        return t1.getMembers().size() >= t2.getMembers().size();
    }
    friend bool operator!=(const Trybe& t1, const Trybe& t2) {
        return t1.getMembers().size() != t2.getMembers().size();
    }

    void setMemberLevel() {
        auto& members = getMembers();
        if (members.empty()) {
            throw MyException2("Empty trybe");
        }
        cout << "Members in the trybe:" << endl;
        for (size_t i = 0; i < members.size(); i++) {
            cout << i + 1 << ". " << *members[i] << endl;
        }
        int memberIndex, newLevel;
        cout << "Select member number: ";
        cin >> memberIndex;
        if (memberIndex < 1 || memberIndex > (int)members.size()) {
            throw MyException2("Invalid number selection: " + to_string(memberIndex) + " Please select an actual member!");
        }
        cout << "Enter new level: ";
        cin >> newLevel;
        members[memberIndex - 1]->setLevel(newLevel);
        cout << "Level updated successfully." << endl;
    }
};

ostream& operator<<(ostream& os, const Trybe& trybe) {
    os << "Trybe ID: " << trybe.trybe_id << ", Name: " << trybe.trybe_name;
    os << "\nNumber of members: " << trybe.people.size();
    if (!trybe.people.empty()) {
        os << "\nMembers:";
        int cnt = 0;
        for (const auto& member : trybe.people) {
            os << "\n - ";
            if (member) {
                os << ++cnt << "-  Type: " << typeid(*member).name() << ", ";
                os << "  Name: " << member->getName() << ", Level: " << member->getLevel() << "\n\n";
            } else {
                os << "nullptr";
            }
        }
    }
    return os;
}

istream& operator>>(istream& is, Trybe& trybe) {
    cout << "Enter Trybe ID: ";
    is >> trybe.trybe_id;
    is.ignore();
    cout << "Enter Trybe Name: ";
    getline(is, trybe.trybe_name);
    return is;
}

class Executioner : public Member {
public:
    Executioner(const string& _name, int _age) : Member(_name, _age) {}

    void setLevel(int level) override {
        Member::setLevel(level);
        cout << "You setted the level of Executioner with id: " << memberCount << " to: " << level << "\n";
    }

    void action(Member &m) override {
        cout << "Executioner action" << endl;
        cout << "Executioner " << name << " is killing " << m.getName() << endl;
        Member::recordKill(name);
        level += 10;
        cout << getName() << "'s new level: " << getLevel() << endl;
    }

    void randomKill(shared_ptr<Trybe> latestTrybe) {
        if (!latestTrybe) {
            cout << "No trybes in the world!\n";
            return;
        }
        auto& members = latestTrybe->getMembers();
        if (members.empty()) {
            cout << "The last trybe has no members!\n";
            return;
        }
        srand(static_cast<unsigned>(time(nullptr)));
        cout << "These are the members available in the tribe!" << endl << endl;
        for (auto& w : members) {
            cout << typeid(*w).name() << " " << w->getName() << endl;
        }
        cout << endl;
        int idx = rand() % members.size();
        if (dynamic_pointer_cast<Leader>(members[idx]) != nullptr) {
            throw MyException2("Executioner can't delete the Leader, abort kill!");
        } else {
            cout << "Executioner - " << this->getName() << " randomly executed member of type: "
                 << typeid(*members[idx]).name() << "-" << members[idx]->getName()
                 << " (level " << members[idx]->getLevel() << ")\n";
            Member::recordKill(name);
            int boost = 5;
            cout << "Level + " << boost << endl;
            this->level += 5;
            cout << "Executioner - " << this->name << " has the level: " << this->level << endl;
            latestTrybe->removeMemberAt(idx);
            cout << "Members remaining: " << latestTrybe->getMembers().size() << endl;
            cout << "These are the members remaining: " << endl;
            for (auto& w : members) {
                cout << typeid(*w).name() << " " << w->getName() << endl;
            }
        }
    }
};

class Healer : public Member {
public:
    Healer(const string& _name, int _age) : Member(_name, _age) {}

    void setLevel(int level) override {
        Member::setLevel(level);
        cout << "You setted the level of Healer with id: " << memberCount << " to: " << level << "\n";
    }

    void action(Member &m) override {
        cout << "Healer " << getName() << " is healing " << m.getName() << endl;
        cout << "Increasing " << m.getName() << "'s level by 5" << endl;
        m.setLevel(m.getLevel() + 5);
        cout << m.getName() << "'s new level: " << m.getLevel() << endl;
    }
};

class Leader : public Member {
public:
    Leader(const string& _name, int _age) : Member(_name, _age) {}

    void action(Member &m) override {
        int newLevel = m.getLevel() + 30;
        m.setLevel(newLevel);
        cout << m.getName() << "'s level increased to " << newLevel << "!" << endl;
    }
};





class TrybeWorld {
private:
    stack<shared_ptr<Trybe>> trybes;
    TrybeWorld() {}
    TrybeWorld(const TrybeWorld&) = delete;
    TrybeWorld& operator=(const TrybeWorld&) = delete;
public:
    static TrybeWorld& getInstance() {
        static TrybeWorld instance;
        return instance;
    }
    void addTrybe(shared_ptr<Trybe> t) {
        trybes.push(t);
    }
    shared_ptr<Trybe> getLastTrybe() {
        if (!trybes.empty())
            return trybes.top();
        return nullptr;
    }
    const stack<shared_ptr<Trybe>>& getAllTrybes() const {
        return trybes;
    }
    void init() {
        shared_ptr<Trybe> latest = getLastTrybe();
        if (!latest) {
            throw MyException2("No trybes left!");
        }
        auto& members = latest->getMembers();
        for (auto& m : members) {
            shared_ptr<Executioner> ex = dynamic_pointer_cast<Executioner>(m);
            if (ex) {
                cout << "Found an Executioner. Initiating random kill.\n";
                ex->randomKill(latest);
                return;
            }
        }
        cout << "No Executioner found in the last Trybe.\n";
    }
    void displayAllMembers() {
        shared_ptr<Trybe> lastTrybe = getLastTrybe();
        if (lastTrybe) {
            lastTrybe->printAllMembers();
        } else {
            cout << "No trybes available." << endl;
        }
    }
    void createNewTrybe() {
        shared_ptr<Trybe> newTrybe = make_shared<Trybe>();
        cin >> *newTrybe;
        addTrybe(newTrybe);
        int choice, result = 1;
        cout << "If you want to add members to the trybe, press 1" << endl;
        cout << "Press 0 to add trybe empty!" << endl;
        while (result != 0) {
            cin >> choice;
            switch (choice) {
                case 1:
                    addMemberToLastTrybe();
                    break;
                default:
                    result = 0;
                    break;
            }
            cout << "If you want to keep adding members to the trybe, press 1" << endl;
            cout << "If you want to stop, press any key!" << endl;
        }
        cout << "New trybe created and added to the world." << endl;
    }
    void addMemberToLastTrybe() {
        shared_ptr<Trybe> lastTrybe = getLastTrybe();
        if (!lastTrybe) {
            cout << "No trybes available. Create a trybe first." << endl;
            return;
        }
        shared_ptr<Member> newMember = Member::createMember(cin);
        if (newMember) {
            int level;
            cout << "What should the level of this member be?" << endl;
            cout << "Level: ";
            cin >> level;
            newMember->setLevel(level);
            lastTrybe->addMember(newMember);
            cout << "New member added to the last trybe." << endl;
        }
    }
    void setMemberLevel() {
        shared_ptr<Trybe> lastTrybe = getLastTrybe();
        if (!lastTrybe) {
            cout << "No trybes available." << endl;
            return;
        }
        lastTrybe->setMemberLevel();
    }
    void displayTrybeInfo() {
        shared_ptr<Trybe> lastTrybe = getLastTrybe();
        if (lastTrybe) {
            cout << *lastTrybe << endl;
        } else {
            cout << "No trybes available." << endl;
        }
    }
    void updateAnyTrybeInfo() {
        if (trybes.empty()) {
            cout << "No trybes available." << endl;
            return;
        }
        vector<shared_ptr<Trybe>> allTrybes;
        stack<shared_ptr<Trybe>> tempStack = trybes;
        while (!tempStack.empty()) {
            allTrybes.push_back(tempStack.top());
            tempStack.pop();
        }
        cout << "Available trybes:" << endl;
        for (int i = 0; i < (int)allTrybes.size(); i++) {
            cout << i + 1 << ". " << allTrybes[i]->getTrybeName() << endl;
        }
        int choice;
        cout << "Select trybe to update (1-" << allTrybes.size() << "): ";
        cin >> choice;
        if (choice < 1 || choice > (int)allTrybes.size()) {
            cout << "Invalid selection." << endl;
            return;
        }
        cin >> *allTrybes[choice - 1];
        cout << "Trybe information updated successfully." << endl;
        allTrybes[choice - 1]->printAllMembers();
    }
    void updateTrybeInfo() {
        shared_ptr<Trybe> lastTrybe = getLastTrybe();
        if (lastTrybe) {
            cin >> *lastTrybe;
            cout << "Trybe information updated successfully." << endl;
        } else {
            cout << "No trybes available." << endl;
        }
    }
    void updateTrybe(vector<shared_ptr<Trybe>> t) {
        while (!trybes.empty()) {
            trybes.pop();
        }
        for (auto& w : t) {
            trybes.push(w);
        }
    }
    void riot() {
        vector<shared_ptr<Trybe>> activeTrybes;
        stack<shared_ptr<Trybe>> copyStack = trybes;
        while (!copyStack.empty()) {
            activeTrybes.push_back(copyStack.top());
            copyStack.pop();
        }
        if (activeTrybes.size() < 2) {
            cout << "Not enough trybes to riot!!" << endl;
            return;
        }
        cout << "Choose which trybe goes first:" << "\n\n\n";
        int cnt = 0;
        for (auto& w : activeTrybes) {
            cout << ++cnt << "  " << w->getTrybeName() << endl;
        }
        cout << "Choose from (1-" << activeTrybes.size() << ")\n\n";
        int firstTrybeIndex;
        cin >> firstTrybeIndex;
        bool chosen = false;
        while (!chosen) {
            if (firstTrybeIndex < 1 || firstTrybeIndex > (int)activeTrybes.size()) {
                cout << "Choose a valid trybe!" << "\n\n";
                cin >> firstTrybeIndex;
            } else {
                chosen = true;
            }
        }
        firstTrybeIndex--;
        cout << "Choose which trybe to fight against:" << "\n\n\n";
        cnt = 0;
        for (auto& w : activeTrybes) {
            if (cnt != firstTrybeIndex) {
                cout << ++cnt << "  " << w->getTrybeName() << endl;
            } else {
                cnt++;
            }
        }
        int secondTrybeIndex;
        cout << "Choose from (1-" << activeTrybes.size() << ")\n\n";
        chosen = false;
        cin >> secondTrybeIndex;
        while (!chosen) {
            if (secondTrybeIndex < 1 || secondTrybeIndex > (int)activeTrybes.size() ||
                secondTrybeIndex == firstTrybeIndex + 1) {
                cout << "Choose a valid trybe!" << "\n\n";
                cin >> secondTrybeIndex;
            } else {
                chosen = true;
            }
        }
        secondTrybeIndex--;
        // Main battle loop
        bool battleContinues = true;
        int currentAttacker = firstTrybeIndex;
        int currentDefender = secondTrybeIndex;
        int rounds = 1;
        cout << "\n=== BATTLE BEGINS ===" << endl;
        vector<bool> trumpCard(2, false);
        while (battleContinues) {
            // Check if either trybe has no members left
            if (activeTrybes[firstTrybeIndex]->getMembers().empty()) {
                cout << "\n" << activeTrybes[secondTrybeIndex]->getTrybeName() << " WINS!" << endl;
                cout << activeTrybes[firstTrybeIndex]->getTrybeName() << " has no members left." << endl;
                battleContinues = false;
                break;
            }
            if (activeTrybes[secondTrybeIndex]->getMembers().empty()) {
                cout << "\n" << activeTrybes[firstTrybeIndex]->getTrybeName() << " WINS!" << endl;
                cout << activeTrybes[secondTrybeIndex]->getTrybeName() << " has no members left." << endl;
                battleContinues = false;
                break;
            }
            int option;
            // Display current turn
            cout << "\n=== " << activeTrybes[currentAttacker]->getTrybeName() << "'s TURN ===" << endl;
            vector<shared_ptr<Member>> attackingMembers = activeTrybes[currentAttacker]->getMembers();

            cout << "Choose which member will act:" << endl;
            for (int i = 0; i < (int)attackingMembers.size(); i++) {
                cout << i + 1 << ". " << attackingMembers[i]->getName()
                     << " (Level " << attackingMembers[i]->getLevel() << ")"
                     << " ( " << typeid(*attackingMembers[i]).name() << " )" << endl;
            }
            cout<<"If you want to use an Item, press 100"<<endl;
            int memberIndex;
            cout << "Choose from (1-" << attackingMembers.size() << "): ";
            cin >> memberIndex;
            if(memberIndex==100){
                cout<<"These are the Items available for this trybe!";
                int itemCount = activeTrybes[currentAttacker]->getItemCount();
            if (itemCount == 0) {
                cout << "No items available in " << activeTrybes[currentAttacker]->getTrybeName() << "'s repository." << endl;
                return;
            }
        cout << "Available items in " << activeTrybes[currentAttacker]->getTrybeName() << ":\n";
        for (int i = 0; i < itemCount; ++i) {
            Item* item = activeTrybes[currentAttacker]->getItemAt(i);
            string typeName;
            if (dynamic_cast<Amulet*>(item)) typeName = "Amulet";
            else if (dynamic_cast<Totem*>(item)) typeName = "Totem";
            else typeName = "Unknown Item";
            cout << i + 1 << ". " << typeName
                 << " (Value: " << item->getValue() << ", Power: " << item->getPower() << ")" << endl;
        }
        cout << "Choose an item to use (1-" << itemCount << "): ";
        int choice;
        cin >> choice;
        while (choice < 1 || choice > itemCount) {
            cout << "Invalid choice. Choose a number between 1 and " << itemCount << ": ";
            cin >> choice;
        }
        Item* chosenItem = activeTrybes[currentAttacker]->getItemAt(choice - 1);
        if (!chosenItem) {
            cout << "Invalid item selection." << endl;
            return;
        }
        Amulet* amulet = dynamic_cast<Amulet*>(chosenItem);
        Totem* totem = dynamic_cast<Totem*>(chosenItem);
        if (amulet) {
            // Amulet effect: protect a random member from being eliminated
            vector<shared_ptr<Member>> members = activeTrybes[currentAttacker]->getMembers();
            if (!members.empty()) {
                srand(static_cast<unsigned>(time(nullptr)));
                int idx = rand() % members.size();
                shared_ptr<Member> memberToProtect = members[idx];
                cout << "An Executioner strikes at " << memberToProtect->getName()
                     << ", but the Amulet protects them! " << memberToProtect->getName()
                     << " survives the attack." << endl;
            } else {
                cout << "There are no members to protect with the Amulet." << endl;
            }
        } else if (totem) {
            // Totem effect: increase level of a random Executioner
            vector<shared_ptr<Member>> members = activeTrybes[currentAttacker]->getMembers();
            vector<shared_ptr<Executioner>> execs;
            for (auto& m : members) {
                shared_ptr<Executioner> ex = dynamic_pointer_cast<Executioner>(m);
                if (ex) execs.push_back(ex);
            }
            if (!execs.empty()) {
                srand(static_cast<unsigned>(time(nullptr)));
                int idx = rand() % execs.size();
                shared_ptr<Executioner> ex = execs[idx];
                int increaseAmount = chosenItem->getPower();
                int oldLevel = ex->getLevel();
                ex->setLevel(oldLevel + increaseAmount);
                cout << "The Totem glows, empowering Executioner " << ex->getName()
                     << "! Their level increases from " << oldLevel << " to " << ex->getLevel() << "." << endl;
            } else {
                cout << "No Executioner in the trybe to empower with the Totem." << endl;
            }
        } else {
            // Default effect for unknown item types
            cout << "The item is used, but nothing noticeable happens..." << endl;
        }
        // Remove the used item from the Trybe's repository
        activeTrybes[currentAttacker]->removeItemAt(choice - 1);
        cout << "The item has been consumed in the riot." << endl;
            }
            else{
                while (memberIndex < 1 || memberIndex > (int)attackingMembers.size()) {
                cout << "Invalid choice. Choose from (1-" << attackingMembers.size() << "): ";
                cin >> memberIndex;
            }
            shared_ptr<Member> actingMember = attackingMembers[memberIndex - 1];
            cout << "\n" << actingMember->getName() << " (" << typeid(*actingMember).name() << ") acts!" << endl;
            // Handle different member types
            shared_ptr<Healer> healer = dynamic_pointer_cast<Healer>(actingMember);
            shared_ptr<Executioner> executioner = dynamic_pointer_cast<Executioner>(actingMember);
            shared_ptr<Leader> leader = dynamic_pointer_cast<Leader>(actingMember);
            if (healer) {
                // Healer can heal a member of their own trybe
                cout << "Choose a member to heal:" << endl;
                for (int i = 0; i < (int)attackingMembers.size(); i++) {
                    cout << i + 1 << ". " << attackingMembers[i]->getName()
                         << " (Level " << attackingMembers[i]->getLevel() << ")" << endl;
                }
                int targetIndex;
                cout << "Choose from (1-" << attackingMembers.size() << "): ";
                cin >> targetIndex;
                while (targetIndex < 1 || targetIndex > (int)attackingMembers.size()) {
                    cout << "Invalid choice. Choose from (1-" << attackingMembers.size() << "): ";
                    cin >> targetIndex;
                }
                healer->action(*attackingMembers[targetIndex - 1]);
            } else if (executioner) {
                vector<shared_ptr<Member>> defendingMembers = activeTrybes[currentDefender]->getMembers();
                cout << "Choose a member to attack:" << endl;
                for (int i = 0; i < (int)defendingMembers.size(); i++) {
                    cout << i + 1 << ". " << defendingMembers[i]->getName()
                         << " (Level " << defendingMembers[i]->getLevel() << ")" << endl;
                }
                int targetIndex;
                cout << "Choose from (1-" << defendingMembers.size() << "): ";
                cin >> targetIndex;
                while (targetIndex < 1 || targetIndex > (int)defendingMembers.size()) {
                    cout << "Invalid choice. Choose from (1-" << defendingMembers.size() << "): ";
                    cin >> targetIndex;
                }
                if (executioner->getLevel() >= defendingMembers[targetIndex - 1]->getLevel()) {
                    executioner->action(*defendingMembers[targetIndex - 1]);
                    activeTrybes[currentDefender]->removeMemberAt(targetIndex - 1);
                    cout << "Member eliminated!" << endl;
                } else {
                    cout << "Attack failed! Executioner's level (" << executioner->getLevel()
                         << ") is lower than target's level ("
                         << defendingMembers[targetIndex - 1]->getLevel() << ")." << endl;
                }
            } else if (leader) {
                if (!trumpCard[rounds % 2]) {
                    cout << "Leader " << actingMember->getName() << " inspires the trybe!" << endl;
                    cout << "Everybody gets upgraded!!" << endl << endl;
                    for (int i = 0; i < (int)attackingMembers.size(); i++) {
                        leader->action(*attackingMembers[i]);
                    }
                    trumpCard[rounds % 2] = true;
                } else {
                    cout << "trump card already used!!";
                }
            }
            }
            
            updateTrybe(activeTrybes);
            if (!(activeTrybes[firstTrybeIndex]->getMembers().empty() ||
                  activeTrybes[secondTrybeIndex]->getMembers().empty())) {
                // Swap attacker and defender for next round
                int temp = currentAttacker;
                currentAttacker = currentDefender;
                currentDefender = temp;
                char continueChoice;
                cout << "\nContinue the battle? (y/n): ";
                cin >> continueChoice;
                if (tolower(continueChoice) != 'y') {
                    cout << "Battle ended by user." << endl;
                    battleContinues = false;
                }
                rounds++;
            }
        }
        cout << "\n=== BATTLE ENDS ===" << endl;
        updateTrybe(activeTrybes);
    }
};

void displayMenu() {
    int choice = 1;
    cout << "\n=== Trybe World Management System ===" << endl;
    cout << "\n=== Welcome Challenger! ===" << endl;
    while (choice != 0) {
        cout << "\n--- Menu Options ---" << endl;
        cout << "1. Eliminate a random member from the last trybe" << endl;
        cout << "2. Display all members in the last trybe" << endl;
        cout << "3. Create a new trybe" << endl;
        cout << "4. Add a new member to the last trybe" << endl;
        cout << "5. Set level for a member in the last trybe" << endl;
        cout << "6. Display detailed information about the last trybe" << endl;
        cout << "7. Update trybe information" << endl;
        cout << "8. Riot" << endl;
        cout << "9. Kill stats" << endl;
        cout << "0. Exit" << endl;
        cout << "\nEnter your choice: ";
        cin >> choice;
        TrybeWorld& world = TrybeWorld::getInstance();
        switch (choice) {
            case 1:
                try {
                    world.init();
                } catch (MyException2 &e) {
                    cout << e.what() << endl;
                }
                break;
            case 2:
                world.displayAllMembers();
                break;
            case 3:
                world.createNewTrybe();
                break;
            case 4:
                world.addMemberToLastTrybe();
                break;
            case 5:
                try {
                    world.setMemberLevel();
                } catch (MyException2 &e) {
                    cout << e.what() << endl;
                }
                break;
            case 6:
                world.displayTrybeInfo();
                break;
            case 7:
                world.updateAnyTrybeInfo();
                break;
            case 8:
                world.riot();
                break;
            case 9:
                Member::printKillStats();
                break;
            case 0:
                cout << "Exiting... Thank you for playing!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }
}

int main() {
    shared_ptr<Trybe> t1 = make_shared<Trybe>("Water");
    shared_ptr<Trybe> t2 = make_shared<Trybe>("Fire");
    TrybeWorld::getInstance().addTrybe(t1);
    TrybeWorld::getInstance().addTrybe(t2);

    shared_ptr<Member> m4 = make_shared<Executioner>("Alex", 30);
    shared_ptr<Member> m5 = make_shared<Executioner>("Gaby", 28);
    shared_ptr<Member> leader = make_shared<Leader>("Stefan", 20);
    shared_ptr<Member> healer = make_shared<Healer>("Teodora", 20);
    shared_ptr<Member> m1 = make_shared<Executioner>("Andrei", 30);
    shared_ptr<Member> m2 = make_shared<Healer>("Mirela", 28);
    shared_ptr<Member> leader1 = make_shared<Leader>("Ion", 20);
    shared_ptr<Member> healer1 = make_shared<Healer>("Alexandra", 20);

    m4->setLevel(10);
    m5->setLevel(15);
    leader->setLevel(30);
    healer->setLevel(110);
    m1->setLevel(100);
    m2->setLevel(70);
    leader1->setLevel(1000);
    healer1->setLevel(17);

    t1->addMember(m4);
    t1->addMember(m5);
    t1->addMember(leader);
    t1->addMember(healer);
    t2->addMember(m1);
    t2->addMember(m2);
    t2->addMember(leader1);
    t2->addMember(healer1);

    // Add some items to the second Trybe's repository for demonstration
    t2->addItem(new Amulet(1, 10));
    t2->addItem(new Amulet(15, 11));
    t2->addItem(new Totem(11, 21));

     t1->addItem(new Amulet(1, 10));
    t1->addItem(new Amulet(15, 11));
    t1->addItem(new Totem(11, 21));

    cout << "\n=== Items in Trybe " << t2->getTrybeName() << " ===" << endl;
    t2->printAllItems();

    displayMenu();
    return 0;
}
