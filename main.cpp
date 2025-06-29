//================================================ XONIX ===========================================================================
//======================================= A MAAZ & HUSSAIN PRODUCTION ===========================================================


#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
#include <fstream>
#include <iostream>
#include <cmath>
using namespace std;
using namespace sf;

const int M = 20; 
const int N = 40;
const int MAX_HIGH_SCORES = 5;

void circle(int& enemyCount);
void zigzag(int& enemyCount);


int grid[M][N] = {0};
int ts = 18; // tile size
const int statusBarHeight = 60; 

// Game status variables
int score = 0;
int moves = 0;
int moves2 = 0;
int level = 1;
float gameTime = 0;
float switchTime = 0;
int blocksConstructed = 0;
int  blocksConstructed2 = 0;
bool isNewHighScore = false;
bool isNewHighScore2 = false;
int bonusCounter = 0;
int powerUps = 0;
int powerUps2 = 0;
bool powerUpActive = false;
bool powerUpActive2 = false;
Clock powerUpClock;
Clock powerUpClock2;
bool powerKeyHeld = false;
bool isConstructing2 = false;
int previousBlockCount2 = 0;
int score2 = 0;
string playerName2 = "Player2";



struct PlayerScore {
    string playerName;
    int score;
    string difficulty;
    float timeTaken; 

    PlayerScore() {
        playerName = "";
        score = 0;
        difficulty = "";
        timeTaken = 0;
    }

    PlayerScore(string name, int s, string diff, float t) {
        playerName = name;
        score = s;
        difficulty = diff;
        timeTaken = t;
    }
};


PlayerScore highScores[MAX_HIGH_SCORES];
PlayerScore highScores2[MAX_HIGH_SCORES];
int highScoreCount = 0;
int highScoreCount2 = 0;

struct Enemy {
    int x, y, dx, dy;
    Enemy() {
        x = y = 300;
        dx = 4 - rand() % 8;
        dy = 4 - rand() % 8;
    }
    void move() {
        x += dx; if (grid[y/ts][x/ts] == 1 || grid[y/ts][x/ts] == 4) { dx = -dx; x += dx; }
        y += dy; if (grid[y/ts][x/ts] == 1 || grid[y/ts][x/ts] == 4) { dy = -dy; y += dy; }
        
    }

    float orbitAngle;   
    float orbitRadius;   
    float orbitSpeed;    
    float centerX, centerY; 
};

void drop(int y, int x);
void drop2(int b, int c);
int  countFilledBlocks();
int  countFilledBlocks2();
void saveScore(string& playerName, int score, const string& difficulty, float timeTaken);
void saveScore2(string& playerName2, int score2, string difficulty, float timeTaken);
void loadScores();
void addScore(string playerName, int score, string difficulty, float gameTime);
void addScore2(string playerName2, int score2, string difficulty, float gameTime);

template <typename T>
T mymin(T a, T b) {
    return (a < b) ? a : b;
}

template <typename T>
T mymax(T a, T b) {
    return (a > b) ? a : b;
}

template <typename T>
T myclamp(T value, T min_val, T max_val) {
    return mymax(min_val, mymin(value, max_val));
}




//=================================================== MAIN FUNCTION ================================================================

