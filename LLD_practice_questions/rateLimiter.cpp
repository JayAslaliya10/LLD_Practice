

// -----------------------------------  STRATEGY  /  IMPLEMENTATION  -----------------------------------


#include<bits/stdc++.h>

using namespace std;
using namespace std::chrono;


struct RateLimitResult{
    bool allowed;
    int remaining;
    optional<long long> retryAfterMs;
};


class RateLimitAlgorithm{
    virtual ~RateLimitAlgorithm()=default;
    virtual RateLimitResult allow(const string& clientId)=0;
    virtual string name() const=0;
};


class FixedWindowCounterAlgorithm: public RateLimitAlgorithm{
    private:
        struct State{
            int count=0;
            system_clock::time_point windowStart=system_clock::now();
        };
        unordered_map<string, State> states;
        int maxRequests;
        long long window;

    public:
        FixedCounterAlgorithm()
        
        string name()

        RateLimitResult allow()
};


class SlidingWindowAlgorithm: public RateLimitAlgorithm{
    private:

    public:
        SlidingWindowAlgorithm()
        
        string name()

        RateLimitResult allow()
};


class TokenBucketAlgorithm: public RateLimitAlgorithm{
    private:

    public:
        TokenBucketAlgorithm()
        
        string name()

        RateLimitResult allow()
};


class LeakyBucketAlgorithm: public RateLimitAlgorithm{
    private:

    public:
        LeakyBucketAlgorithm()
        
        string name()

        RateLimitResult allow()
};