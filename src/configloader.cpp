#include "configloader.h"
#include "grid.h"

namespace Flexomata {

void ConfigLoader::load_grid_data(const std::string& stringset, Grid* grid_ptr) const {
    bool reading_line = false;
    size_t reading_x = 0;
    size_t reading_y = 0;

    std::stringstream ss_config_text(stringset);
    std::string line;

    std::vector<size_t> dummy_data;

    while (std::getline(ss_config_text, line)) {
        if (reading_line) {
            if (line == "END_GRID") {
                reading_line = false;
                continue;
            }
            
            std::stringstream ss_tok(line);
            std::string tok;

            size_t buffer_reading_x = 0;

            while (std::getline(ss_tok, tok, ' ')) {
                
                if (tok.empty()) {
                    continue;
                }

                dummy_data.push_back(std::stoi(tok));
                //grid_ptr->set_pixel(reading_x, reading_y, std::stoi(tok));

                ++buffer_reading_x;
            }

            if (buffer_reading_x != reading_x && reading_x) {
                throw std::logic_error("Inconsistent number of columns.");
            }

            reading_x = buffer_reading_x;

            ++reading_y;
            continue;
        }

        if (line == "GRID") {
            reading_line = true;
        }
    }

    grid_ptr->initialize_grid(reading_x, reading_y);
    grid_ptr->set_data(dummy_data);

}

ConfigLoader::ConfigLoader(const std::string& config_path, Grid* grid_ptr, construct_from_path) {
    std::string config_file_text;

    std::ifstream config_file(config_path);

    std::string line;

    while (std::getline(config_file, line)) {
        config_file_text += line + "\n";
    }

    this->load_grid_data(config_file_text, grid_ptr);
}

ConfigLoader::ConfigLoader(const std::string& config_text, Grid* grid_ptr, construct_from_text) {
    this->load_grid_data(config_text, grid_ptr);
}

}