int main() {
    
    Clock spawnClock;
    srand(time(0));

    RenderWindow window(VideoMode(N * ts, M * ts + statusBarHeight), "XONIX - A Maaz & Hussain Production"); // Added larger status bar
    window.setFramerateLimit(60);

    
    loadScores();

    // TEXTURES
    Texture t1, t2, t3, tbackground;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");
    tbackground.loadFromFile("images/background_4.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3), sBackground(tbackground);
    sBackground.setScale(
        (float)N * ts / sBackground.getTexture()->getSize().x,
        (float)M * ts / sBackground.getTexture()->getSize().y
    );
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    // FONTS & TEXT
    Font fonts;
    fonts.loadFromFile("fonts/arial.ttf");

    Text menu;
    Text menuOptions[2];
    Text submenuOptions[3];
    Text submenuOptions1[5];
    Text endMenuOptions[3];
    Text scoreboardText[11]; 
    Text playerNameInput;
    Text playerName2Input;
    
    // status bar text elements
    Text levelText, timeText, movesText, scoreText, speedText, powerupText, bonusText;
    Text moves2Text, score2Text, powerup2Text, bonus2Text;
    

    levelText.setFont(fonts);
    timeText.setFont(fonts);
    movesText.setFont(fonts); 
    scoreText.setFont(fonts);
    speedText.setFont(fonts);
    powerupText.setFont(fonts);
    bonusText.setFont(fonts);
    moves2Text.setFont(fonts);
    score2Text.setFont(fonts);
    powerup2Text.setFont(fonts);
    bonus2Text.setFont(fonts);
    levelText.setCharacterSize(24); 
    timeText.setCharacterSize(24); 
    movesText.setCharacterSize(24); 
    scoreText.setCharacterSize(24);
    speedText.setCharacterSize(24);
    powerupText.setCharacterSize(24);
    bonusText.setCharacterSize(24);
    levelText.setFillColor(Color::White); 
    timeText.setFillColor(Color::White); 
    movesText.setFillColor(Color::White); 
    scoreText.setFillColor(Color::White);
    speedText.setFillColor(Color::White);
    powerupText.setFillColor(Color::White);
    bonusText.setFillColor(Color::White);

    // initialize scoreboard title
    scoreboardText[0].setString(" ");
    scoreboardText[0].setOrigin(scoreboardText[0].getLocalBounds().width/2, 0);
    scoreboardText[0].setPosition(N*ts/2.f, 60);
    scoreboardText[0].setCharacterSize(50);
    scoreboardText[0].setFillColor(Color::Black);


    // initialize text objects
    menu.setFont(fonts);
    for (int i = 0; i < 2; ++i) menuOptions[i].setFont(fonts);
    for (int i = 0; i < 3; ++i) submenuOptions[i].setFont(fonts);
    for (int i = 0; i < 5; ++i) submenuOptions1[i].setFont(fonts);
    for (int i = 0; i < 3; ++i) endMenuOptions[i].setFont(fonts);
    for (int i = 0; i < 11; ++i) scoreboardText[i].setFont(fonts);
    playerNameInput.setFont(fonts);

    // Menu strings
    menuOptions[0].setString("START GAME"); menuOptions[0].setPosition(260, 160); menuOptions[0].setCharacterSize(32);
    menuOptions[1].setString("SCORE-BOARD"); menuOptions[1].setPosition(245, 210); menuOptions[1].setCharacterSize(32);

    submenuOptions[0].setString("SINGLE PLAYER");  submenuOptions[0].setPosition(250, 160); submenuOptions[0].setCharacterSize(30);
    submenuOptions[1].setString("DOUBLE PLAYER"); submenuOptions[1].setPosition(245, 210); submenuOptions[1].setCharacterSize(30);
    submenuOptions[2].setString("BACK");       submenuOptions[2].setPosition(570, 260); submenuOptions[2].setCharacterSize(20);

    submenuOptions1[0].setString("EASY");        submenuOptions1[0].setPosition(310, 120); submenuOptions1[0].setCharacterSize(30);
    submenuOptions1[1].setString("NORMAL");      submenuOptions1[1].setPosition(285, 160); submenuOptions1[1].setCharacterSize(30);
    submenuOptions1[2].setString("HARD");        submenuOptions1[2].setPosition(305, 200); submenuOptions1[2].setCharacterSize(30);
    submenuOptions1[3].setString("CONTINUOUS"); submenuOptions1[3].setPosition(250, 240); submenuOptions1[3].setCharacterSize(30);
    submenuOptions1[4].setString("BACK");        submenuOptions1[4].setPosition(570, 260); submenuOptions1[4].setCharacterSize(20);

    // End Menu options
    endMenuOptions[0].setString("RESTART GAME"); endMenuOptions[0].setPosition(240, 120); endMenuOptions[0].setCharacterSize(32);
    endMenuOptions[1].setString("SCORE-BOARD"); endMenuOptions[1].setPosition(245, 160); endMenuOptions[1].setCharacterSize(32);
    endMenuOptions[2].setString("EXIT GAME"); endMenuOptions[2].setPosition(270, 200); endMenuOptions[2].setCharacterSize(32);
 

    // Game variables
    bool oneplayer = true;
    int selectedOptionMenu = 0;
    int selectedsubOptionMenu = 0;
    int selectedsubOptions1Menu = 0;
    int selectedEndMenuOption = 0;
    int stateofMenu = 0; 
    float elapse = 0;


    int enemyCount = 4;
    Enemy a[10];

    bool Game = true;
    int x = 0, y = 0, dx = 0, dy = 0;
    //For second player.
    int c = N - 2, b = M - 2, dc =0, db=0;
    float timer = 0, delay = 0.07f;
    Clock clock;
    Clock gameTimer;
    Clock switchTimer;
    
    bool isConstructing = false;
    int previousBlockCount = 0;
    string currentDifficulty = "EASY";
    string playerName = "Player : ";
    string playerName2 = "Player : ";
    string inputText = "";
    string InputText2 = "";
    bool enteringName = false;
    float gameOverTimer = 0;

    

    // Initialize grid borders
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M-1 || j == N-1)
                grid[i][j] = 1;

    while (window.isOpen()) {
    
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();

            if (e.type == Event::KeyPressed) {
                switch (stateofMenu) {
                    case 0: { // MAIN MENU
                        if (e.key.code == Keyboard::Up) {
                            if (selectedOptionMenu == 0)
                                selectedOptionMenu = 1;
                            else
                                selectedOptionMenu -= 1;
                        } else if (e.key.code == Keyboard::Down) {
                            if (selectedOptionMenu == 1)
                                selectedOptionMenu = 0;
                            else
                                selectedOptionMenu += 1;
                        } else if (e.key.code == Keyboard::Enter) {
                            if (selectedOptionMenu == 0)
                                stateofMenu = 1;
                            else if (selectedOptionMenu == 1) {
                                stateofMenu = 4; 
                                loadScores(); 
                            }
                        }
                        break;
                    }

                    case 1: { // Mode Submenu
                        if (e.key.code == Keyboard::Up) {
                            if (selectedsubOptionMenu == 0)
                                selectedsubOptionMenu = 2;
                            else
                                selectedsubOptionMenu -= 1;
                        } else if (e.key.code == Keyboard::Down) {
                            if (selectedsubOptionMenu == 2)
                                selectedsubOptionMenu = 0;
                            else
                                selectedsubOptionMenu += 1;
                        } else if (e.key.code == Keyboard::Enter) {
                            if (selectedsubOptionMenu == 2)
                                stateofMenu = 0;
                            else {
                                oneplayer = (selectedsubOptionMenu == 0);
                                stateofMenu = 2;
                            }
                        }
                        break;
                    }

                    case 2: { // Difficulty Submenu
                        if (e.key.code == Keyboard::Up) {
                            if (selectedsubOptions1Menu == 0)
                                selectedsubOptions1Menu = 4;
                            else
                                selectedsubOptions1Menu -= 1;
                        } else if (e.key.code == Keyboard::Down) {
                            if (selectedsubOptions1Menu == 4)
                                selectedsubOptions1Menu = 0;
                            else
                                selectedsubOptions1Menu += 1;
                        } else if (e.key.code == Keyboard::Enter) {
                            if (selectedsubOptions1Menu == 4) {
                                stateofMenu = 1;
                            } else {
                                // Set the difficulty and level
                                switch (selectedsubOptions1Menu) {
                                    case 0: level = 1; currentDifficulty = "EASY"; break;
                                    case 1: level = 2; currentDifficulty = "NORMAL"; break;
                                    case 2: level = 3; currentDifficulty = "HARD"; break;
                                    case 3: level = 0; currentDifficulty = "CONTINUOUS"; break;
                                }
                                
                                stateofMenu = 5;
                                inputText  = "Player : ";
                                InputText2 = "Player : ";
                            }
                        }
                        break;
                    }

                    case 3: { // End Menu
                        if (e.key.code == Keyboard::Up) {
                            if (selectedEndMenuOption == 0)
                                selectedEndMenuOption = 2; 
                            else
                                selectedEndMenuOption -= 1;
                        } else if (e.key.code == Keyboard::Down) {
                            if (selectedEndMenuOption == 2) 
                                selectedEndMenuOption = 0;
                            else
                                selectedEndMenuOption += 1;
                        } else if (e.key.code == Keyboard::Enter) {
                            if (selectedEndMenuOption == 0) {
                                stateofMenu = 1; 
                            } else if (selectedEndMenuOption == 1) {
                                stateofMenu = 4; 
                                loadScores();
                            } else if (selectedEndMenuOption == 2) {
                                window.close();
                            }
                        }
                        break;
                    }

                    case 4: { // SCOREBOARD
                        if (e.key.code == Keyboard::Escape || e.key.code == Keyboard::Enter) {
                            stateofMenu = 0;
                        }
                        break;
                    }

                    case 5: { // Name entry
                        if (e.key.code == Keyboard::Enter) {
                            playerName = inputText;
                            playerName2 = InputText2;
                            stateofMenu = -1; 
                            
                            // Reseting game variables
                            score = 0;
                            score2 = 0;
                            moves = 0;
                            moves2 = 0;
                            gameTime = 0;
                            isConstructing = false;
                            isConstructing2 = false;
                            gameTimer.restart();
                            
                            // Set enemy count based on difficulty
                            
                            if (currentDifficulty == "EASY") {
                                enemyCount = 2;
                            }
                            else if (currentDifficulty == "NORMAL") {
                                enemyCount = 4;
                            }
                            else if (currentDifficulty == "HARD") {
                                enemyCount = 6;
                            }
                            else if (currentDifficulty == "CONTINUOUS") {
                                enemyCount = 2;        
                                spawnClock.restart();  
                            }

                            // Reset enemies
                            for (int i = 0; i < enemyCount; i++) {
                                a[i] = Enemy();
                            }
                            
                            // Clear the grid except for borders
                            for (int i = 1; i < M-1; i++)
                                for (int j = 1; j < N-1; j++)
                                    grid[i][j] = 0;
                                    
                            // Reset player position
                            x = 0; y = 0; dx = 0; dy = 0;
                            c = N - 1, b = M - 1, dc =0, db=0;
                            Game = true;
                        } else if (e.key.code == Keyboard::Backspace) {
                            
                            if (oneplayer && !inputText.empty()) {
                                inputText.pop_back();  
                            }
                
                            else if (!oneplayer) {
                                if (!inputText.empty()) {
                                    inputText.pop_back();  
                                }
                                if (!InputText2.empty()) {
                                    InputText2.pop_back();  
                                }
                            }
                        }
                        
                        else if (e.key.code == Keyboard::Escape) {
                            stateofMenu = 2; 
                        }
                        break;
                    }

                    default: { // In-game controls
                        if (e.key.code == Keyboard::Escape) {
                            
                            for (int i = 1; i < M-1; i++)
                                for (int j = 1; j < N-1; j++)
                                    grid[i][j] = 0;
                            x = y = 0; dx = dy = 0;
                            c = b = 0; dc = db = 0; // 2 player
                            
                            Game = true;
                            score = 0;
                            score2 = 0;
                            moves = 0;
                            moves2 = 0;
                            gameTime = 0;
                            isConstructing = false;
                            isConstructing2 = false;
                            stateofMenu = 0;
                        } else if (e.key.code == Keyboard::Space) {
                            if (powerUps > 0 && !powerUpActive && e.key.code == Keyboard::Space) {
                                powerUpActive = true;
                                powerUpClock.restart();
                                powerUps--;
                            }
                        } else if (e.key.code == Keyboard::L){
                            if (powerUps2 > 0 && !powerUpActive2 && e.key.code == Keyboard::L) {
                                powerUpActive2 = true;
                                powerUpClock2.restart();
                                powerUps2--;
                            }
                        } else if (e.key.code == Keyboard::P) {
                             Game = !Game;
                        }
                        break;
                    }
                }
            } 
            else if (e.type == Event::TextEntered && stateofMenu == 5) {
                // Handle text input for 1 Player
                if (oneplayer) {
                    
                    if (e.text.unicode < 128 && e.text.unicode != 8 && e.text.unicode != 13) {
                        if (inputText.length() < 15) { 
                            inputText += static_cast<char>(e.text.unicode);
                        }
                    }
                }
                // Handle text input for 2 Player
                else {
                    
                    if (e.text.unicode < 128 && e.text.unicode != 8 && e.text.unicode != 13) {
                        if (inputText.length() < 15) { 
                            inputText += static_cast<char>(e.text.unicode);
                        }
                        if (InputText2.length() < 15) { 
                            InputText2 += static_cast<char>(e.text.unicode);
                        }
                    }
                }
            }
            
        }

    // Game update while playing
    float time = clock.getElapsedTime().asSeconds();
    clock.restart();
    timer += time;

    if (stateofMenu == -1 && Game) {
        // timers
        gameTime = gameTimer.getElapsedTime().asSeconds();
        switchTime = switchTimer.getElapsedTime().asSeconds();
        if (powerUpActive && powerUpClock.getElapsedTime().asSeconds() > 3.0f)
            powerUpActive = false;
        if (powerUpActive2 && powerUpClock2.getElapsedTime().asSeconds() > 3.0f)
            powerUpActive2 = false;

        if(switchTime >= 30.0f){
            //circle();
            switchTimer.restart();
        } else {
            //zigzag();
            switchTimer.restart();

        }
        //=================================== SINGLE PLAYER BRANCH ================================================================
        if (oneplayer) {

            // Player 1 input (arrows)
            if (Keyboard::isKeyPressed(Keyboard::Left))  { dx = -1; dy =  0; }
            if (Keyboard::isKeyPressed(Keyboard::Right)) { dx =  1; dy =  0; }
            if (Keyboard::isKeyPressed(Keyboard::Up))    { dx =  0; dy = -1; }
            if (Keyboard::isKeyPressed(Keyboard::Down))  { dx =  0; dy =  1; }

            if (timer > delay) {
                // P1: start new loop?
                if (grid[y][x] == 1 && (dx != 0 || dy != 0)) {
                    isConstructing      = false;
                    previousBlockCount  = countFilledBlocks();
                }

                // P1: move
                x += dx;  y += dy;
                x = myclamp(x, 0, N-1);
                y = myclamp(y, 0, M-1);

                // P1: self-collision?
                if (grid[y][x] == 2 && selectedsubOptionMenu == 1) {
                    Game = false;
                    addScore(playerName, score, currentDifficulty, gameTime);
                    gameOverTimer = 0;
                }

                // P1: leave trail
                if (grid[y][x] == 0) {
                    grid[y][x] = 2;
                    isConstructing = true;
                }

                timer = 0;
            }

            // Enemy movement & collision with P1’s trail
            for (int i = 0; i < enemyCount; ++i) {
                if (!powerUpActive) a[i].move();
                int ey = a[i].y/ts, ex = a[i].x/ts;
                if (grid[ey][ex] == 2) {
                    Game = false;
                    addScore(playerName, score, currentDifficulty, gameTime);
                    gameOverTimer = 0;
                }
            }

            
            if (grid[y][x] == 1 && isConstructing) {
                dx = dy = 0;
                moves++;
                isConstructing = false;

                for (int i = 0; i < enemyCount; ++i)
                    drop(a[i].y/ts, a[i].x/ts);

                int beforeFill = countFilledBlocks();
                for (int i = 0; i < M; ++i)
                    for (int j = 0; j < N; ++j) {
                        if (grid[i][j] == -1) grid[i][j] = 0;
                        else if (grid[i][j] == 2 || grid[i][j] == 0)
                            grid[i][j] = 1;
                    }
                int afterFill = countFilledBlocks();
                blocksConstructed = afterFill - beforeFill;

                int rewardThreshold = 10, bonusMultiplier = 1;
                if (bonusCounter >= 5) {
                    rewardThreshold    = 5;
                    bonusMultiplier    = (blocksConstructed > 5) ? 4 : 1;
                } else if (bonusCounter >= 3) {
                    rewardThreshold    = 5;
                    bonusMultiplier    = (blocksConstructed > 5) ? 2 : 1;
                } else {
                    rewardThreshold    = 10;
                    bonusMultiplier    = (blocksConstructed > 10) ? 2 : 1;
                }
                if (bonusMultiplier > 1) bonusCounter++;
                score += blocksConstructed * bonusMultiplier;

                static int nextPowerUpScore = 50;
                if (score >= nextPowerUpScore) {
                    powerUps++;
                    nextPowerUpScore += 30;
                }

                int totalBlocks  = (M-2)*(N-2);
                int filledBlocks = countFilledBlocks();
                if (currentDifficulty != "CONTINUOUS" && filledBlocks >= totalBlocks * 0.95 && selectedsubOptionMenu == 1) {
                    Game = false;
                    addScore(playerName, score, currentDifficulty, gameTime);
                    gameOverTimer = 0;
                }
                if (currentDifficulty == "CONTINUOUS" && filledBlocks >= totalBlocks * 0.75) {
                    level++;
                    for (int i = 1; i < M-1; ++i)
                        for (int j = 1; j < N-1; ++j)
                            grid[i][j] = 0;
                }
            }

        //==================================== DOUBLE PLAYER BRANCH ================================================================

        } else {
            // Player 1 input 
            if (Keyboard::isKeyPressed(Keyboard::Left))  { dx = -1; dy =  0; }
            if (Keyboard::isKeyPressed(Keyboard::Right)) { dx =  1; dy =  0; }
            if (Keyboard::isKeyPressed(Keyboard::Up))    { dx =  0; dy = -1; }
            if (Keyboard::isKeyPressed(Keyboard::Down))  { dx =  0; dy =  1; }

            // Player 2 input 
            if (Keyboard::isKeyPressed(Keyboard::A)) { dc = -1; db =  0; }
            if (Keyboard::isKeyPressed(Keyboard::D)) { dc =  1; db =  0; }
            if (Keyboard::isKeyPressed(Keyboard::W)) { dc =  0; db = -1; }
            if (Keyboard::isKeyPressed(Keyboard::S)) { dc =  0; db =  1; }

            if (timer > delay) {
                // —— P1 move & trail ——
                if (grid[y][x] == 1 && (dx != 0 || dy != 0)) {
                    isConstructing      = false;
                    previousBlockCount  = countFilledBlocks();
                }
                
                x += dx;  y += dy;
                x = myclamp(x, 0, N-1);
                y = myclamp(y, 0, M-1);

                if (grid[y][x] == 2) {
                    Game = false;
                    addScore(playerName, score, currentDifficulty, gameTime);
                    gameOverTimer = 0;
                }
                if (grid[y][x] == 0 || grid[y][x] == 4) {
                    grid[y][x] = 2;
                    isConstructing = true;
                }

                // —— P2 move & trail ——
                if (grid[b][c] == 4 && (dc != 0 || db != 0)) {
                    isConstructing2      = false;
                    previousBlockCount2  = countFilledBlocks2();
                }

                c += dc;  b += db;
                c = myclamp(c, 0, N-1);
                b = myclamp(b, 0, M-1);
                
                if (grid[b][c] == 3) {
                    Game = false;
                    addScore2(playerName2, score2, currentDifficulty, gameTime);
                    gameOverTimer = 0;
                }
                // Only allow trail after P2 has moved
                if (grid[b][c] == 0) {
                    grid[b][c] = 3;
                    isConstructing2 = true;
                }

                

                timer = 0;
                
            }

            // Enemy movement & collision with P1’s trail
            for (int i = 0; i < enemyCount; ++i) {
                if (!powerUpActive) a[i].move();
                int ey = a[i].y/ts, ex = a[i].x/ts;
                if (grid[ey][ex] == 2) {
                    Game = false;
                    addScore(playerName, score, currentDifficulty, gameTime);
                    gameOverTimer = 0;
                }
            }

            // —— P1 loop-complete logic ——
            if (grid[y][x] == 1 && isConstructing) {
                dx = dy = 0;
                moves++;
                isConstructing = false;

                for (int i = 0; i < enemyCount; ++i)
                    drop(a[i].y/ts, a[i].x/ts);

                int beforeFill = countFilledBlocks();
                for (int i = 0; i < M; ++i)
                    for (int j = 0; j < N; ++j) {
                        if (grid[i][j] == -1) grid[i][j] = 0;
                        else if (grid[i][j] == 2 || grid[i][j] == 0)
                            grid[i][j] = 1;
                    }
                int afterFill = countFilledBlocks();
                blocksConstructed = afterFill - beforeFill;

                int rewardThreshold = 10, bonusMultiplier = 1;
                if (bonusCounter >= 5) {
                    rewardThreshold    = 5;
                    bonusMultiplier    = (blocksConstructed > 5) ? 4 : 1;
                } else if (bonusCounter >= 3) {
                    rewardThreshold    = 5;
                    bonusMultiplier    = (blocksConstructed > 5) ? 2 : 1;
                } else {
                    rewardThreshold    = 10;
                    bonusMultiplier    = (blocksConstructed > 10) ? 2 : 1;
                }
                if (bonusMultiplier > 1) bonusCounter++;
                score += blocksConstructed * bonusMultiplier;

                static int nextPowerUpScore = 50;

                if (score >= nextPowerUpScore) {
                    powerUps++;
                    nextPowerUpScore += 30;
                }

                int totalBlocks  = (M-2)*(N-2);
                int filledBlocks = countFilledBlocks();
                if (currentDifficulty != "CONTINUOUS" && filledBlocks >= totalBlocks * 0.75) {
                    Game = false;
                    addScore(playerName, score, currentDifficulty, gameTime);
                    gameOverTimer = 0;
                }
                if (currentDifficulty == "CONTINUOUS" && filledBlocks >= totalBlocks * 0.75) {
                    level++;
                    for (int i = 1; i < M-1; ++i)
                        for (int j = 1; j < N-1; ++j)
                            grid[i][j] = 0;
                }
            }

            // Enemy movement & collision with P2’s trail
            for (int i = 0; i < enemyCount; ++i) {
                if (!powerUpActive2) a[i].move();
                int ey = a[i].y/ts, ex = a[i].x/ts;
                if (grid[ey][ex] == 3) {
                    Game = false;
                    addScore2(playerName2, score2, currentDifficulty, gameTime);
                    gameOverTimer = 0;
                }
            }
            
            
            if ((grid[b][c] == 1 || grid[b][c] == 4)&& isConstructing2){
                dc = db = 0;
                moves2++;
                isConstructing2 = false;
            
                for (int i = 0; i < enemyCount; ++i)
                    drop2(a[i].y/ts, a[i].x/ts);
            
                int beforeFill2 = countFilledBlocks2();
                for (int i = 0; i < M; ++i)
                    for (int j = 0; j < N; ++j) {
                        if (grid[i][j] == -1) grid[i][j] = 0;
                        else if (grid[i][j] == 3 || grid[i][j] == 0)
                            grid[i][j] = 4;
                    }
                int afterFill2 = countFilledBlocks2();
                blocksConstructed2 = afterFill2 - beforeFill2;
                
                int rewardThreshold2 = 10, bonusMultiplier2 = 1;
                if (bonusCounter >= 5) {
                    rewardThreshold2    = 5;
                    bonusMultiplier2    = (blocksConstructed2 > 5) ? 4 : 1;
                } else if (bonusCounter >= 3) {
                    rewardThreshold2    = 5;
                    bonusMultiplier2    = (blocksConstructed2 > 5) ? 2 : 1;
                } else {
                    rewardThreshold2    = 10;
                    bonusMultiplier2    = (blocksConstructed2 > 10) ? 2 : 1;
                }
                if (bonusMultiplier2 > 1) bonusCounter++;
                score2 += blocksConstructed2 * bonusMultiplier2;
            
                static int nextPowerUpScore2 = 50;
                if (score2 >= nextPowerUpScore2) {
                    powerUps2++;
                    nextPowerUpScore2 += 30;
                }
            
                int totalBlocks2  = (M-2)*(N-2);
                int filledBlocks2 = countFilledBlocks2();
                if (currentDifficulty != "CONTINUOUS" && filledBlocks2 >= totalBlocks2 * 0.75) {
                    Game = false;
                    addScore2(playerName2, score2, currentDifficulty, gameTime);
                    gameOverTimer = 0;
                }
                if (currentDifficulty == "CONTINUOUS" && filledBlocks2 >= totalBlocks2 * 0.75) {
                    level++;
                    for (int i = 1; i < M-1; ++i)
                        for (int j = 1; j < N-1; ++j)
                            grid[i][j] = 0;
                }
            }
            //======================================== PLAYER VS PLAYER COLLISION ==================================================

            // Exact overlap:
            if (x == c && y == b) {
                if (isConstructing && isConstructing2) {
                    
                    Game = false;
                    addScore(playerName,  score,  currentDifficulty, gameTime);
                    addScore2(playerName2, score2, currentDifficulty, gameTime);
                } 
            
                else if (isConstructing && !isConstructing2) {
                    
                    for (int i = 0; i < M; ++i)
                        for (int j = 0; j < N; ++j)
                            if (grid[i][j] == 2) grid[i][j] = 0;

                    // Reset P1's position and direction
                    x = 0; y = 0;
                    dx = dy = 0;
                    isConstructing = false;

                    addScore2(playerName2, score2, currentDifficulty, gameTime);
                    addScore(playerName, score, currentDifficulty, gameTime);
                } 
                else if (!isConstructing && isConstructing2) {
                    
                    for (int i = 0; i < M; ++i)
                        for (int j = 0; j < N; ++j)
                            if (grid[i][j] == 3) grid[i][j] = 0;

                    // Reset P2's position and direction
                    c = N - 2; b = M - 2;
                    dc = db = 0;
                    isConstructing2 = false;
                    addScore(playerName, score, currentDifficulty, gameTime);
                    addScore2(playerName2, score2, currentDifficulty, gameTime);
                }
            }

        }
    }

    
        //==========================================================================================================================

        

        // DRAW
        window.clear();
        window.draw(sBackground);

        
        if (stateofMenu == -1 && (currentDifficulty == "CONTINUOUS"||currentDifficulty == "EASY"||currentDifficulty == "NORMAL"||currentDifficulty == "HARD")){
            float timeforspeed = spawnClock.getElapsedTime().asSeconds();
            if(timeforspeed >= 20.0f){
                for(int i =0;i<enemyCount; i++){    
                    a[i].dx *= 1.5;
                    a[i].dy *= 1.5;
                      
                } 
                spawnClock.restart(); 
            }
        }
        if (currentDifficulty == "CONTINUOUS") {
            float elapsed = spawnClock.getElapsedTime().asSeconds();
            if (elapsed >= 20.0f && enemyCount <10){
                // spawn 2 more enemies
                for (int i = 0; i < 2; ++i)
                    a[enemyCount + i] = Enemy();
                
                    enemyCount += 2;
                
                spawnClock.restart();
            }
        }
        
        
        if (stateofMenu >= 0 && stateofMenu != 5) {
            const char* titles[] = { "MAIN MENU", "MODE", "DIFFICULTY", "GAME OVER", "SCOREBOARD" };
            menu.setFont(fonts);
            menu.setString(titles[stateofMenu]);
            menu.setOrigin(menu.getLocalBounds().width/2, 0);
            menu.setPosition(N*ts/2.f, 60);
            menu.setCharacterSize(50);
            menu.setFillColor(Color::Black);
            window.draw(menu);

            if (stateofMenu == 0) { // Main Menu
                for (int i = 0; i < 2; ++i) {
                    menuOptions[i].setFillColor(i == selectedOptionMenu ? Color::Red : Color::White);
                    window.draw(menuOptions[i]);
                }
            } else if (stateofMenu == 1) { // Player Selection
                for (int i = 0; i < 3; ++i) {
                    submenuOptions[i].setFillColor(i == selectedsubOptionMenu ? Color::Red : Color::White);
                    window.draw(submenuOptions[i]);
                }
            } else if (stateofMenu == 2) { // Difficulty Selection
                for (int i = 0; i < 5; ++i) {
                    submenuOptions1[i].setFillColor(i == selectedsubOptions1Menu ? Color::Red : Color::White);
                    window.draw(submenuOptions1[i]);
                }
            } else if (stateofMenu == 3) { // Game Over Menu
                for (int i = 0; i < 3; ++i) { 
                    endMenuOptions[i].setFillColor(i == selectedEndMenuOption ? Color::Red : Color::White);
                    window.draw(endMenuOptions[i]);
                }
                // Display final score
                Text finalScore;
                finalScore.setFont(fonts);
                finalScore.setString("YOUR SCORE: " + to_string(score));
                finalScore.setPosition(255, 240);
                finalScore.setCharacterSize(25);
                finalScore.setFillColor(Color::Yellow);
                window.draw(finalScore);

                // Display new high score msg
                if (isNewHighScore) {
                    Text newHighScoreText;
                    newHighScoreText.setFont(fonts);
                    newHighScoreText.setString("NEW HIGH SCORE!");
                    newHighScoreText.setPosition(265, 265);
                    newHighScoreText.setCharacterSize(22);
                    newHighScoreText.setFillColor(Color::Green);
                    window.draw(newHighScoreText);
                }

            } else if (stateofMenu == 4) { // Scoreboard
                window.draw(scoreboardText[0]); 
                
                // Display high scores
                int displayLimit = highScoreCount < 10 ? highScoreCount : 10;
                for (int i = 0; i < displayLimit; i++) {
                    string entry = to_string(i+1) + ". " + highScores[i].playerName + 
                                  " - " + to_string(highScores[i].score) + 
                                  " (" + highScores[i].difficulty + ")" +
                                  "- Time: " + to_string((int)highScores[i].timeTaken);

                    scoreboardText[i+1].setString(entry);
                    scoreboardText[i+1].setPosition(180, 150 + i*30);
                    scoreboardText[i+1].setCharacterSize(20);
                    scoreboardText[i+1].setFillColor(i == 0 ? Color::Green : Color::White);
                    window.draw(scoreboardText[i+1]);
                }
                
                // Back instruction
                Text backText;
                backText.setFont(fonts);
                backText.setString("Press ESC to return");
                backText.setPosition(450, 270);
                backText.setCharacterSize(20);
                backText.setFillColor(Color::Red);
                window.draw(backText);
            }
        } else if (stateofMenu == 5) { // Name Entry Screen
            // Title
            Text nameTitle;
            nameTitle.setFont(fonts);
            nameTitle.setString("NAME");
            nameTitle.setOrigin(nameTitle.getLocalBounds().width/2, 0);
            nameTitle.setPosition(N*ts/2.f - 20, 60);
            nameTitle.setCharacterSize(50);
            nameTitle.setFillColor(Color::Black);
            window.draw(nameTitle);
            
         // --- Player 1 Input Box ---
        if(oneplayer){
        RectangleShape inputBox1(Vector2f(300, 40));
        inputBox1.setPosition(230, 200);
        inputBox1.setFillColor(Color(50, 50, 50));
        inputBox1.setOutlineThickness(2);
        inputBox1.setOutlineColor(Color::White);
        window.draw(inputBox1);
                
        // --- Player 1 Name Text ---
        playerNameInput.setString(inputText + " ");
        playerNameInput.setPosition(240, 205);
        playerNameInput.setCharacterSize(30);
        playerNameInput.setFillColor(Color::White);
        window.draw(playerNameInput);
        } else {
                
        // --- Player 2 Input Box ---
        RectangleShape inputBox1(Vector2f(300, 40));
        inputBox1.setPosition(230, 200);
        inputBox1.setFillColor(Color(50, 50, 50));
        inputBox1.setOutlineThickness(2);
        inputBox1.setOutlineColor(Color::White);
        window.draw(inputBox1);
                
        // --- Player 1 Name Text ---
        playerNameInput.setString(inputText + " ");
        playerNameInput.setPosition(240, 205);
        playerNameInput.setCharacterSize(30);
        playerNameInput.setFillColor(Color::White);
        window.draw(playerNameInput);

            
        RectangleShape inputBox2(Vector2f(300, 40));
        inputBox2.setPosition(230, 260);
        inputBox2.setFillColor(Color(50, 50, 50));
        inputBox2.setOutlineThickness(2);
        inputBox2.setOutlineColor(Color::White);
        window.draw(inputBox2);
                
        // --- Player 2 Name Text ---
        playerName2Input.setString(InputText2 + " ");
        playerName2Input.setPosition(240, 265);
        playerName2Input.setCharacterSize(30);
        playerName2Input.setFillColor(Color::White);
        window.draw(playerName2Input);
        }
            // Instructions
            Text instructions;
            instructions.setFont(fonts);
            instructions.setString("* Press ENTER to confirm or ESC to go back");
            instructions.setPosition(200, 315);
            instructions.setCharacterSize(18);
            instructions.setFillColor(Color::Red);
            window.draw(instructions);
            
            // Display difficulty info
            Text diffInfo;
            diffInfo.setFont(fonts);
            diffInfo.setString("Difficulty: " + currentDifficulty);
            diffInfo.setPosition(300, 160);
            diffInfo.setCharacterSize(20);
            diffInfo.setFillColor(Color::Yellow);
            window.draw(diffInfo);
        } else {
            // Draw game grid
            for (int i = 0; i < M; ++i)
                for (int j = 0; j < N; ++j) {
                    if (grid[i][j] == 0) {
                        continue;
                    }
                    if (grid[i][j] == 1) {
                        sTile.setTextureRect(IntRect(0,0,ts,ts));   // P1’s filled
                        sTile.setColor(Color::White);
                    }
                    else if (grid[i][j] == 2) {
                        sTile.setTextureRect(IntRect(54,0,ts,ts));  // P1’s trail
                        sTile.setColor(Color::White);
                    }
                    else if (grid[i][j] == 3) {
                        sTile.setTextureRect(IntRect(54,0,ts,ts));  // P2’s trail
                        sTile.setColor(Color(255,165,0));           // orange
                    }
                    else if (grid[i][j] == 4) {
                        sTile.setTextureRect(IntRect(0,0,ts,ts));   // P2’s conquered
                        sTile.setColor(Color::Yellow);                 // yellow
                    }
                    sTile.setPosition(j*ts, i*ts);
                    window.draw(sTile);
                    sTile.setColor(Color::White);  //reset 
                    
                }

            // Draw player
            sTile.setTextureRect(IntRect(36,0,ts,ts));
            sTile.setPosition(x*ts, y*ts);
            window.draw(sTile);

            //PLayer 2
            // Draw player 2
            sTile.setTextureRect(IntRect(36,0,ts,ts)); 
            sTile.setColor(Color::Magenta);            
            sTile.setPosition(c*ts, b*ts);
            window.draw(sTile);
            sTile.setColor(Color::White);              //reset


            // Draw enemies
            sEnemy.rotate(10);
            for (int i = 0; i < enemyCount; ++i) {
                sEnemy.setPosition(a[i].x, a[i].y);
                window.draw(sEnemy);
            }
            // Draw status bar background
            RectangleShape statusBar(Vector2f(N*ts, statusBarHeight));
            statusBar.setPosition(0, M*ts);
            statusBar.setFillColor(Color(0, 0, 60)); 
            window.draw(statusBar);

            
            
            string levelDisplay = currentDifficulty == "CONTINUOUS" ? "LEVEL: X" : ("EASY" ? "LEVEL: 1" : ("NORMAL" ? "LEVEL: 2" :("HARD" ? "LEVEL: 3" : " ") ));
            levelText.setString(levelDisplay);
            levelText.setCharacterSize(14);
            levelText.setPosition(20, M*ts + 5); 

            int minutes = static_cast<int>(gameTime) / 60;
            int seconds = static_cast<int>(gameTime) % 60;
            timeText.setString("TIME: " + to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds));
            timeText.setCharacterSize(14);  
            timeText.setPosition(120, M*ts + 5);  

            movesText.setString("MOVES: " + to_string(moves));
            movesText.setCharacterSize(14);  
            movesText.setPosition(220, M*ts + 5);  

            scoreText.setString("SCORE: " + to_string(score));
            scoreText.setCharacterSize(14);  
            scoreText.setPosition(320, M*ts + 5);  


            // SPEED
            speedText.setString("SPEED: 200%");
            speedText.setCharacterSize(14);  
            speedText.setPosition(20, M*ts + 30);  
            speedText.setFillColor(Color::Red);

            // POWER-UPS (increased size)
            powerupText.setString("POWER-UPS: " + to_string(powerUps));
            powerupText.setCharacterSize(14);  
            powerupText.setPosition(120, M*ts + 30);  

            if(selectedsubOptionMenu == 1){
            // Powerups 2 player
            powerup2Text.setString("POWER-UPS 2: " + to_string(powerUps2));
            powerup2Text.setCharacterSize(14);  
            powerup2Text.setPosition(250, M*ts + 30);  

            // MOVE 2
            moves2Text.setString("MOVES 2: " + to_string(moves2));
            moves2Text.setCharacterSize(14);  
            moves2Text.setPosition(420, M*ts + 5);

            // SCORE 2nd player
            score2Text.setString("SCORE 2: " + to_string(score2));
            score2Text.setCharacterSize(14);  
            score2Text.setPosition(520, M*ts + 5);
            }
            // BONUS 
            string bonusStr = "BONUS: ×1";
            if (bonusCounter >= 5)
                bonusStr = "BONUS: ×4";
            else if (bonusCounter >= 3)
                bonusStr = "BONUS: ×2";
            bonusText.setString(bonusStr);
            bonusText.setCharacterSize(14);  
            bonusText.setPosition(400, M*ts + 30);  
            bonusText.setFillColor(Color::Yellow);

            // Draw status text
            window.draw(levelText);
            window.draw(timeText);
            window.draw(movesText);
            window.draw(scoreText);
            window.draw(speedText);
            window.draw(powerupText);
            window.draw(bonusText);
            window.draw(moves2Text);
            window.draw(score2Text);
            window.draw(powerup2Text);

            if (!Game) {
                window.draw(sGameover);
                
                // Auto transition to end menu after 2 seconds
                gameOverTimer += time;
                if (gameOverTimer > 2.0f) {
                    stateofMenu = 3; // Go to end menu
                }
            }
        }

        window.display();
    }

    return 0;
}

