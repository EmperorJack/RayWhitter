# RayWhitter
A whitted ray tracer written in C++ and informed by PBRT.
Repo hosted at https://github.com/EmperorJack/RayWhitter

Libraries used:
- *glm* for vector and matrix math.
- *jpge* for jpeg compression.

## Usage
CMake files are provided for building the project, CMake verion 3.7 is required.

Program arguments: `./RayWhitter "path_to_scene_file" "out_file_name.jpg"`

Example usage: `./Raywhitter "../res/teapot.txt" "teapot.jpg"`

A number of scene files are available in `/res`:
- spheres.txt
- teapot.txt
- bunny.txt
- demo.txt
- scene.txt

Jpeg is the only supporting image output format.

*Note:*
The scene files and example usage above assume the following directory structure for referencing resources:
- build-release/
  - RayWhitter executable
  - ...
- res/
  - teapot.txt
  - teapot.obj
  - ...
- src/
  - ...
- ...

Make sure to change the paths to obj files in the scene files if using a different directory structure.
