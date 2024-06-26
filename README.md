# h2inc-llvm

A FOSS reimplementation of Microsoft's h2inc tool using libclang (LLVM), written in C++17.

It takes C headers and converts them to MASM-compatible `.inc` files for use in assembly code.

## Notes

- It currently assumes the following h2inc arguments: `/G3 /Zp4 /w /c /WIN32`

## Goals

- Full feature parity with the original h2inc.
- NASM output mode and other dialects.
- Command-line switches similar to the original h2inc.

## License

MIT License

Copyright (c) 2024 erysdren (it/they/she)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
