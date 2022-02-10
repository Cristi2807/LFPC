#include <iostream>
#include <map>
#include <set>
#include <iomanip>
#include <utility>
#include <queue>


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
    unsigned long long pos = to_check.find(final_state);

    if (pos + 1) return to_check + '*';
    else return to_check;
}

bool is_final_state(const string &to_check, const string &final_state) {
    unsigned long long pos = to_check.find(final_state);

    if (pos + 1) return true;
    else return false;

}


struct Transition {

    Transition(string state_1, string via, set<string> state_2) {
        this->state_1 = move(state_1);
        this->state_2 = move(state_2);
        this->via = move(via);
    }

    string state_1;
    string via;
    set<string> state_2;

};

int main() {


    string init_state;
    string final_state;


    cout << "Initial state:";
    cin >> init_state;

    cout << "Final state:";
    cin >> final_state;

    set<string> alfabet;


    cout << "Relations:" << endl;
    string read_relation;

    while (read_relation != "end") {
        getline(cin, read_relation);

        if (read_relation != "end" and !read_relation.empty()) {
            string state_1, state_2, via;
            unsigned long long equal_position, space_position;

            equal_position = read_relation.find('=');
            space_position = read_relation.find(' ');

            state_1 = read_relation.substr(0, equal_position);
            via = read_relation.substr(equal_position + 1, space_position - 1 - equal_position);
            state_2 = read_relation.substr(space_position + 1, read_relation.length() - 1 - space_position);

            relations[{state_1, via}].insert(state_2);
            alfabet.insert(via);
        }
    }


    queue<Transition> queue_new_states;

    for (const auto &i: relations) {
        queue_new_states.push(Transition(i.first.first, i.first.second, i.second));
    }


    while (!queue_new_states.empty()) {

        relations[{queue_new_states.front().state_1, queue_new_states.front().via}] = queue_new_states.front().state_2;

        if (queue_new_states.front().state_2.size() > 1) {
            string new_state = state_name_from_set((queue_new_states.front().state_2));

            for (const auto &j: alfabet) {
                set<string> new_state_set;

                for (const auto &k: queue_new_states.front().state_2) {
                    set_uniune(&new_state_set, &relations[{k, j}]);
                }

                if (relations[{new_state, j}] != new_state_set)
                    queue_new_states.push(Transition(new_state, j, new_state_set));
            }

        }

        queue_new_states.pop();
    }


    set<string> active_states;
    set_active_states(init_state, &active_states);



//   afisare prin tranzitii (q0 -> a q1)
//
//    for (const auto &i: relations) {
//        if (!state_name_from_set(i.second).empty() and active_states.count(i.first.first))
//            cout << format_final_state(i.first.first,final_state) << " ->" << i.first.second << " " << format_final_state(state_name_from_set(i.second),final_state) << endl;
//
//    }



//afisare prin tabel

    int width = 15;

    cout << setw(width) << "";
    for (const auto &i: alfabet) {
        cout << setw(width) << i;
    }
    cout << endl;
    cout << endl;

    for (const auto &i: active_states) {
        cout << setw(width) << format_final_state(i, final_state);
        for (const auto &j: alfabet) {
            if (!relations[{i, j}].empty())
                cout << setw(width) << format_final_state(state_name_from_set(relations[{i, j}]), final_state);
            else
                cout << setw(width) << "-";
        }
        cout << endl;
    }
    cout << endl;


    for (const auto &i: alfabet) {
        if (!relations[{init_state, i}].empty() and
            !is_final_state(state_name_from_set(relations[{init_state, i}]), final_state)) {
            cout << "String  " << i << "  cannot be obtained!";
            break;
        }
    }


    return 0;

}