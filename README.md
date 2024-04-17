# PROJECT DESCRIPTION

A robot that starts in one of a four rooms setting needs to find and grab an apple, that is located in one of the four rooms. Those rooms are connected by doors, that can be open or closed. There are doors that connect rooms 0 and 1, rooms 1 and 2, rooms 1 and 3 and rooms 2 and 3. The robot can either grab the apple if both are in the same room, open a door to an adjacent room if it is closed, or move to an adjacent room if the door between them is open. In our case, the initial state consists of the robot starting in room 0 and the apple located in room 3. Doors 0 to 1, 1 to 2 and 2 to 3 are open whereas door 1 to 3 is closed. 

The problem definition and the tree search (Breadth First Search) to solve it is specified in the ProblemSpecification&Search document. In the .cpp it can be found a simple implementation of the solution in C++.
