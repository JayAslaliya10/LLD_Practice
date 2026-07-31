

// -----------------------------------  FACADE  -----------------------------------

#include<bits/stdc++.h>
#include<iomanip>

using namespace std;


class Seat{
    private:
        int seatNumber;
        bool booked;

    public:
        Seat(int number) : seatNumber(number), booked(false) {}

        int getSeatNumber() const{ return seatNumber; }

        bool isBooked() const{ return booked; }

        void book(){ booked=true; }

        void cancel(){ booked=false; }
};


class Movie{
    private:
        string movieId;
        string name;
        string venue;
        string date;
        double basePrice;
        vector<Seat> seats;
        int totalSeats;

    public:
        Movie(string id, string name, string venue, string date, double basePrice, int totalSeats) : movieId(id), name(name), venue(venue), date(date), basePrice(basePrice), totalSeats(totalSeats){
            for(int i=1; i<=totalSeats; i++) seats.push_back(Seat(i));
        }

        string getMovieId() const{ return movieId; }

        string getName() const{ return name; }

        string getVenue() const{ return venue; }

        string getDate() const{ return date; }

        double getBasePrice() const{ return basePrice; }

        int getTotalSeats() const{ return totalSeats; }

        int getAvailableSeats() const{
            int available=0;

            for(auto &seat: seats){
                if(!seat.isBooked()) available++;
            }

            return available;
        }

        bool bookSeat(int seatNumber){
            if(seatNumber<1 || seatNumber>totalSeats) return false;
            if(seats[seatNumber-1].isBooked()) return false;

            seats[seatNumber-1].book();
            return true;
        }

        bool cancelSeat(int seatNumber){
            if(seatNumber<1 || seatNumber>totalSeats) return false;
            if(!seats[seatNumber-1].isBooked()) return false;

            seats[seatNumber-1].cancel();
            return true;
        }

        bool isSeatAvailable(int seatNumber) const{
            if(seatNumber<1 || seatNumber>totalSeats) return false;
            return !seats[seatNumber-1].isBooked();
        }

        void displayInfo() const{
            cout << "Movie: " << name << " | ID: " << movieId
                 << " | Venue: " << venue << " | Date: " << date
                 << " | Price/seat: $" << fixed << setprecision(2) << basePrice
                 << " | Available: " << getAvailableSeats() << "/" << totalSeats << endl;
        }
};


class Booking{
    private:
        string bookingId;
        string customerName;
        Movie* movie;
        vector<int> seatNumbers;
        double totalPrice;

    public:
        Booking(string bookingId, string customerName, Movie* movie, vector<int> seatNumbers, double totalPrice) : bookingId(bookingId), customerName(customerName), movie(movie), seatNumbers(seatNumbers), totalPrice(totalPrice) {}

        string getBookingId() const{ return bookingId; }

        string getCustomerName() const{ return customerName; }

        Movie* getMovie() const{ return movie; }

        const vector<int>& getSeatNumbers() const{ return seatNumbers; }

        int getSeatCount() const{ return static_cast<int>(seatNumbers.size()); }

        double getTotalPrice() const{ return totalPrice; }


        void displayInfo() const{
            cout << "Booking: " << bookingId << " | Customer: " << customerName
                 << " | Movie: " << movie->getName() << " | Seats: [";
            for(size_t i=0; i<seatNumbers.size(); i++){
                cout << seatNumbers[i];
                if(i+1<seatNumbers.size()) cout << ", ";
            }
            cout << "] (" << seatNumbers.size() << " seat" << (seatNumbers.size()!=1 ? "s" : "")
                 << ") | Total Price: $" << fixed << setprecision(2) << totalPrice << endl;
        }
};


class BookingSystem{
    private:
        vector<Movie*> movies;
        vector<Booking*> bookings;
        int bookingIdCounter;

    public:
        BookingSystem() : bookingIdCounter(1) {}

        ~BookingSystem(){
            for (auto& movie : movies) delete movie;
            for (auto& booking : bookings) delete booking;
        }

        void addMovie(Movie* movie){ movies.push_back(movie); }

