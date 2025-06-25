#include "flexomata.h"
#include <algorithm>

void make_palatable(std::vector<size_t>& data) {
    std::replace(data.begin(), data.end(), (size_t)1, (size_t)'#');
    std::replace(data.begin(), data.end(), (size_t)0, (size_t)' ');
}
void print_data_vector(const size_t max_i, const size_t max_j, const std::vector<size_t>& data) {
    for (size_t j = 0; j < max_j; ++j) {
        for (size_t i = 0; i < max_i; ++i) {
            std::cout << (char)data[j * max_i + i];
        }
        std::cout << "\n";
    }
}

int main() {

    try {

        Flexomata::SimulationScene sim = Flexomata::SimulationScene("../config.txt", Flexomata::SimulationScene::construct_from_predefined_path{});
        
        const Flexomata::Grid* grid_ptr = sim.get_grid();

        Flexomata::Types::RuleFunc highlife = [grid_ptr](size_t x, size_t y) -> size_t {
            const size_t neighbor_count = grid_ptr->get_neighbor_count(x, y, 1);
            const size_t alive = grid_ptr->get_pixel(x, y);
            if (alive) {
                return (neighbor_count == 2 || neighbor_count == 3);
            } else {
                return (neighbor_count == 3 || neighbor_count == 6);
            }
        };

        sim.set_rule(highlife);

        while (true) {
            std::cin.get();

            std::vector<size_t> data = grid_ptr->get_data();
            make_palatable(data);

            print_data_vector(grid_ptr->get_width(), grid_ptr->get_height(), data);
            sim.enforce_rule_once();
        }

    } catch (const std::exception& e) {
        Flexomata::Errors::handle_exception(e);
    }

    return 0;
}