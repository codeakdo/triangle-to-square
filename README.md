# OpenGL Triangle to Square Transformation 
<p>
  <img alt="Version" src="https://img.shields.io/badge/version-1.0.0-blue.svg?cacheSeconds=2592000" />
  <a href="#" target="_blank">
    <img alt="License: ISC" src="https://img.shields.io/badge/License-ISC-yellow.svg" />
  </a>
</p>

This project demonstrates interactive OpenGL rendering by building a square from triangles using mouse input. The application showcases basic OpenGL concepts including shader creation, vertex buffer management, and user interaction.

## Description

The program allows you to build a square by adding triangles one by one through mouse clicks:
- Left-click adds triangles in a clockwise direction
- Right-click adds triangles in a counter-clockwise direction
- Clicking with the opposite button removes triangles in reverse order

When four triangles are added, they form a complete square centered on the origin of the coordinate system.

## Features

- OpenGL 3.3 Core Profile implementation
- GLSL shader programming
- Interactive triangle creation
- Dynamic vertex buffer management
- Mouse-based user interface

## Dependencies

- GLEW (The OpenGL Extension Wrangler Library)
- GLFW (Graphics Library Framework)
- GLM (OpenGL Mathematics)
- C++11 or higher
- OpenGL 3.3+ compatible graphics hardware

## Building the Project

### On macOS with Xcode

This project includes an Xcode project file that can be opened and built directly.

1. Make sure you have the required libraries installed:
   ```
   brew install glew glfw glm
   ```

2. Open the Xcode project file `Opengl 4 triangle.xcodeproj`
3. Build and run the project from Xcode

### On other platforms

1. Install the required dependencies (GLEW, GLFW, GLM)
2. Create a build configuration for your preferred build system
3. Compile the `main.cpp` file with appropriate linker flags for OpenGL, GLEW, and GLFW

## Usage

1. Run the application
2. Left-click to add triangles clockwise
3. Right-click to add triangles counter-clockwise
4. Continue clicking to add up to 4 triangles
5. Click with the opposite button to remove triangles
6. Close the window to exit the application

## Author

* Website: https://github.com/codeakdo
* Github: [@codeakdo](https://github.com/codeakdo)

## Show your support

Give a ⭐️ if this project helped you!

***
This Project was Coded by ❤️ codeakdo
