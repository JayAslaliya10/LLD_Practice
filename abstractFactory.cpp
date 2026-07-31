#include<bits/stdc++.h>

using namespace std;

class Button{
    public:
        virtual void render()=0;
        virtual ~Button(){}
};

class Scrollbar{
    public:
        virtual void render()=0;
        virtual ~Scrollbar(){}
};

class LightButton : public Button{
    public:
        void render() override{
            cout<<"Rendering LightButton"<<endl;
        }
};

class LightScrollbar : public Scrollbar{
    public:
        void render() override{
            cout<<"Rendering LightScrollbar"<<endl;
        }
};

class DarkButton : public Button{
    public:
        void render() override{
            cout<<"Rendering DarkButton"<<endl;
        }
};

class DarkScrollbar : public Scrollbar{
    public:
        void render() override{
            cout<<"Rendering DarkScrollbar"<<endl;
        }
};

class ThemeFactory{
    public:
        virtual unique_ptr<Button> createButton()=0;
        virtual unique_ptr<Scrollbar> createScrollbar()=0;
        virtual ~ThemeFactory(){}
};

class LightFactory : public ThemeFactory{
    public:
        unique_ptr<Button> createButton() override{
            return make_unique<LightButton>();
        }

        unique_ptr<Scrollbar> createScrollbar() override{
            return make_unique<LightScrollbar>();
        }
};

class DarkFactory : public ThemeFactory{
    public:
        unique_ptr<Button> createButton() override{
            return make_unique<DarkButton>();
        }

        unique_ptr<Scrollbar> createScrollbar() override{
            return make_unique<DarkScrollbar>();
        }
};

void renderUI(unique_ptr<ThemeFactory>& factory){
    auto button=factory->createButton();
    auto scrollbar=factory->createScrollbar();

    button->render();
    scrollbar->render();
}

int main(){
    unique_ptr<ThemeFactory> factory= make_unique<LightFactory>();
    renderUI(factory);

    factory=make_unique<DarkFactory>();
    renderUI(factory);

    return 0;
}