

// -----------------------------------  FACADE  ,  STRATEGY  -----------------------------------


#include<bits/stdc++.h>

using namespace std;


class User{
    private:
        string userId;
        string name;
        string email;
        unordered_map<string, double> balances;
    
    public:
        User(strig userId, string name, string email) : userId(userId), name(name), email(email) {}

        string getUserId() const{ return userId; }

        string getName() const{ return name; }

        string getEmail() const{ return email; }

        const unordered_map<string, double> getBalances(){ return balances; }

        void updateBalance(string& otherUserId, double amount){ balances[otherUserId]+=amount; }

        double getBalanceWith(string &otherUserId) const{
            auto it=balances.find(otherUserId);
            return it!=balances.end() ? it->second : 0.0;
        }
};


class Group{
    private:
        string groupId;
        string name;
        vector<string> memberIds;
        vector<string> expenseIds;

    public:
        Group(string groupId, string name) : groupId(groupId), name(name) {}

        string getGroupId() const{ return groupId; }

        string getName() const{ return name; }

        vector<string>& getMemberIds() const{ return memberIds; }

        vector<string>& getExpenseIds() const{ return expenseIds; }

        bool hasMember(string& userId) const{ return find(memberIds.begin(), memberIds.end(), userId) != memberIds.end(); }

        void addMemberId(string& userId){ if(!hasMember(userId)) memberIds.push_back(userId); }

        void addExpenseId(string& expenseId){ expenseIds.push_back(expenseId); }
};


enum class SplitType{
    EQUAL,
    PERCENTAGE,
    EXACT
};

class SplitStrategy{
    public:
        virtual ~SplitStrategy()=default;
        virtual unordered_map<string, double> calculateShares(double totalAmount, vector<string>& participants, unordered_map<string, double>& shareInputs) const=0;
};


class EqualSplitStrategy : public SplitStrategy{
    public:
        unordered_map<string, double> calculateShares(double totalAmount, vector<string>& participants, unordered_map<string, double>&) const override{
            unordered_map<string, double> shares;
            int n=participants.size();

            double base=floor((totalAmount/n)*100.0)/100.0;
            double distributed=base*n;
            double remaining=round((totalAmount-distribured)*100.0)/100.0;

            shares[participants[0]]=base+remaining;
            for(int i=1; i<n; i++) shares[participants[i]]=base;

            return shares;
        }
};


class PercentageSplitStrategy : public SplitStrategy{
    public:
        unordered_map<string, double> calculateShares(double totalAmount, vector<string>& participants, unordered_map<string, double>& shareInputs) const override{
            double sumPercent=0.0;

            for(string& p: participants){
                if(!shareInputs.count(p)) throw invalid_argument("Missing percentage for participant: " + p);
                sumPercent+=shareInput[p];
            }

            if(abs(sumPercent-100.0)>0.01) throw invalid_argument("Percentages must sum to 100");

            unordered_map<string, double> shares;
            for(string& p: participants) shares[p] = totalAmount*(shareInputs[p]/100.0);

            return shares;
        }
};


class ExactSplitStrategy : public SplitStrategy{
    public:
        unordered_map<string, double> calculateShares(double totalAmount, vector<string>& participants, unordered_map<string, double>& shareInputs) const override{
            double sum=0.0;

            for(string& p: participants){
                if(!shareInputs.count(p)) throw invalid_argument("Missing exact amount for participant: " + p);
                sums+=shareInputs[p];
            }

            if(abs(sum-totalAmount)>0.01) throw invalid_argument("Exact shares must sum to total amount");

            return shareInputs;
        }
};


class Expense{
    private:
        string expenseId;
        string description;
        string groupId;
        string paidBy;
        double totalAmount;
        vector<string> participants;
        unordered_map<string, double> shares;
        SplitType type;

    public:
        Expense(string expenseId, string description, string groupId, double totalAmount, string paidBy, vector<string>& participants, unordered_map<string, double> shares, SplitType type) : expenseId(expenseId), description(description), groupId(groupId), totalAmount(totalAmount), paidBy(paidBy), participants(participants), shares(shares), type(type) {}

        string getExpenseId() const{ return expenseId; }

        string getDescription() const{ return description; }
        
        string getGroupId() const{ return groupId; }

        string getPaidBy() const{ return paidBy; }

        double getTotalAmount() const{ return totalAmount; }

        const vector<string>& getParticipants() const{ return participants; }

        const unordered_map<string, double>& getShares() const{ return shares; }

        void displayInfo() const{
            //   DISPLAY INFO
        }
};


class SplitWiseSystem{
    private:
        vector<unique_ptr<User>> users;
        vector<unique_ptr<Group>> groups;
        vector<unique_ptr<Expense>> expenses;
        unordered_map<SplitType, unique_ptr<SplitStrategy>> strategies;
        int userIdCounter;
        int groupIdCounter;
        int expenseIdCounter;

        User* findUser(string& userId) const{
            for(auto& user: users) if(user->getUserId()==userId) return user.get();
            return nullptr;
        }

        Group* findGroup(string& groupId) const{
            for(auto& group: groups) if(group->getUserId()==groupId) return group.get();
            return nullptr;
        }

