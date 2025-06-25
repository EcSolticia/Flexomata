# Flexomata

Flexomata is a simple framework for handling cellular automata simulation in C++ as a static library. The user may provide a "rule function" to operate per each grid cell along with an initial configuration. Flexomata can thereafter apply the rule to each cell and offers access to the evolved state of the grid.

> [!WARNING]
> In spite of being fully functional as is, Flexomata is still under initial development. Notably, its API could undergo multiple minor changes prior to finalization.

# Installation

## Supported Platforms
- Linux / MacOS / Unix-like systems (Using CMake + Make).
- Microsoft Windows (Using CMake + Make with MinGW).

## Build Instructions
### Prerequisites
Ensure that the following programs are installed on your system:
- CMake (version 3.10 or above).
- The Make build system.
    - Natively available on Linux/MacOS.
    - On Windows, use MinGW.
- A C++ compiler that supports C++17 (For example, g++11 or above).
    - Natively available on Linux/MacOS.
    - On Windows, use MinGW.
- Git (to clone the repository).

### Commands (Linux/MacOS)
If you installed `git`, `cmake`, `make`, and `g++` using a dedicated package manager, they are likely already added to `PATH`, and the associated commands should be accessible from your terminal. Otherwise, you might need some manual setup. 
```
git clone https://github.com/EcSolticia/Flexomata.git
cd Flexomata
mkdir build
cd build
cmake ..
make
```

### Commands (Windows)
Open a terminal emulator or command prompt where `git`, `cmake`, `make` (via MinGW), and `g++` (via MinGW) are accessible. You may have to add the appropriate MinGW `bin` directory to your system's `Path` environment variable.
```
git clone https://github.com/EcSolticia/Flexomata.git
cd Flexomata
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make
```

# Getting Started:  Hello, Flexomata!
A successful execution of the build instructions is expected to yield `libFlexomata.a` in directory `Flexomata/build`. To use the library in a project using CMake, you have to specify Flexomata's include directory and link your project to the generated library. 

The following portion of this section will exemplify the process through creating a simulation of Conway's Game of Life using Flexomata.

### 1. Create your project
```
mkdir GameOfLife
cd GameOfLife
touch main.cpp
touch CMakeLists.txt
```
### 2. Set up CMakeLists.txt
#### 1. Make sure to require a CMake version above 3.10 through
```CMake
cmake_minimum_required(VERSION 3.10)
```
If you intend to use or require newer CMake versions, you may replace the "3.10" with your desired version.
For this walkthrough, I will stick to 3.10.

#### 2. Set the name of your project, optionally specify the language (the "CXX" part).
```CMake
project(GameOfLife CXX)
```

#### 3. Tell CMake to build an executable from `main.cpp`.
```CMake
add_executable(${PROJECT_NAME} main.cpp)
```

#### 4. Specify the Flexomata include directories
```CMake
target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/path/to/include
)
```
Make sure to replace the placeholder path `${CMAKE_CURRENT_SOURCE_DIR}/path/to/include` with the actual include directory in your system.

#### 5. Specify that the Flexomata library be linked to your project
```CMake
target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/path/to/lib/libFlexomata.a)
```

Again, be sure to replace the placeholder path `${CMAKE_CURRENT_SOURCE_DIR}/path/to/lib/libFlexomata.a` with the actual path to the library in your system.

#### 6. Specify the C++ standard to use
```CMake
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
```

Flexomata uses the C++17 standard.

#### Overall, we obtain:
```CMake
cmake_minimum_required(VERSION 3.10)

project(GameOfLife CXX)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/path/to/include
)

target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/path/to/lib/libFlexomata.a)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
```

This will direct how CMake generates our buildfiles.

### 3. Write the source file
The comments explain each segment of the code.
```C++
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
```

### 4. Compile your executable and run!
In your project directory, create a new directory named `build`, change directory to `build` (`cd build`), and run
```
cmake ..
```
on Linux/MacOS to generate the Makefiles, and
```
cmake -G "MinGW Makefiles" ..
```
On Windows to generate the MinGW Makefiles.

Finally, run
```
make
```
In the `build` directory, which should link the library and compile your executable.

You may wish to run the executable to see if it behaves as expected.

> The program we built here simulates Conway's Game of Life on our initial configuration Grid and pattern. Press enter to simulate the next step. Pressing Ctrl + C, or a similar key combination depending on your terminal and platform can be used to terminate the program.

# Reference
For a reference on the key namespaces, classes, accessible member functions, and Flexomata's configuration parsing, see [DOCS.md](DOCS.md).

# License
Flexomata is distributed under the [MIT License](https://github.com/EcSolticia/Flexomata/blob/main/LICENSE).