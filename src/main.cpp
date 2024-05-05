#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <unordered_map>
#include <cassert>

std::unordered_map<std::string, int> qualification_chances;
int total_possibilities = 0;
std::vector<std::string> teams = {
    "RCB", "CSK", "MI", "GT", "PK", "LSG", "KKR", "RR", "DC", "SRH"
};

struct S_PT_ROW {
    int number;
    std::string team;
    int matches;
    int wins;
    int loses;
    int ties;
    int n_r;
    int points;
    float nrr;

    S_PT_ROW(int n, std::string tm, int m, int w, int l, int t, int nr, int pts, float _nrr) {
        number = n; team = tm; matches = m; wins = w; loses = l; ties = t; n_r = nr; points = pts; nrr = _nrr;
    }
};

struct S_MATCH_ROW {
    int no;
    std::string home_team;
    std::string away_team;
    std::string result;

    int ht_runs;
    int ht_balls;
    int at_runs;
    int at_balls;
};

struct S_NRR_ROW {
    int my_runs = 0;
    int my_balls = 0;
    int opp_runs = 0;
    int opp_balls = 0;
};

std::vector<S_MATCH_ROW> fetching_data();
std::vector<S_PT_ROW> init_points_table();
std::unordered_map<std::string, S_NRR_ROW> init_nrr_details();
void add_result(
    const std::string& home_team, const std::string& away_team, const std::string& result, 
    int my_run, int my_balls, int opp_run, int opp_balls,
    std::vector<std::vector<std::string>>& pointsTable,
    std::unordered_map<std::string, std::vector<int>>& nrr_details
);
void remove_result(
    const std::string& home_team, const std::string& away_team, const std::string& result, 
    int my_run, int my_balls, int opp_run, int opp_balls,
    std::vector<std::vector<std::string>>& pointsTable,
    std::unordered_map<std::string, std::vector<int>>& nrr_details
);
void simulate_qualification(int index, std::vector<S_PT_ROW> &pt, std::vector<S_MATCH_ROW> &matches, std::unordered_map<std::string, S_NRR_ROW>& nrr_details);
void print_qualification_chances(const std::vector<S_PT_ROW> &pt);
std::string print_points_table(std::vector<S_PT_ROW> &pt);


int main() {
    std::vector<S_MATCH_ROW> data = fetching_data();
    std::vector<S_PT_ROW> points_table = init_points_table();
    std::unordered_map<std::string, S_NRR_ROW> nrr_details = init_nrr_details();

    qualification_chances.clear();
    total_possibilities = 0;
    simulate_qualification(0, points_table, data, nrr_details);
    print_qualification_chances(points_table);

    // std::cout << print_points_table(points_table) << std::endl;
    
    return 0;
}

std::vector<S_MATCH_ROW> fetching_data() {
    std::ifstream file("src/data.csv");
    if (!file.is_open()) {
        std::cerr << "Error opening file\n";
        throw;
    }

    std::vector<S_MATCH_ROW> data;
    std::string line;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        S_MATCH_ROW row;
        std::string field;

        if (std::getline(ss, field, ',')) { row.no = stoi(field); }
        if (std::getline(ss, field, ',')) { row.home_team = field; }
        if (std::getline(ss, field, ',')) { row.away_team = field; }
        if (std::getline(ss, field, ',')) { row.result = field; }
        if (std::getline(ss, field, ',')) { row.ht_runs = stoi(field); }
        if (std::getline(ss, field, ',')) { row.ht_balls = stoi(field); }
        if (std::getline(ss, field, ',')) { row.at_runs = stoi(field); }
        if (std::getline(ss, field, ',')) { row.at_balls = stoi(field); }

        data.push_back(row);
    }

    file.close();
    return data;
}

std::string print_points_table(std::vector<S_PT_ROW> &pt) {
    std::string ans;
    std::cout << "+---------+---------+---------+---------+---------+---------+---------+---------+---------+\n";
    std::cout << "|No.      |Team     |Matches  |Win      |Lose     |Tie      |N/R      |Points   |NRR      |\n";


    for (size_t i=0; i<pt.size(); i++) {
        std::cout << "+---------+---------+---------+---------+---------+---------+---------+---------+---------+\n";
        
        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].number;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].team;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].matches;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].wins;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].loses;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].ties;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].n_r;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].points;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << std::fixed << std::setprecision(5) << pt[i].nrr;

        std::cout << "|\n";
    }
    std::cout << "+---------+---------+---------+---------+---------+---------+---------+---------+---------+\n";

    return ans;
}

