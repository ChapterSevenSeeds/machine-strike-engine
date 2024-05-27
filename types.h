#pragma once
#include <array>
#include <string>
#include <stdexcept>
#include "coord.h"

template <typename T>
class BoardType {
    public:
        BoardType() = default;
        BoardType(std::initializer_list<std::initializer_list<T>> list) {
            int row = 0;
            for (auto &row_data : list) {
                int column = 0;
                for (auto &data : row_data) {
                    this->data[row][column] = data;
                    column++;
                }
                row++;
            }
        }

        BoardType(std::initializer_list<T> item) {
            for (int row = 0; row < 8; row++) {
                for (int column = 0; column < 8; column++) {
                    this->data[row][column] = *item.begin();
                }
            }
        }

        std::array<std::array<T, 8>, 8> data;
        T &operator[](Coord coord) {
            return data[coord.row][coord.column];
        }
};

#define todo(msg) throw std::runtime_error(std::string("TODO: ") + msg);