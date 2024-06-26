/*
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
*/

/**
 * h2inc options and configuration
 */
struct h2inc_config
{
	/**
	 * Struct member alignment
	 */
	int Zp = 2;

	/**
	 * Target processor
	 * 0 = 8086
	 * 1 = 80186
	 * 2 = 80286
	 * 3 = 80386
	 * 4 = 80486
	 * 5 = Pentium
	 * 6 = Pentium Pro
	 */
	int G = 0;

	/**
	 * Compile without linking
	 */
	bool c = false;

	/**
	 * Disable all warnings
	 */
	bool w = false;

	/**
	 * Convert flat model C prototypes to MASM prototypes
	 */
	bool WIN32 = false;

	/**
	 * Suppress startup banner
	 */
	bool nologo = false;
};

/** Run h2inc
 * @param src = source filename to process
 * @param dst = destination file to write
 * @param config = h2inc options
 * @return true for success, false for failure
 */
bool h2inc(const char *src, const char *dst, h2inc_config &config);
