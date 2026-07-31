

// -----------------------------------  FACADE  -----------------------------------


#include<bits/stdc++.h>

using namespace std;


enum class Direction{
    UP,
    DOWN,
    IDLE
};

string directionToString(Direction d){
    switch(d){
        case Direction::UP : return "UP";
        case Direction::DOWN : return "DOWN";
        case Direction::IDLE : return "IDLE";
    }
    return "UNKNOWN";
}


class Elevator{
    private:
        int id;
        int currentFloor;
        Direction direction;
        set<int> upStops;
        set<int, greater<int>> downStops;

    public:
        Elevator(int id, int startFloor) : id(id), currentFloor(startFloor), direction(Direction::IDLE) {};

        int getId() const{ return id; }

        int getCurrentFloor() const{ return currentFloor; }

        Direction getDirection() const{ return direction; }

        bool hasPendingStops() const{ return !upStops.empty() || !downStops.empty(); }

        void addStop(int floor){
            if(floor==currentFloor) return;

            if(floor>currentFloor) upStops.insert(floor);
            else downStops.insert(floor);

            if(direction==Directions::IDLE) direction=(floor>currentFloor) ? Direction::UP : Direction::DOWN;
        }

        bool canServeNow(int floor, Direction requestedDirection) const{
            if(direction==Direction::IDLE) return true;
            if(direction==Direction::UP && requestedDirection==Direction::UP && floor>=currentFloor) return true;
            if(direction==Direction::DOWN && requestedDirection==Direction::DOWN && floor<=currentFloor) return true;

            return false;
        }

        int distanceTo(int floor) const{ return abs(currentFloor-floor); }

        void step(){
            if(direction==Direction::IDLE){
                if(!upStops.empty()) direction=Direction::UP;
                else if(!downStops.empty()) direction=Direction::DOWN;
                else return;
            }

            if(direction==Direction::UP){
                currentFloor++;
                if(upStops.count(currentFloor)){
                    upStops.erase(currentFloor);
                    cout << "    [Elevator " << id << "] stops at floor " << currentFloor << " (doors open)" << endl;
                }
                
                if(upStops.empty()) direction=downStops.empty() ? Direction::IDLE : Direction::DOWN;
            }
            else if(direction==Direction::DOWN){
                currentFloor--;
                if(downStops.count(currentFloor)){
                    downStops.erase(currentFloor);
                    cout << "    [Elevator " << id << "] stops at floor " << currentFloor << " (doors open)" << endl;
                }
                
                if(downStops.empty()) direction=upStops.empty() ? Direction::IDLE : Direction::UP;
            }
        }

        void displayStatus() const{
            //   DISPLAY
        }
};


class ElevatorController{
    private:
        int numFloors;
        vector<unique_ptr<Elevator>> elevators;

        bool isValidFloor(int floor) const{ return  floor>=1 && floor<=numFloors; }

        Elevator* findElevatorById(int id){
            for(auto& e: elevators){
                if(e->getId()==id) return e.get();
            }

            return nullptr;
        }

    public:
        ElevatorController(int numFloors, int numElevators, int startFloor=1) : numFloors(numFloors){
            for(int i=1; i<=numElevators; i++) elevators.push_back(make_unique<Elevator>(i, startFloor));
        }

        ElevatorController(int numFloors, vector<int> startFloors) : numFloors(numFloors){
            int idCounter=1;
            for(int sf: startFloors) elevators.push_back(make_unique<Elevator>(idCounter++, sf));
        }

        bool requestElevator(int floor, Direction direction){
            if(!isValidFloor) return false;

            if(direction!=Direction::UP && direction!=Direction::DOWN) return false;

            for(auto& e: elevators){
                if(e->getCurrentFloor()==floor && (e->getDirection()==Direction::IDLE || e->getDirection()==direction)){
                    cout << "  Hall call at floor " << floor << " already served (elevator "<< e->getId() << " is right there)." << endl;
                    return true;
                }
            }

            Elevator* best=nullptr;
            for(auto& e: elevators){
                if(e->canServeNow(floor, direction)){
                    if(!best || e->distanceTo(floor) < best->distanceTo(floor)) best = e.get();
                }
            }

            if(!best){
                for(auto& e: elevators){
                    if(!best || e->distanceTo(floor) < best->distanceTo(floor)) best = e.get();
                }
            }

            best->addStop(floor);
            cout << "  Hall call at floor " << floor << " (" << directionToString(direction) << ") assigned to elevator " << best->getId() << endl;

            return true;
        }

        bool selectFloor(int elevatorId, int floor){
            if(!isValidFloor(floor)) return false;

            Elevator* e=findElevatorById(elevatorId);
            if(!e) return false;

            e->addStop(floor);
            cout << "  Destination floor " << floor << " selected inside elevator " << elevatorId << endl;

            return true;
        }

        void step(){
            cout << "\n--- tick ---" << endl;
            for(auto& e: elevators) e->step();
            displayStatus();
        }

        void displayStatus(){
            //   DISPLAY
        }
};