# Usage Documentation

[Flexomata](https://github.com/EcSolticia/Flexomata) is a simple framework for handling cellular automata simulation in C++ as a static library. The user may provide a "rule function" to operate per each grid cell along with an initial configuration. Flexomata can thereafter apply the rule to each cell and offers access to the evolved state of the grid.

## Accessible Classes and Namespaces

### `Flexomata` (Namespace)
Encapsulates everything pertaining to the library.

Classes within the namespace include:

- `SimulationScene`
- `Grid`
- `DeferredConfigLoader`
- `ConfigLoader` (an internal class used to handle config loading)

### `SimulationScene` (`flexomata.h`)

Creates a separate "scene" for a particular simulation, with predefined Grid dimensions and an initial configuration. Is the entry point for all pertinent simulation functionalities. Other accessible classes can only be accessed through an instance of `SimulationScene`.

#### Accessible Member Functions
- `const Grid* get_grid() const`:
Get access to the associated `Grid` object.
- `DeferredConfigLoader* get_deferred_configloader()`:
Get access to the `DeferredConfigLoader` object. Can be used to specify the config programmatically for it to be manually loaded through `DeferredConfigLoader::load_config_into_target`.
- `void set_rule(const Flexomata::Types::RuleFunc& rule)`:
Set the simulation rule. This will determine the specific cellular automata that is being simulated.
- `void enforce_rule_once()`:
Apply the simulation rule to the grid exactly once.
- `void enforce_rule(const size_t by_steps)`:
Apply the simuation rule to the grid exactly `by_steps` times.
- `SimulationScene(const int argc, char** argv)`:
Constructor to initialize the scene using initial configuration specified in a file from the terminal, specifically the second argument. Example: `./FlexomataApp path/to/config/file.txt`. In the absence of the argument, Flexomata looks for `config.txt` in the current working directory, and if it exists, attempts to load the initial configuration from that.

- `SimulationScene(const std::string& config_text, construct_from_text)`: 
Constructor to initialize the scene using initial configuration specified in a string.
- `SimulationScene(const std::string& config_path, construct_from_predefined_path)`:
Constructor to initialize the scene using initial configuration from a predefined file (relative to the current working directory of the terminal).
- `SimulationScene(const size_t grid_width, const size_t grid_height, construct_from_deferred_config)`:
Constructor to initialize the scene without a predefiend configuration. Creates a `DeferredConfigLoader` object and allows access to it through `SimulationScene::get_deferred_configloader`.

#### Notes
- `SimulationScene` manages the memory of pertinent `Grid` and `DeferredConfigLoader` objects automatically. The former is currently stack-allocated. The latter is heap-allocated through a smart pointer, and hence is automatically freed as the `SimulationScene` object goes out of scope.
- To use Flexomata through the `SimulationScene` class, you have to include and only include `flexomata.h` from the include directory. 
- It may be possible to achieve a lower-level control over the behavior of the simulation by accessing the other classes more directly, but that is not tested. Furthermore, this is not something Flexomata is designed for and around, and hence is not documented as of now.

### `Types` (`types.h`)
Namespace for Flexomata-specific types. Defined within the scope of `Flexomata` as `Flexomata::Types`.

- `typedef std::function<size_t(size_t, size_t)> RuleFunc`:
Type for functions that specify the cellular automata rules.

### `Errors` (`flexomata.h`)
Namespace for error-handling. Defined within the scope of `Flexomata` as `Flexomata::Errors`.

#### Accessible Member Functions
- `void handle_exception(const std::exception& e)`:
Handles passed in exception. It is expected that the code involving interactions with Flexomata is structure in the following manner:
```C++
try {
    // Interact with Flexomata here
} catch (const std::exception& e) {
    Flexomata::Errors::handle_exception(e);
}
```

### `Grid` (`grid.h`)

Handles the cellular automata grid. Accessible member functions allow reading the current state and dimensions.

Exhibits a "toroidal", wrap-around behavior on the boundaries.

#### Enums
```C++
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
```
#### Accessible Member Functions
- `bool is_initialized() const`: 
Check whether the `Grid` object is initialized.
- `size_t get_width() const`:
Get the number of columns of the grid.
- `size_t get_height() const`:
Get the number of rows of the grid.
- `void print_data() const`
Print the grid in its current state to the command line.
- `size_t get_pixel(const size_t x, const size_t y) const`
Get the value of a specific cell on the grid.
- `size_t get_neighbor(const size_t x, const size_t y, enum Direction dir) const`:
Get the value/state of a specific neighbor of a particular cell location on the grid.
- `size_t get_neighbor_count(const size_t x, const size_t y, const size_t of_state) const`
Get the number of neighboring cells of a specific value/state.

### `DeferredConfigLoader` (`deferred_configloader.h`)

#### Accessible Member Functions
- `void set_config_pixel(const size_t x, const size_t y, const size_t value)`:
Set the specific value of a cell on the initial grid configuration.
- `void load_config_into_target() const`:
Load the specified configuration into the target grid, as returned by `SimulationScene::get_grid`.

## Configuration Parsing
Each instance of `SimulationScene`, and hence each simulation instance, must be initialized with a configuration string. The configurations string is either passed on explicitly, or read from a file. As of now, the configuration only specifies the initial state of the grid. That is to say, it specifies the value or state of each cell in the grid, represented as a non-negative integer.

The block specifying the grid configuration must begin with the `GRID` keyword, and only that keyword, in a single line. It must be followed by rows of non-negative integers separated by whitespace. Each row must have the same number of integers to maintain consistency.

Optionally, the block can be explicitly closed with the `END_GRID` keyword. This is completely unnecessary as of now, however.

### Example of a Configuration File
```
GRID
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
END_GRID
```
