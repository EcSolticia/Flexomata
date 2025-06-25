#include "flexomata.h"
#include "configloader.h"
#include "deferred_configloader.h"

void Flexomata::Errors::handle_exception(const std::exception& e) {
    if (typeid(e) == typeid(std::invalid_argument)) {
        std::cerr << "invalid_argument: " << e.what() << '\n';
    } else if (typeid(e) == typeid(std::domain_error)) {
        std::cerr << "domain_error: " << e.what() << '\n';
    } else if (typeid(e) == typeid(std::out_of_range)) {
        std::cerr << "out_of_range: " << e.what() << '\n';
    } else if (typeid(e) == typeid(std::logic_error)) {
        std::cerr << "logic_error: " << e.what() << '\n';
    } else if (typeid(e) == typeid(std::runtime_error)) {
        std::cerr << "runtime_error: " << e.what() << '\n';
    } else {
        std::cerr << "std::exception: " << e.what() << '\n';
    }
}

const std::string Flexomata::Arguments::get_first_argument(const int argc, char** argv) {
    std::string config_path;

        if (argc == 1) {
            config_path = "config.txt";
        }

        // assume argv[1] to be the configuration file path
        // currently ignores additional arguments
        if (argc > 2) {
            std::cout << "Flexomata only accepts one command-line argument. The rest will be ignored.\n";
        }

        if (config_path.empty()) {
            config_path = argv[1];
        }

        return config_path;
}

void Flexomata::Arguments::handle_file_existence(const std::string& path) {
    bool exists = std::filesystem::exists(path);
    if (!exists) {
        throw std::runtime_error("No file exists at path " + path);
    }
}

const std::string Flexomata::Arguments::get_valid_argument(const int argc, char** argv) {
    const std::string config_path = get_first_argument(argc, argv);
    handle_file_existence(config_path);
    return config_path;
}

namespace Flexomata {

const Grid* SimulationScene::get_grid() const {
    return &this->grid;
}

DeferredConfigLoader* SimulationScene::get_deferred_configloader() {
    if (!this->deferred_configloader) {
        throw std::runtime_error("To use the deferred configloader, construct the SimulationScene accordingly.");
    }
    return this->deferred_configloader.get();
}

void SimulationScene::set_rule(const Flexomata::Types::RuleFunc& rule) {
    this->rule = rule;
}

void SimulationScene::enforce_rule_once() {
    if (!this->rule) {
        throw std::runtime_error("No rule to enforce.");
    }

    const size_t max_j = grid.get_height();
    const size_t max_i = grid.get_width();

    Grid grid_buffer = grid;

    for (size_t j = 0; j < max_j; ++j) {
        for (size_t i = 0; i < max_i; ++i) {

            grid_buffer.set_pixel(i, j, rule(i, j));

        }
    }

    grid = grid_buffer;
}
void SimulationScene::enforce_rule(const size_t by_steps) {
    for (size_t i = 0; i < by_steps; ++i) {
        enforce_rule_once();
    }
}

SimulationScene::SimulationScene(const int argc, char** argv) {
    this->grid = Grid();

    std::string config_path = Flexomata::Arguments::get_valid_argument(argc, argv);
    ConfigLoader configloader = ConfigLoader(config_path, &this->grid, ConfigLoader::construct_from_path{});
}

SimulationScene::SimulationScene(const std::string& config_text, construct_from_text) {
    this->grid = Grid();
    ConfigLoader configloader = ConfigLoader(config_text, &this->grid, ConfigLoader::construct_from_text{});
}

SimulationScene::SimulationScene(const std::string& config_path, construct_from_predefined_path) {
    this->grid = Grid();
    ConfigLoader configloader = ConfigLoader(config_path, &this->grid, ConfigLoader::construct_from_path{});
}

SimulationScene::SimulationScene(const size_t grid_width, const size_t grid_height, construct_from_deferred_config) {
    this->grid = Grid();
    this->deferred_configloader = std::make_unique<DeferredConfigLoader>(
        grid_width, 
        grid_height, 
        &this->grid
    );
}

SimulationScene::~SimulationScene() = default;

}