#include "media.h"

Media::Media(int stock) : total_stock_(stock), remaining_stock_(stock) {}

bool Media::UpdateStock(int stock) {
    if (remaining_stock_ + stock < 0) {
        return false;
    }

    if (stock < 0) {
        borrowed_ += (stock * -1);
    } else {
        borrowed_ -= stock;
    }
    remaining_stock_ += stock;
    total_stock_ = remaining_stock_ + borrowed_;
    return true;
}

bool Media::InStock() const {
    return remaining_stock_ != 0;
}

bool Media::CheckOut() {
    return UpdateStock(-1);
}

bool Media::Return() {
    return UpdateStock(1);
}

int Media::GetTotalStock() const {
    return total_stock_;
}

int Media::GetRemainingStock() const {
    return remaining_stock_;
}

int Media::GetBorrowedStock() const {
    return borrowed_;
}