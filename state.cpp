#include<bits/stdc++.h>

using namespace std;

class TrafficLight;

class TrafficState{
    public:
        virtual void next(TrafficLight& trafficLight)=0;
        virtual string getState() const=0;
        virtual ~TrafficState() {}
};

class TrafficLight{
    private:
        unique_ptr<TrafficState> state;

    public:
        void setState(unique_ptr<TrafficState> newState){
            if(state) cout<< "\n[State Change] ==> State changed from (" <<state->getState()<< ") to (" <<newState->getState()<< ")" << endl;
            else cout << "\n[State Init] ==> Initializing state to (" << newState->getState() << ")" << endl;

            state=move(newState);
        }

        void next(){
            if(state) state->next(*this);
            else cout<<"Traffic light has no state."<<endl;
        }
};

class RedLightState : public TrafficState{
    public:
        string getState() const override{
            return "Red Light State";
        }

        void next(TrafficLight& trafficLight) override;
};

class GreenLightState : public TrafficState{
    public:
        string getState() const override{
            return "Green Light State";
        }

        void next(TrafficLight& trafficLight) override;
};

class YellowLightState : public TrafficState{
    public:
        string getState() const override{
            return "Yellow Light State";
        }

        void next(TrafficLight& trafficLight) override;
};

void RedLightState::next(TrafficLight& trafficLight){
    trafficLight.setState(make_unique<GreenLightState>());
}

void GreenLightState::next(TrafficLight& trafficLight){
    trafficLight.setState(make_unique<YellowLightState>());
}

void YellowLightState::next(TrafficLight& trafficLight){
    trafficLight.setState(make_unique<RedLightState>());
}


int main(){
    TrafficLight traffic;

    traffic.setState(make_unique<RedLightState>());

    traffic.next();
    traffic.next();
    traffic.next();

    return 0;
}