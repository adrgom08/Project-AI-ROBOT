// Programming assignment consisting of an implementation of a scenario where a robot can move between adjacent rooms,
// open the doors between rooms if they are closed, and grabbing an apple when it finds it in a room 

// by Adrián Gómez Lamuedra 

#include <iostream>
#include <stack>
#include <vector>
#include <string>

using namespace std;

struct State {
	int robotPos;
	vector<bool> doors;
	bool appleGrabbed;

	bool operator== (const State& s) {
		if (s.robotPos != robotPos || s.appleGrabbed != appleGrabbed) return false;
		for (int i = 0; i < 4; ++i) {
			if (s.doors[i] != doors[i]) return false;
		}
		return true;
	}
};

struct NodeDouble {
	State state;
	NodeDouble* next;
	NodeDouble* prev;

public:
	NodeDouble() { next = NULL, prev = NULL; }
};

struct NodeSimple {
	State state;
	NodeSimple* next;

public:
	NodeSimple() { next = NULL; }
};

void insert_open(NodeSimple** head_open, NodeDouble** head_closed, NodeDouble** parents, NodeDouble** father, State const& s) { // Insert in the back of the open list
	NodeSimple* back = new NodeSimple();
	back->state = s;

	NodeDouble* current_closed = *head_closed; // We search for the node in the closed list 
	if (current_closed != NULL) { // If the closed list is empty then the node is not there
		while (current_closed != NULL) {
			if (current_closed->state == s) return; // The new node has already been expanded 
			current_closed = current_closed->next;
		}
	}

	if (*head_open == NULL) { // Id the list is empty so the head is the new node and we are done
		*head_open = back;
	}
	else {
		NodeSimple* current_open = *head_open; // We are going to check if the node is already in the open list
		while (current_open->next != NULL) {
			if (current_open->state == s) return; // The new node has already been expanded 
			current_open = current_open->next;
		}
		if (current_open->state == s) return; // We must check the last element 

		current_open->next = back; // The old last node has the new one as the next node
	}

	NodeDouble* current_parents = *parents;
	while (current_parents->next != NULL) { // No need to check empty list because at least initial state is in it 
		current_parents = current_parents->next;
	}
	
	NodeDouble* child = new NodeDouble();
	child->state = s;
	child->prev = *father; // The node saves its father 

	current_parents->next = child;
}

void insert_closed(NodeDouble** head, NodeDouble** parents, NodeDouble** back) {
	NodeDouble* current_parents = *parents;
	NodeDouble* last = *back;
	while (!(current_parents->state == last->state)) { // No need to check empty list because at least initial state is in it 
		current_parents = current_parents->next;
	}
	last->prev = current_parents->prev; // We add the node with pointer to its parent
	NodeDouble* current = *head;
	if (current == NULL) { // If the list is empty the new node is the head
		*head = last;
		return;
	}

	while (current->next != NULL) { // Insertion of a node in the closed list can be done just once, no repeated nodes 
		current = current->next;
	}
	current->next = last;
}

NodeDouble* delete_open(NodeSimple** head) {
	NodeSimple* front = *head; // Take the first node
	NodeDouble* copy = new NodeDouble(); // Change it to a double link and copy the state
	copy->state = front->state;
	*head = front->next; // Delete the node from open

	return copy;
}

void pickUp(NodeSimple** head_open, NodeDouble** head_closed, NodeDouble** parents, NodeDouble** father, int applePos, State const& s) {
	if (s.robotPos == applePos) { // We chcek if the robot is in the kitchen (or other specified room)
		State s1 = s;
		s1.appleGrabbed = 1; // Operator 1
		insert_open(head_open, head_closed, parents, father, s1); // We add a node with the apple picked to open
	}
}