void add_result(const std::string& home_team, const std::string& away_team, const std::string& result, 
        int ht_runs, int ht_balls, int at_runs, int at_balls,
        std::vector<S_PT_ROW>& pointsTable,
        std::unordered_map<std::string, S_NRR_ROW>& nrr_details
) {
    nrr_details[home_team].my_runs += ht_runs;
    nrr_details[home_team].my_balls += ht_balls;
    nrr_details[home_team].opp_runs += at_runs;
    nrr_details[home_team].opp_balls += at_balls;
    
    nrr_details[away_team].my_runs += at_runs;
    nrr_details[away_team].my_balls += at_balls;
    nrr_details[away_team].opp_runs += ht_runs;
    nrr_details[away_team].opp_balls += ht_balls;

    if (result == "H") {
        assert(ht_runs > at_runs);
    } else if (result == "A") {
        assert(at_runs > ht_runs);
    }

    for (size_t i=0; i<pointsTable.size(); i++) {
        if (pointsTable[i].team == home_team) {
            if (result == "H") {
                pointsTable[i].matches = pointsTable[i].matches + 1;
                pointsTable[i].wins = pointsTable[i].wins + 1;
                pointsTable[i].points = pointsTable[i].points + 2;
                pointsTable[i].nrr = 
                    (nrr_details[home_team].my_runs * 6.0 / nrr_details[home_team].my_balls) - 
                    (nrr_details[home_team].opp_runs * 6.0 / nrr_details[home_team].opp_balls);
            } else if (result == "A") {
                pointsTable[i].matches = pointsTable[i].matches + 1;
                pointsTable[i].loses = pointsTable[i].loses + 1;
                pointsTable[i].nrr = 
                    (nrr_details[home_team].my_runs * 6.0 / nrr_details[home_team].my_balls) - 
                    (nrr_details[home_team].opp_runs * 6.0 / nrr_details[home_team].opp_balls);
            }
        } else if (pointsTable[i].team == away_team) {
            if (result == "A") {
                pointsTable[i].matches = pointsTable[i].matches + 1;
                pointsTable[i].wins = pointsTable[i].wins + 1;
                pointsTable[i].points = pointsTable[i].points + 2;
                pointsTable[i].nrr = 
                    (nrr_details[away_team].my_runs * 6.0 / nrr_details[away_team].my_balls) - 
                    (nrr_details[away_team].opp_runs * 6.0 / nrr_details[away_team].opp_balls);
            } else if (result == "H") {
                pointsTable[i].matches = pointsTable[i].matches + 1;
                pointsTable[i].loses = pointsTable[i].loses + 1;
                pointsTable[i].nrr = 
                    (nrr_details[away_team].my_runs * 6.0 / nrr_details[away_team].my_balls) - 
                    (nrr_details[away_team].opp_runs * 6.0 / nrr_details[away_team].opp_balls);

            }
        }
    }


    sort(pointsTable.begin(), pointsTable.end(), [](const S_PT_ROW& a, const S_PT_ROW& b){
        return a.points == b.points ? a.nrr > b.nrr : a.points > b.points;
    });
    for (size_t i=0; i<pointsTable.size(); i++) {
        pointsTable[i].number = i+1;
    }
}

