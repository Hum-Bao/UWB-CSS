/**
 *  Media class - interface class used to store and manage the stock of a movie type of a movie
 *
 * @author Kyle Ricks
 * @date 28 May 2024
 *
 * A movie has at lease one media type (DVD, blue-ray, etc.) with a corrosponding stock
 */

#ifndef MEDIA_H_
#define MEDIA_H_

class Media {
 public:
    explicit Media(int stock);
    bool UpdateStock(int stock);

    [[nodiscard]] bool InStock() const;
    bool CheckOut();
    bool Return();

    [[nodiscard]] int GetTotalStock() const;
    [[nodiscard]] int GetRemainingStock() const;
    [[nodiscard]] int GetBorrowedStock() const;

 private:
    int total_stock_;
    int remaining_stock_;
    int borrowed_ = 0;
};
#endif  // MEDIA_H_
