// https://en.wikipedia.org/wiki/Rule_30

#include "flexomata.h"

namespace Rule30Helpers {
    const char state_to_char(const size_t state) {
        return (state == 1) ? '0' : '.';
    }

    void print_line(const std::vector<size_t>& data) {
        std::string line;
        for (size_t pixel: data) {
            line = line + state_to_char(pixel);
        }

        std::cout << line << std::endl;
    }
}

int main() {

    std::string config_text = "GRID\n"
                              "0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0\nEND_GRID";


    try {
        Flexomata::SimulationScene sim = Flexomata::SimulationScene(config_text, Flexomata::SimulationScene::construct_from_text{});

        const Flexomata::Grid* grid_ptr = sim.get_grid();

        Flexomata::Types::RuleFunc rule30 = [grid_ptr](size_t x, size_t y) -> size_t {
            const size_t left = grid_ptr->get_neighbor(x, y, Flexomata::Grid::LEFT);
            const size_t right = grid_ptr->get_neighbor(x, y, Flexomata::Grid::RIGHT);
            const size_t center = grid_ptr->get_pixel(x, y);

            return (left == !(right || center));
        };

        sim.set_rule(rule30);

        int steps_left = 14;

        while (--steps_left >= 0) {

            Rule30Helpers::print_line(grid_ptr->get_data());
            
            sim.enforce_rule_once();
        }
        
    } catch (const std::exception& e) {
        Flexomata::Errors::handle_exception(e);
    }

    // Optional for interactive shells. Ensures that the console window on Windows does not close immediately.
    std::cin.get();

    return 0;
}