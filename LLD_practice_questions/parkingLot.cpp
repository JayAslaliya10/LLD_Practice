

// -----------------------------------  FACADE  -----------------------------------

#include<bits/stdc++.h>

using namespace std;

enum class VehicleType {
    CAR,
    MOTORCYCLE,
    TRUCK,
    BUS
};

class Vehicle{
    private:
        string licensePlate;
        VehicleType type;
        string color;

    public:
        Vehicle(string licensePlate, VehicleType type, string color) : licensePlate(licensePlate), type(type), color(color) {}
    
        string getLicensePlate() const{ return licensePlate; }

        VehicleType getType() const{ return type; }

        string getColor() const{ return color; }

        void displayInfo() const{
            //  DISPLAY
        }
};


enum class SpotType{
    COMPACT,
    REGULAR,
    LARGE
};

class ParkingSpot{
    private:
        int spotNumber;
        SpotType type;
        Vehicle* vehicle;
        bool available;

    public:
        ParkingSpot(int spotNumber, SpotType type) : spotNumber(spotNumber), type(type), vehicle(nullptr), available(true) {}

        int getSpotNumber() const { return spotNumber; }

        SpotType getSpotType() const { return type; }

        Vehicle* getVehicle() const { return vehicle; }

        bool isAvailable() const { return available; }

        bool canFitVehicle(Vehicle* vehicle) const{
            if(!vehicle) return false;

            switch(vehicle->getType()){
                case VehicleType::MOTORCYCLE: return true;
                case VehicleType::CAR: return type != SpotType::COMPACT;

                case VehicleType::TRUCK:
                case VehicleType::BUS: return type == SpotType::LARGE;
            }

            return false;
        }

        bool parkVehicle(Vehicle* vehicle){
            if(!available || !canFitVehicle(vehicle)) return false;

            this->vehicle=vehicle;
            available=false;

            return true;
        }

        Vehicle* removeVehicle(){
            if(!vehicle) return nullptr;

            Vehicle* removed=vehicle;
            vehicle=nullptr;
            available=true;

            return removed;
        }

        void displayInfo() const{
            //  DISPLAY JO BHI DIKHANA HO
        }
};

class ParkingLot{
    private:
        int capacity;
        int availableSpots;
        vector<ParkingSpot*> spots;
        map<string, ParkingSpot*> occupiedSpots;

        ParkingSpot* findAvailableSpot(const Vehicle* vehicle) const{
            for(auto spot: spots){
                spot->displayInfo();
            }
        }

    public:
        ParkingLot(int compact, int regular, int large) : capacity(compact+regular+large), availableSpots(capacity){
            int spotNumber=1;

              for (int i = 0; i < compact; i++) spots.push_back(new ParkingSpot(spotNumber++, SpotType::COMPACT));
              for (int i = 0; i < regular; i++) spots.push_back(new ParkingSpot(spotNumber++, SpotType::REGULAR));
              for (int i = 0; i < large; i++) spots.push_back(new ParkingSpot(spotNumber++, SpotType::LARGE));
        }

        ~ParkingLot(){
            for(auto spot: spots) delete spot;
        }

        int getCapacity() const { return capacity; }

        int getAvailableSpots() const { return availableSpots; }

        bool parkVehicle(Vehicle *vehicle){
            if(!vehicle) return false;

            if (occupiedSpots.find(vehicle->getLicensePlate()) != occupiedSpots.end()) return false;

            ParkingSpot *spot=findAvailableSpot(vehicle);
            if(!spot) return false;

            if (spot->parkVehicle(vehicle)) {
                occupiedSpots[vehicle->getLicensePlate()] = spot;
                availableSpots--;
                return true;
            }

            return false;
        }

        Vehicle* removeVehicle(string &licensePlate){
            auto it=occupiedSpots.find(licensePlate);

            if (it == occupiedSpots.end()) return nullptr;
    
            ParkingSpot* spot = it->second;
            Vehicle* vehicle = spot->removeVehicle();
            
            if (vehicle) {
                occupiedSpots.erase(it);
                availableSpots++;
            }

            return vehicle;
        }

        ParkingSpot* findVehicle(string &licensePlate){
            auto it = occupiedSpots.find(licensePlate);
            return it != occupiedSpots.end() ? it->second : nullptr;
        }

        void displayInfo() const{
            //  DISPLAY
        }

        void displayOccupancy() const{
            // DISPLAY
        }
};