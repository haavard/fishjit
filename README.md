fishjit
=======
fishjit is a just-in-time compiler for the [><>][1] esoteric programming
language. It is written in C using the [DynASM][2] dynamic assembler.
Currently only supports the amd64 architecture.

Known limitations:
 - No i386 support
 - Instructions `p` and `g` not implemented
 - Code is maximally 256 by 256 characters
 - Lots and lots of bugs

Installation
------------
fishjit requires the [uthash][3] hash table library. DynASM is included via a
git submodule.

    $ git clone https://github.com/haavardp/fishjit.git
    $ cd fishjit
    $ git submodule update --init
    $ make
    $ sudo make install

To run the included tests, run `make check`.

Example
-------
    $ cat helloworld.fish
    !v"hello, world"ar!
     >l?!;o
    $ fish helloworld.fish
    hello, world

License
-------
Copyright (c) 2016, Håvard Pettersson <mail@haavard.me>

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

[1]: https://esolangs.org/wiki/Fish
[2]: http://luajit.org/dynasm.html
[3]: https://troydhanson.github.io/uthash/
