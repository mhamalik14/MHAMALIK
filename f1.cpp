#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <limits>

using namespace std;

// ANSI colors
const string RED = "\033[31m";
const string BLUE = "\033[34m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string RESET = "\033[0m";

int main() {
    size_t trackLength;

    cout << "Welcome to the " << YELLOW << "Turbo Text Grand Prix!" << RESET << "\n";

    // Ask user for track length
    while (true) {
        cout << "Enter the track length (minimum 10): ";
        if (cin >> trackLength && trackLength >= 10) {
            break;
        } else {
            cout << "Invalid input. Please enter a number >= 10.\n";
            cin.clear(); // clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
        }
    }

    int carA = 0, carB = 0;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 2);
    auto randStep = [&]() { return 1 + 2 * dist(gen); };

    cout << "Track length: " << trackLength << "\n\n";

    bool raceOver = false;
    while (!raceOver) {
        carA += randStep();
        carB += randStep();

        string trackA(trackLength, '.');
        string trackB(trackLength, '.');

        if (carA >= static_cast<int>(trackLength)) carA = static_cast<int>(trackLength) - 1;
        if (carB >= static_cast<int>(trackLength)) carB = static_cast<int>(trackLength) - 1;

        // Insert colored cars
        trackA[static_cast<size_t>(carA)] = 'A';
        trackB[static_cast<size_t>(carB)] = 'B';

        // Print tracks with colors
        for (size_t i = 0; i < trackLength; i++) {
            if (i == static_cast<size_t>(carA)) cout << RED << 'A' << RESET;
            else cout << trackA[i];
        }
        cout << GREEN << " |" << RESET << "\n";

        for (size_t i = 0; i < trackLength; i++) {
            if (i == static_cast<size_t>(carB)) cout << BLUE << 'B' << RESET;
            else cout << trackB[i];
        }
        cout << GREEN << " |" << RESET << "\n";

        cout << string(trackLength, '=') << GREEN << "|" << RESET << "\n\n";

        // Check winner
        if (static_cast<size_t>(carA) >= trackLength - 1 && static_cast<size_t>(carB) >= trackLength - 1) {
            cout << "It’s a tie!\n";
            raceOver = true;
        } else if (static_cast<size_t>(carA) >= trackLength - 1) {
            cout << RED << "Farrari wins!" << RESET << "\n";
            raceOver = true;
        } else if (static_cast<size_t>(carB) >= trackLength - 1) {
            cout << BLUE << "McLaren wins!" << RESET << "\n";
            raceOver = true;
        }

        this_thread::sleep_for(chrono::milliseconds(400));
    }

    return 0;
}



