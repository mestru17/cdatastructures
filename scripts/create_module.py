#!/usr/bin/env python3


import inspect
from pathlib import Path
import sys


def main():
    if len(sys.argv) != 2:
        print("Usage: python3 create_module.py <module_name>")
        exit(1)

    module_name = sys.argv[1]
    module_dir = Path() / "src" / module_name
    source_file = module_dir / (module_name + ".c")
    header_file = source_file.with_suffix(".h")
    test_file = module_dir / (module_name + "test.c")
    make_file = module_dir / "Makefile"
    files = [source_file, header_file, test_file, make_file]

    if module_dir.exists():
        print(f"Module '{module_name}' already exists at '{module_dir}'.")
        exit(1)

    print(f"Creating module '{module_name}' in {module_dir}.")
    module_dir.mkdir()
    for file in files:
        file.touch()
        if file != make_file:
            write_license_comment(file)
    write_header_file(header_file)
    write_test_file(test_file, header_file)
    write_make_file(make_file, module_name, test_file, source_file)

    print(
        f"Created files:",
        *files,
        sep="\n",
    )


def write_license_comment(file: Path):
    with file.open("a") as f:
        f.write(
            inspect.cleandoc(
                f"""
                // MIT License
                //
                // Copyright (c) 2022 Mathias Estrup
                //
                // Permission is hereby granted, free of charge, to any person obtaining a copy
                // of this software and associated documentation files (the "Software"), to deal
                // in the Software without restriction, including without limitation the rights
                // to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
                // copies of the Software, and to permit persons to whom the Software is
                // furnished to do so, subject to the following conditions:
                //
                // The above copyright notice and this permission notice shall be included in
                // all copies or substantial portions of the Software.
                //
                // THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
                // IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
                // FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
                // AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
                // LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
                // OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
                // SOFTWARE."""
            )
            + "\n"
        )


def write_header_file(header_file: Path):
    with header_file.open("a") as f:
        guard_name = header_file.name.swapcase().replace(".", "_")
        f.write(
            inspect.cleandoc(
                f"""
            #ifndef {guard_name}
            #define {guard_name}

            #endif"""
            )
        )


def write_test_file(test_file: Path, header_file: Path):
    with test_file.open("a") as f:
        f.write(
            "\n".join(
                [
                    "",
                    "#include <stdio.h>",
                    "",
                    f'#include "{header_file.name}"',
                    "",
                    "int main() {",
                    '  printf("Hello, World!\\n");',
                    "  return 0;",
                    "}",
                ]
            )
        )


def write_make_file(
    make_file: Path, module_name: str, test_file: Path, source_file: Path
):
    with make_file.open("a") as f:
        f.write(
            inspect.cleandoc(
                f"""
            CC=gcc
            CFLAGS=-g -Wall

            ROOT=../..
            OBJ=$(ROOT)/obj/{module_name}
            BIN=$(ROOT)/bin

            OBJS=$(OBJ)/{test_file.with_suffix(".o").name} $(OBJ)/{source_file.with_suffix(".o").name}
            BINS=$(BIN)/{test_file.stem}

            all: $(BINS)

            $(BINS): $(OBJS) | $(BIN)
                $(CC) $(CFLAGS) -o $@ $^

            $(OBJ)/%.o: %.c | $(OBJ)
                $(CC) $(CFLAGS) -c $< -o $@

            $(OBJ):
                mkdir -p $@

            $(BIN):
                mkdir -p $@

            clean:
                rm -rf $(OBJ) $(BINS)"""
            ).replace("    ", "\t")
        )


if __name__ == "__main__":
    main()
