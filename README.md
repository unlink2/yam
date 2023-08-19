# yam

Yam is a lightweight payload crafting tool. It mapes raw binary data into a usable format.
It takes files, raw string and other things as input and converts it to a target payload (e.g. a C char array)

## Table of content

- [Installation](#Installation)
- [Usage](#Usage)
- [License](#License)
- [Contributing](#Contributing)
- [TODO](#TODO)

## Installation

To build this program you will require a recent C compiler, premake, `libargtable2` and `libcmocka` for unit tests.

```sh
premake gmake # to build gnu make file 
make # to compile all targets 
```

## Usage

1) Rename `yam` to the project's name in all files and folders 

## License

This program is distributed under the terms of the MIT License.

## Contributing

## TODO

- Implement data sources and outputs
- Allow use of partial files (e.g. from offset to offset)
- Sources: File, Hex string input, regular string, padding,
- Outputs: raw binary, C char-array, python array
