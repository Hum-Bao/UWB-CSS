#include "moviefactory.h"
#include <memory>
#include <string>
#include <utility>
#include "classics.h"
#include "comedy.h"
#include "drama.h"

std::shared_ptr<Comedy> MovieFactory::CreateComedy(char media, int stock,
                                                   std::string director,
                                                   std::string title,
                                                   int year) {
    return std::make_shared<Comedy>(media, stock, std::move(director),
                                    std::move(title), year);
}

std::shared_ptr<Drama> MovieFactory::CreateDrama(char media, int stock,
                                                 std::string director,
                                                 std::string title, int year) {
    return std::make_shared<Drama>(media, stock, std::move(director),
                                   std::move(title), year);
}

std::shared_ptr<Classics> MovieFactory::CreateClassics(char media, int stock,
                                                       std::string director,
                                                       std::string title,
                                                       std::string actor,
                                                       int month, int year) {
    return std::make_shared<Classics>(media, stock, std::move(director),
                                      std::move(title), std::move(actor), month,
                                      year);
}
