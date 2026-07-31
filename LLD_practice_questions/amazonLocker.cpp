

// -----------------------------------  FACADE,  STRATEGY  -----------------------------------


#include<bits/stdc++.h>

using namespace std;
using namespace std::chrono;

enum class PackageSize{
    SMALL,
    MEDIUM,
    LARGE
};

enum class LockerState{
    AVAILABLE,
    OCCUPIED
};


string sizeToString(PackageSize s){
    switch(s){
        case PackageSize::SMALL : return "SMALL";
        case PackageSize::MEDIUM : return "MEDIUM";
        case PackageSize::LARGE : return "LARGE";
    }
    return "UNKNOWN";
}


class LockerException: public runtime_error{
    public:
        explicit LockerException(const string& msg) : runtime_error(msg) {}
};
class NoLockerAvailableException: public LockerException{
    public:
        explicit NoLockerAvailableException(const string& msg) : LockerException(msg) {}
};
class InvalidTokenException: public LockerException{
    public:
        explicit InvalidTokenException(const string& msg) : LockerException(msg) {}
};
class TokenAlreadyUsedException: public LockerException{
    public:
        explicit TokenAlreadyUsedException(const string& msg) : LockerException(msg) {}
};
class TokenExpiredException: public LockerException{
    public:
        explicit TokenExpiredException(const string& msg) : LockerException(msg) {}
};


class Locker{
    private:
        int id;
        PackageSize size;
        LockerState state;
        string currentTokenId;

    public:
        Locker(int id, PackageSize size) : id(id), size(size), state(LockerState::AVAILABLE) {}

        int getId() const{ return id; }

        PackageSize getSize() const{ return size; }

        LockerState getState() const{ return state; }

        string getCurrentTokenId() const{ return currentTokenId; }

        void assignPackage(const string& tokenId){
            state=LockerState::OCCUPIED;
            currentTokenId=tokenId;
        }

        void open() const{ cout << "  >> Locker " << id << " (" << sizeToString(size) << ") door OPENED." << endl; }

        void release(){
            state=LockerState::AVAILABLE;
            currentTokenId.clear();
        }
};


struct AccessToken{
    string tokenId;
    int lockerId;
    system_clock::time_point issuedAt;
    system_clock::time_point expiresAt;
    bool used;

    bool isExpired() const{ return system_clock::now()>expiresAt; }
};


class LockerAllocationStrategy{
    public:
        virtual ~LockerAllocationStrategy()=default;
        virtual Locker* findAvailableLocker(vector<unique_ptr<Locker>>& lockers, PackageSize requestedSize)=0;
};

class StrictSizeMatchStrategy: public LockerAllocationStrategy{
    public:
        Locker* findAvailableLocker(vector<unique_ptr<Locker>>& lockers, PackageSize requestedSize) override{
            for(auto &locker: lockers){
                if(locker->getSize()==requestedSize && locker->getState()==LockerState::AVAILABLE) return locker.get();
            }

            return nullptr;
        }
};

class FallbackToLargerStrategy: public LockerAllocationStrategy{
    public:
        Locker* findAvailableLocker(vector<unique_ptr<Locker>>& lockers, PackageSize requestedSize) override{
            for(auto& locker: lockers){
                if(locker->getSize()==requestedSize && locker->getState()==LockerState::AVAILABLE) return locker.get();
            }

            for(int candidateSize=static_cast<int>(requestedSize)+1; candidateSize<=static_cast<int>(PackageSize::LARGE); candidateSize++){
                for(auto& locker : lockers){
                    if(static_cast<int>(locker->getSize())==candidateSize && locker->getState()==LockerState::AVAILABLE) return locker.get();
                }
            }

            return nullptr;
        }
};


class LockerSystem{
    private:
        vector<unique_ptr<Locker>> lockers;
        unordered_map<string, AccessToken> tokens;
        int tokenCounter;
        unique_ptr<LockerAllocationStrategy> allocationStrategy;
        system_clock::duration expiryDuration;

        void addLocker(int id, PackageSize size){ lockers.push_back(make_unique<Locker>(id, size)); }

    public:
        void initializeLockers(int small, int medium, int large){
            int nextId=static_cast<int>(lockers.size())+1;

            for(int i=0; i<small; i++) addLocker(nextId++, PackageSize::SMALL);
            for(int i=0; i<medium; i++) addLocker(nextId++, PackageSize::MEDIUM);
            for(int i=0; i<large; i++) addLocker(nextId++, PackageSize::LARGE);
        }

        LockerSystem(int numSmall, int numMedium, int numLarge, unique_ptr<LockerAllocationStrategy> strategy, system_clock::duration expiryDuration=hours(24*7)) : tokenCounter(0), allocationStrategy(move(strategy)), expiryDuration(expiryDuration) {
            initializeLockers(numSmall, numMedium, numLarge);
        }

        Locker* findLockerById(int id){
            for(auto& locker: lockers){
                if(locker->getId()==id) return locker.get();
            }

            return nullptr;
        }

        string depositPackage(PackageSize size){
            Locker* locker=allocationStrategy->findAvailableLocker(lockers, size);
            if(!locker) throw NoLockerAvailableException("No available locker for size : " + sizeToString(size));

            string tokenId="T"+to_string(++tokenCounter);
            auto now=system_clock::now();

            AccessToken token{tokenId, locker->getId(), now, now+expiryDuration, false};
            tokens[tokenId]=token;

            locker->assignPackage(tokenId);
            locker->open();

            cout << "Package deposited. Access token issued: " << tokenId << endl;

            return tokenId;
        }

        void pickupPackage(const string& tokenId){
            auto it=tokens.find(tokenId);
            if(it==tokens.end()) throw InvalidTokenException("Invalid access token: '" + tokenId + "' does not exist");

            AccessToken& token=it->second;

            if(token.used) throw TokenAlreadyUsedException("Access token '" + tokenId + "' has already been used");
            
            if(token.isExpired()) throw TokenExpiredException("Access token '" + tokenId + "' has expired");

            Locker* locker=findLockerById(token.lockerId);
            locker->open();
            locker->release();

            token.used=true;

            cout << "  Package picked up successfully using token " << tokenId << endl;
        }

        vector<int> staffOpensExpiredLockers(){
            vector<int> openedLockerIds;

            for(auto& [tokenId, token]: tokens){
                if(!token.used && token.isExpired()){
                    Locker* locker=findLockerById(token.lockerId);

                    cout << "  [STAFF] Expired token " << tokenId << " -> ";

                    locker->open();
                    locker->release();

                    token.used=true;

                    openedLockerIds.push_back(locker->getId());
                }
            }

            return openedLockerIds;
        }

        void displayStatus() const{
            cout << "\n--- Locker Status ---" << endl;
            for(auto& locker: lockers){
                cout << "  Locker " << locker->getId() << " [" << sizeToString(locker->getSize()) << "] : "
                     << (locker->getState() == LockerState::AVAILABLE ? "AVAILABLE" : "OCCUPIED (token " + locker->getCurrentTokenId() + ")")
                     << endl;
            }
        }
};


