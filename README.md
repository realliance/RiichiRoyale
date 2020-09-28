# Riichi Royale
*Team "The One Cow" for Fall 2020 Performant Programming in Python*

## Getting Started
This getting start guide assumes you are running either Arch Linux or Ubuntu (whether standalone or within WSL).

Install the following dependencies:

- `cmake`

- Clang 10 ([clang-10](https://packages.ubuntu.com/bionic/clang-10)<sup>ubuntu</sup> or [clang](https://www.archlinux.org/packages/extra/x86_64/clang/)<sup>arch</sup>)

Clone the repository, with recursive submodule cloning:

```
git clone --recursive git@github.com:HartleyAHartley/RiichiRoyale.git
```

Run the following commands within the project directory:
```
# Install python dependencies locally
pip install --user -r requirements.txt

# Bootstrap CMake Environment
mkdir build/
cmake -S . -B build

# Build Project
cmake --build build

# Run Project
python main.py
```

## Testing and Documentation

TODO: Add environment command for running python and clang tests, as well as generating python documentation.

## Good Resources

[DocDevs Mirror of Pygame Documentation](https://devdocs.io/pygame/): Standard Pygame API Docs. Pygame has a ratelimiter on their website of 50 visits per hour, which is very annoying while developing.

[Pygame Example Projects](https://github.com/ternus/pygame-examples): Great showing of how everything in pygame pieces together.

[Pybind11 Docs](https://pybind11.readthedocs.io/en/latest/): The documentation for pybind11, which is what we use for building the C++ python module.