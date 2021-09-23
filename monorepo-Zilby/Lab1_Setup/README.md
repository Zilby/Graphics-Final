# Lab 1 - Setup

> We are going to test our environment for OpenGL, practice C++, and finally practice with git

**Lab materials must be pushed to your repository one week from now before the next class begins**

**Read:** *Labs are designed to be finished in class if you work diligently, but expected to take 1-2 hours outside of class. They are often more 'tutorial' in style, and sometimes the solution may even be provided in pieces for you to type in and experiment. Copying & Pasting is discouraged however--make mistakes, play, and you will further learn the material.*

## Modify this section

- How many hours did it take you to complete this lab? 1
- Did you collaborate with any other students/TAs/Professors? No
- Did you use any external resources? (Cite them below) None.
- (Optional) What was your favorite part of the lab? Not having too many issues haha
- (Optional) How would you improve the lab? 

## Logistics

You will be working on your own laptop today. Historically, lab 1 is the most difficult lab, because supporting multiple architectures and operating systems can be tricky. Nevertheless, we will get through it!

## Description

The first task to becoming a graphics programmer, is to figure out how to configure your system to run graphics applications. Very likely you have written many console-based applications that run in a terminal or perhaps other environments. I would like everyone to leave today understanding how to setup a graphics application. This includes how to compile and run the support code provided for an SDL applicaiton. This support code, may also serve as a template for when you build your assignments (or personal projects) from scratch in the future.

## Part 1 - Setting up your Environment

