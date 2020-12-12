# REXTerm
REXTerm is a program that displays .xp files layers as frames in the terminal.
The program uses the [REXReader++](https://github.com/gamepopper/REXReader-CPlusPlus) library to read .xp files

# Installation guide
I apologise in advance since the compilation is kinda messy.

To compile this program you need the ncurses library and the zlib library to be installed

## Step 1: compile GondTerm
when inside the REXTerm folder use the following command
g++ -o GondTerm main.cpp ./REXReader++/RexReader.cpp -lncurses -lz

## Step 2: compile REXTerm
when inside the REXTerm folder use the following command
g++ -o REXTerm REXTerm.cpp"

## Step 3: compile cp437
Cp437 is an amazing program used to emulate the cp437 terminal that can be found at this link https://github.com/keaston/cp437.
I have included the source code to make the process of compilation a bit faster.
Once you have compiled the cp437 source by using the make command copy the resulting binary in the same folder as the GondTerm binary

## Step 4: Run!
Once you have all the compiled binaries in the same folder you should be able to run the program by executing REXTerm

**The GondTerm, REXTerm and cp437 files need to be in the same directory**

# AdditionalResources folder
In this folder you will find the font for the terminal and a few .xp files to test the program with

