#include <iostream>
#include <vector>
#include <fstream>
#include <string>


using namespace std;
ifstream file("data/myfile.csv");

struct  team {
    string name ;
    string localtown;
    string stadium;
};
struct Match {
    string hometeam;
    string awayteam;
    string localtown;
    string stadium;
    int leg;
    int weekend;
};
vector<team> readTeamsFromCSV(const string& filename) {
    vector<team> teams;

    if (!file.is_open()) {
        cerr << "Failed to open file!" << endl;
        return teams;
    }

    string line, name, localTown, stadium;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);

        name = line.substr(0, pos1);
        localTown = line.substr(pos1 + 1, pos2 - pos1 - 1);
        stadium = line.substr(pos2 + 1);

        teams.push_back({name, localTown, stadium});
    }

    file.close();
    return teams;
}
vector<Match> generatematches(vector<team>& teams) {
    vector<Match> fixtures; // Holds the generated fixtures
    int numTeams = teams.size(); // Total number of teams
    int weekendCounter = 1;      // Start from weekend #1

    for (int i = 0; i < numTeams; ++i) {
        for (int j = i + 1; j < numTeams; ++j) {
            fixtures.push_back({teams[i].name, teams[j].name, teams[i].stadium, teams[i].localtown, 1, weekendCounter});
            fixtures.push_back({teams[j].name, teams[i].name, teams[j].stadium, teams[j].localtown, 2, weekendCounter + (numTeams - 1)});

            if (fixtures.size() % 2 == 0) {
                weekendCounter++;
            }
        }
    }

    return fixtures;
}

int main() {
    // Load teams from the CSV file
    string csvFilename = "data/myfile.csv";  // Make sure the file path is correct
    vector<team> teams = readTeamsFromCSV(csvFilename);

    // Check if teams were loaded
    if (teams.empty()) {
        cerr << "No teams found in the CSV file!" << endl;
        return 1;
    }

    // Generate fixtures based on the teams
    vector<Match> fixtures = generatematches(teams);

    // Print the generated fixtures
    for (const auto& match : fixtures) {
        cout << "Weekend " << match.weekend
             << ": " << match.hometeam
             << " vs " << match.awayteam
             << " at " << match.stadium
             << " (Leg " << match.leg << ")\n";
    }

    return 0;
}
void saveFixturesToCSV(const string& filename, const vector<Match>& fixtures) {
    ofstream file(filename);

    if (!file.is_open()) {
        cerr << "Failed to open file for writing!" << endl;
        return;
    }

    file << "Home Team,Away Team,Stadium,Local Town,Leg,Weekend\n";
    for (const auto& fixture : fixtures) {
        file << fixture.hometeam << "," << fixture.awayteam << ","
             << fixture.stadium << "," << fixture.localtown << ","
             << fixture.leg << "," << fixture.weekend << "\n";
    }

    file.close();
}