        string createBooking(string customerName, Movie* movie, vector<int> seatNumbers){
            if(seatNumbers.empty()) return "";

            set<int> uniqueSeats(seatNumbers.begin(), seatNumbers.end());
            if(uniqueSeats.size() != seatNumbers.size()) return "";

            for(int seatNumber : seatNumbers){
                if(!movie->isSeatAvailable(seatNumber)) return "";
            }

            for(int seatNumber : seatNumbers){ movie->bookSeat(seatNumber); }

            string bookingId = "B" + to_string(bookingIdCounter++);
            double totalPrice = movie->getBasePrice() * seatNumbers.size();

            Booking* booking = new Booking(bookingId, customerName, movie, seatNumbers, totalPrice);
            bookings.push_back(booking);
            return bookingId;
        }

        string createBooking(string customerName, Movie* movie, int seatNumber){ return createBooking(customerName, movie, vector<int>{seatNumber}); }

        bool cancelBooking(string bookingId){
            Booking* booking = findBooking(bookingId);
            if (!booking) return false;

            for(int seatNumber : booking->getSeatNumbers()) booking->getMovie()->cancelSeat(seatNumber);

            auto it = find(bookings.begin(), bookings.end(), booking);
            if (it != bookings.end()) {
                bookings.erase(it);
                delete booking;
                return true;
            }
            return false;
        }

        double getBookingTotalPrice(string bookingId) const{
            Booking* booking = findBooking(bookingId);
            if(!booking) return -1.0;
            return booking->getTotalPrice();
        }

        bool tryGetBookingTotalPrice(string bookingId, double& outPrice) const{
            Booking* booking = findBooking(bookingId);
            if(!booking) return false;
            outPrice = booking->getTotalPrice();
            return true;
        }

        void displayAllMovies() const{
            cout << "\nAvailable Movies:" << endl;
            for (const auto& movie : movies) {
                movie->displayInfo();
                cout << "------------------------" << endl;
            }
        }

        void displayAllBookings() const{
            cout << "\nCurrent Bookings:" << endl;
            for (const auto& booking : bookings) {
                booking->displayInfo();
                cout << "------------------------" << endl;
            }
        }

        void displayAvailableSeats(Movie* movie) const{
            cout << "\nAvailable seats for Movie " << movie->getName() << ":" << endl;
            for (int i = 1; i <= movie->getTotalSeats(); i++) {
                if (movie->isSeatAvailable(i)) cout << i << " ";
            }
            cout << endl;
        }

        Movie* findMovie(string movieId) const{
            for (auto movie : movies) {
                if (movie->getMovieId() == movieId) return movie;
            }
            return nullptr;
        }

        Booking* findBooking(string bookingId) const{
             for (auto booking : bookings) {
                if (booking->getBookingId() == bookingId) return booking;
            }
            return nullptr;
        }
};


int main(){
    BookingSystem system;

    Movie* m1 = new Movie("M1", "Inception", "PVR Cinemas", "2026-07-25", 12.50, 6);
    system.addMovie(m1);

    system.displayAllMovies();

    // --- Multi-seat booking ---
    string b1 = system.createBooking("Alice", m1, vector<int>{1, 2, 3});
    cout << "\nBooking created (Alice, seats 1-3): " << b1 << endl;
    system.displayAvailableSeats(m1);

    // --- Single-seat booking still works via the convenience overload ---
    string b2 = system.createBooking("Bob", m1, 5);
    cout << "\nBooking created (Bob, seat 5): " << b2 << endl;
    system.displayAvailableSeats(m1);

    system.displayAllBookings();

    // --- Total price lookups ---
    cout << "\nTotal price for " << b1 << ": $" << system.getBookingTotalPrice(b1) << endl;
    cout << "Total price for " << b2 << ": $" << system.getBookingTotalPrice(b2) << endl;

    double price;
    if(system.tryGetBookingTotalPrice("BOGUS", price))
        cout << "Total price for BOGUS: $" << price << endl;
    else
        cout << "Total price for BOGUS: booking not found" << endl;

    // --- Atomicity test: request overlaps an already-booked seat (seat 2 taken by Alice) ---
    string b3 = system.createBooking("Charlie", m1, vector<int>{2, 4, 6});
    cout << "\nAttempted booking (Charlie, seats 2,4,6 -- seat 2 already taken): "
         << (b3.empty() ? "FAILED as expected (no partial booking)" : b3) << endl;
    system.displayAvailableSeats(m1);   // seats 4 and 6 must still be free -- proves no partial booking happened

    // --- Cancel Alice's multi-seat booking ---
    cout << boolalpha << "\nCancel " << b1 << ": " << system.cancelBooking(b1) << endl;
    system.displayAvailableSeats(m1);   // seats 1,2,3 should be free again
    system.displayAllBookings();

    return 0;
}