void go(NodeSimple** head_open, NodeDouble** head_closed, NodeDouble** parents, NodeDouble** father, State const& s) {
	switch (s.robotPos) {
	case 0: // Starting point dining room
		if (s.doors[0] == true) { // Operator 2, door 0 has to be open
			State s2 = s;
			s2.robotPos = 1;
			insert_open(head_open, head_closed, parents, father, s2);
		}
		break;
	case 1: // Starting point family room
		if (s.doors[0] == true) { // Operator 3, door 0 has to be open
			State s3 = s;
			s3.robotPos = 0;
			insert_open(head_open, head_closed, parents, father, s3);
		}
		if (s.doors[1] == true) { // Operator 4, door 1 has to be open
			State s4 = s;
			s4.robotPos = 2;
			insert_open(head_open, head_closed, parents, father, s4);
		}
		if (s.doors[2] == true) { // Operator 6, door 2 has to be open
			State s6 = s;
			s6.robotPos = 3;
			insert_open(head_open, head_closed, parents, father, s6);
		}
		break;
	case 2: // Starting point living room
		if (s.doors[1] == true) { // Operator 5, door 1 has to be open
			State s5 = s;
			s5.robotPos = 1;
			insert_open(head_open, head_closed, parents, father, s5);
		}
		if (s.doors[3] == true) { // Operator 8, door 3 has to be open
			State s8 = s;
			s8.robotPos = 3;
			insert_open(head_open, head_closed, parents, father, s8);
		}
		break;
	case 3: // Starting point kitchen
		if (s.doors[2] == true) { // Operator 7, door 2 has to be open
			State s7 = s;
			s7.robotPos = 1;
			insert_open(head_open, head_closed, parents, father, s7);
		}
		if (s.doors[3] == true) { // Operator 9, door 3 has to be open
			State s9 = s;
			s9.robotPos = 2;
			insert_open(head_open, head_closed, parents, father, s9);
		}
		break;
	}
}

void openDoor(NodeSimple** head_open, NodeDouble** head_closed, NodeDouble** parents, NodeDouble** father, State const& s) {
	if (s.robotPos == 0 || s.robotPos == 1) { // Operator 10, robot has to be in pos 0 or 1
		if (s.doors[0] == false) { // Door 0 has to be closed 
			State s10 = s;
			s10.doors[0] = true;
			insert_open(head_open, head_closed, parents, father, s10);
		}
	}
	if (s.robotPos == 1 || s.robotPos == 2) { // Operator 11, robot has to be in pos 1 or 2
		if (s.doors[1] == false) { // Door 1 has to be closed 
			State s11 = s;
			s11.doors[1] = true;
			insert_open(head_open, head_closed, parents, father, s11);
		}
	}
	if (s.robotPos == 1 || s.robotPos == 3) { // Operator 12, robot has to be in pos 1 or 3
		if (s.doors[2] == false) { // Door 2 has to be closed 
			State s12 = s;
			s12.doors[2] = true;
			insert_open(head_open, head_closed, parents, father, s12);
		}
	}
	if (s.robotPos == 2 || s.robotPos == 3) { // Operator 13, robot has to be in pos 2 or 3
		if (s.doors[3] == false) { // Door 3 has to be closed 
			State s13 = s;
			s13.doors[3] = true;
			insert_open(head_open, head_closed, parents, father, s13);
		}
	}
}

string transformRoom(int i) { // We operate with integers but print on screen the whole name
	if (i == 0) return "dining room";
	else if (i == 1) return "family room";
	else if (i == 2) return "living room";
	else return "kitchen";
}

stack<string> backtrackSolution(NodeDouble** goal) {
	stack<string> v;

	NodeDouble* parent = *goal;
	while (parent->prev != NULL) {
		if (parent->state.appleGrabbed !=
			parent->prev->state.appleGrabbed) { // if appleGrabbed changes is because the apple has been picked 
			v.push("Pick up apple\n");
		}
		else if (parent->state.robotPos != parent->prev->state.robotPos) { // If robotpos changes is because it has moved
			v.push("Go from " + transformRoom(parent->prev->state.robotPos) + " to " +
				transformRoom(parent->state.robotPos) + "\n");
		}
		else { // In any other case a door has been open
			int room1 = 0, room2 = 0;
			for (int i = 0; i < 4; ++i) { // We search for the door that has been open from the room where robot is
				if (parent->prev->state.doors[i] != parent->state.doors[i]) {
					if (i == 0) { // We check the door that has been open and the room from where it was open
						if (parent->state.robotPos == 0) {
							room1 = 0; room2 = 1;
						}
						else {
							room1 = 1; room2 = 0;
						}
					}
					else if (i == 1) {
						if (parent->state.robotPos == 1) {
							room1 = 1; room2 = 2;
						}
						else {
							room1 = 2; room2 = 1;
						}
					}
					else if (i == 2) {
						if (parent->state.robotPos == 1) {
							room1 = 1; room2 = 3;
						}
						else {
							room1 = 3; room2 = 1;
						}
					}
					else {
						if (parent->state.robotPos == 2) {
							room1 = 2; room2 = 3;
						}
						else {
							room1 = 3; room2 = 2;
						}
					}
					break;
				}
			}
			v.push("Open door from " + transformRoom(room1) +
				" to " + transformRoom(room2) + "\n");
		}

		parent = parent->prev; // We backtrack to its parent
	}
	return v; // We return the solution as a stack of strings to be printed
}

