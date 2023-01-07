# CPPND: Capstone Project - Sphere Ray Tracer

![assert](./assets/image.jpeg)

This render allows users to render as many spheres as they want. Spheres are render in random location and the output is a `.ppm` file.

This project follows Peter Shirley's book [Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)

The code in this project follows the book pretty closely but this projects makes the following improvements:
- Use multithreading to render images quicker.
- Use multithreading to split out the creation of the spheres.
- Update classes to close off member data.
- Update functionality to allow users how many sphere they would like to render.

## File and Class Structure

```
.
├── CMakeLists.txt
├── CODEOWNERS
├── LICENSE.md
├── README.md
├── assets
│   └── image.jpeg
├── include
│   ├── camera.h
│   ├── color.h
│   ├── constants.h
│   ├── hittable.h
│   ├── hittableList.h
│   ├── material.h
│   ├── ray.h
│   ├── sphere.h
│   └── vec3.h
├── main.sh
└── src
    └── main.cpp
```

As listed above, all of the project's header files are located in directory `include` while the cpp files can be found in directory `src`.

`main.cpp` contains the `main` method that runs the program. The `main` method takes in the users input. This input determines how many spheres will be rendered
on the output image.

Once the number of spheres is determined, threads are created to distribute the load. `randomScene` is the method that creates all of the spheres. This is done by creating an instance of `HittableList` (vector of shared pointers) and start adding instances of the `Sphere` class. The `Sphere` class uses a `Point3`, size and `Material` to determine its properties and location. To provide more context, `Point3` is a group of (x, y, z) coordinates while `Material` is a class that is inherited by the material classes we want for the sphere (`Lambertian`, `Metal`, `Dielectric`).

