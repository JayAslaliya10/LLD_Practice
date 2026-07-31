

// -----------------------------------  FACADE  -----------------------------------


#include<bits/stdc++.h>

using namespace std;


enum class RoomType{
    STANDARD,
    DELUXE,
    SUITE
};

enum class RoomStatus{
    AVAILABLE,
    OCCUPIED,
    UNDER_MAINTENANCE
};


class Room{
    private:
        int  roomNumber;
        RoomType type;
        RoomStatus status;
        double pricePerNight;
        int capacity;

    public:
        Room(int roomNumber, RoomType type) : roomNumber(roomNumber), type(type), status(RoomStatus::AVAILABLE){
            switch(type){
                case RoomType::STANDARD :
                    capacity=2;
                    pricePerNight=100;
                    break;
                case RoomType::DELUXE :
                    capacity=4;
                    pricePerNight=250;
                    break;
                case RoomType::SUITE :
                    capacity=8;
                    pricePerNight=500;
                    break;
            }
        }

        int getRoomNumber() const{ return roomNumber; }

        RoomType getRoomType() const{ return type; }

        double getPricePerNight() const{ return pricePerNight; }

        int getCapacity() const{ return capacity; }

        RoomStatus getStatus() const{ return status; }

        void setStatus(RoomStatus status){ this->status=status; }

        void displayInfo() const{
            cout << "Room " << roomNumber << endl;
            cout << "Type: ";
            switch (type) {
                case RoomType::STANDARD: cout << "Standard"; break;
                case RoomType::DELUXE: cout << "Deluxe"; break;
                case RoomType::SUITE: cout << "Suite"; break;
            }
            cout << endl;
            
            cout << "Status: ";
            switch (status) {
                case RoomStatus::AVAILABLE: cout << "Available"; break;
                case RoomStatus::OCCUPIED: cout << "Occupied"; break;
                case RoomStatus::UNDER_MAINTENANCE:  cout << "Under Maintenance"; break;
            }
            cout << endl;
            
            cout << "Price per night: $" << fixed << setprecision(2) << pricePerNight << endl;
            cout << "Capacity: " << capacity << " persons" << endl;
        }
};


class Guest{
    private:
        int guestId;
        string name;
        string email;
        string phone;
        string address;

    public:
        Guest(int guestId, string name, string email, string phone, string address) : guestId(guestId), name(name), email(email), phone(phone), address(address) {}

        int getGuestId() const{ return  guestId; }

        string getName() const{ return name; }

        string getEmail() const{ return email; }

        string getPhone() const{ return phone; }

        string getAddress() const{ return address; }

        void displayInfo() const{
            cout << "Guest: " << name << " (ID: " << guestId << ")" << endl;
            cout << "Email: " << email << endl;
            cout << "Phone: " << phone << endl;
            cout << "Address: " << address << endl;
        }
};


enum class BookingStatus{
    CONFIRMED,
    CHECKED_IN,
    CHECKED_OUT,
    CANCELLED
};


class Booking{
    private:
        int bookingId;
        Guest* guest;
        Room* room;
        string checkInDate;
        string checkOutDate;
        int numberOfNights;
        double totalAmount;
        BookingStatus status;

    public:
        Booking(int bookingId, Guest* guest, Room* room, string checkInDate, string checkOutDate, int numberOfNights) : bookingId(bookingId), guest(guest), room(room), checkInDate(checkInDate), checkOutDate(checkOutDate), numberOfNights(numberOfNights), status(BookingStatus::CONFIRMED){
            totalAmount=room->getPricePerNight()*numberOfNights;
        }

        int getBookingId() const{ return bookingId; }
        
        Guest* getGuest() const{ return guest; }

        Room* getRoom() const{ return room; }

        string getCheckInDate() const{ return checkInDate; }

        string getCheckOutDate() const{ return checkOutDate; }

        int getNumberOfNights() const{ return numberOfNights; }

        double getTotalAmount() const{ return totalAmount; }

        BookingStatus getStatus() const{ return status; }

        void setStatus(BookingStatus status) { this->status=status; }

        void displayInfo() const{
            //  DISPLAY
        }
};


class HotelManager{
    private:
        vector<Room*> rooms;
        vector<Guest*> guests;
        vector<Booking*> bookings;
        int bookingIdCounter;