pair<bool, stack<string>> algorithm(State const& start, int applePos) {
	NodeSimple* head_open = new NodeSimple(); 
	NodeDouble* head_parents = new NodeDouble();
	NodeDouble* head_closed = NULL; // Closed = []

	head_open->state = start; // Open = [start]
	head_parents->state = start;

	while (head_open != NULL) {
		NodeDouble* x = delete_open(&head_open); // x = remove from open

		if (x->state.appleGrabbed == 1) { // We return the solution when x->state is goal state
			NodeDouble* current = head_parents;
			while (!(current->state == x->state)) { // As we have not inserted x to closed, we have to search for its parent
				current = current->next;
			}
			x->prev = current->prev;
			return { true, backtrackSolution(&x) };
		}
		else {
			insert_closed(&head_closed, &head_parents, &x); // Add x to closed

			pickUp(&head_open, &head_closed, &head_parents, &x, applePos,x->state); // Generate sons of x and add them to open
			go(&head_open, &head_closed, &head_parents, &x, x->state);
			openDoor(&head_open, &head_closed, &head_parents, &x, x->state);
		}
	}
	return { false, {} }; // We have not found a solution
}

int main() {
	int robotPos, applePos, aux;
	vector<bool> doors(4);


	cout << "Choose an initial position for robot (0: dining room, 1: family room, 2: living room, 3: kitchen): ";
	cin >> robotPos;
	cout << "Choose an initial position for apple (0: dining room, 1: family room, 2: living room, 3: kitchen): ";
	cin >> applePos;
	while (0 > robotPos || robotPos > 3 || 0 > applePos || applePos > 3) {
		cout << "\nThe position of the apple and the robot must be between 0 and 3\n";

		cout << "Choose an initial position for robot (0: dining room, 1: family room, 2: living room, 3: kitchen): ";
		cin >> robotPos;
		cout << "Choose an initial position for apple (0: dining room, 1: family room, 2: living room, 3: kitchen): ";
		cin >> applePos;
	}

	cout << "\nChoose a door configurarion (0: closed, 1: open):\n";
	cout << "Door between dining room and family room: ";
	cin >> aux;
	while (aux < 0 || aux > 1) {
		cout << "\nNumber must be either 0 or 1\n";

		cout << "Door between dining room and family room: ";
		cin >> aux;
	}
	doors[0] = aux;

	cout << "Door between family room and living room: ";
	cin >> aux;
	while (aux < 0 || aux > 1) {
		cout << "\nNumber must be either 0 or 1\n";

		cout << "Door between family room and living room: ";
		cin >> aux;
	}
	doors[1] = aux;

	cout << "Door between family room and kitchen: ";
	cin >> aux; 
	while (aux < 0 || aux > 1) {
		cout << "\nNumber must be either 0 or 1\n";

		cout << "Door between family room and kitchen: ";
		cin >> aux;
	}
	doors[2] = aux;

	cout << "Door between living room and kitchen: ";
	cin >> aux; 
	while (aux < 0 || aux > 1) {
		cout << "\nNumber must be either 0 or 1\n";

		cout << "Door between living room and kitchen:  ";
		cin >> aux;
	}
	doors[3] = aux;

	State initial = { robotPos, doors, false };

	pair<bool, stack<string>> result = algorithm(initial, applePos);

	if (result.first) {
		cout << "\nA possible solution is: \n";
		while (!result.second.empty()) {
			cout << result.second.top();
			result.second.pop();
		}
	}
	else cout << "\nThere is no solution for these specifications: \n";

	return 0;
}
