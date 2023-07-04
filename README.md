# Develop a parallelized simulation system using OpenMP to analyze and present the trajectory of 1,000 balls

## Introduction
Considered 1000 balls in a specified rectangular space and I had to
run 720000 iterations by parallezing the code by OpenMP to show trajectory of particles where attraction of force was
taken into account.

## Installation
1. Clone the repository:

   ```shell
   git clone https://github.com/oxone-999/Coding-Problem-Search-Engine.git

2. Install dependencies:
   Install mayavi and its dependencies

   ```shell
   pip install mayavi
   ```
   
   ```shell
   pip install PyQt5
   ```
   Intall any other library or framework required if you dont have for mayavi, refer its official documentation if error comes
   
## Usage

- Run main_omp.cpp to make various trajectory files as mentioned in the Trajectory folder
- Trajectory.txt contains inital x,y,z coordinates of the balls
- Keep vis.py in the same folder where rest of the Trajectory files are like for in this case move vis.py in Trajectory folder and then run it.

## Result

- Achieved a remarkable 340% improvement by leveraging OMP directives, accelerating simulation speed from 8.4 hours to 2.4 hours
### 3D Visualiser
Balls present in the rectangular space
![Screenshot 2023-07-04 134216](https://github.com/abhirajrs999/Many-Body-Problem/assets/84474314/ef56d635-7152-4a06-8d6d-8dc0adc65c3f)


![Screenshot 2023-07-04 134233](https://github.com/abhirajrs999/Many-Body-Problem/assets/84474314/712e86ed-571e-4cc5-8128-7b212e7a6108)