Follow the respective tutorial for your operating system. I have provided some links below. [Lazy Foo](http://lazyfoo.net/tutorials/SDL/01_hello_SDL/index.php) also has a nice set for a variety of operating systems.

* [Linux](http://www.mshah.io/comp/Spring18/graphics/Lab/1/linux.html) (Tested on Ubuntu)
* Windows
  * Option 1: 
    * Download mysys from here: https://drive.google.com/file/d/116IIh7n8aoWXKDiQeonZjxXEkPPh3RME/view?usp=sharing
    * unzip the contents to a location on your machine.
    * From mysys64 launch the mingw64 terminal(a blue icon).
      * Note, you may have to navigate to the lab folder wherever you have cloned this repo. (e.g. cd /c/Users)
    * Try running the build script with `python build.py` and then execute lab.exe
  * Option 2: 
    * [MinGW 64](http://www.mshah.io/comp/Spring18/graphics/Lab/1/MinGW.html) (Tested on Windows 10)
      * These are some more scattered instructions on installation.
  * Option 3: (Least preferred)
      * Use visual studio here: http://www.mshah.io/comp/Spring18/graphics/Lab/1/windows.html
* [Mac](http://www.mshah.io/comp/Spring18/graphics/Lab/1/mac.html) (Tested on High Sierra 10.13.2 Operating System)
 * Something else? Let your instructor know. Typically labs and assignments are tested on all three configurations above (though in the future there will be 1 set of directions to follow after you are setup!)
* Remote graphics through a terminal (Optional--and here primarily for very limited situations)

## Part 2 - Simple DirectMedia Layer (SDL) (Reading ~2 minutes)

SDL("Simple DirectMedia Layer") is a library that allows us to draw a window to the screen. We will be using SDL version 2.0+ for this course. SDL itself is an abstraction layer that can create windows for the appropriate platform(i.e. Windows, Mac, Linux, mobile, etc.), without us have to do the real nitty gritty details of setting up a window. While there are many

- If you want to know how to setup a window in your native operating system platform the long way (without using a library like SDL), here are some examples from the web: [Linux](https://stackoverflow.com/questions/15091514/how-do-i-create-a-window-in-opengl-on-linux-instead-of-using-glut), [Linux X11](https://www.opengl.org/discussion_boards/showthread.php/177999-GCC-OpenGL-without-glut?p=1239444&viewfull=1#post1239444), [Graphics Context in Mac](https://developer.apple.com/library/content/documentation/GraphicsImaging/Conceptual/drawingwithquartz2d/dq_context/dq_context.html#//apple_ref/doc/uid/TP30001066-CH203-CJBDCHAC), [Native Windows](http://www.falloutsoftware.com/tutorials/gl/gl2.htm)
  - (Note: It is always worth knowing how things work behind the scenes! In doing so however, you may further appreciate the work done by SDL for us!)
- We are not going to spend time arguing if other libraires (glut, freeglut, glfw, etc.) are the better or the best.
  - Try the one that fits your need
  - I choose SDL2 because it is what modern commercial games use, it is free, and it supports many desktop environments (as well as mobile if you are interested!)

## Part 3 - More on OpenGL (Reading ~ 2 minutes)

Which version are we using for this course? The answer is that we will be using version 3.3.

- There is some confusion as to which version of OpenGL versions exist.
- OpenGL has a Compatibility Profile which contains all of the features of OpenGL (i.e. all of the old stuff)
- OpenGL has a "Core Profile" which includes only new features that have not been deprecated. This is the future, though most graphics systems support legacy.
  - The Core profile has the old "fixed pipeline" in OpenGL. The fixed functino pipeline was actually quite nice for learning purposes, but is not efficient compared to modern day programs.
  - A [Forward compatibility](https://www.khronos.org/opengl/wiki/OpenGL_Context#Forward_compatibility) graphics context, will enforce the removal of all deprecated featuers.
- The specification for OpenGL 3.3 is located [here](https://www.khronos.org/registry/OpenGL/specs/gl/glspec33.core.pdf). Note this is not required reading by any means, but it is nice to know the spec exists, and this is something you might reference if you work at Intel, NVidia, AMD, etc.
- A relatively detailed version of the OpenGL version history is [here](https://www.khronos.org/opengl/wiki/History_of_OpenGL).

## Part 4 - Quick dip into C++14

Provided below is a first C++ code snippet so you can get up to speed. Our first assignment will help us take a deeper dive into C++.

```
// A first C++ program
// Standard libraries that we include.
#include <iostream>
#include <string>

// Specially named function that is the entry point into our program.
int main()
{
    
// ========================== for - loops ====================
     // Allocate an array on the stack.
     // This array cannot be referenced outside the main function.
     int myArray[10];
     
     // A loop that pre-increments 'i' by one each iteration of the
     // loop.
     for(int i =0; i < 10 ; ++i) {
        // If we are modifying values , then use a regular for-loop
        myArray[i] = i ;
        // cout is also part of the standard library, and prints
        // text to standard output (i.e. typically our terminal window)
        std::cout << i << " ";
     }
    
     // Here we create a string type. It is part of the 
     // standard library namespace, which is why it begins with
     // std::
     std::string s= "hello";
     
     // Here is a different kind of loop.
     for(char c : s) {
     // Range-based for loops are useful if we are not modifying any values.
     // Ranged-based for loops were introduced in C++11 and are a new popular feature
     // in many other programming languages, and now C++
        std::cout << c ;
     }
     
     // More information can be found here: http://en.cppreference.com/w/cpp/language/range-for
}
```


- Type out(i.e. DO NOT just copy and paste) the above code sample in part4/example.cpp.
- Compile and run with the following on the terminal:
  - Linux Option 1: `clang++ -std=c++14 example.cpp -o example`
  - Linux Option 2: `g++ -std=c++14 example.cpp -o example`
  - Mac Option 1:   `clang++ -std=c++14 example.cpp -o example`
  - Mac Option 2:   `g++ -std=c++14 example.cpp -o example`
  - Mac Option 3:   Create a console-based XCode project
  - Windows Option 1: `g++ -std=c++14 example.cpp -o example`
  - Windows Option 2: Create a console-based Visual Studio C++ project
- Finally, make sure to do a 'git add', 'git commit', and 'git push' of example.cpp to practice working with git.

## Helpful Resources

Some additional resources to help you through this lab assignment

- http://lazyfoo.net/tutorials/SDL/ - This is the page where I learned how to setup SDL. Part of this lab is inspired by this websites tutorial.
- https://www.learncpp.com/
- http://www.cplusplus.com/

## Deliverables

- Run `python build.py` and execute the **lab** executable to show me a working SDL application that has a red screen.
  - If you make further modifications, that is great!
- Modify and push the part4/example.cpp file to your repository.

You must complete this lab by next class. You will run your lab in front of me at the start of the next lab when class starts and tell me what your group # was for that week.

## Rubric

You (and any partner(s)) will receive the same grade from a scale of 0-2.

- 0 for no work completed by the deadline
- 1 for some work completed, but something is not working properly
- 2 for a completed lab (with possible 'going further' options completed)

## Going Further

What is that, you finished Early? Did you enjoy this lab? Here are some (optional) ways to further this assignment.

- Start writing some OpenGL 3.3 from here! https://learnopengl.com I imagine many will finish soon. This is now dedicated graphics time!
- Look at some SDL 2 tutorial on the net.
- Look for OpenGL 3.3 tutorials on the net.

## Found a bug?

If you found a mistake (big or small, including spelling mistakes) in this lab, kindly send me an e-mail. It is not seen as nitpicky, but appreciated! (Or rather, future generations of students will appreciate it!)

- Fun fact: The famous computer scientist Donald Knuth would pay folks one $2.56 for errors in his published works. [[source](https://en.wikipedia.org/wiki/Knuth_reward_check)]
- Unfortunately, there is no monetary reward in this course :)
