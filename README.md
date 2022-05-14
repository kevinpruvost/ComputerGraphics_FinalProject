# Assignment 6 of Fundamentals of Computer Graphics: Illumination Effect

<p align="center">
  <img src="https://github.com/kevinpruvost/OpenImages/blob/miscellaneous/images/1200px-Tsinghua_University_Logo.svg.png" width=250/><br/><br/>
</p>

# Introduction

For this assignment, the main theme was `Illumination Effect`.

The objectives here were:

1) Load the 3D model, then rotate and zoom it with the mouse or keyboard.

2) Using the combination of object’s material, ambient light, diffuse light and specular light

to generate different illumination effect.

3) All faces have the same color.

Depending on the different calculation ways for normals ("vertex" normal vs "face" normal), you will get a smooth model or a less-smooth one (Figure 1,2,3 vs Figure 4). You only need to implement one of them.

# Compilation

This project and all of my CG projects will be compiled with CMake, if you open the project directly with Visual Studio, you should be able to directly compile it.
Though, as CMake permits it, you will be easily able to compile on other platforms.

# Demonstration

## Content

For my Assignment, I got all these points covered:

- 3D Model loaded and movable with the ImGUI interface
- Illumination effect (ambient, diffuse & specular) added
- All faces have the same color
- Every normals calculation methods are implemented (flat vs. smooth)
- Bonus:
    - Display Mode (Vertices, Wireframe, Faces)
    - Enable/Disable GUI
    - X,Y,Z axis are displayed (in the bottom-left corner) to know where the models movement will occur
    - Both Light source & illuminated object are movable
    - Light properties, Material properties and Mesh Type (flat or smooth) are editable

## How to use it

[![Demo](embed.png)](https://youtu.be/ZxSr5YX9_Ec)

You can launch the `Assignment6.exe`directly, if you already have Visual C++ Redistribuable.

The program was compiled in Release mode.

As it is displayed in the program, here are the controls by order of priority for the assignment:

## Controls

- Enable/Disable GUI: T
- Wireframe Color Change: P
- Camera/Cursor Lock: L
- Change Display Mode: C
- Sun Movement:
    - X : ⬅️➡️
    - Y: ⬆️ ⬇️
    - Z: LSHIFT + ⬅️➡️
- Camera Movement:
    - Forward: W (or Z on AZERTY layout)
    - Backward: S
    - Left: A (or Q on AZERTY layout)
    - Right: D
    - Up: Space
    - Down: Left CTRL

## Additional Interactions

- You can change the Display Mode on the `Display Mode`menu.
- You can change the FPS cap/limit
- You can set/unset Auto-Rotation
- You can change the Time multiplier (multiplies deltaTime)
- You can change parameters related to light, material & mesh

# Code Architecture

The Complete Documentation is available in the project, in the Code Architecture part will only be explained a small description of what classes do.

## Documentation

There is a documentation available [here](https://kevinpruvost.github.io/ComputerGraphics_FinalProject/) or in `docs/index.html` if you want to have a better view on the classes I made.