        string generateUserId() const{ return "U"+to_string(userIdCounter++); }

        string generateGroupId() const{ return "G"+to_string(groupIdCounter++); }

        string generateExpenseId(){ return "E"+to_string(expenseIdCounter++); }

    public:
        SplitWiseSystem() : userIdCounter(1), groupIdCounter(1), expenseIdCounter(1){
            strategies[SplitType::EQUAL]=make_unique<EqualSplitStrategy>();
            strategies[SplitType::PERCENTAGE]=make_unique<PercentageSplitStrategy>();
            strategies[SplitType::EXACT]=make_unique<ExactSplitStrategy>();
        }

        string addUser(string& name, string& email){
            string id=generateUserId();
            users.push_back(make_unique<User>(id, name, email));
            
            return id;
        }

        string createGroup(string& name, vector<string>& memberIds){
            string id=createGroupId();
            auto group=make_unique<Group>(id, name);

            for(auto& m: memberIds) group->addMemberId(m);
            groups.push_back(group);

            return id;
        }

        bool addMemberToGroup(string& groupId, string& memberId){
            Group* group=findGroup(groupId);
            User* user=findUser(memberId);

            if(!group || !user) return false;

            group->addMemberId(memberId);

            return true;
        }

        string addExpense(string groupId, string description, double totalAmount, string paidBy, vector<string>& participants, SplitType type, unordered_map<string, double> shareInputs={}){
            Group* group=findGroup(groupId);
            User* payer=findUser(paidBy);

            if(!group || !payer) return "";

            unordered_map<string, double> shares;

            try{
                shares=strategies[type]->calculateShares(totalAmount, participants, shareInputs);
            }catch(const invalid_argument&){
                return "";
            }

            string expenseId=generateExpenseId();
            auto expense=make_unique<Expense>(expenseId, description, groupId, totalAmount, paidBy, participants, shares, type);
            group->addExpenseId(expenseId);

            for(string& p: participants){
                if(p==paidBy) continue;
                
                User* participant=findUser(p);
                double share=shares[p];

                payer->updateBalance(p, +share);
                participant->updateBalance(p, -share);
            }

            expenses.push_back(expense);
            return expenseId;
        }

        double getBalance(string& userA, string& userB) const{
            User* A=findUser(userA);
            if(!A) return 0.0;

            return A->getBalanceWith(userB);
        }

        void displayAllBalances(string& userId) const{
            User* u = findUser(userId);
            if(!u) return;

            cout << "Balances for " << u->getName() << ":" << endl;
            
            for(auto& [otherId, amount] : u->getBalances()){
                if(abs(amount) < 0.01) continue;

                User* other = findUser(otherId);
                string otherName = other ? other->getName() : otherId;

                if(amount > 0) cout << "  " << otherName << " owes " << u->getName() << " $" << fixed << setprecision(2) << amount << endl;
                else cout << "  " << u->getName() << " owes " << otherName << " $" << fixed << setprecision(2) << -amount << endl;
            }

        }

        bool settleUp(string payer, string payee, double amount){
            User* A=find(payer);
            User* B=find(payee);
            
            if(!A || !B || amount<=0) return false;

            A->updateBalance(payee, +amount);
            B->updateBalance(payer, -amount);

            return true;
        }

        vector<Expense>* getUserTransactionHistory(string& userId) const{
            vector<Expense*> result;

            for(auto& e : expenses){
                if(e->getPaidBy() == userId) { result.push_back(e.get()); continue; }
                auto& parts = e->getParticipants();
                if(find(parts.begin(), parts.end(), userId) != parts.end()) result.push_back(e.get());
            }

            return result;
        }

        vector<Expense>* getGroupExpenses(string& groupId) const{
            vector<Expense*> result;

            for(auto& e : expenses){
                if(e->getGroupId() == groupId) result.push_back(e.get());
            }
            
            return result;
        }
};




int main(){
    SplitwiseSystem sw;

    string alice = sw.addUser("Alice", "alice@example.com");
    string bob   = sw.addUser("Bob", "bob@example.com");
    string carol = sw.addUser("Carol", "carol@example.com");

    string trip = sw.createGroup("Goa Trip", {alice, bob, carol});

    sw.addExpense(trip, "Dinner", 300.0, alice, {alice, bob, carol}, SplitType::EQUAL);
    sw.addExpense(trip, "Hotel", 200.0, bob, {alice, bob, carol}, SplitType::PERCENTAGE,
                  {{alice,50.0},{bob,30.0},{carol,20.0}});
    sw.addExpense(trip, "Cab", 150.0, carol, {alice, bob, carol}, SplitType::EXACT,
                  {{alice,60.0},{bob,40.0},{carol,50.0}});

    sw.displayAllBalances(alice);

    double before = sw.getBalance(alice, carol);
    sw.settleUp(carol, alice, before);
    cout << "Alice-Carol balance after settling: $" << sw.getBalance(alice, carol) << endl;

    for(auto* e : sw.getGroupExpenses(trip)) e->displayInfo();

    return 0;
}