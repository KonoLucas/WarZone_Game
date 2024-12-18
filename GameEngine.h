#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include <iostream>
#include <vector>
#include <map>  // Include map for temporary storage of reinforcements

#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "CommandProcessor.h"
#include "LoggingObserver.h"


class CommandProcessor;

/*-------------------- Phases for Part 3 -------------------------*/
extern const std::string START;
extern const std::string MAPLODADED;
extern const std::string PLAYERSADDED;
extern const std::string MAPVALIDATED;
extern const std::string ASSIGN_REINFORCEMENT;
extern const std::string ISSUE_ORDERS;
extern const std::string EXECUTE_ORDERS;
extern const std::string WIN;
/*---------------------------------------------------------------*/

/*---Assginment 3 Part 2 TournamentParameters definition---*/
struct TournamentParameters {
    vector<string> mapFiles;
    vector<string> playerStrategies;// should save players (with strategy)in playerlist
    int numberOfGames;
    int maxTurns;
};

class GameEngine : public Subject, public ILoggable{
private:
    

public:

    Map* selectedMap;                // The game map
    std::vector<Player*> playerList; // List of players
    Deck deck;                       // The deck of cards for the game
    vector <Player*> eliminatedPlayers;
    Player *currentPlayer;
    vector <Player*> playerOder;
    string currentState;  // The current state of the game added by Yuqian Cai
    CommandProcessor *commandProcessor;

    /*---Assginment 3 Part 2 attributes---*/
    TournamentParameters params;
    vector<vector<string>> tournamentResults; // Simulated results data
    vector<string> mapResults;

    bool tournamentMode = false;
    bool isDraw = false;




    //Constructor && Destructor
    GameEngine();  // Constructor to initialize the game
    ~GameEngine(); // Destructor to clean up resources
    void setProcessor(CommandProcessor *processor);
     // Method to get the player list for testing
    std::vector<Player*> getPlayerList() const; 

    // Handles the gameplay phase (assigning reinforcement, issuing orders, executing orders)
    void playGame();

    // Function to draw and play cards
    void playCards(Player* player);

    /*  --------------  Game state part ------------------*/
    // Get the current game state as a string
    std::string getCurrentState();

    //setter
    void setcurrentState(std::string newGameState);
  
    // Helper function to transition between states
    void transition(const std::string& newState);
    void setMap(Map *selectedMap);


    /* Assignment 2 -------------- Part 2 ------------------*/
    // By lucasS
    // Additional game setup and management
    
    
    
    GameEngine* startupPhase();              
    // Handles the startup phase commands and game setup
    // 1) use the loadmap <filename> command to select a map from a list of map files as stored in a directory,
    // which results in the map being loaded in the game.
    // 2) use the validatemap command to validate the map (i.e. it is a connected graph, etc – see assignment 1).
    // 3) use the addplayer <playername> command to enter players in the game (2-6 players)

    void gamestart(GameEngine &game); // Handles the game start operations after setup is complete
    // a) fairly distribute all the territories to the players
    // b) determine randomly the order of play of the players in the game
    // c) give 50 initial army units to the players, which are placed in their respective reinforcement pool
    // d) let each player draw 2 initial cards from the deck using the deck’s draw() method
    void AssignTerritories();         // Assigns territories to players
    void assignTerritoryToPlayer(Territory* territory, Player* player);
    void randomizeOrderOfPlay();      // Randomly determines the order of play for players


    /* End  -------------- Part 2 ------------------*/





    /* -------------- Part 3 ------------------*/
    Player* winner;                  // Pointer to the player who won the game
    // Game phases
    void reinforcementPhase();        // Reinforcement phase of the game
    void issueOrdersPhase();          // Issuing orders phase of the game
    void executeOrdersPhase();        // Execution of orders phase
    void resetGame(); //Not sure if it's part of MainLoop...
    void removeEliminatedPlayers(); //Helper function for mainGameLoop

    std::map<Player*, int> savedReinforcements;
    void removeEliminatedPlayer();
    // Main game loop
    void mainGameLoop();            // Main game loop that handles phase transitions and checks win condition
     /* -------------- End of P3 ------------------*/


    // Command processing
    void handleUserCommand(const std::string& command, GameEngine &game);  // Processes user commands

    // Print and display
    void printWelcomeMessage();       // Prints the welcome message when the game starts
    //Part5: Override the stringToLog function from ILoggable
    string stringToLog() const override;

    /*--------------Assignement3 _ Part2-----------------*/
    void startTournament(const TournamentParameters& params);
    string displayTournamentResults() const;
    bool isTournamentMode() const;
    void setTournamentMode(bool mode);
    // void updateTournamentResults(const vector<vector<string>>& newResults) ;
    void initializeTournamentPlayers(const vector<string>& strategies);
    void setupTournament(const TournamentParameters& params) ;
};
#endif // GAMEENGINE_H
