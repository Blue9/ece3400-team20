# Milestone 3

## Maze Exploration using Depth First Search

To be able to map the entire maze, a right hand wall traversal is not sufficient, as it will not be able to reach isolated elements in the body of the maze. Instead, our robot will implement a depth first search (DFS) to map the entire maze.

### Depth First Search Algorithm
To begin we have an empty stack to signify the nodes which on our "frontier". The frontier is all of the intersections in which we have been adjacent to and could have moved too, but have not yet visited. At each intersection we add all of adjacent nodes that do not have a wall inbetween to the fronteir stack with the condition that we have not yet visited said node. This can be checked by looking at the 2D array we have to map the maze from **Lab 3**.

The image below shows how the DFS tree is created and the corresponding frontier stack:
  
  ***DFS TREE PIC***

To choose which node to navigate to the top element of the frontier stack is popped giving the coordinates of the closest unexplored node in the DFS tree. For the majority of exploration this node will be adjacent to the current so we can simply turn or continue straight. If the node is not adjacent, we must backtrack untill it is adjacent, requiring another data structure: a stack of previous moves. This stack will add the past move to the stack so we can retrace our path to find the next node in the frontier set. The robot will continue to retrace until the next node is adjacent when it will move to the node and then repeat the process. We also ensure that when backtracking the moves are not added to the previous moves stack to avoid oscillations in movement.

When elements are pushed to the frontier stack, if available, the node which is directly in front of the robot will be placed on the top of the stack to prioritize forward movement over turning. This is becuase turning at an intersection takes significantly more time than moving straight and thus should be avoided as given the option.

The order nodes are pushed to the stack is LEFT, RIGHT, FRONT, so the following would be the mapping of a 4x5 maze with a single wall:

   ***DFS MAP PIC***
   
*NOTE: The above algorithm and graphics were taken from ECE 3400 LEC16 - 2018*

### Algorithm Integration


### Testing
