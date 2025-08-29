#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>

using namespace std;

// ANSI colors
const string RED = "\033[31m";
const string BLUE = "\033[34m";
const string GREEN = "\033[32m";
const string MAGENTA = "\033[35m";
const string YELLOW = "\033[33m";
const string RESET = "\033[0m";

// Racer struct
struct Racer {
    char symbol;
    string color;
    int position = 0;
};

int main() {
    cout << " Welcome to the " << YELLOW << "Turbo Text Grand Prix!" << RESET << " \n";

    // Ask mode
    int modeChoice;
    cout << "Choose mode:\n";
    cout << "1. Normal Race (2 racers)\n";
    cout << "2. Chaos Mode (4 racers)\n";
    cout << "Enter choice: ";
    cin >> modeChoice;

    // Ask track length
    size_t trackLength;
    cout << "Enter track length (>= 10): ";
    cin >> trackLength;
    if (trackLength < 10) {
        cerr << "Track length must be at least 10.\n";
        return 1;
    }

    // Setup racers
    vector<Racer> racers;
    if (modeChoice == 2) { // Chaos Mode
        racers = {
            {'A', RED, 0},
            {'B', BLUE, 0},
            {'C', GREEN, 0},
            {'D', MAGENTA, 0}
        };
        cout << " Chaos Mode: 4 racers on the track! \n\n";
    } else { // Normal
        racers = {
            {'A', RED, 0},
            {'B', BLUE, 0}
        };
        cout << " Normal Race: 2 racers ready! \n\n";
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> stepDist(0, 2); // 1, 3, 5 steps
    auto randStep = [&]() { return 1 + 2 * stepDist(gen); };

    uniform_int_distribution<int> commDist(0, 2); // commentary variety

    bool raceOver = false;
    while (!raceOver) {
        // move racers
        for (auto &r : racers) {
            r.position += randStep();
            if (r.position >= static_cast<int>(trackLength))
                r.position = static_cast<int>(trackLength) - 1;
        }

        // draw track
        for (auto &r : racers) {
            string track(trackLength, '.');
            track[static_cast<size_t>(r.position)] = r.symbol;

            for (size_t i = 0; i < trackLength; i++) {
                if (i == static_cast<size_t>(r.position))
                    cout << r.color << r.symbol << RESET;
                else
                    cout << track[i];
            }
            cout << GREEN << " |" << RESET << "\n";
        }

        cout << string(trackLength, '=') << GREEN << "|" << RESET << "\n";

        // Commentary
        Racer *leader = &racers[0];
        bool tie = false;
        for (auto &r : racers) {
            if (r.position > leader->position) {
                leader = &r;
                tie = false;
            } else if (r.position == leader->position && &r != leader) {
                tie = true;
            }
        }

        cout << YELLOW;
        if (tie) {
            cout << "It's neck and neck! \n";
        } else {
            switch (commDist(gen)) {
                case 0: cout << "Car " << leader->symbol << " surges ahead! \n"; break;
                case 1: cout << "Car " << leader->symbol << " is flying! \n"; break;
                case 2: cout << "Car " << leader->symbol << " takes the lead! \n"; break;
            }
        }
        cout << RESET << "\n";

        // Check winners
        vector<Racer*> winners;
        for (auto &r : racers) {
            if (static_cast<size_t>(r.position) >= trackLength - 1) {
                winners.push_back(&r);
            }
        }

        if (!winners.empty()) {
            if (winners.size() > 1) {
                cout << "It's a tie between ";
                for (size_t i = 0; i < winners.size(); i++) {
                    cout << winners[i]->color << winners[i]->symbol << RESET;
                    if (i < winners.size() - 1) cout << " & ";
                }
                cout << "! \n";
            } else {
                cout << winners[0]->color << "Car " << winners[0]->symbol 
                     << RESET << " wins! \n";
            }
            raceOver = true;
        }

        this_thread::sleep_for(chrono::milliseconds(500)); // animation speed
    }

    return 0;
}