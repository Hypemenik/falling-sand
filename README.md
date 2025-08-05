# Falling Sand Simulation


![](https://github.com/Hypemenik/falling-sand/blob/main/src/fallingsand.gif)

![](https://github.com/Hypemenik/falling-sand/blob/main/src/falingsand_opengl.gif)

OpenglVersion:

A simple 2D falling sand simulation built with SFML and C++. This project demonstrates basic particle simulation principles, including gravity and simple collision detection, allowing users to interactively "draw" sand particles that fall and accumulate.

## Features

* **Interactive Drawing**: Click and drag to place sand particles on the screen.
* **Gravity Simulation**: Sand particles fall downwards, simulating gravity.
* **Basic Collision**: Particles settle on top of each other and on the "ground".
* **Selectable Colors**: Choose between different sand colors (e.g., yellow and red) to place.
* **Pause/Resume**: Toggle simulation pause with a right-click.
* **Reset**: Clear the entire simulation grid.

## Technologies Used

* **C++**: The core programming language.
* **SFML (Simple and Fast Multimedia Library)**: Used for graphics rendering, window management, and input handling.

## Getting Started

Follow these instructions to get a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

You need to have SFML installed on your system.

* **SFML**:
    * **macOS (with Homebrew):**
        ```bash
        brew install sfml
        ```
    * **Linux (Debian/Ubuntu):**
        ```bash
        sudo apt-get install libsfml-dev
        ```
    * **Windows:** Download the appropriate development libraries from the [official SFML website](https://www.sfml-dev.org/download.php) and set up your IDE according to their tutorials.

* **C++ Compiler**: A C++17 compatible compiler (e.g., Clang, GCC, MSVC).

### Installation and Build

1.  **Clone the repository:**
    ```bash
    git clone [https://github.com/Hypemenik/falling-sand.git](https://github.com/Hypemenik/falling-sand.git)
    cd falling-sand
    ```

2.  **Build the project:**
    This project uses a `Makefile`. Open your terminal in the `falling-sand` directory and run:
    ```bash
    make
    ```
    This command will compile the source code and create an executable in the `bin/` directory.

### Running the Application

After successful compilation, you can run the simulation from the terminal:

```bash
/bin/main
