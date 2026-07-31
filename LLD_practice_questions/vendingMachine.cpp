

// -----------------------------------  STATE  -----------------------------------

#include<bits/stdc++.h>

using namespace std;


class VendingMachine;

class MachineState{
    public:
        virtual ~MachineState()=default;
        virtual void selectItem(VendingMachine* context, string itemCode)=0;
        virtual void insertCoin(VendingMachine* context, double amount)=0;
        virtual void dispenseItem(VendingMachine* context)=0;
};

class IdleState: public MachineState{
    public: 
        void selectItem(VendingMachine* context, string itemCode) override{
            cout << "Item selected: " << itemCode << endl;
            context->setSelectedItem(itemCode)`;
            context->setState(new ItemSelectedState());
        }

        void insertCoin(VendingMachine* context, double amount) override{
            cout << "Please select an item before inserting coins." << endl;
        }

        void dispenseItem(VendingMachine* context) override{
            cout << "No item selected. Nothing to dispense." << endl;
        }
};

class ItemSelectedState: public MachineState{
    public:
        void selectItem(VendingMachine* context, string itemCode) override{
            cout << "Item already selected: " << context->getSelectedItem() << endl;
        }

        void insertCoin(VendingMachine* context, double amount) override{
            cout << "Inserted $" << amount << " for item: " << context->getSelectedItem() << endl;
            context->setInsertedAmount(amount);
            context->setState(new HasMoneyState());
        }

        void dispenseItem(VendingMachine* context) override{
            cout << "Insert coin before dispensing." << endl;
        }
};

class HasMoneyState: public MachineState{
    public:
        void selectItem(VendingMachine* context, string itemCode) override{
            cout << "Item already selected: " << context->getSelectedItem() << endl;
        }

        void insertCoin(VendingMachine* context, double amount) override{
            cout << "Money already inserted." << endl;
        }

        void dispenseItem(VendingMachine* context) override{
            cout << "Dispensing item: " << context->getSelectedItem() << endl;
            context->setState(new DispensingState());
            cout << "Item dispensed successfully." << endl;
            context->reset();
        }
};

class DispensingState: public MachineState{
    public:
        void selectItem(VendingMachine* context, string itemCode) override{
            cout << "Item already selected: " << context->getSelectedItem() << endl;
        }

        void insertCoin(VendingMachine* context, double amount) override{
            cout << "Money already inserted." << endl;
        }

        void dispenseItem(VendingMachine* context) override{
            cout << "Already dispensing. Please wait." << endl;
        }
};

class VendingMachine{
    private:
        MachineState* currState;
        string selectedItem;
        double insertedAmount;

    public:
        VendingMachine() : selectedItem(""), insertedAmount(0.0), currState(new IdleState()) {}

        ~VendingMachine() { delete currState; }

        void setState(MachineState* newState){
            delete currState;
            currState=newState;
        }

        void setSelectedItem(string item){ selectedItem=item; }

        void setInsertedAmount(double amount){ insertedAmount=amount; }

        string getSelectedItem(){ return selectedItem; }

        void selectItem(string item){ currState->selectItem(this, item); }

        void insertCoin(double amount){ currState->insertCoin(this, amount); }

        void dispenseItem(){ currState->dispenseItem(this); }

        void reset(){
            selectedItem="";
            insertedAmount=0.0;
            setState(new IdleState());
        }
};