        Room* findRoom(int& roomNumber) const{
            for(auto room: rooms){
                if(room->getRoomNumber()==roomNumber) return room;
            }

            return nullptr;
        }

        Guest* findGuest(int& guestId) const{
            for(auto guest: guests){
                if(guest->getGuestId()==guestId) return guest;
            }

            return nullptr;
        }

        Booking* findBooking(int& bookingId) const{
            for(auto booking: bookings){
                if(booking->getBookingId()==bookingId) return booking;
            }

            return nullptr;
        }

    public:
        ~HotelManager(){
            for(auto room: rooms) delete room;
            for(auto guest: guests) delete guest;
            for(auto booking: bookings) delete booking;
        }

        HotelManager() : bookingIdCounter(1) {}

        void addRoom(Room* room){ rooms.push_back(room); }

        void addGuest(Guest* guest){ guests.push_back(guest); }

        Booking* createBooking(int guestId, int roomNumber, string checkInDate, string checkOutDate, int numberOfNights){
            Guest* guest=findGuest(guestId);
            Room* room=findRoom(roomNumber);

            if(!guest || !room || room->getStatus()!=RoomStatus::AVAILABLE) return nullptr;

            Booking* booking=new Booking(bookingIdCounter++, guest, room, checkInDate, checkOutDate, numberOfNights);
            bookings.push_back(booking);

            return booking;
        }

        bool checkIn(int bookingId){
            Booking* booking=findBooking(bookingId);

            if(!booking || booking->getStatus()!=BookingStatus::CONFIRMED) return false;

            booking->setStatus(BookingStatus::CHECKED_IN);
            booking->getRoom()->setStatus(RoomStatus::OCCUPIED);

            return true;
        }

        bool checkOut(int bookingId){
            Booking* booking=findBooking(bookingId);

            if(!booking || booking->getStatus()!=BookingStatus::CHECKED_IN) return false;

            booking->setStatus(BookingStatus::CHECKED_OUT);
            booking->getRoom()->setStatus(RoomStatus::AVAILABLE);

            return true;
        }

        bool cancelBooking(int bookingId){
            Booking* booking=findBooking(bookingId);

            if(!booking || booking->getStatus()!=BookingStatus::CONFIRMED) return false;

            booking->setStatus(BookingStatus::CANCELLED);
            booking->getRoom()->setStatus(RoomStatus::AVAILABLE);

            return true;
        }

        void displayAvailableRooms() const{
            //   DISPLAY AVAILABLE ROOMS
        }

        void displayBookingHistory(int guestId) const{
            //  DISPLAY BOOKING HISTORY
        }

        void displayAllGuests() const{
            //   DISPLAY ALL GUESTS
        }

        void displayAllBookings() const{
            //  DISPLAY ALL BOOKINGS
        }
};




#include "HotelManager.hpp"
#include <iostream>

int main() {
    HotelManager hotel;
    
    // Add rooms
    Room* room1 = new Room("101", RoomType::STANDARD, 100.0, 2);
    Room* room2 = new Room("201", RoomType::DELUXE, 150.0, 3);
    Room* room3 = new Room("301", RoomType::SUITE, 250.0, 4);
    
    hotel.addRoom(room1);
    hotel.addRoom(room2);
    hotel.addRoom(room3);
    
    // Add guests
    Guest* guest1 = new Guest("G001", "John Doe", "john@example.com",
                            "+1-555-0123", "123 Main St");
    hotel.addGuest(guest1);
    
    // Display available rooms
    hotel.displayAvailableRooms();
    
    // Create a booking
    Booking* booking = hotel.createBooking("G001", "101", "2024-01-01",
                                         "2024-01-03", 2);
    if (booking) {
        std::cout << "\nBooking created successfully!" << std::endl;
        booking->displayInfo();
        
        // Check in
        if (hotel.checkIn(booking->getBookingId())) {
            std::cout << "\nChecked in successfully!" << std::endl;
        }
        
        // Display available rooms after check-in
        hotel.displayAvailableRooms();
        
        // Check out
        if (hotel.checkOut(booking->getBookingId())) {
            std::cout << "\nChecked out successfully!" << std::endl;
        }
        
        // Display booking history
        hotel.displayBookingHistory("G001");
    }
    
    return 0;
} 