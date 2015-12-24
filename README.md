# DSNView
I'm currently working on porting my NodeJS dsnview project to C. This is mostly
a learning exercise for me, but hopefully it'll eventually be usable.

This is a little project I'm working on that includes a library and a frontend
for accessing and viewing live data from NASA's Deep Space Network.

Feel free to download, use, and contribute to this codebase, but as usual, a
standard disclaimer: If this breaks your code or computer, I take no
responsibility.

## Building
The following libraries (with headers) are required to build and run dsnview:
 * libxml2
 * libcurl
 
To build the project:

   $ cd build
   $ cmake ..
   $ make

It should successfully compile and spit out a binary called dsnview in your
build directory.

## Usage
To run the application, from the build directory, run:

    $ ./dsnview

## Contributing
1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :-)

## Credits
  * mepholic - project maintainer
  
## License
MIT License