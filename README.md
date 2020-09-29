# Riichi Royale
*Team "The One Cow" for Fall 2020 Performant Programming in Python*

## Getting Started
This getting start guide assumes you are running either Arch Linux or Ubuntu (whether standalone or within WSL).

Install the following dependencies:

- `git`

- `cmake`

- `clang` (or [clang-10](https://packages.ubuntu.com/bionic/clang-10) if on Ubuntu < 20.04LTS)

Clone the repository:

```
git clone git@github.com:HartleyAHartley/RiichiRoyale.git
```

Run the following commands within the project directory:
```
# Install python dependencies locally
pip install --user -r requirements.txt

# Create build directory for cmake
mkdir build/

# Bootstrap CMake Environment
cmake -S . -B build

# Or bootstrap CMake with tool building enabled
cmake -S . -B build -DPACKAGE_TOOLS=ON

# Or, if wanting to use Clang

C=clang CXX=clang++ cmake -S . -B build

# Build Project
cmake --build build

# Run Project
python main.py
```

## Testing and Documentation

The python test suite is located in `test/` and can be run with `python test`

The C++ test suite is located in `cxxtests/` and can be run with `make -C build check`

## Good Resources

[DocDevs Mirror of Pygame Documentation](https://devdocs.io/pygame/): Standard Pygame API Docs. Pygame has a ratelimiter on their website of 50 visits per hour, which is very annoying while developing.

[Pygame Example Projects](https://github.com/ternus/pygame-examples): Great showing of how everything in pygame pieces together.

[Pybind11 Docs](https://pybind11.readthedocs.io/en/latest/): The documentation for pybind11, which is what we use for building the C++ python module.