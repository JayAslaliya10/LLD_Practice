

// -----------------------------------  STRATEGY  /  IMPLEMENTATION  -----------------------------------


#include<bits/stdc++.h>

using namespace std;
using namespace std::chrono;


// struct RateLimitResult{
//     bool allowed;
//     int remaining;
//     optional<long long> retryAfterMs;
// };


class RateLimitAlgorithm{
    virtual ~RateLimitAlgorithm()=default;
    virtual RateLimitResult allow(system_clock::time_point now)=0;
    virtual string name() const=0;
};


class FixedWindowCounterAlgorithm: public RateLimitAlgorithm{
    private:
        int count=0;
        system_clock::time_point windowStart;
        bool initialized=false;
        int maxRequests;
        long long windowMs;

    public:
        FixedCounterAlgorithm(int maxRequests, long long window) : maxRequests(maxRequests), window(window) {}
        
        string name() const override{ return "FixedCounterAlgorithm"; }

        RateLimitResult allow(system_clock::time_point now) override{
            if(!initialized){
                windowStart=now;
                initialized=true;
            }

            auto elapsedTime=duration_cast<milliseconds>(now-windowStart).count();

            if(elapsedTime>=windowMs){
                windowStart=now;
                count=0;
                elapsedTime=0;
            }

            if(count<maxRequests){
                count++;
                // return {true, maxRequests-count, nullopt};
                return true;
            }

            // return {false, 0, windowMs-elapsedTime};
            return false;
        }
};


class SlidingWindowAlgorithm: public RateLimitAlgorithm{
    private:
        deque<system_clock::time_point> logs;
        int maxRequests;
        long long windowMs;

    public:
        SlidingWindowAlgorithm(int maxRequests, long long windowMs) : maxRequests(maxRequests), windowMs(windowMs) {}
        
        string name() const override{ return "SlidingWindowAlgorithm"; }

        RateLimitResult allow(system_clock::time_point now) override{
            while(!logs.empty() && duration_cast<milliseconds>(now-logs.front()).count()>=windowMs) logs.pop_front();

            if(logs.size()<maxRequests){
                logs.push_back(now);
                // return {true, maxRequests-logs.size(), nullopt};
                return true;
            }

            // return {false, 0, windowMs-duration_cast<milliseconds>(now-logs.front()).count()};
            return false;
        }
};


class TokenBucketAlgorithm: public RateLimitAlgorithm{
    private:
        bool initialized=false;
        int tokens;
        int capacity;
        int refillPerSec;
        system_clock::time_point lastRefill;

    public:
        TokenBucketAlgorithm(int capacity, int refillPerSec) capacity(capacity), refillPerSec(refillPerSec) {}
        
        string name() const override{ return "TokenBucketAlgorithm"; }

        RateLimitResult allow(system_clock::time_point now) override{
            if(!initialized){
                initialized=true;
                tokens=capacity;
                lastRefill=now;
            }

            auto elapsedTime=duration_cast<milliseconds>(now-lastRefill).count();

            tokens=min(capacity, tokens+(elapsedTime*refillPerSec));
            lasRefill=now;

            if(tokens>=1){
                tokens--;
                // return {true, tokens, nullopt};
                return true;
            }

            // return {false, 0, ceil((1-tokens)/refillPerSec*1000)};
            return false;
        }
};


class LeakyBucketAlgorithm: public RateLimitAlgorithm{
    private:
        bool initialized=false;
        int capacity;
        int leakIntervalSec;
        system_clock::time_point lastLeakCheck;
        dequeue<system_clock::time_point> queue;

    public:
        LeakyBucketAlgorithm(int capacity, int leakIntervalSec) capacity(capacity), leakIntervalSec(leakIntervalSec) {}
        
        string name() const override{ return "LeakyBucketAlgorithm"; }

        RateLimitResult allow(system_clock::time_point now) override{
            if(!initialized){
                initialized=true;
                lastLeakCheck=now;
            }

            auto elapsedTime=duration_cast<milliseconds>(now-lastLeakCheck).count();

            int leaksToApply=elapsedTime/leakIntervalSec;

            for(int i=0; i<leaksToApply && !queue.empty(); i++) queue.pop_front();

            if(leaksToApply>0) lastLeakCheck+=(duration_cast<system_clock::duration>(duration<milliseconds>(leaksToApply*leakIntervalSec)));

            if(queue.size()<capacity){
                queue.push_back(now);
                // return {true, capacity-queue.size(), nullopt};
                return true;
            }

            // return {false, 0, ceil((leakIntervalSec-timeSinceLastLeak)*1000)};
            return false;
        }
};


class RateLimiterService{
    private:
        unique_ptr<RateLimiterAlgorithm> strategy;

    public:
        RateLimiterService(unique_ptr<RateLimiterAlgorithm> initialStrategy) strategy(move(intialStrategy)) {}

        void setStrategy(unique_ptr<RateLimiterAlgorithm> newStrategy){
            cout << "  [RateLimiterService] " << strategy->name() << " -> " << newStrategy->name() << endl;
            strategy=move(newStrategy);
        }

        // RateLimitResult checkRequest(const string& clientId, system_clock::time_point now)
        RateLimitResult checkRequest(const string& clientId, system_clock::time_point now){
            (void)clientId;
            return strategy->allow(now);
        }
};


nt main(){
    RateLimiterService service(make_unique<TokenBucketRateLimiter>(3, 1.0));

    cout << boolalpha;
    cout << "userA req1 -> " << service.checkRequest("userA").allowed << endl;
    cout << "userB req1 (shares the SAME bucket) -> " << service.checkRequest("userB").allowed << endl;
    cout << "userC req1 (bucket now empty) -> " << service.checkRequest("userC").allowed << endl;
    cout << "userA req2 (rejected -- global limit hit) -> " << service.checkRequest("userA").allowed << endl;

    cout << "\n[setStrategy] swap to FixedWindow(2 req / 10s), applies to everyone" << endl;
    service.setStrategy(make_unique<FixedWindowRateLimiter>(2, 10000));

    cout << "userA -> " << service.checkRequest("userA").allowed << endl;
    cout << "userB (counts against same global window) -> " << service.checkRequest("userB").allowed << endl;
    cout << "userC (global limit of 2 already hit) -> " << service.checkRequest("userC").allowed << endl;

    return 0;
}