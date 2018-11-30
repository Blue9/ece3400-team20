# Milestone 4

## Motivation 

In choosing our maze traversal algorithm, we decided on using DFS as opposed to BFS due to its lower space complexity.  At a
high level, we decided to create two stacks--one containing past moves and one containing an unexplored frontier set.  At each
intersection, we update the surroundings, storing the adjacent walls in memory, and at the same time, we push new moves onto a 
stack.  Now we peek at the top of the stack and if it's a possible move, we move there.  Else, we backtrack by popping off of 
stack.

## Implementing DFS for Maze Traversal

### Updating surroundings

### Moving to a new location

### Backtracking

### Implementation/Code
