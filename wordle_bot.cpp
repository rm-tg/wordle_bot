#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct Node {
    pair<string, int> datum;
    Node* next;
    Node* prev;
};

void calc_chances(Node*);
void play_game(Node*&);
void delete_node(Node*&, Node*&);
bool if_delete_word(string, int*, int*, string);
bool has_repeat(string);

void calc_chances(Node* first) {
    string input_file = "possible_words.txt";
    ifstream fin;
    fin.open(input_file);
    if (!fin.is_open()) {
        cout << "open failed" << endl;
        return;
    }

    vector<string> possible_words;
    string curr_word;
    while (fin.peek() != EOF) {
        getline(fin, curr_word);
        possible_words.push_back(curr_word);
    }

    input_file = "all_eng_words.txt";
    ifstream fin_2;
    fin_2.open(input_file);
    if (!fin_2.is_open()) {
        cout << "open failed" << endl;
        return;
    }
    /*
    vector<string> all_eng_words;
    while (fin) {
        getline(fin, curr_word);
        all_eng_words.push_back(curr_word);
    }
    all_eng_words.pop_back();
    */
    int count[26][5] = {0};
    while (fin_2.peek() != EOF) {
        getline(fin_2, curr_word);
        for (int i = 0; i < 5; i++) {
            count[curr_word[i] - 97][i]++;
        }
    }
    //cout << count;
    Node* itr = first;
    int curr_score = 0;
    for (int i = 0; i < possible_words.size(); i++) {
        for (int j = 0; j < 5; j++) {
            curr_score += count[possible_words[i][j] - 97][j];
        }
        itr->datum = { possible_words[i], curr_score };
        itr->next = new Node();
        itr->next->prev = itr;
        itr = itr->next;
        curr_score = 0;
    }
    itr = itr->prev;
    delete itr->next;
    itr->next = nullptr;

    /*
    while (first != nullptr) {
        cout << first->datum.first << " " << first->datum.second << endl;
        first = first->next;
    }
    */
    /*
    int curr_score = 0;
    int max_score = 0;
    string max_word;
    for (int i = 0; i < possible_words.size()-1; i++) {
        for (int j = 0; j < 5; j++) {
            curr_score += count[possible_words[i][j]-97][j];
        }
        if (curr_score > max_score) {
            max_score = curr_score;
            max_word = possible_words[i];
        }
        fout << curr_score << endl;
        curr_score = 0;
    }
    cout << max_word << " " << max_score << endl;
    */
}

void play_game(Node*& scores) {
    //0 for black, 1 for green, 2 for yellow
    string input;
    //last word
    string word = "serai";
    cout << word << endl;
    //yellow
    int include[26] = { 0 };
    for (int i = 0; i < 5; i++) {
        cin >> input;
        //black
        int exclude[26] = { 0 };
        //green
        string templt = "     ";

        for (int j = 0; j < input.size(); j++) {
            if (input[j] == '0') {
                exclude[word[j]-97] = 1;
            } else if (input[j] == '1') {
                templt[j] = word[j];
            } else {
                include[word[j]-97] = 1;
            }
        }
        Node* itr = scores;
        Node* max_score = new Node();
        *max_score = { { "     ", -1 }, nullptr, nullptr };
        Node* max_score_no_dul = new Node();
        *max_score_no_dul = { { "     ", -1 }, nullptr, nullptr };
        //cout << include << endl;
        while (itr != nullptr) {
            //cout << itr->datum.first << endl;
            if (if_delete_word(itr->datum.first, exclude, include, templt)) {
                delete_node(itr, scores);
            } else {
                //cout << include << endl;
                if (!has_repeat(itr->datum.first)) {
                    if (itr->datum.second > max_score_no_dul->datum.second) {
                        max_score_no_dul = itr;
                    }
                }
                if (itr->datum.second > max_score->datum.second) {
                    max_score = itr;
                }
                itr = itr->next;
            }
        }
        //cout << include << endl;
        if (max_score_no_dul->datum.second != -1) {
            word = max_score_no_dul->datum.first;
            delete_node(max_score_no_dul, scores);
        } else {
            word = max_score->datum.first;
            delete_node(max_score, scores);
        }
        cout << word << endl;
        Node* first = scores;
        /*
        while (first != nullptr) {
            cout << first->datum.first << " " << first->datum.second << endl;
            first = first->next;
        }
        */
    }
}

void delete_node(Node* &itr, Node* &scores) {
    Node* del = itr;
    if (itr->prev != nullptr) {
        itr->prev->next = itr->next;
    }
    if (itr->next != nullptr) {
        itr->next->prev = itr->prev;
    }
    if (itr->prev == nullptr) {
        scores = scores->next;
    }
    itr = itr->next;
    delete del;
}

bool if_delete_word(string word, int* exclude, int*include_src, string templt) {
    int include[26];
    for (int i = 0; i < 26; i++) {
        include[i] = include_src[i];
    }
    for (int i = 0; i < word.size(); i++) {
        //check for green
        if (templt[i] != ' ' && word[i] != templt[i]) {
            return true;
        }
        //check for black
        else if (exclude[word[i] - 97] == 1) {
            return true;
        }
        //check for yellow
        include[word[i] - 97] = 0;
    }
    for (int i = 0; i < 26; i++) {
        if (include[i] == 1) {
            return true;
        }
    }
    return false;
}

bool has_repeat(string word) {
    int letters[26] = { 0 };
    for (int i = 0; i < 5; i++) {
        if (letters[word[i] - 97] != 0) {
            return true;
        } else {
            letters[word[i] - 97] = 1;
        }
    }
    return false;
}

int main() {
    Node* scores = new Node();
    calc_chances(scores);
    play_game(scores);
    //cout << has_repeat("abbdf");
    /*
    int exclude[26] = { 0 };
    exclude[0] = 0;
    exclude[1] = 0;
    int include[26] = { 0 };
    include[4] = 1;
    include[6] = 1;
    string templt = "     ";
    cout << if_delete_word("abcde", exclude, include, templt);
    cout << if_delete_word("aeghj", exclude, include, templt);
    */
    return 0;
}