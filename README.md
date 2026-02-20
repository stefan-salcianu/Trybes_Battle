# Trybe World Management System 

A C++ console-based RPG and simulation game that models a world of battling tribes. This project serves as a showcase of advanced **Object-Oriented Programming (OOP)**, memory management, and modern C++ practices.

##  Gameplay Features
* **Build Tribes:** Create and manage tribes (`Trybe`) using a custom template-based `Repository`.
* **Recruit Members:** Add specific classes like **Executioner** (Attack), **Healer** (Support), and **Leader** (AoE Buffs).
* **Equip Magic Items:** Use items like `Amulet` (protection) and `Totem` (power boosts) during fights.
* **The Riot:** Engage in turn-based, multi-round battles between tribes until only one survives!

##  Tech Stack & C++ Concepts
* **Modern C++ (C++17):** Safe memory management using `std::shared_ptr`.
* **OOP Core:** Inheritance, Runtime Polymorphism (virtual functions), and Encapsulation.
* **Design Patterns:** Singleton Pattern (`TrybeWorld`) and Factory-like object creation.
* **STL Containers:** Heavy use of `std::vector`, `std::stack`, and `std::map` (for global kill stats).
* **Advanced C++:** Custom Exceptions (`MyException2`), Template classes, and Operator Overloading (`<<`, `>>`, `==`, etc.).

##  How to Run
Compile the game using a modern C++ compiler (requires C++11 or higher):
```bash
g++ -std=c++17 main.cpp -o trybeworld
./trybeworld
