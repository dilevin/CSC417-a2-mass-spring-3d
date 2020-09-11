# Physics-Based Animation – Mass-Spring Systems on Three Dimensions
[![](https://github.com/dilevin/CSC2549-a2-mass-spring-3d/workflows/Build-CSC2549-Assignment-Two/badge.svg)](https://github.com/dilevin/CSC2549-a2-mass-spring-3d/actions)

> **To get started:** Clone this repository and all its [submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules) dependencies using:
> 
>     git clone --recursive https://github.com/dilevin/CSC2549-a2-mass-spring-3d.git

**Do not fork:** Clicking "Fork" will create a _public_ repository. If you'd like to use GitHub while you work on your assignment, then mirror this repo as a new _private_ repository: https://stackoverflow.com/questions/10065526/github-how-to-make-a-fork-of-public-repository-private

## Introduction

The second assignment has two purposes is really where we start doing Computer Graphics. The main focus will be on pushing the ideas we explored in 1D (the previous assignment) to larger scale examples (bunnies!) in 3D. 

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

## Compilation for Testing

Compiling the code in the above manner will yield working, but very slow executables. To run the code at full speed, you should compile it in release mode. Starting in the **build directory**, do the following:

    cmake .. -DCMAKE_BUILD_TYPE=Release
    
Followed by:

    make 
  
Your code should now run significantly (sometimes as much as ten times) faster. 

If you are using Windows, make sure to use `x64`. Running `cmake ..` should have created a Visual Studio solution file
called `a2-mass-spring-3d.sln` that you can open and build from there. Building the project will generate an .exe file. Building the project will generate an .exe file. Move the .exe file to the `build` folder so that links to the input mesh aren't broken.

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

![Poking and prodding Terry the Bunny](images/a2_results.gif)

**Github does not render the math in this Markdown. Please look at README.html to see the equations in their proper form**

### A Spring and Two Masses in 3D

The [previous assignment](https://github.com/dilevin/CSC2549-a1-mass-spring-1d) had you implement a simple simulation of a coupled mass and spring in one-dimension. In this assignment we are going to level that up to three dimensions. Specifically, the assignment 2 code reads in a a 3D *tetrahedral* mesh (created using [TetWild](https://github.com/Yixin-Hu/TetWild)) and interprets all the edges of this tetrahedral mesh as springs.  The user (you!!) will be able to poke and prod this bunny by clicking on vertices, in order to elicit, springy wobbly motion.   
  
In 1D, we used the one-dimensional position of the mass particle as our generalized coordinate. In 3D we will use the obvious extension, which is that the generalized coordinate of each mass particle will be its 3D position. For the simple case of a spring connecting two masses we will define the <img src="/tex/9d4aa2a49ae05f1b2e473e729fcbf6b4.svg?invert_in_darkmode&sanitize=true" align=middle width=36.52961069999999pt height=21.18721440000001pt/> vector

<p align="center"><img src="/tex/7be45e546e4ad63ec66772efa6530edf.svg?invert_in_darkmode&sanitize=true" align=middle width=77.88792989999999pt height=118.35734295pt/></p>

where subscripts are used to denote the particle each variable is associated with. Analogous with the 1D case, <img src="/tex/1be3f94745612d82c41d76bd8b023d1f.svg?invert_in_darkmode&sanitize=true" align=middle width=48.56657849999999pt height=30.648287999999997pt/>, where the time derivative is taken component-wise. 

The first **important** difference between the 1D and 3D cases, is the nature of our spring itself. in 1D we assumed the spring has *zero rest-length* -- in other words, it has zero potential energy when the distance between the 1D particle, and the wall to which it is affixed, is zero. This won't work for 3D shapes since it will cause them to collapse to a point (like a bunny in a blackhole). So we need to define a new potential energy, one which encourages the spring to maintain its original length. 

We are going to define a way to measure the deformation of our spring, relative to its rest, or undeformed, length -- such a measure is called a *strain* measure.

Lets define <img src="/tex/a8a9bada5e416a18738886d6bf25adfe.svg?invert_in_darkmode&sanitize=true" align=middle width=11.780891099999991pt height=26.76175259999998pt/> to be the original length of of our spring. Then we can define a simple strain measure as <img src="/tex/638b75cc95af840a82845a304888c931.svg?invert_in_darkmode&sanitize=true" align=middle width=37.10042654999999pt height=26.76175259999998pt/>. This has all the properties we want in an effective strain measure:

1. It's <img src="/tex/29632a9bf827ce0200454dd32fc3be82.svg?invert_in_darkmode&sanitize=true" align=middle width=8.219209349999991pt height=21.18721440000001pt/> when the spring has length <img src="/tex/a8a9bada5e416a18738886d6bf25adfe.svg?invert_in_darkmode&sanitize=true" align=middle width=11.780891099999991pt height=26.76175259999998pt/> (rather than being <img src="/tex/29632a9bf827ce0200454dd32fc3be82.svg?invert_in_darkmode&sanitize=true" align=middle width=8.219209349999991pt height=21.18721440000001pt/> when length is <img src="/tex/29632a9bf827ce0200454dd32fc3be82.svg?invert_in_darkmode&sanitize=true" align=middle width=8.219209349999991pt height=21.18721440000001pt/>)
2. It's invariant to rigid body transformations (i.e the strain isn't effected by translating or rotating the spring)

Now in keeping with the variational approach to mechanics, we can use this strain to define a potential energy which we can use to construct our equations of motion.

Let's define the potential energy for a single spring to be

<p align="center"><img src="/tex/12a584e333f11160905eefd682d15eac.svg?invert_in_darkmode&sanitize=true" align=middle width=126.81032265pt height=32.990165999999995pt/></p>

where <img src="/tex/63bb9849783d01d91403bc9a5fea12a2.svg?invert_in_darkmode&sanitize=true" align=middle width=9.075367949999992pt height=22.831056599999986pt/> is the mechanical stiffness of the spring, like in 1D.

Let's express <img src="/tex/a9a3a4a202d80326bda413b5562d5cd1.svg?invert_in_darkmode&sanitize=true" align=middle width=13.242037049999992pt height=22.465723500000017pt/> in terms of <img src="/tex/e73485aa867794d51ccd8725055d03a3.svg?invert_in_darkmode&sanitize=true" align=middle width=9.97711604999999pt height=14.611878600000017pt/>, which stores the current positions of the end-points of the spring. We are going to do this by introducing some useful matrices that will come in handy later when we need to take derivatives of the potential energy. 

We can compute the vector <img src="/tex/013ebda4bf7c7d81eb37f56822826b02.svg?invert_in_darkmode&sanitize=true" align=middle width=20.47934294999999pt height=22.831056599999986pt/> which points from one end of the spring to the other as

<p align="center"><img src="/tex/928572f0e184ccf4ae3d028f29bd5eff.svg?invert_in_darkmode&sanitize=true" align=middle width=134.74849905pt height=42.16451745pt/></p>

so that means the length of the spring can be written as 

<p align="center"><img src="/tex/1cc7e3dbeec0766d0c8ae74b93c612e0.svg?invert_in_darkmode&sanitize=true" align=middle width=110.83658025pt height=19.726228499999998pt/></p>

Thus <img src="/tex/774f4f90fe03a4a422386bda1d1323cb.svg?invert_in_darkmode&sanitize=true" align=middle width=225.5464398pt height=44.51174640000002pt/> which gives us the potential energy as a function of the generalized coordinates.

The kinetic energy, <img src="/tex/2f118ee06d05f3c2d98361d9c30e38ce.svg?invert_in_darkmode&sanitize=true" align=middle width=11.889314249999991pt height=22.465723500000017pt/> is defined analogously to the 1D case. In 1D, the kinetic energy was  <img src="/tex/59b53dc7fa3fac3a5de25d0b28697b6c.svg?invert_in_darkmode&sanitize=true" align=middle width=39.487902299999995pt height=27.77565449999998pt/> where <img src="/tex/6877f647e069046a8b1d839a5a801c69.svg?invert_in_darkmode&sanitize=true" align=middle width=9.97711604999999pt height=22.41366929999999pt/> was the scalar, 1D velocity. We can interpret this as the magnitude squared of the 1D velocity and thus, a reasonable 3D substitute would be 

<p align="center"><img src="/tex/dbfd190d5db201b6264f3e4a13305572.svg?invert_in_darkmode&sanitize=true" align=middle width=120.78269115pt height=32.990165999999995pt/></p>

Because <img src="/tex/2f118ee06d05f3c2d98361d9c30e38ce.svg?invert_in_darkmode&sanitize=true" align=middle width=11.889314249999991pt height=22.465723500000017pt/> is only a function of the generalized velocity and <img src="/tex/a9a3a4a202d80326bda413b5562d5cd1.svg?invert_in_darkmode&sanitize=true" align=middle width=13.242037049999992pt height=22.465723500000017pt/> is just a function of the generalized coordinate, you should be able to convince yourself that the equations of motion for a single, 3D spring are

<p align="center"><img src="/tex/5e0578a06c62ff0f465fa03ce12b93a2.svg?invert_in_darkmode&sanitize=true" align=middle width=174.6158865pt height=63.91248765pt/></p>

Now that we've seen the basics of how a single spring can be simulated, let's see what happens when we consider a whole system consisting of many springs! 

### Mass-Spring Systems  in 3D

Now let's consider the case where we have <img src="/tex/0e51a2dede42189d77627c4d742822c3.svg?invert_in_darkmode&sanitize=true" align=middle width=14.433101099999991pt height=14.15524440000002pt/> springs and <img src="/tex/55a049b8f161ae7cfeb0197d75aff967.svg?invert_in_darkmode&sanitize=true" align=middle width=9.86687624999999pt height=14.15524440000002pt/> particles (like in the case of the tetrahedral mesh representing Terry the bunny).

Whereas in the previous section, our generalized coordinate was <img src="/tex/c29c3af6a694adbdb5313c0f3c1f74f9.svg?invert_in_darkmode&sanitize=true" align=middle width=36.52961069999999pt height=21.18721440000001pt/> scalar values for two particles, in the general case, it becomes an <img src="/tex/d9ca9a53959d4cba4aef1dacd14757e9.svg?invert_in_darkmode&sanitize=true" align=middle width=38.17727759999999pt height=21.18721440000001pt/> vector

<p align="center"><img src="/tex/910aa6d8ef4ffba80222b80cc05a0d43.svg?invert_in_darkmode&sanitize=true" align=middle width=79.46140784999999pt height=207.12536955pt/></p>

We'll start with the kinetic energy of a mass-spring system because its the same formula as for a single spring ! However, because it will be convenient later on, we will replace the scalar mass <img src="/tex/0e51a2dede42189d77627c4d742822c3.svg?invert_in_darkmode&sanitize=true" align=middle width=14.433101099999991pt height=14.15524440000002pt/> with an <img src="/tex/2be744f3276b5219af5f8dd5f793e02c.svg?invert_in_darkmode&sanitize=true" align=middle width=39.82494449999999pt height=19.1781018pt/> diagonal matrix <img src="/tex/fb97d38bcc19230b0acd442e17db879c.svg?invert_in_darkmode&sanitize=true" align=middle width=17.73973739999999pt height=22.465723500000017pt/> (with the diagonal entries set to <img src="/tex/0e51a2dede42189d77627c4d742822c3.svg?invert_in_darkmode&sanitize=true" align=middle width=14.433101099999991pt height=14.15524440000002pt/>) which gives us the slightly modified kinetic energy

<p align="center"><img src="/tex/334a385f8e6ad094ea0ec2e078fb8d22.svg?invert_in_darkmode&sanitize=true" align=middle width=124.089306pt height=32.990165999999995pt/></p>

where <img src="/tex/fb97d38bcc19230b0acd442e17db879c.svg?invert_in_darkmode&sanitize=true" align=middle width=17.73973739999999pt height=22.465723500000017pt/> is called the *mass* matrix or *inertia* tensor. We'll see later in the course that mass matrices can get a lot more complicated than just diagonal. 

It's really the force computation that gets effected most by the presence of multiple springs, but its easiest to see this by starting with the potential energy. 

The potential energy of our entire mass spring system, is the sum of the potential energy of all the springs

<p align="center"><img src="/tex/515f2fea6c876d6893d44e09bbdd73fa.svg?invert_in_darkmode&sanitize=true" align=middle width=173.58462989999998pt height=47.35857885pt/></p>

where <img src="/tex/0ce8759ba31b0074488195cb976c220e.svg?invert_in_darkmode&sanitize=true" align=middle width=17.424597299999988pt height=14.611878600000017pt/> and <img src="/tex/5fccae2360e2ad15a17c271b818363aa.svg?invert_in_darkmode&sanitize=true" align=middle width=17.424597299999988pt height=14.611878600000017pt/> are the generalized coordinates (the world space positions) of the two end points of the <img src="/tex/3def24cf259215eefdd43e76525fb473.svg?invert_in_darkmode&sanitize=true" align=middle width=18.32504519999999pt height=27.91243950000002pt/> spring. 

To make things a little easier conceptually, we are going to introduce *another* set of convenience matrices <img src="/tex/d28140eda2d12e24b434e011b930fa23.svg?invert_in_darkmode&sanitize=true" align=middle width=14.730823799999989pt height=22.465723500000017pt/>. Each <img src="/tex/d28140eda2d12e24b434e011b930fa23.svg?invert_in_darkmode&sanitize=true" align=middle width=14.730823799999989pt height=22.465723500000017pt/> performs the following useful function 

<p align="center"><img src="/tex/eab2140542c15cc1cd5677f4d679ef0a.svg?invert_in_darkmode&sanitize=true" align=middle width=87.13924725pt height=39.452455349999994pt/></p>

An <img src="/tex/d28140eda2d12e24b434e011b930fa23.svg?invert_in_darkmode&sanitize=true" align=middle width=14.730823799999989pt height=22.465723500000017pt/> matrix (<img src="/tex/771ad45c675e93868d1ac8e760891e05.svg?invert_in_darkmode&sanitize=true" align=middle width=38.17727759999999pt height=21.18721440000001pt/>) selects, from all the particle positions, the end-point positions of the <img src="/tex/3def24cf259215eefdd43e76525fb473.svg?invert_in_darkmode&sanitize=true" align=middle width=18.32504519999999pt height=27.91243950000002pt/> spring. Why is this convenient ? Because we can now rewrite the potential energy of the mass-spring system in terms of <img src="/tex/e73485aa867794d51ccd8725055d03a3.svg?invert_in_darkmode&sanitize=true" align=middle width=9.97711604999999pt height=14.611878600000017pt/>. 

So we can express the potential energy of the mass-spring system like this

<p align="center"><img src="/tex/64757241892ac2208e3e39797836c8ae.svg?invert_in_darkmode&sanitize=true" align=middle width=155.3155461pt height=47.35857885pt/></p>

and now we can take derivatives of this in order to compute, say the forces. Let's see what happens if we compute the gradient of this energy with respect to the generalized coordinates. Then we get

<p align="center"><img src="/tex/c708494d94f49258634b21e76bb182dd.svg?invert_in_darkmode&sanitize=true" align=middle width=199.15742219999998pt height=47.35857885pt/></p>

and because we know that the energy, in this case, only depends on the end points of the spring (<img src="/tex/0ce8759ba31b0074488195cb976c220e.svg?invert_in_darkmode&sanitize=true" align=middle width=17.424597299999988pt height=14.611878600000017pt/> and <img src="/tex/5fccae2360e2ad15a17c271b818363aa.svg?invert_in_darkmode&sanitize=true" align=middle width=17.424597299999988pt height=14.611878600000017pt/>) we can make one final modification:

<p align="center"><img src="/tex/3020f1e8ccf3aa6be938930e43517000.svg?invert_in_darkmode&sanitize=true" align=middle width=217.42650765pt height=47.35857885pt/></p>

Let's unpack this a bit. First, you should be convinced that <img src="/tex/dff5447820ac8d06d24bb1b4a649b5d0.svg?invert_in_darkmode&sanitize=true" align=middle width=64.69755599999999pt height=33.20539859999999pt/> is just computing the potential energy gradient of  one spring with respect to its end-points, exactly like what we discussed in the previous section. The only mysterious piece is our <img src="/tex/42ed465cddb3405d6d47efce8aaf49ea.svg?invert_in_darkmode&sanitize=true" align=middle width=20.561102099999992pt height=27.6567522pt/>. That matrix is distributing the local end-point forces into the global force vector, so that they they act on the proper mass particles in the system. This summation and distribution of local forces is called **assembly** because it assembles the global force vector from local contributions. Assembly of local quantities comes up all the time in simulation, in fact we are going to see it used one more time in the next section. 

**Implementation Note:** You could implement assembly by mirroring the formula above. In other words, instantiate a sparse <img src="/tex/d28140eda2d12e24b434e011b930fa23.svg?invert_in_darkmode&sanitize=true" align=middle width=14.730823799999989pt height=22.465723500000017pt/> matrix for each spring. However that is a waste of space and resources. Instead it's better to implement the *action* of <img src="/tex/42ed465cddb3405d6d47efce8aaf49ea.svg?invert_in_darkmode&sanitize=true" align=middle width=20.561102099999992pt height=27.6567522pt/> by indexing directly into the global force vector. 

### Linearly-Implicit Time Integration

As we discussed in the previous assignment, making the right choice of time integrator is crucial for the stability, appearance and performance of a simulation. In this assignment we are going to implement one of the most famous time integrators in computer graphics, the *Linearly-Implicit Time Integrator*. This integrator is not fully-implicit like backward Euler, but it is efficient to compute and reasonably stable, thus it is a good initial place to start when integrating an elastic system like these mass-springs. 

You should be able to convince yourself that, given the potential and kinetic energies above, that the backward Euler update for a 3D, mass-spring system is

<p align="center"><img src="/tex/7ebed92dd239476c6e8ebe6c52533e3e.svg?invert_in_darkmode&sanitize=true" align=middle width=299.72579999999994pt height=104.8440393pt/></p>

Our big problem is estimating the effect of the forces at <img src="/tex/a1c582165945dd7f107dd03d3afa46e2.svg?invert_in_darkmode&sanitize=true" align=middle width=31.586827799999988pt height=26.76175259999998pt/>. Rather than do this fully, the linearly-implicit integrator linearizes the system around the current state via [Taylor expansion](https://en.wikipedia.org/wiki/Taylor_series) and uses this approximation to update the velocity. 

We proceed by exploiting the fact that <img src="/tex/a3661a1e80a6dcb9653ea41e10c924bb.svg?invert_in_darkmode&sanitize=true" align=middle width=141.40393905pt height=26.76175259999998pt/> and making the assumption that \Delta t is sufficiently small (i.e we try and find a \Delta t so that the simulation doesn't explode). Then we can replace the above update equations with

<p align="center"><img src="/tex/bf61c06bcb4d2058543902308ee2900c.svg?invert_in_darkmode&sanitize=true" align=middle width=394.92000075pt height=149.34812639999998pt/></p>

Importantly we see the appearance of the force gradient, <img src="/tex/d6328eaebbcd5c358f426dbea4bdbf70.svg?invert_in_darkmode&sanitize=true" align=middle width=15.13700594999999pt height=22.465723500000017pt/>, called the *Stiffness matrix* (this will come up again and again :) ) which encodes the change in the forcing behaviour of the object, in a local region around the current state. Because our *generalized forces* are the negative gradient of the potential energy function, the Stiffness matrix is given by

<p align="center"><img src="/tex/d4cd1979979b32d8626bcbf0221e65c7.svg?invert_in_darkmode&sanitize=true" align=middle width=114.11055975pt height=38.973783749999996pt/></p>

or <img src="/tex/d6328eaebbcd5c358f426dbea4bdbf70.svg?invert_in_darkmode&sanitize=true" align=middle width=15.13700594999999pt height=22.465723500000017pt/> is the negative Hessian of the potential energy. **Warning:** missing negative signs are the number one affliction for otherwise happy physics simulators. 

Ok, let's do one more rearrangement of these update equations to get them in their final, solvable form:

<p align="center"><img src="/tex/67e0dffe6250804ca285759cb9b51862.svg?invert_in_darkmode&sanitize=true" align=middle width=325.37646735pt height=104.8440393pt/></p>

Now we see where linearly-implicit time integration gets its performance, it requires solving a single, sparse, symmetric linear system. **Note:** A fun game is to convince yourself that <img src="/tex/d6328eaebbcd5c358f426dbea4bdbf70.svg?invert_in_darkmode&sanitize=true" align=middle width=15.13700594999999pt height=22.465723500000017pt/> is both sparse (argue from the connectivity of the mass spring system) and symmetric (argue from Newton's 3rd law).  

In practice, once we have *assembled* (there's that word again) <img src="/tex/fb97d38bcc19230b0acd442e17db879c.svg?invert_in_darkmode&sanitize=true" align=middle width=17.73973739999999pt height=22.465723500000017pt/> and <img src="/tex/d6328eaebbcd5c358f426dbea4bdbf70.svg?invert_in_darkmode&sanitize=true" align=middle width=15.13700594999999pt height=22.465723500000017pt/> we can form <img src="/tex/a966829bd31d3d09f9d885e48ee36310.svg?invert_in_darkmode&sanitize=true" align=middle width=95.04571229999998pt height=27.94539330000001pt/> and solve this linear system using off-the-shelf tools. For this assignment you can use the Simplicial LDLT solver built into the [Eigen](https://eigen.tuxfamily.org/dox/group__TopicSparseSystems.html) library. 

Let's end this section by talking about the assembly of <img src="/tex/d6328eaebbcd5c358f426dbea4bdbf70.svg?invert_in_darkmode&sanitize=true" align=middle width=15.13700594999999pt height=22.465723500000017pt/>. In the same way as for the forces, we can directly compute the second derivative of our energy

<p align="center"><img src="/tex/5959b10a05dbfd7aa528fd9c0f5e7b68.svg?invert_in_darkmode&sanitize=true" align=middle width=253.20746714999999pt height=70.02733649999999pt/></p>

Where <img src="/tex/82a33f49a74d0533eb56e74fbe05c8a2.svg?invert_in_darkmode&sanitize=true" align=middle width=18.315301949999988pt height=22.465723500000017pt/> is the local spring energy Hessian (so the Hessian of the <img src="/tex/3def24cf259215eefdd43e76525fb473.svg?invert_in_darkmode&sanitize=true" align=middle width=18.32504519999999pt height=27.91243950000002pt/> spring energy wrt to its end points). 

**WARNING:** to assemble the stiffness matrix replace <img src="/tex/82a33f49a74d0533eb56e74fbe05c8a2.svg?invert_in_darkmode&sanitize=true" align=middle width=18.315301949999988pt height=22.465723500000017pt/> with <img src="/tex/655ca15e2b101fb431577b12d4442580.svg?invert_in_darkmode&sanitize=true" align=middle width=18.61211054999999pt height=22.465723500000017pt/>, the per-spring stiffness matrix. 

Again, you could implement this using a whole bunch of <img src="/tex/d28140eda2d12e24b434e011b930fa23.svg?invert_in_darkmode&sanitize=true" align=middle width=14.730823799999989pt height=22.465723500000017pt/> matrices, but instead its better to implement it via direct indexing. In particular, in Eigen, you can construct a list of [``Eigen::Triplet``](https://eigen.tuxfamily.org/dox/group__TutorialSparse.html) objects that can be used to directly initialize a sparse matrix. 

**Hint:** a good test for any integrator is to test that your object doesn't move when no forces are exerted on it. 

### Fixed Displacement Boundary Conditions

A Mass-Spring system acting purely under gravity is very boring, it just falls straight down (a rigid motion!). What's the point of doing all this work of we don't get a wiggly bunny out of it? In order to generate wiggliness, we need to, at a minimum, fix parts of our object to the environment. These are the *boundary conditions* for the physics simulation. Because we are fixing the position of particles, these are *Dirichlet* boundary conditions. There are a few ways to go about this, but we are going to take a projection approach -- we will express our generalized coordinates using a smaller set of variables. 

Given that a fixed vertex, by definition, does not move, it makes sense to choose all other vertex positions as our subspace variables. We can now construct a linear subspace which rebuilds all the positions of our particles from this subspace --

<p align="center"><img src="/tex/2a4e5140cc24a6819aad9b19c97d3a86.svg?invert_in_darkmode&sanitize=true" align=middle width=133.40574059999997pt height=19.3534671pt/></p>

Let's say we have <img src="/tex/2f2322dff5bde89c37bcae4116fe20a8.svg?invert_in_darkmode&sanitize=true" align=middle width=5.2283516999999895pt height=22.831056599999986pt/> fixed vertices. Then <img src="/tex/622925c6024d1d57e193b773b84742a9.svg?invert_in_darkmode&sanitize=true" align=middle width=9.97711604999999pt height=23.28771720000001pt/> is an <img src="/tex/39d6e5322d2e920a00e2a3c33c348593.svg?invert_in_darkmode&sanitize=true" align=middle width=56.19105524999999pt height=24.65753399999998pt/> length vector which includes the positions of all our non-fixed particles. <img src="/tex/600e0d9f114e266f3656de01fd1a394a.svg?invert_in_darkmode&sanitize=true" align=middle width=42.862247999999994pt height=14.611878600000017pt/> is a <img src="/tex/ce0fd5ce4c3daccc9742067d69bac5aa.svg?invert_in_darkmode&sanitize=true" align=middle width=18.08608559999999pt height=21.18721440000001pt/> vector which stores a <img src="/tex/29632a9bf827ce0200454dd32fc3be82.svg?invert_in_darkmode&sanitize=true" align=middle width=8.219209349999991pt height=21.18721440000001pt/>'s in the positions of our non-fixed particles and the fixed position of fixed particles. It's the matrix <img src="/tex/df5a289587a2f0247a5b97c1e8ac58ca.svg?invert_in_darkmode&sanitize=true" align=middle width=12.83677559999999pt height=22.465723500000017pt/> that performs the magic of stitching these things together. 

<img src="/tex/df5a289587a2f0247a5b97c1e8ac58ca.svg?invert_in_darkmode&sanitize=true" align=middle width=12.83677559999999pt height=22.465723500000017pt/> is a bit like the selection matrices, the <img src="/tex/d28140eda2d12e24b434e011b930fa23.svg?invert_in_darkmode&sanitize=true" align=middle width=14.730823799999989pt height=22.465723500000017pt/>'s used to explain assembly. It selects out non-fixed vertex positions from <img src="/tex/622925c6024d1d57e193b773b84742a9.svg?invert_in_darkmode&sanitize=true" align=middle width=9.97711604999999pt height=23.28771720000001pt/> and places them in the correct position in the global <img src="/tex/e73485aa867794d51ccd8725055d03a3.svg?invert_in_darkmode&sanitize=true" align=middle width=9.97711604999999pt height=14.611878600000017pt/> vector. Fixed positions are then filled in with values from <img src="/tex/600e0d9f114e266f3656de01fd1a394a.svg?invert_in_darkmode&sanitize=true" align=middle width=42.862247999999994pt height=14.611878600000017pt/>. If we take <img src="/tex/622925c6024d1d57e193b773b84742a9.svg?invert_in_darkmode&sanitize=true" align=middle width=9.97711604999999pt height=23.28771720000001pt/> as the generalized coordinates of our physical system, we will directly encode these fixed boundary conditions into the simulator. 

This is exactly what the assignment 2 simulator does. In practice, it is  useful to implement <img src="/tex/df5a289587a2f0247a5b97c1e8ac58ca.svg?invert_in_darkmode&sanitize=true" align=middle width=12.83677559999999pt height=22.465723500000017pt/> as a sparse matrix, and you will get the chance to do just that in assignment 2. 

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

Given a point on the screen (i.e a mouse position clicked by the user), find all vertices within a given radius. **For this method, and this method alone** you are allowed to use the ``igl::unproject`` and ``igl::ray_mesh_intersect`` functions. I have provided the appropriate ray shooting function for you to use in the code as well. 

### include/linear_implicit_euler.h

Implement the linearly implicit Euler time integrator. 

