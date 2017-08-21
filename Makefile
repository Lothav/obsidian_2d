all:
	clang++-4.0 -std=c++1z -Wall -Wpedantic -Wextra -fvisibility=hidden -fno-strict-aliasing -fno-builtin-memcmp -fno-rtti -fno-exceptions -O3 src/main.cpp -o build/obsidian2d.out
