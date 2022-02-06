#include <iostream>
#include <map>
#include <set>


using namespace std;

map<pair<string, string>, set<string> > relations;

void set_uniune(set<string> *a, set<string> *b) {

    for (const auto &i: *b) {
        a->insert(i);
    }

}

string state_name_from_set(const set<string> &a) {
    string name;

    for (const auto &i: a) {
        name += i;
    }
    return name;
}

void set_active_states(const string &state, set<string> *active) {

    if (!active->count(state)) {
        active->insert(state);

        for (const auto &i: relations) {
            if (i.first.first == state && !i.second.empty()) set_active_states(state_name_from_set(i.second), active);
        }
    }
}

string format_final_state(const string &to_check, const string &final_state) {
    int pos = to_check.find(final_state);

    if (pos + 1) return to_check + '*';
    else return to_check;
}


int main() {


    string init_state;
    string final_state;


    cout << "Initial state:";
    cin >> init_state;

    cout << "Final state:";
    cin >> final_state;

    set<string> alfabet;

    cout << "Alfabetul:" << endl;
    string read_alfabet;

    while (read_alfabet != "end") {
        getline(cin, read_alfabet);
        if (read_alfabet != "end" and !read_alfabet.empty()) {
            alfabet.insert(read_alfabet);
        }
    }


    cout << "Relations:" << endl;
    string read_relation;

    while (read_relation != "end") {
        getline(cin, read_relation);

        if (read_relation != "end" and !read_relation.empty()) {
            string state_1, state_2, via;
            int equal_position, space_position;

            equal_position = read_relation.find('=');
            space_position = read_relation.find(' ');

            state_1 = read_relation.substr(0, equal_position);
            via = read_relation.substr(equal_position + 1, space_position - 1 - equal_position);
            state_2 = read_relation.substr(space_position + 1, read_relation.length() - 1 - space_position);

            relations[{state_1, via}].insert(state_2);
        }
    }


    for (const auto &i: relations) {

        if (i.second.size() > 1) {
            string new_state = state_name_from_set((i.second));
            for (const auto &j: alfabet) {
                for (const auto &k: i.second) {

                    set_uniune(&relations[{new_state, j}], &relations[{k, j}]);

                }
            }

        }
    }


    set<string> active_states;
    set_active_states(init_state, &active_states);


    for (const auto &i: relations) {
        if (!state_name_from_set(i.second).empty() and active_states.count(i.first.first))
            cout << format_final_state(i.first.first,final_state) << " ->" << i.first.second << " " << format_final_state(state_name_from_set(i.second),final_state) << endl;

    }




    return 0;

}