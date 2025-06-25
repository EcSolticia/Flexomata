#include "grid.h"
#include "deferred_configloader.h"
#include <string>

namespace Flexomata {

void DeferredConfigLoader::load_config_into_target() const {
    if (!target_grid_ptr) {
        throw std::runtime_error("Cannot load config into non-existent target");
    }

    target_grid_ptr->initialize_grid(intermediary_grid.get_width(), intermediary_grid.get_height());

    target_grid_ptr->set_data(intermediary_grid.get_data());
}

void DeferredConfigLoader::set_config_pixel(const size_t x, const size_t y, const size_t value) {
    intermediary_grid.set_pixel(x, y, value);
}

DeferredConfigLoader::DeferredConfigLoader(const size_t width, const size_t height, Grid* target_grid_ptr) {
    intermediary_grid.initialize_grid(width, height);
    this->target_grid_ptr = target_grid_ptr;
}

}