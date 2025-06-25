#pragma once
#include <iostream>
#include <vector>
#include <functional>

namespace Flexomata {

class Grid {
    size_t width;
    size_t height;

    std::vector<size_t> data;

    static bool are_init_vars(const size_t width, const size_t height) {
        return (width && height);
    }

public:
    enum Direction {
        TOP_LEFT = 0,
        TOP = 1,
        TOP_RIGHT = 2,
        LEFT = 3,
        RIGHT = 4,
        BOTTOM_LEFT = 5,
        BOTTOM = 6,
        BOTTOM_RIGHT = 7
    };

    bool is_initialized() const;

    size_t get_width() const;
    size_t get_height() const;

    void print_data() const;
    const std::vector<size_t> get_data() const;

    // Use post-init
    void set_data(const std::vector<size_t>& dummy_data);
    
    size_t get_pixel(const size_t x, const size_t y) const;
    void set_pixel(const size_t x, const size_t y, const size_t value);

    size_t get_neighbor(const size_t x, const size_t y, enum Direction dir) const;
    
    size_t get_neighbor_count(const size_t x, const size_t y, const size_t of_state) const;

    // Use pre-init
    void initialize_grid(const size_t width, const size_t height);

    Grid();
};

}