

// -----------------------------------  STATE ,  FACADE  -----------------------------------

#include<bits/stdc++.h>

using namespace std;

class Elevator;


class ElevatorState{
    public:
        virtual ~ElevatorState()=default;
        virtual void step(Elevator& elevator)=0;
        virtual void addStop(Elevator& elevator, int floor)=0;
        virtual string getName() const=0;
};


class IdleState: public ElevatorState;
class MovingUpState: public ElevatorState;
class MovingDownState: public ElevatorState;


class Elevator{
    private:
        int id;
        int currentFloor;
        unique_ptr<ElevatorState> state;
        set<int> upStops;
        set<int, greater<int>> downStops;

    public:
        Elevator(int id, int startFloor) : id(id), currentFloor(startFloor), state(make_unique<IdleState>()) {}

        int getId() const{ return id; }

        int getCurrentFloor() const{ return currentFloor; }

        string getStateName() const{ return state->getName; }

        set<int>& getUpStops() const{ return upStops; }

        set<int>& getDownStops() const{ return downStops; }

        bool hasUpStops() const{ return !upStops.empty(); }

        bool hasDownStops() const{ return !downStops.empty(); }

        void moveOneFloorUp(){ currentFloor++; }

        void moveOneFloorDown(){ currentFloor--; }

        void setState(unique_ptr<ElevatorState> newState){
            cout << "    [Elevator " << id << "] " << state->getName() << " -> " << newState->getName() << endl;
            state=move(newState);
        }

        void addDestination(int floor){
            if(floor==currentFloor){
                cout << "  Floor " << floor << " is the current floor -- no-op." << endl;
                return;
            }

            if(floor>currentFloor) upStops.insert(floor);
            else downStops.insert(floor);

            state->addStop(*this, floor);
        }

        void step(){ state->step(*this); }

        void displayInfo(){
            //  DISPLAY
        }
};


class IdleState: public ElevatorState{
    public:
        void step(Elevator& elevator) override{ (void)elevator; }

        void addStop(Elevator& elevator, int floor) override{
            if(floor>elevator.getCurrentFloor()) elevator.setState(make_unique<MovingUpState>());
            else elevator.setState(make_unique<MovingDownState>());
        }

        string getName() const override{ return "IDLE"; }
};


class MovingUpState: public ElevatorState{
    public:
        void step(Elevator& elevator) override{
            elevator.moveOneFloorUp();

            auto& upStops=elevator.getUpStops();
            if(upStops.count(elevator.getCurrentFloor())){
                upStops.erase(elevator.getCurrentFloor());
                cout << "    [Elevator " << elevator.getId() << "] stops at floor "<< elevator.getCurrentFloor() << " (doors open)" << endl;
            }

            if(elevator.hasUpStops()) return;

            if(elevator.hasDownStops()){
                elevator.setState(make_unique<MovingDownState>());
                return;
            }

            elevator.setState(make_unique<IdleState>());
        }

        void addStop(Elevator& elevator, int floor) override{
            (void)elevator;
            (void)floor;
        }

        string getName() const override{ return "IDLE"; }
};


class MovingDownState: public ElevatorState{
    public:
        void step(Elevator& elevator) override{
            elevator.moveOneFloorDown();

            auto& downStops=elevator.getDownStops();
            if(downStops.count(elevator.getCurrentFloor())){
                downStops.erase(elevator.getCurrentFloor());
                cout << "    [Elevator " << elevator.getId() << "] stops at floor "<< elevator.getCurrentFloor() << " (doors open)" << endl;
            }

            if(elevator.hasDownStops()) return;

            if(elevator.hasUpStops()){
                elevator.setState(make_unique<MovingUpState>());
                return;
            }

            elevator.setState(make_unique<IdleState>());
        }

        void addStop(Elevator& elevator, int floor) override{
            (void)elevator;
            (void)floor;
        }

        string getName() const override{ return "IDLE"; }
};


class ElevatorSystem{
    private:
        int numFloors;
        vector<unique_ptr<Elevator>> elevators;

        bool isValidFloor(int floor) const{ return floor>=1 && floor<=numFloors; }

        Elevator* findElevatorById(int id){
            for(auto& e: elevators){
                if(e->getId()==id) return e.get();
            }

            return nullptr;
        }

    public:
        ElevatorSystem(int numFloors, int numElevators, int startFloor=1) : numFloors(numFloors){
            for(int i=1; i<=numElevators; i++) elevators.push_back(make_unique<Elevator>(i, startFloor));
        }

        bool requestElevator(int floor){
            if(!isValidFloor(floor)) return false;

            Elevator* best=nullptr;
            for(auto&e: elevators){
                if(!best || abs(e->getCurrentFloor-floor)<abs(best->getCurrentFloor-floor)) best=e.get();
            }

            best->addDestination(floor);
            return true;
        }

        bool selectFloor(int elevatorId, int floor){
            if(!isValidFloor(floor)) return false;

            Elevator* e = findElevatorById(elevatorId);
            if(!e) return false;
            e->addDestination(floor);

            return true;
        }

        void step(){
            cout << "\n--- tick ---" << endl;
            for(auto& e : elevators) e->step();
            displayStatus();
        }

        void displayStatus() const{ for(auto& e : elevators) e->displayStatus(); }
};