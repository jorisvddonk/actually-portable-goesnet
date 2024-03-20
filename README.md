# αcτµαlly pδrταblε goεsneτ

This project contains [αcτµαlly pδrταblε εxεcµταblε](https://justine.lol/ape.html) variants of the GOESNET modules from [Noctis IV (Plus)](https://github.com/jorisvddonk/Noctis-IV-Plus) to access the Noctis IV Starmap and Guide across a wide variety of _modern_ operating systems (not MS-DOS, unfortunately). The modules make use of [Cosmopolitan Libc](https://justine.lol/cosmopolitan/) to achieve this. No effort has been made to improve the output of the modules such that they fit better on your terminal's character width, but if you dear reader want to give that a try, go ahead! Also, note that not all modules have been ported yet. Feel free to contribute!

## compiling

To compile, you need a Cosmopolitan toolchain installed. Then, it's as simple as invoking `cosmocc -o <module.com> <module>.c`. For example, to compile the 'cat' module, use `cosmocc -o cat.com cat.c`

## porting

Here are some non-exhaustive porting hints:

1. Original source code uses "\\" as a path separator. Switch that over to "/".
2. The original source code uses numeric flags in calls to `open`. Switch that over to the relevant constants defines by Cosmopolitan depending on the use case, e.g. `O_RDONLY` for read-only file access.
3. You can replace calls to `tell (fh)` with `lseek(fh, 0, SEEK_CUR)`.