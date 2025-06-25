#pragma once

#include <iostream> // For terminal I/O
#include <stdexcept> // For error-handling
#include <string>
#include <filesystem>
#include <memory>
#include <optional>

#include "types.h"

#include "grid.h"

namespace Flexomata {
    namespace Arguments {

        const std::string get_first_argument(const int argc, char** argv);

        void handle_file_existence(const std::string& path);

        const std::string get_valid_argument(const int argc, char** argv);

    }
}

namespace Flexomata {
    namespace Errors {
        void handle_exception(const std::exception& e);
    }
}

namespace Flexomata {

class DeferredConfigLoader;

class SimulationScene {

    Grid grid;
    std::unique_ptr<DeferredConfigLoader> deferred_configloader;

    FlexomataTypes::RuleFunc rule;

public:
    const Grid* get_grid() const;
    DeferredConfigLoader* get_deferred_configloader();

    void enforce_rule_once();
    void enforce_rule(const size_t by_steps);

    void set_rule(const FlexomataTypes::RuleFunc& rule);

    SimulationScene(const int argc, char** argv);

    struct construct_from_text{};
    struct construct_from_predefined_path{};

    SimulationScene(const std::string& config_text, construct_from_text);
    SimulationScene(const std::string& config_path, construct_from_predefined_path);

    struct construct_from_deferred_config{};

    SimulationScene(const size_t grid_width, const size_t grid_height, construct_from_deferred_config);

    ~SimulationScene();
};

}