#include<bits/stdc++.h>

using namespace std;

class Observer{
    public:
        virtual void update(string stock, double price) = 0;
        virtual ~Observer() {}
};

class Subject{
    public:
        virtual void addObserver(shared_ptr<Observer> observer)=0;
        virtual void removeObserver(shared_ptr<Observer> observer)=0;
        virtual void notify()=0;
        virtual ~Subject() {}
};

class StockMarket : public Subject{
    private:
        vector<shared_ptr<Observer>> observers;
        string stock;
        double price;

    public:
        void changePrice(string stock, double price){
            this->stock=stock;
            this->price=price;
            notify();
        }

        void addObserver(shared_ptr<Observer> observer) override{
            observers.push_back(observer);
        }

        void removeObserver(shared_ptr<Observer> observer) override{
            observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
        }

        void notify() override{
            for(auto& observer:observers) observer->update(stock, price);
        }
};

class MobileApp : public Observer{
    void update(string stock, double price) override{
        cout<<"[1] MOBILE APP ==> Price of stock "<<stock<<" is : "<<price<<endl;
    }
};

class WebDashboard : public Observer{
    void update(string stock, double price) override{
        cout<<"[2] WEB DASHBOARD ==> Price of stock "<<stock<<" is : "<<price<<endl;
    }
};



int main(){
    auto stockMarket=make_unique<StockMarket>();

    auto mobileApp=make_shared<MobileApp>();
    auto webDashboard=make_shared<WebDashboard>();

    stockMarket->addObserver(mobileApp);
    stockMarket->addObserver(webDashboard);

    stockMarket->changePrice("ANY", 2.46);
    stockMarket->changePrice("AAPL", 305.21);

    stockMarket->removeObserver(mobileApp);

    stockMarket->changePrice("ANY", 3.59);

    return 0;
}