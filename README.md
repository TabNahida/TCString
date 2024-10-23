# TCString

TCString is a custom C string implementation designed for efficient memory management by utilizing a buffer size that is always the closest power of two to the actual string size. This approach optimizes memory usage and facilitates dynamic growth, making it an effective alternative to standard C string functions.

## Features

- **Dynamic Buffer Management**: Allocates memory with buffer sizes that are powers of two, ensuring efficient memory usage and minimizing reallocations.
- **String Operations**: Supports common string operations, including concatenation, substring extraction, and more.
- **Inline Functions**: Provides inline functions for improved performance and ease of use.
- **Memory Management**: Includes clear and free functions for proper memory management.

## Getting Started

### Prerequisites

- C17 compatible compiler
- [xmake](https://xmake.io) build system

### Installation

To use TCString, clone the repository and include `TCString.h` in your project:

```sh
# Clone the repository
git clone https://github.com/yourusername/TCString.git
```

Include `TCString.h` in your code:

```c
#include "include/TCString.h"
```

You can also add TCString as a package through xmake:

```lua
add_repositories("tab-repo https://github.com/yourusername/repo-xmake.git")
add_requires("tcstring")
```

Then, include it in your target:

```lua
target("example")
    set_kind("binary")
    add_files("src/*.c")
    add_packages("tcstring")
```

### Example Usage

```c
#include "TCString.h"
#include <stdio.h>

int main() {
    TCString *myStr = makeTCString("Hello, ");
    apndTCString(myStr, "World!");
    printf("%s\n", myStr->buffer);  // Output: Hello, World!

    TCString *subStr = subTCString(myStr, 0, 5);
    printf("%s\n", subStr->buffer);  // Output: Hello

    freeTCString(subStr);
    freeTCString(myStr);
    return 0;
}
```

### Build with xmake

This project uses [xmake](https://xmake.io) for building. To build and run tests, use the following commands:

1. Ensure xmake is installed.
2. Navigate to the project root directory.
3. Run the following commands:

```sh
# Configure the project
xmake f -c

# Build the project
xmake

# Run the built target
xmake run MainTest
```

Here is the `xmake.lua` configuration file used for the project:

```lua
add_rules("mode.release", "mode.debug")

add_languages("c17")

target("tcstring")
    set_kind("headeronly")
    add_headerfiles("include/TCString.h")
target_end()

target("MainTest")
    set_kind("binary")
    add_files("src/main.c")
    add_deps("tcstring")
    add_includedirs("include")
target_end()
```

## Directory Structure

```
TCString/
├── include/
│   └── TCString.h
├── src/
│   └── main.c
├── xmake.lua
└── README.md
```

## Contributing

Contributions are welcome! If you have ideas for improving TCString, feel free to open an issue or submit a pull request.

## License

This project is licensed under the MIT License.