void remove_result(const std::string& home_team, const std::string& away_team, const std::string& result, 
        int ht_runs, int ht_balls, int at_runs, int at_balls,
        std::vector<S_PT_ROW>& pointsTable,
        std::unordered_map<std::string, S_NRR_ROW>& nrr_details
) {
    nrr_details[home_team].my_runs -= ht_runs;
    nrr_details[home_team].my_balls -= ht_balls;
    nrr_details[home_team].opp_runs -= at_runs;
    nrr_details[home_team].opp_balls -= at_balls;
    
    nrr_details[away_team].my_runs -= at_runs;
    nrr_details[away_team].my_balls -= at_balls;
    nrr_details[away_team].opp_runs -= ht_runs;
    nrr_details[away_team].opp_balls -= ht_balls;


    for (size_t i=0; i<pointsTable.size(); i++) {
        if (pointsTable[i].team == home_team) {
            if (result == "H") {
                pointsTable[i].matches = pointsTable[i].matches - 1;
                pointsTable[i].wins = pointsTable[i].wins - 1;
                pointsTable[i].points = pointsTable[i].points - 2;
                pointsTable[i].nrr = 
                    (nrr_details[home_team].my_runs * 6.0 / nrr_details[home_team].my_balls) - 
                    (nrr_details[home_team].opp_runs * 6.0 / nrr_details[home_team].opp_balls);
            } else if (result == "A") {
                pointsTable[i].matches = pointsTable[i].matches - 1;
                pointsTable[i].loses = pointsTable[i].loses - 1;
                pointsTable[i].nrr = 
                    (nrr_details[home_team].my_runs * 6.0 / nrr_details[home_team].my_balls) - 
                    (nrr_details[home_team].opp_runs * 6.0 / nrr_details[home_team].opp_balls);
            }
        } else if (pointsTable[i].team == away_team) {
            if (result == "A") {
                pointsTable[i].matches = pointsTable[i].matches - 1;
                pointsTable[i].wins = pointsTable[i].wins - 1;
                pointsTable[i].points = pointsTable[i].points - 2;
                pointsTable[i].nrr = 
                    (nrr_details[away_team].my_runs * 6.0 / nrr_details[away_team].my_balls) - 
                    (nrr_details[away_team].opp_runs * 6.0 / nrr_details[away_team].opp_balls);
            } else if (result == "H") {
                pointsTable[i].matches = pointsTable[i].matches - 1;
                pointsTable[i].loses = pointsTable[i].loses - 1;
                pointsTable[i].nrr = 
                    (nrr_details[away_team].my_runs * 6.0 / nrr_details[away_team].my_balls) - 
                    (nrr_details[away_team].opp_runs * 6.0 / nrr_details[away_team].opp_balls);

            }
        }
    }


    sort(pointsTable.begin(), pointsTable.end(), [](const S_PT_ROW& a, const S_PT_ROW& b){
        return a.points == b.points ? a.nrr > b.nrr : a.points > b.points;
    });
    for (size_t i=0; i<pointsTable.size(); i++) {
        pointsTable[i].number = i+1;
    }
}

std::vector<S_PT_ROW> init_points_table() {
    /**
     * No, Team, Matches, Wins, Loss, Tie, N/R, Points
    */
    std::vector<S_PT_ROW> pointsTable;
    for (size_t i=0; i<teams.size(); i++) {
        S_PT_ROW r(i+1, teams[i], 0, 0, 0, 0, 0, 0, 0.0);
        pointsTable.push_back(r);
    }
    return pointsTable;
}

std::unordered_map<std::string, S_NRR_ROW> init_nrr_details() {
    std::unordered_map<std::string, S_NRR_ROW> nrr_details;
    for (const auto& team: teams) {
        S_NRR_ROW r;
        nrr_details[team] = r;
    }
    return nrr_details;
}

void simulate_qualification(int index, std::vector<S_PT_ROW> &pt, std::vector<S_MATCH_ROW> &matches, std::unordered_map<std::string, S_NRR_ROW>& nrr_details) {
    if (index >= 70) {
        total_possibilities += 1;
        for (int i=0; i<4; i++) {
            qualification_chances[pt[i].team]++;
        }
        return;
    }

    std::string home = matches[index].home_team;
    std::string away = matches[index].away_team;
    std::string result = matches[index].result;

    int ht_runs = matches[index].ht_runs;
    int ht_balls = matches[index].ht_balls;
    int at_runs = matches[index].at_runs;
    int at_balls = matches[index].at_balls;

    if (result != "-") {
        add_result(home, away, result, ht_runs, ht_balls, at_runs, at_balls, pt, nrr_details);
        simulate_qualification(index+1, pt, matches, nrr_details);
        return;
    }

    add_result(home, away, "H", 181, 120, 180, 120, pt, nrr_details);
    simulate_qualification(index+1, pt, matches, nrr_details);
    remove_result(home, away, "H", 181, 120, 180, 120, pt, nrr_details);

    add_result(home, away, "A", 180, 120, 181, 120, pt, nrr_details);
    simulate_qualification(index+1, pt, matches, nrr_details);
    remove_result(home, away, "A", 180, 120, 181, 120, pt, nrr_details);
}

void print_qualification_chances(const std::vector<S_PT_ROW> &pt) {
    std::cout << "+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+\n";
    std::cout << "|No.      |Team     |Matches  |Win      |Lose     |Tie      |N/R      |Points   |NRR      |Chances  |\n";

    for (size_t i=0; i<pt.size(); i++) {
        std::cout << "+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+\n";
        
        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].number;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].team;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].matches;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].wins;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].loses;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].ties;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].n_r;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << pt[i].points;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << std::fixed << std::setprecision(5) << pt[i].nrr;

        std::cout << "|";
        std::cout << std::left << std::setw(9);
        std::cout << std::fixed << std::setprecision(5) << (qualification_chances[pt[i].team] * 100 / float(total_possibilities));
        std::cout << "|\n";
    }
    std::cout << "+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+\n";
}

