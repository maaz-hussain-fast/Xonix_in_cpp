Xonix: A Classic Arcade Remake in C++

--- A Maaz & Hussain Production ---

This project recreates the classic Xonix arcade game, delivering a fast-paced, grid-based experience built entirely in C++ using SFML, without relying on modern OOP constructs. Designed as a final project for Programming Fundamentals, this game integrates progressively challenging mechanics, multiplayer features, and a dynamic scoring system — all while pushing the boundaries of procedural programming.

The catch: No classes, structs, or STL containers like vector or list were allowed! The entire logic is built using arrays, pointers, and functions, showcasing how strong gameplay design is possible with just the basics.

🎮 Gameplay Overview
In Xonix, the player navigates a grid, aiming to capture territory while avoiding enemies. As the player draws trails to enclose areas, any collision with enemies or invalid moves (like reversing direction while building) results in death. Strategy, timing, and precision are key!

🚀 Features
🧱 Core Mechanics
Player can move in four directions: Up, Down, Left, Right

Movement rules include:

No reverse direction allowed while building tiles.

Enemy collision with incomplete tile trails = instant death.

Two player support with competitive interaction on a single board.

🕹 Game Modes
Single Player and Two Player support.

Start Menu:

Start Game

Select Difficulty

View Scoreboard

End Menu:

Final Score display (highlighted if it's a new high score)

Options: Restart, Main Menu, Exit

⚔️ Difficulty Levels
Easy – 2 enemies

Medium – 4 enemies

Hard – 6 enemies

Continuous Mode – Starts with 2 enemies; 2 more added every 20 seconds

👾 Enemy AI
Speed increases every 20 seconds.

After 30 seconds, half of the enemies adopt geometric movement patterns like:

Zig-zag

Circular paths

Each pattern is modularly implemented in separate, well-commented functions.

🧮 Movement & Score Tracking
Move counter tracks how many times a player starts building.

Timer shows elapsed game time.

Scoring system:

1 point per tile captured

×2 points for >10 tiles in a move (later reduced to 5 based on bonuses)

×4 points after higher combo thresholds

Power-Ups unlocked at 50, 70, 100, 130...:

Freeze enemies (and opponent, in 2-player mode) for 3 seconds

Stored and stacked in inventory

🏆 Scoreboard
Stores Top 5 scores (score + time)

Scoreboard updated automatically upon game over

Stored in an external .txt file

👥 Two Player Mode
Shared game board, with:

Arrow Keys for Player 1

WASD for Player 2

Interactive collision mechanics between players:

If both players collide while constructing tiles, both die.

Touching another player’s tile = death.

Power-ups affect both enemies and the opponent

💾 Setup and Usage
🛠 Prerequisites
SFML installed

C++ compiler (supporting C++17 or later)

Linux-based build system recommended

⚙️ Build Instructions (Linux)
bash
Copy
Edit
sudo apt update
sudo apt install cmake build-essential libsfml-dev make
git clone https://github.com/your_username/xonix-cpp.git
cd xonix-cpp
mkdir build && cd build
cmake ..
make
./xonix
🧠 Implementation Highlights
Dynamic Memory: Manages enemy behaviors, scoring logic, and power-up inventory.

Procedural Architecture: Each functionality (movement, collision, scoring, rendering) modularized using functions.

File I/O: Handles scoreboard read/write securely.

Strict Constraints: No STL containers, no OOP — just arrays, loops, and pure logic!

🔊 Bonus Features
Sound effects for tile capture, scoring bonuses, and power-up activation.

Background color changes upon power-up usage for visual feedback.

🧪 Challenges Overcome
Building a complete game without any object-oriented paradigms pushed the team to think creatively with core constructs. From managing enemy AI patterns to interactive two-player collisions, this project demonstrates how even procedural C++ can yield professional-grade results under tight constraints.

🛠 Future Enhancements
Implement customizable control mappings

Add animations and improved graphics

Introduce new enemy types with smarter AI

🙏 Acknowledgements
Inspired by the original Xonix arcade game (1984)

Starter code base from: https://github.com/embeddedmz/16-Games