After our spheres are created, we create four threads to distribute the process of writing out rgb values to the image. To do this, we loop through all pixels for the width of the image based on the height chucks we distributed in our threads. In this process, we calculate our uv values to create an instance of a `Ray`. Once we have a ray, we use it to create a pixel color which is an rgb value. Once our threads are completed, we write these values onto our output image.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./Capstone > image.ppm`.

## Expected Program Behavior

After running the command `./Capstone > image.ppm` in the `build` directory, the output will be an `.ppm` file. This file will be viewable if you have an extension in vscode or use an online tool to view it. For an example of the output you can refer to the image above.

## Project Scripts

Run the following scripts to easily perform project actions.
```bash
./main.sh               // Starts project from build.
```

# Rubric

__README (All Rubric Points REQUIRED)__

| DONE | CRITERIA | MEETS SPECIFICATIONS | WHERE |
| -- | -- | -- | -- |
| :heavy_check_mark: | A README with instructions is included with the project |The README is included with the project and has instructions for building/running the project. If any additional libraries are needed to run the project, these are indicated with cross-platform installation instructions. You can submit your writeup as markdown or pdf.| |
| :heavy_check_mark: | The README indicates which project is chosen. | The README describes the project you have built. The README also indicates the file and class structure, along with the expected behavior or output of the program. | |
| :heavy_check_mark: | The README includes information about each rubric point addressed. | The README indicates which rubric points are addressed. The README also indicates where in the code (i.e. files and line numbers) that the rubric points are addressed. | |

__Compiling and Testing (All Rubric Points REQUIRED)__

| DONE | CRITERIA | MEETS SPECIFICATIONS | WHERE |
| -- | -- | -- | -- |
| :heavy_check_mark: | The submission must compile and run. | The project code must compile and run without errors. We strongly recommend using cmake and make, as provided in the starter repos. If you choose another build system, the code must compile on any reviewer platform. |

__Loops, Functions, I/O__

| DONE | CRITERIA | MEETS SPECIFICATIONS | WHERE |
| -- | -- | -- | -- |
| :heavy_check_mark: | The project demonstrates an understanding of C++ functions and control structures.| A variety of control structures are used in the project. The project code is clearly organized into functions.| `main.cpp`, `all *h files` |
| | The project reads data from a file and process the data, or the program writes data to a file. | The project reads data from an external file or writes data to a file as part of the necessary operation of the program.| |
| :heavy_check_mark: | The project accepts user input and processes the input.|The project accepts input from a user as part of the necessary operation of the program.| `main.cpp::104`|

__Object Oriented Programming__

| DONE | CRITERIA | MEETS SPECIFICATIONS | WHERE |
| -- | -- | --| -- |
| :heavy_check_mark: | The project uses Object Oriented Programming techniques. | The project code is organized into classes with class attributes to hold the data, and class methods to perform tasks. | `camera.h::6`, `hittable.h::21`, `hittableList.h::9`, `material.h::9`, `material.h::14`, `material.h::36`, `material.h::53`, `ray.h::6`, `sphere.h::8`, `vec3.h::9` |
| :heavy_check_mark: | Classes use appropriate access specifiers for class members. | All class data members are explicitly specified as public, protected, or private.| `camera.h::44`, `hittableList.h::19`, `material.h::48`, `material.h::75`, `ray.h::16`, `sphere.h::15` `vec3.h::59` |
| :heavy_check_mark: | Class constructors utilize member initialization lists. | All class members that are set to argument values are initialized through member initialization lists.| `hittableList.h::12`, `material.h::16`, `hittableList.h::38`, `hittableList.h::55`, `ray.h::9`, `sphere.h::11`, `vec3.h::12` |
| | Classes abstract implementation details from their interfaces. | All class member functions document their effects, either through function names, comments, or formal documentation. Member functions do not change program state in undocumented ways.| |
| | Classes encapsulate behavior. | Appropriate data and functions are grouped into classes. Member data that is subject to an invariant is hidden from the user. State is accessed via member functions.| |
| | Classes follow an appropriate inheritance hierarchy. | Inheritance hierarchies are logical. Composition is used instead of inheritance when appropriate. Abstract classes are composed of pure virtual functions. Override functions are specified.|  |
| | Overloaded functions allow the same function to operate on different parameters. | One function is overloaded with different signatures for the same function name. |
| :heavy_check_mark: | Derived class functions override virtual base class functions. | One member function in an inherited class overrides a virtual base class member function. | `hittable.h::23`, `material.h::11`, `sphere.h::13` |
| | Templates generalize functions in the project. | One function is declared with a template that allows it to accept a generic parameter.| |

__Memory Management__

| DONE | CRITERIA | MEETS SPECIFICATIONS | WHERE |
| -- | -- | -- | -- |
| | The project makes use of references in function declarations. | At least two variables are defined as references, or two functions use pass-by-reference in the project code.||
| | The project uses destructors appropriately. | At least one class that uses unmanaged dynamically allocated memory, along with any class that otherwise needs to modify state upon the termination of an object, uses a destructor. | |
| | The project uses scope / Resource Acquisition Is Initialization (RAII) where appropriate. | The project follows the Resource Acquisition Is Initialization pattern where appropriate, by allocating objects at compile-time, initializing objects when they are declared, and utilizing scope to ensure their automatic destruction.| |
| | The project follows the Rule of 5. | For all classes, if any one of the copy constructor, copy assignment operator, move constructor, move assignment operator, and destructor are defined, then all of these functions are defined.| |
| | The project uses move semantics to move data, instead of copying it, where possible. | For classes with move constructors, the project returns objects of that class by value, and relies on the move constructor, instead of copying the object. | |
| :heavy_check_mark: | The project uses smart pointers instead of raw pointers. | The project uses at least one smart pointer: unique_ptr, shared_ptr, or weak_ptr. The project does not use raw pointers.| `main.cpp::54-55`, `main.cpp::72`, `main.cpp::77`, `main.cpp::80`, `main.cpp::84`, `main.cpp::133-136` |

__Concurrency__

| DONE | CRITERIA | MEETS SPECIFICATIONS | WHERE |
| -- | -- | -- | -- |
| :heavy_check_mark: | The project uses multithreading. | The project uses multiple threads in the execution. | `main.cpp::163` |
| :heavy_check_mark: | A promise and future is used in the project. | A promise and future is used to pass data from a worker thread to a parent thread in the project code.| `main.cpp::91` |
| :heavy_check_mark: | A mutex or lock is used in the project. | A mutex or lock (e.g. std::lock_guard or `std::unique_lock) is used to protect data that is shared across multiple threads in the project code.| `main.cpp::83` |
| | A condition variable is used in the project. | A std::condition_variable is used in the project code to synchronize thread execution.| |