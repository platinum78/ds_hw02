# Programming Assignment #2 - Maze Escape

## 1. Just tell me how to use it!

Using the program is simple. Just following the instruction below.

1. Open the assignment directory from the console.
2. Compile the source via gcc by typing `gcc main.c -o main`
3. Put your input file in `{ASSIGNMENT_DIR}/io`.
4. Run the program with no option.
5. The output file will emerge in the same diredtory with the input file.

This program is tested on Linux os (Ubuntu 18.04). Please use Linux when evaluating this assignment.

<br>

## 2. Objectives

In this assignment, the followings are considered.

1. Finding accurate paths
2. Modulization through usage of functions (i.e. simple `main` function)
3. Simplification of the problem

In this CoC, the program finds the shortest path.

<br>

## 3. Problem Analysis

### 1. Algorithms

The identical algorithm with that from class is applied for plain path-finding.

### 2. Warping?
The real problem is that there are some *'warping zones'* in this assignment.
This makes the problem seem more difficult, but actually it's just a trick,
and does not make the problem significantly more difficult.

Since it costs no energy to move between warping points,
how far the two parping points are does not matter at all.
Therefore, what we should consider is to minimizing the energy cost while moving
from the starting point to the warping zone, and then from another warping zone to the exit point.
The problem indeed reduced into finding two shortest route!

One thing to note is that, unlike finding the nearest warping zone from the starting point,
it is unclear which warping point is the closest to the exit point.
Therefore, to find the shortest path between the warping zone to the exit point,
we can't use forward path. Instead, we can use a backward path, which starts from the exit point.

After that, we can flip the path, and then merge it with the path from starting point to warping zone.

In summary, the operating procedure of the program is as follows.

1. For warpless paths, run route-finding algorithm learned in class.
2. For warpable paths,
   * Find a forward path from the starting point to warping zone.
   * Find a backward path from the exit point to warping zone.
   * Flip the backward path to make a forward path.
   * Merge two paths into one.
3. Compare the warpless and warpable paths, and write one with shorter distance.

<br>

## 4. Performance Analysis