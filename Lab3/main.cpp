#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <utility>
#include <vector>
#include <regex>

using namespace std;

struct Token {

    Token(string id, string val) {
        this->Identifier = move(id);
        this->Value = move(val);
    }

    string Identifier;
    string Value;


};

int main() {


    map<string, Token> tokenization;


    tokenization.insert({"=", Token("EQUAL", "=")});
    tokenization.insert({"+", Token("PLUS", "+")});
    tokenization.insert({"-", Token("MINUS", "-")});
    tokenization.insert({"*", Token("STAR", "*")});
    tokenization.insert({"/", Token("SLASH", "/")});
    tokenization.insert({"<", Token("LESS", "<")});
    tokenization.insert({">", Token("GREATER", ">")});
    tokenization.insert({",", Token("COMMA", ",")});
    tokenization.insert({";", Token("SEMICOLON", ";")});
    tokenization.insert({"(", Token("LPAREN", "(")});
    tokenization.insert({")", Token("RPAREN", ")")});
    tokenization.insert({"{", Token("LBRACE", "{")});
    tokenization.insert({"}", Token("RBRACE", "}")});
    tokenization.insert({"#", Token("COMM_LN", "#")});
    tokenization.insert({"int", Token("TYPE", "int")});
    tokenization.insert({"float", Token("TYPE", "float")});
    tokenization.insert({"string", Token("TYPE", "string")});
    tokenization.insert({"bool", Token("TYPE", "bool")});
    tokenization.insert({"void", Token("TYPE", "void")});
    tokenization.insert({"char", Token("TYPE", "char")});
    tokenization.insert({"if", Token("IF", "if")});
    tokenization.insert({"then", Token("THEN", "then")});
    tokenization.insert({"else", Token("ELSE", "else")});
    tokenization.insert({"for", Token("FOR", "for")});
    tokenization.insert({"while", Token("WHILE", "while")});
    tokenization.insert({"true", Token("TRUE", "true")});
    tokenization.insert({"false", Token("FALSE", "false")});
    tokenization.insert({"return", Token("RETURN", "return")});
    tokenization.insert({"const", Token("CONST", "const")});
    tokenization.insert({"and", Token("AND", "and")});
    tokenization.insert({"or", Token("OR", "or")});


    vector<Token> tokens;

    ifstream file("Program.txt");
    string code_line;

    while (getline(file, code_line)) {

        if (code_line[0] != '#' and !code_line.empty()) {

            for (const auto &key: tokenization) {
                if (key.first.length() == 1 and !key.second.Identifier.empty()) {
                    size_t found;
                    found = code_line.find(key.first);
                    while (found != string::npos) {
                        code_line.insert(found + key.first.length(), " ");
                        code_line.insert(found, " ");
                        found = code_line.find(key.first, found + 2);
                    }
                }
            }

            istringstream ss(code_line);

            string word;

            while (ss >> word) {

                regex number_format(R"(((?:^|\s)([+-]?[[:digit:]]*\.?[[:digit:]]+)(?=$|\s)))");
                regex identifier_format("^[a-zA-Z_][a-zA-Z0-9_]*$");

                if (tokenization.find(word) != tokenization.end()) tokens.emplace_back(tokenization.at(word));
                else if (regex_match(word, number_format)) tokens.emplace_back(Token("NUMBER", word));
                else if (word[0] == '\"' and word[word.length() - 1] == '\"')
                    tokens.emplace_back(Token("STRING", word));
                else if (word[0] == '\'' and word[word.length() - 1] == '\'') tokens.emplace_back(Token("CHAR", word));
                else if (regex_match(word, identifier_format)) tokens.emplace_back(Token("IDENTIFIER", word));
                else tokens.emplace_back(Token("!__ILLEGAL__!", "Unknown word"));
            }
            tokens.emplace_back(Token("EOLN", "eoln"));
        } else if (code_line[0] == '#') {
            tokens.emplace_back(tokenization.at("#"));
            tokens.emplace_back(Token("EOLN", "eoln"));
        } else if (code_line.empty()) tokens.emplace_back(Token("EOLN", "eoln"));
    }

    tokens.pop_back();
    tokens.emplace_back(Token("EOF", "eof"));

    file.close();


    for (int i = 0; i < tokens.size() - 1; ++i) {
        if (tokens[i].Identifier == "TYPE" and tokens[i + 1].Identifier != "IDENTIFIER") {
            tokens[i + 1].Value = "Wrong identifier declaration. Expected IDENTIFIER, found "+tokens[i+1].Identifier+" "+tokens[i+1].Value;
            tokens[i + 1].Identifier = "!__ILLEGAL__!";
        }
    }

    for (auto &token: tokens)
        cout << token.Identifier << "  " << token.Value << endl;


    return 0;
}