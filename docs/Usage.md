# Usage (g++)


Given all dependency requirements are satisfied, this code runs from the following commands:

First clone and enter the repository:
```bash
git clone https://github.com/DOWingard/Minkowski-Sum-Real-Time-Visualizer-with-Convex-Triangulation.git
cd Minkowski-Sum-Real-Time-Visualizer-with-Convex-Triangulation
```
Then, 
```bash 
make task-name NAME=<program-name>
```
Upon compiling, you can run the program with 
```bash
./<program-name>
```

Provide any name desired for the tasks, but established tasks in .vscode/tasks.json are given by the following `task-name`:

main:
```bash
main
```
Minkowski Sum Unit Test:
```bash
test-minkowski
```
Decomposition Unit Test:
```bash
test-triangulator
```

## Dependencies
This code is compiled on Ubuntu, including the following external libraries:
* GLEW
* GLFW
* GLM

## Ubuntu
```bash
sudo apt install libglew-dev libglfw3-dev libglm-dev
```

This code should run no problem on windows and mac as well (I have no genuine confidence in this) with:

## Windows:
```powershell
winget install glew
winget install glfw
winget install glm
```

## MacOS:

with Homebrew:
```bash
brew install glew glfw glm
```
with Conda/Miniconda:
```bash
conda install -c conda-forge glew glfw2 glm
```

Manual Download and Extract:
```bash
# GLEW
curl -L https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz -o glew.tgz

# GLFW
curl -L https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.zip -o glfw.zip

# GLM
curl -L https://github.com/g-truc/glm/releases/download/0.9.9.8/glm-0.9.9.8.zip -o glm.zip
```