//=================================================== MAIN FUNCTION END ============================================================

void circle(int& enemyCount){
    for(int i = 0; i < enemyCount; i++){

    }

}
void zigzag(int &enemyCount){
    for(int i = 0; i < enemyCount; i++){

    }   
}

void drop(int y, int x) {
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (grid[y-1][x] == 0) drop(y-1, x);
    if (grid[y+1][x] == 0) drop(y+1, x);
    if (grid[y][x-1] == 0) drop(y, x-1);
    if (grid[y][x+1] == 0) drop(y, x+1);
}

void drop2(int b, int c) {
    if (grid[b][c] == 0) grid[b][c] = -1;
    if (grid[b-1][c] == 0) drop2(b-1, c);
    if (grid[b+1][c] == 0) drop2(b+1, c);
    if (grid[b][c-1] == 0) drop2(b, c-1);
    if (grid[b][c+1] == 0) drop2(b, c+1);
}

//================================================= BLOCK COUNT FOR PLAYER 1 =======================================================

int countFilledBlocks() {
    int count = 0;
    for (int i = 1; i < M-1; i++) {
        for (int j = 1; j < N-1; j++) {
            if (grid[i][j] == 1) count++;
        }
    }
    return count; 
}

//================================================= BLOCK COUNT FOR PLAYER 2 =======================================================

