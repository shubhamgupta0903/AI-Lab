#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

class Board {
public:
    char board[9] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };

    void display() {
        cout << "\n 0 | 1 | 2     " << board[0] << " | " << board[1] << " | " << board[2] << "\n";
        cout << "---+---+---   ---+---+---\n";
        cout << " 3 | 4 | 5     " << board[3] << " | " << board[4] << " | " << board[5] << "\n";
        cout << "---+---+---   ---+---+---\n";
        cout << " 6 | 7 | 8     " << board[6] << " | " << board[7] << " | " << board[8] << "\n\n";
    }

    bool validMove(int move) {
        return (move >= 0 && move < 9 && board[move] == ' ');
    }

    void playMove(int move, char marker) {
        board[move] = marker;
    }

    bool winning() {
        // Check all winning combinations
        return ((board[0] != ' ' && ((board[0] == board[1] && board[1] == board[2]) ||
                                     (board[0] == board[3] && board[3] == board[6]) ||
                                     (board[0] == board[4] && board[4] == board[8]))) ||
                (board[4] != ' ' && ((board[1] == board[4] && board[4] == board[7]) ||
                                     (board[3] == board[4] && board[4] == board[5]) ||
                                     (board[2] == board[4] && board[4] == board[6]))) ||
                (board[8] != ' ' && ((board[6] == board[7] && board[7] == board[8]) ||
                                     (board[2] == board[5] && board[5] == board[8]))));
    }

    bool draw() {
        return count(begin(board), end(board), ' ') == 0;
    }

    string boardString() {
        return string(board, 9);
    }
};

class MenacePlayer {
public:
    unordered_map<string, vector<int>> matchboxes;
    vector<pair<string, int>> movesPlayed;
    int numWin = 0, numDraw = 0, numLose = 0;

    void startGame() {
        movesPlayed.clear();
    }

    int getMove(Board& board) {
        string boardStr = board.boardString();
        if (matchboxes.find(boardStr) == matchboxes.end()) {
            vector<int> newBeads;
            for (int i = 0; i < 9; ++i)
                if (board.board[i] == ' ') newBeads.push_back(i);
            matchboxes[boardStr] = newBeads;
        }

        vector<int>& beads = matchboxes[boardStr];
        if (!beads.empty()) {
            int move = beads[rand() % beads.size()];
            movesPlayed.push_back({ boardStr, move });
            return move;
        } else {
            return -1; // No valid move
        }
    }

    void winGame() {
        for (auto& move : movesPlayed) {
            matchboxes[move.first].push_back(move.second);
            matchboxes[move.first].push_back(move.second);
            matchboxes[move.first].push_back(move.second);
        }
        ++numWin;
    }

    void drawGame() {
        for (auto& move : movesPlayed) {
            matchboxes[move.first].push_back(move.second);
        }
        ++numDraw;
    }

    void loseGame() {
        for (auto& move : movesPlayed) {
            auto& beads = matchboxes[move.first];
            beads.erase(remove(beads.begin(), beads.end(), move.second), beads.end());
        }
        ++numLose;
    }

    void printStats() {
        cout << "Learned " << matchboxes.size() << " boards\n";
        cout << "Wins: " << numWin << ", Draws: " << numDraw << ", Losses: " << numLose << "\n";
    }
};

void playGame(MenacePlayer& first, MenacePlayer& second) {
    first.startGame();
    second.startGame();
    Board board;
    bool gameEnd = false;

    while (!gameEnd) {
        // First player's move
        int move = first.getMove(board);
        if (move == -1) {
            second.winGame();
            first.loseGame();
            gameEnd = true;
        } else {
            board.playMove(move, 'X');
            if (board.winning()) {
                first.winGame();
                second.loseGame();
                gameEnd = true;
            } else if (board.draw()) {
                first.drawGame();
                second.drawGame();
                gameEnd = true;
            }
        }

        if (gameEnd) break;

        // Second player's move
        move = second.getMove(board);
        if (move == -1) {
            first.winGame();
            second.loseGame();
            gameEnd = true;
        } else {
            board.playMove(move, 'O');
            if (board.winning()) {
                second.winGame();
                first.loseGame();
                gameEnd = true;
            } else if (board.draw()) {
                first.drawGame();
                second.drawGame();
                gameEnd = true;
            }
        }
    }
}

int main() {
    srand(time(0));

    MenacePlayer menace1, menace2;

    // Train MENACE by having it play against itself 1000 times
    for (int i = 0; i < 1000; ++i) {
        playGame(menace1, menace2);
    }

    menace1.printStats();
    menace2.printStats();

    return 0;
}
