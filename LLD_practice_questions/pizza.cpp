

// -----------------------------------  BUILDER  -----------------------------------

#include<bits/stdc++.h>

using namespace std;

class Pizza{
    private:
        string base;
        string cheese;
        string topping;
        bool extraCheese;

        Pizza(const string& base, const string& cheese, const string& topping, const bool& extraCheese) : base(base), cheese(cheese), topping(topping), extraCheese(extraCheese) {}
        
    public:
        void showPizza() const{
            cout<<"Base : "<<base<<endl;
            cout<<"Cheese : "<<cheese<<endl;
            cout<<"Topping : "<<topping<<endl;
            cout<<"Extra Cheese : "<<extraCheese<<endl;
        }

        class Builder{
            private:
                string base;
                string cheese;
                string topping;
                bool extraCheese;
            
            public:
                Builder &setBase(const string& base_){
                    base=base_;
                    return *this;
                }

                Builder &setCheese(const string& cheese_){
                    cheese=cheese_;
                    return *this;
                }
                
                Builder &setTopping(const string& topping_){
                    topping=topping_;
                    return *this;
                }

                Builder &setExtraCheese(const bool& extraCheese_){
                    extraCheese=extraCheese_;
                    return *this;
                }

                Pizza build() const{
                    return Pizza(base, cheese, topping, extraCheese);
                }
        };
};

class PizzaDirector{
    public:
        Pizza makeSimpleCheesePizza(){
            return Pizza::Builder().setBase("Thin Crust").setCheese("Mozerella").setTopping("Parmesan").setExtraCheese(true).build();
        }
};

int main(){
    PizzaDirector director;

    Pizza pizza=director.makeSimpleCheesePizza();
    pizza.showPizza();
}