int countFilledBlocks2() {
    int count2 = 0;
    for (int i = 1; i < M-1; i++) {
        for (int j = 1; j < N-1; j++) {
            if (grid[i][j] == 4) count2++;
        }
    }
    return count2; 
}

//================================================= SAVE SCORE FOR PLAYER 1 ========================================================

void saveScore(string& playerName, int score, const string& difficulty, float timeTaken) {
    ofstream outFile("scores.txt", ios::app);
    if (!outFile) return;
    outFile << "1," 
            << playerName << ',' 
            << score      << ',' 
            << difficulty << ',' 
            << timeTaken  << '\n';
}

//================================================= SAVE SCORE FOR PLAYER 2 ========================================================


void saveScore2(string& playerName2, int score2, string difficulty, float timeTaken) {
    ofstream outFile("scores.txt", ios::app);
    if (!outFile) return;
    outFile << "2,"
            << playerName2 << ','
            << score2       << ','
            << difficulty   << ','
            << timeTaken    << '\n';
}


//==================================================== SCORE LOADING FUNCTION ======================================================
void loadScores() {
    highScoreCount  = 0;
    highScoreCount2 = 0;

    ifstream in("scores.txt");
    if (!in) return;

    string line;
    while (getline(in, line)) {
        // Find the first four commas
        vector<size_t> C;
        for (size_t i = 0; i < line.size() && C.size() < 4; ++i)
            if (line[i] == ',') C.push_back(i);
        if (C.size() < 4) continue;  

        string tag   = line.substr(0, C[0]);            
        string name  = line.substr(C[0]+1, C[1]-C[0]-1);
        int    score = stoi( line.substr(C[1]+1, C[2]-C[1]-1) );
        string diff  = line.substr(C[2]+1, C[3]-C[2]-1);
        float  time  = stof( line.substr(C[3]+1) );

        if (tag == "1") {
            if (highScoreCount < MAX_HIGH_SCORES ||
                score > highScores[MAX_HIGH_SCORES-1].score)
            {
                highScores[highScoreCount < MAX_HIGH_SCORES ? highScoreCount++ : MAX_HIGH_SCORES-1]
                    = PlayerScore(name, score, diff, time);
            }
        }
        else if (tag == "2") {
            if (highScoreCount2 < MAX_HIGH_SCORES ||
                score > highScores2[MAX_HIGH_SCORES-1].score)
            {
                highScores2[highScoreCount2 < MAX_HIGH_SCORES ? highScoreCount2++ : MAX_HIGH_SCORES-1]
                    = PlayerScore(name, score, diff, time);
            }
        }
    }

    
    auto sortDesc = [&](PlayerScore* arr, int cnt){
        for(int i=0;i<cnt-1;i++)
         for(int j=0;j<cnt-i-1;j++)
           if(arr[j].score < arr[j+1].score)
             swap(arr[j],arr[j+1]);
    };
    sortDesc(highScores,  highScoreCount);
    sortDesc(highScores2, highScoreCount2);
}

//========================== SCORE ADDITION FUNCTION FOR PLAYER 1 ================================================================== 

void addScore(string playerName, int score, string difficulty, float gameTime) {
    
    saveScore(playerName, score, difficulty, gameTime);
    
    
    isNewHighScore = false;
    

    loadScores();
    
    // checking for high score
    for (int i = 0; i < highScoreCount; i++) {
        if (highScores[i].playerName == playerName && highScores[i].score == score) {
            
            isNewHighScore = (i == 0);
            break;
        }
    }
}

//========================== SCORE ADDITION FUNCTION FOR PLAYER 2 ================================================================== 

void addScore2(string playerName2, int score2, string difficulty, float gameTime) {
    
    saveScore2(playerName2, score2, difficulty, gameTime);

    
    isNewHighScore2 = false;
    loadScores();

    
    for (int i = 0; i < highScoreCount2; ++i) {
        if (highScores2[i].playerName == playerName2 && highScores2[i].score      == score2) {
            
            isNewHighScore2 = (i == 0);
            break;
        }
    }
}

//=================================================== ALL RIGHTS RESERVED ==========================================================