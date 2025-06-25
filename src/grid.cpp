#include "grid.h"

namespace Flexomata {

bool Grid::is_initialized() const {
    return are_init_vars(this->get_width(), this->get_height());
}

size_t Grid::get_pixel(const size_t x, const size_t y) const {
    if (!this->is_initialized()) {
        throw std::runtime_error("Attempting to get pixel in a non-initialized Grid.");
    }
    
    const size_t idx = y * width + x;
    return this->data[idx];
}

size_t Grid::get_neighbor(const size_t x, const size_t y, enum Direction dir) const {
    const size_t h = get_height();
    const size_t w = get_width();

    size_t new_x = x;
    size_t new_y = y;

    switch (dir) {
        case TOP_LEFT:
            new_x = (x + w - 1) % w;
            new_y = (y + h - 1) % h;
            break;
        case TOP:
            new_y = (y + h - 1) % h;
            break;
        case TOP_RIGHT:
            new_x = (x + 1) % w;
            new_y = (y + h - 1) % h;
            break;
        case LEFT:
            new_x = (x + w - 1) % w;
            break;
        case RIGHT:
            new_x = (x + 1) % w;
            break;
        case BOTTOM_LEFT:
            new_x = (x + w - 1) % w;
            new_y = (y + 1) % h;
            break;
        case BOTTOM:
            new_y = (y + 1) % h;
            break;
        case BOTTOM_RIGHT:
            new_y = (y + 1) % h;
            new_x = (x + 1) % w;
            break;
    }
    return get_pixel(new_x, new_y);
}

size_t Grid::get_neighbor_count(const size_t x, const size_t y, const size_t of_state) const {
    size_t count = 0;
    for (size_t i = 0; i < 8; ++i) {
        count += (get_neighbor(x, y, static_cast<Direction>(i)) == of_state);
    }
    return count;
}

void Grid::print_data() const {
    const size_t max_j = this->get_height();
    const size_t max_i = this->get_width();

    if (!are_init_vars(max_i, max_j)) {
        throw std::runtime_error("Grid not initialized. Nothing to print.");
    }

    for (size_t j = 0; j < max_j; ++j) {
        for (size_t i = 0; i < max_i; ++i) {
            std::cout << this->get_pixel(i, j) << " ";
        }
        std::cout << "\n";
    }
}

const std::vector<size_t> Grid::get_data() const {
    return this->data;
}

size_t Grid::get_width() const {
    return this->width;
}

size_t Grid::get_height() const {
    return this->height;
}

void Grid::set_data(const std::vector<size_t>& dummy_data) {
    if (!this->is_initialized()) {
        throw std::runtime_error("Attempting to set data in a non-initialized Grid.");
    }

    size_t size = this->height * this->width;

    if (dummy_data.size() < size) {
        throw std::runtime_error("Attempting to load the grid data vector with insufficient cell count.");
    }
    
    data.resize(size); // Not strictly necessary
    data.assign(dummy_data.begin(), dummy_data.begin() + size);
}

void Grid::set_pixel(const size_t x, const size_t y, const size_t value) {
    if (!this->is_initialized()) {
        throw std::runtime_error("Attempting to set pixel in a non-initialized Grid.");
    }

    if (this->width <= x || this->height <= y) {
        throw std::out_of_range("Attempting to assign state value to a non-existent Grid coordinate.");
    }

    const size_t idx = this->width * y + x;

    this->data[idx] = value;
}

void Grid::initialize_grid(const size_t width, const size_t height) {
    if (this->is_initialized()) {
        throw std::runtime_error("Attempting to initialize Grid twice.");
    }

    this->width = width;
    this->height = height;

    this->data.resize(width * height);
    std::fill(this->data.begin(), this->data.end(), 0);
}

/* Not directly initialized to simplify the process of
** passing as a pointer to a config loader object, which
** is expected to handle initialization.
*/
Grid::Grid() {
    this->width = 0;
    this->height = 0;
}

}