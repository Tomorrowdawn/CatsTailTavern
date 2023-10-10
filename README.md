# CatsTailTavern
Genius Invokation TCG Simulator, with engine in C++ and python interface

## Prerequisites

```
CMake >= 3.27
Python >= 3.8
```

If you are using Windows, MingW or VS SDK would be needed to compile C++ code. You should guarantee your compiler supports at least C++17.

## Usage

### Python developer

Clone this repo to your local device and run

```
cd cgisim
mkdir build
cd build
cmake ..
cmake --build .
```

A `.pyd` file would be created in `build/`. Copy it to any where you want to import this module and treat it like a simple python module `.py`.

> If you are using Windows, try `cmake --build . --config Release` if you are using MSVC or `cmake .. -G "MinGW Makefiles"`, `make` if you are using MinGW.
>
> If you are using MSVC, that `.pyd` file would be found at `build/Release`. Note: you must manually config Release if you use MSVC, otherwise it would generate a debug file.

Now you can `import cgisim` in your python project.

> TODO: We would support `pip install` in future updates.

### C++ Developer

Almost indentical to python developer, but you should run `cmake .. -Dtest=ON` to generate an executable. And `.pyd` is discarded in that case.

## Feature

1. Modern C++ with highly extensible visitor pattern design. We introduce `std::visit` and `std::variant`, implementing a character in 10 lines. You can code like python but enjoy the speed of C++.
2. Very fast for AI training or searching. 
3. Not a simple simulator: Pre-design lots of mechanics to handle uncertanity in Genius Invokation.
4. We offer a user-friendly Python interface that facilitates seamless migration of existing solutions for reinforcement learning algorithm to GITCG.