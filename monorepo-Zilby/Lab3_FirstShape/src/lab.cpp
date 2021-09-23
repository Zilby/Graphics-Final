// Support Code written by Michael D. Shah
// Last Updated: 9/18/18
// Please do not redistribute without asking permission.
#include "SDLGraphicsProgram.h"

// The main entry piont into our program.
int main( int argc, char* args[] )
{
      // Create an instance of an object for a SDLGraphicsProgram
      SDLGraphicsProgram mySDLGraphicsProgram(1280,720);
      // Print out some useful information
      mySDLGraphicsProgram.getOpenGLVersionInfo();
      // Run our program forever
      mySDLGraphicsProgram.loop();
      // When our program ends, it will exit scope, the
      // destructor will then be called and clean up the program.
	return 0;
}
