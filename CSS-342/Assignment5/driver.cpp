#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "bank.h"
#include "bstree.h"
#include "transaction.h"
using std::string;
using std::vector;

vector<string> Split(string input, string delimiter) {
    vector<string> arr;
    string token;
    while (input.find(delimiter) != input.npos) {
        token = input.substr(0, input.find(delimiter));
        arr.push_back(token);
        input.erase(0, input.find(delimiter) + delimiter.length());
    }
    if (input != "") {
        arr.push_back(input);
    }
    return arr;
}

Transaction ParseTransaction(string transaction) {
    vector<string> split = Split(transaction, " ");
    if (split.size() < 2) {
        throw std::invalid_argument(
            "ERROR: Improperly formatted Transaction received");
    }
    switch (split[0].at(0)) {
        case 'O':
            return Transaction(split[0].at(0), stoi(split[1]), split[2],
                               split[3]);
        case 'D':
        case 'W':
            return Transaction(split[0].at(0), stoi(split[1]), stoi(split[2]),
                               stoi(split[3]));
        case 'T':
            return Transaction(split[0].at(0), stoi(split[1]), stoi(split[2]),
                               stoi(split[3]), stoi(split[4]), stoi(split[5]));
        case 'H':
            return Transaction(split[0].at(0), stoi(split[1]));
        case 'F':
            return Transaction(split[0].at(0), stoi(split[1]), stoi(split[2]));
        default:
            std::cerr << "ERROR: Transaction type: " << split[0].at(0)
                      << " does not exist" << std::endl;
            return Transaction();
    }
}

vector<Transaction> ReadTransactions(string file_name) {
    std::ifstream in_file(file_name);
    if (!in_file.is_open()) {
        std::cerr << "Error: Unable to open file " << file_name << std::endl;
        return std::vector<Transaction>();
    }
    vector<Transaction> transactions;
    string temp;
    while (std::getline(in_file, temp)) {
        transactions.push_back(ParseTransaction(temp));
    }
    in_file.close();
    return transactions;
}

int main(int argc, char* argv[]) {
    if (argv[1] != nullptr) {
        string file_name = string(argv[1]);
        vector<Transaction> transactions = ReadTransactions(file_name);
        Bank bank = Bank();
        for (Transaction t : transactions) {
            bank.AddToQueue(t);
        }
        bank.ProcessQueue();
        std::cout << "\n" << std::endl;
        bank.PrintAccounts();
    } else {
        std::cout << "Please provide a transaction file" << std::endl;
    }
    // INCLUDE AT LEAST 10 TEST CASES
    // Send errors to CERR
    return 0;
}
