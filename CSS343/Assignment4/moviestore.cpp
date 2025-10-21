#include "moviestore.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include "classics.h"
#include "comedy.h"
#include "drama.h"
#include "moviefactory.h"
#include "transaction.h"

const static char MEDIA_TYPE = 'D';

void MovieStore::ReadMovies(const std::string& file) {
    std::ifstream in_file(file);
    if (!in_file.is_open()) {
        std::cerr << "Error: Unable to open file " << file << "\n";
        return;
    }
    std::string temp;
    char type = 0;
    int stock = -1;
    std::string director;
    std::string title;
    int year = -1;
    std::shared_ptr<Comedy> comedy;
    std::shared_ptr<Drama> drama;
    std::shared_ptr<Classics> classics;
    while (std::getline(in_file, temp)) {
        std::stringstream tempstream(temp);
        type = temp[0];
        tempstream.ignore();
        tempstream.ignore();
        tempstream >> stock;
        tempstream.ignore();
        tempstream.ignore();
        getline(tempstream, director, ',');
        tempstream.ignore();
        getline(tempstream, title, ',');
        switch (type) {
            case 'F': {
                tempstream >> year;
                comedy = MovieFactory::CreateComedy(MEDIA_TYPE, stock, director,
                                                    title, year);
                inventory_.Add('F', comedy);
                break;
            }
            case 'D': {
                tempstream >> year;
                drama = MovieFactory::CreateDrama(MEDIA_TYPE, stock, director,
                                                  title, year);
                inventory_.Add('D', drama);
                break;
            }
            case 'C': {
                string first;
                string last;
                int month = -1;
                tempstream >> first;
                tempstream >> last;
                tempstream >> month;
                tempstream >> year;
                classics = MovieFactory::CreateClassics(
                    MEDIA_TYPE, stock, director, title,
                    first.append(" " + last), month, year);
                inventory_.Add('C', classics);
                break;
            }
            default: {
                std::cerr << "INVALID MOVIE TYPE: " << type << "\n";
                break;
            }
        }
    }
    in_file.close();
    //FIX SORT
    inventory_.Sort();
}

void MovieStore::ReadCustomers(const std::string& file) {
    std::ifstream in_file(file);
    if (!in_file.is_open()) {
        std::cerr << "Error: Unable to open file " << file << "\n";
        return;
    }
    string temp;
    int id = -1;
    string first;
    string last;
    while (std::getline(in_file, temp)) {
        std::stringstream tempstream(temp);
        tempstream >> id;
        tempstream >> first;
        tempstream >> last;
        customers_.Insert(id, first, last);
    }

    in_file.close();
}

void MovieStore::ReadCommands(const std::string& file) {
    std::ifstream in_file(file);
    if (!in_file.is_open()) {
        std::cerr << "Error: Unable to open file " << file << "\n";
        return;
    }
    string temp;
    char command = 0;
    int id = -1;
    char media = 0;
    char movie = 0;
    string title;
    int year = -1;
    string director;
    int month = -1;
    string first;
    string last;
    string actor;

    while (std::getline(in_file, temp)) {
        stringstream tempstream(temp);

        tempstream >> command;

        switch (command) {
            case 'B':
            case 'R': {
                tempstream >> id;
                if (!customers_.Contains(id)) {
                    cerr << "ID DOES NOT EXIST: " << id << "\n";
                    break;
                } else {
                    tempstream >> media;
                    if (media != 'D') {
                        cerr << "MEDIA DOES NOT EXIST: " << media << "\n";
                        break;
                    } else {
                        tempstream >> movie;
                        switch (movie) {
                            case 'F':
                            case 'D': {
                                tempstream.ignore();
                                if (movie == 'F') {
                                    std::getline(tempstream, title, ',');
                                    tempstream.ignore();
                                    tempstream >> year;
                                    if (!inventory_.Contains(movie, title,
                                                             year)) {
                                        cerr
                                            << "MOVIE DOES NOT EXIST: " << title
                                            << " " << year << "\n";
                                        break;
                                    }
                                } else {
                                    std::getline(tempstream, director, ',');
                                    tempstream.ignore();
                                    std::getline(tempstream, title, ',');
                                    if (!inventory_.Contains(movie, director,
                                                             title)) {
                                        cerr << "MOVIE DOES NOT EXIST: "
                                             << director << " " << title
                                             << "\n";
                                        break;
                                    }
                                }
                                Transaction temp_transaction;
                                if (command == 'B') {
                                    if (movie == 'F') {
                                        temp_transaction = inventory_.Borrow(
                                            movie, title, year, id);
                                    } else {
                                        inventory_.Borrow(movie, director,
                                                          title, id);
                                    }

                                } else {
                                    if (movie == 'F') {
                                        temp_transaction = inventory_.Return(
                                            movie, title, year, id);
                                        if (temp_transaction.GetID() == -1) {
                                            std::cerr
                                                << "ACTION FAILED: " << command
                                                << " " << id << " " << media
                                                << " " << movie << " " << title
                                                << " " << year << "\n";
                                            break;
                                        } else {
                                            transactions_.Insert(
                                                id, temp_transaction);
                                        }
                                    } else {
                                        temp_transaction = inventory_.Return(
                                            movie, director, title, id);
                                        if (temp_transaction.GetID() == -1) {
                                            std::cerr
                                                << "ACTION FAILED: " << command
                                                << " " << id << " " << media
                                                << " " << movie << " "
                                                << director << " " << title
                                                << "\n";
                                            break;
                                        } else {
                                            transactions_.Insert(
                                                id, temp_transaction);
                                        }
                                    }
                                }
                                break;
                            }
                            case 'C': {
                                tempstream >> month;
                                tempstream >> year;
                                tempstream >> first;
                                tempstream >> last;
                                actor = first.append(" " + last);
                                if (!inventory_.Contains(movie, actor, month,
                                                         year)) {
                                    cerr << "MOVIE DOES NOT EXIST: " << title
                                         << " " << actor << " " << month << " "
                                         << year << "\n";
                                    break;
                                } else {
                                    Transaction temp_transaction;
                                    if (command == 'B') {
                                        temp_transaction = inventory_.Borrow(
                                            movie, actor, month, year, id);
                                    } else {
                                        temp_transaction = inventory_.Return(
                                            movie, actor, month, year, id);
                                    }
                                    if (temp_transaction.GetID() == -1) {
                                        std::cerr << "ACTION FAILED" << command
                                                  << " " << id << " " << media
                                                  << " " << movie << " "
                                                  << month << " " << year << " "
                                                  << actor << "\n";
                                        break;
                                    } else {
                                        transactions_.Insert(id,
                                                             temp_transaction);
                                    }
                                }
                                break;
                            }
                            default: {
                                cerr << "MOVIE TYPE DOES NOT EXIST: " << movie
                                     << "\n";
                                break;
                            }
                        }
                    }
                    break;
                }
                case 'I': {
                    inventory_.Print();
                    break;
                }
                case 'H': {
                    tempstream >> id;
                    if (!customers_.Contains(id)) {
                        cerr << "ID DOES NOT EXIST: " << id << "\n";
                        break;
                    } else {
                        transactions_.Print(id);
                        break;
                    }
                }
                default: {
                    cerr << "COMMAND DOES NOT EXIST: " << command << "\n";
                    break;
                }
            }

                in_file.close();
        }
    }
}