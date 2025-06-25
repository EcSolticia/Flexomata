#include "flexomata.h" // the only header you have to directly include

int main() {
    // Specify the initial configuration
    // This is a "glider" in Conway's Game of Life
    // https://conwaylife.com/wiki/Glider
    const std::string config_text = "GRID\n"
                                    "0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                                    "0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                                    "0 0 0 1 0 0 0 0 0 0 0 0 0 0\n"
                                    "0 0 0 0 1 0 0 0 0 0 0 0 0 0\n"
                                    "0 0 1 1 1 0 0 0 0 0 0 0 0 0\n"
                                    "0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                                    "0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                                    "0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                                    "0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                                    "0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                                    "0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"
                                    "0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";                                    

    try {

        // Create the entry point of the simulation and the interface
        Flexomata::SimulationScene sim = Flexomata::SimulationScene(config_text, Flexomata::SimulationScene::construct_from_text{});

        // Access the Grid object managed by the simulation handler, read-only
        const Flexomata::Grid* grid_ptr = sim.get_grid();

        // Represent the rules of Conway's Game of Life
        // https://conwaylife.com/wiki/Conway%27s_Game_of_Life#Rules
        Flexomata::Types::RuleFunc game_of_life = [grid_ptr](const size_t x, const size_t y) -> size_t {
            const size_t neighbor_count = grid_ptr->get_neighbor_count(x, y, 1);
            const size_t alive = grid_ptr->get_pixel(x, y);
            if (alive) {
                return (neighbor_count == 2 || neighbor_count == 3);
            } else {
                return (neighbor_count == 3);
            }
        };

        // Let the simulation know of the rule to enforce
        sim.set_rule(game_of_life);

        while (true) {
            // Do not execute the following blocks until the return key is pressed
            std::cin.get();
        
            // Simulate and present next step
            sim.enforce_rule_once();
            grid_ptr->print_data();
        }

    } catch (const std::exception& e) {
        // Self-explanatory: handle exceptions
        Flexomata::Errors::handle_exception(e);
    }

    return 0;
}