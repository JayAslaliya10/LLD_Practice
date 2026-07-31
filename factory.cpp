#include<bits/stdc++.h>

using namespace std;

class Notification{
    public:
        virtual void sendNotification(const string& message)=0;
        virtual ~Notification(){}
};

class EmailNotification : public Notification{
    public:
        void sendNotification(const string& message) override{
            cout<<"Email Notificaition : "<<message<<endl;
        }   
};

class SMSNotification : public Notification{
    public:
        void sendNotification(const string& message) override{
            cout<<"SMS Notificaition : "<<message<<endl;
        }
};

class PushNotification : public Notification{
    public:
        void sendNotification(const string& message) override{
            cout<<"Push Notificaition : "<<message<<endl;
        }
};


class NotificationFactory{
    public:
        static unique_ptr<Notification> createNotification(const string& type){
            if (type == "Email") return make_unique<EmailNotification>();
            else if (type == "SMS") return make_unique<SMSNotification>();
            else if (type == "Push") return make_unique<PushNotification>();
            else return nullptr;
        }
};


int main(){
    unique_ptr<Notification> emailNotification = NotificationFactory::createNotification("Email");
    unique_ptr<Notification> smsNotification = NotificationFactory::createNotification("SMS");
    unique_ptr<Notification> pushNotification = NotificationFactory::createNotification("Push");

    if(emailNotification) emailNotification->sendNotification("This is email message");
    if(smsNotification) smsNotification->sendNotification("This is SMS message");
    if(pushNotification) pushNotification->sendNotification("This is Push message");

    return 0;
}