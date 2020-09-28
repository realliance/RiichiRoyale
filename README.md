# Riichi Royale
*Team "The One Cow" for Fall 2020 Performant Programming in Python*

## Getting Started
This getting start guide assumes you are running either Arch Linux or Ubuntu (whether standalone or within WSL).

Install the following dependencies:

- SFML 2 ([libsfml-dev](https://packages.ubuntu.com/bionic/libsfml-dev)<sup>ubuntu</sup> or [sfml](https://www.archlinux.org/packages/community/x86_64/sfml/)<sup>arch</sup>)

- Clang 10 ([clang-10](https://packages.ubuntu.com/bionic/clang-10)<sup>ubuntu</sup> or [clang](https://www.archlinux.org/packages/extra/x86_64/clang/)<sup>arch</sup>)

- Boost Development Headers ([libboost-all-dev](https://packages.ubuntu.com/xenial/libboost-all-dev)<sup>ubuntu</sup> or [boost](https://www.archlinux.org/packages/extra/x86_64/boost/)<sup>arch</sup>)

Clone the repository, with recursive submodule cloning:

```
git clone --recursive git@github.com:HartleyAHartley/RiichiRoyale.git
```

Run the following commands within the project directory:
```
# Install python dependencies locally
pip install --user -r requirements.txt

# Bootstrap CMake Environment
...

# Build Project
...

# Run Project
...

```

## Testing and Documentation

TODO: Add environment command for running python and clang tests, as well as generating python documentation.