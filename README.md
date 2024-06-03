# OpenGL Base for CG

This is the boiler template for OpenGL. It contains the base needed for OpenGL to be up and running with few library packed, in <code>include</code> directory. 

  - GLFW - OpenGL Context (for creating windows, handling inputs...)
  - GLAD - OpenGL Loaders (to load openGL function pointers in ease)
  - GLM - Math Library(includes vectors, matrix,...)
  - Freetype - Text Loading Library (Glyph based)
  - Stb - Image manipulation (reading, writing,...)

##### #Note : For fresh (Apple silicon) make sure to install few dependencies from homebrew
`brew install gcc make freetype glfw glm`

and also check if it is corretly installed in your system afterwards in terminal with commands
- `g++ --version`
- `make --version`

## Follow as :
- Clone the project `git clone https://github.com/Thaparoshan143/OpenGL_Sandbox`
- Change the code as per need with entry point at main.cpp (in sandbox)
  - Makefile is used for the compiling and building. Change the working directory to builds (to use makefile for compiling) in terminal. Inspect and change Makefile according to requirement.
  - type command in terminal : `make build_m` (Platform will be auto detected by Makefile)
  - Then run test (is inside `builds/Exe/`)

**If only basic boiler required, use main branch** : [here](https://github.com/Thaparoshan143/OpenGL_Sandbox)

#### Warning: This boiler is made using mac system (M1). Few things might need change on other patform eg: -
- directory change with / $\rightarrow$ \
- Few of the dll might be missing while .exe launch

<h5>Note: dll should be in same folder as .exe (in window)<h5>