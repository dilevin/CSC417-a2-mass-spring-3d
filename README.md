# Physics-Based Animation – Mass-Spring Systems on Three Dimensions

> **To get started:** Clone this repository and all its [submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules) dependencies using:
> 
>     git clone --recursive https://github.com/dilevin/CSC2549-a2-mass-spring-3d.git

**Do not fork:** Clicking "Fork" will create a _public_ repository. If you'd like to use GitHub while you work on your assignment, then mirror this repo as a new _private_ repository: https://stackoverflow.com/questions/10065526/github-how-to-make-a-fork-of-public-repository-private

## Introduction

Welcome to Physics-Based Animation. This assignment has two purposes, (1) to familiarize you with the development tools used for assignments in the course and (2) to introduce you to some basic physics simulation concepts in one-dimension (1D).  

### Prerequisite installation

On all platforms, we will assume you have installed cmake and a modern c++
compiler on Mac OS X[¹](#¹macusers), Linux[²](#²linuxusers), or
Windows[³](#³windowsusers).

We also assume that you have cloned this repository using the `--recursive`
flag (if not then issue `git submodule update --init --recursive`). 

**Note:** We only officially support these assignments on Ubuntu Linux 18.04 (the OS the teaching labs are running) and OSX 10.13 (the OS I use on my personal laptop). While they *should* work on other operating systems, we make no guarantees. 

**All grading of assignments is done on Linux 18.04**

### Layout

All assignments will have a similar directory and file layout:

    README.md
    CMakeLists.txt
    main.cpp
    include/
      function1.h
      function2.h
      ...
    src/
      function1.cpp
      function2.cpp
      ...
    data/
      ...
    ...

The `README.md` file will describe the background, contents and tasks of the
assignment.

The `CMakeLists.txt` file setups up the cmake build routine for this
assignment.

The `main.cpp` file will include the headers in the `include/` directory and
link to the functions compiled in the `src/` directory. This file contains the
`main` function that is executed when the program is run from the command line.

The `include/` directory contains one file for each function that you will
implement as part of the assignment.

The `src/` directory contains _empty implementations_ of the functions
specified in the `include/` directory. This is where you will implement the
parts of the assignment.

The `data/` directory contains _sample_ input data for your program. Keep in
mind you should create your own test data to verify your program as you write
it. It is not necessarily sufficient that your program _only_ works on the given
sample data.

## Compilation for Debugging

This and all following assignments will follow a typical cmake/make build
routine. Starting in this directory, issue:

    mkdir build
    cd build
    cmake ..

If you are using Mac or Linux, then issue:

    make

## Compliation for Testing

Compiling the code in the above manner will yield working, but very slow executables. To run the code at full speed, you should compile it in release mode. Starting in the **build directory**, do the following:

    cmake .. -DCMAKE_BUILD_TYPE=Release
    
Followed by:

    make 
  
Your code should now run signfigantly (sometimes as much as ten times) faster. 

If you are using Windows, then running `cmake ..` should have created a Visual Studio solution file
called `a1-mass-spring-1d.sln` that you can open and build from there. Building the project will generate an .exe file.

Why don't you try this right now?

## Execution

Once built, you can execute the assignment from inside the `build/` using 

    ./a2-mass-spring-3d

## Background
Its happening! We are finally doing some computer graphics ! You can tell because this assignment has a bunny in it (their name is Terry). 

The goal of this assignment is to take what you've learned about 1D mass-spring systems and move it to 3D. While much of what we previously covered on variational mechanics and time integration still applies, you will soon see that implementations in 3D become much more complicated. Crucially, this assignment will tackle three concepts which will be important for the remainder of the course 

1. the potential energy of a 3D, non-zero rest length spring  
2. the notion of assembly. Assembly is a process which builds global linear algebra operations describing the motion of a whole object (bunny) from smaller operators describing the behaviour of individual pieces (in this case springs). 
3. The linearly implicit time integrator in 3D -- arguably the most popular time integration scheme in computer graphics.


**Github does not render the math in this Markdown. Please look at README.html to see the equations in their proper form**

### Groundrules

Implementations of nearly any task you're asked to implemented in this course can be found online. Do not copy these and avoid googling for code; instead, search the internet for explanations. Many topics have relevant wikipedia articles. Use these as references. Always remember to cite any references in your comments.

### Implementation Notes

For this course most functions will be implemented in **.cpp** files. In this assignment the only exception is that time integrators are implemented in **.h** files. This is due to the use of lambda functions to pass force data to the time integration algorithms.

### src/T_particle.cpp 

Compute the kinetic energy of a single mass particle.

### src/V_gravity_particle.cpp

Compute the gravitational potental energy of a single mass particle.

### src/V_spring_particle_particle.cpp

Compute the potential energy of a spring which connects two particles. 

### src/dV_gravity_particle_dq.cpp

Compute the gradient of the gravitational potential energy for a single particle.

### src/dV_spring_particle_particle_dq.cpp

Compute the forces exerted by a single spring on its end-points.
 
### src/d2V_spring_particle_particle_dq2.cpp

Compute the per-spring hessian of the spring potential energy. 

### src/mass_matrix_paricles.cpp

Compute the sparse, diagonal mass matrix that stores the mass of each particle in the mass-spring on its diagonal.

### src/assemble_forces.cpp

Iterate through each spring in the mesh, compute the per-spring forces and assemble the global force vector.

### src/assemble_stiffness.cpp

Iterate through each spring in the mesh, compute the per-spring stiffness matrix and assemble the global, sparse stiffness matrix. To do this, you should construct a list of ``Eigen::Triplet`` objects and then set your sparse matrix using these [triplets](https://eigen.tuxfamily.org/dox/group__TutorialSparse.html).

### src/fixed_point_constraints.cpp

Compute the sparse projection matrix which projects out fixed point vertices. 

### src/pick_nearest_vertices.cpp

Given a point on the screen (i.e a mouse position clicked by the user), find all vertices within a given radius. **For this method, and this method alone** you are allowed to use the ``igl::unproject`` and ``igl::ray_mesh_intersect`` functions. I have provided the approriate ray shooting function for you to use in the code as well. 

### include/linear_implicit_euler.h

Implement the linearly implicit Euler time integrator. 

