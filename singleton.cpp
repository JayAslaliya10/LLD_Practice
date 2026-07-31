#include<bits/stdc++.h>

using namespace std;

class Printer{
    private:
        static Printer *instance;

        Printer(){ cout<<"Printer Initialized"; }

    public:
        static Printer *getInstance(){
            if(instance==nullptr) instance=new Printer();
            return instance;
        }

        void log(string str){
            cout<<"[LOG] : "<<str<<endl;
        }
};

Printer *Printer::instance=nullptr;

int main(){
    Printer *printer1=Printer::getInstance();
    printer1->log("Starting system...");

    Printer *printer2=Printer::getInstance();
    printer2->log("Running system...");

    if(printer1==printer2) cout<<"Both instances same"<<endl;
    else cout<<"Different instances"<<endl;

    return 0;
}