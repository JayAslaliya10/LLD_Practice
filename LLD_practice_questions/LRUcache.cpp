

// -----------------------------------  DATA STRUCTURE IMPLEMENTATION -----------------------------------

#include<bits/stdc++.h>

using namespace std;


template<typename T, typename V>
struct Node{
    K key;
    V value;
    Node* prev;
    Node* next;

    Node(K key, V value) : key(key), value(value), prev(nullptr), next(nullptr) {}
};


template<typename T, typename V>
class DoublyLinkedList{
    private:
        Node<K, V>* head;
        Node<K, V>* tail;
        int size_;

    public:
        DoublyLinkedList() : head(nullptr), tail(nullptr), size_(0) {}

        ~DoublyLinkedList(){
            while(head){
                Node<K, V>* temp=head;
                head=head->next;
                delete temp;
            }
        }

        Node<K, V>* addToFront(K key, V value){
            Node<K, V>* newNode=new Node<K, V>(key, value);

            if(!head) head=tail=newNode;
            else{
                newNode->next=head;
                head->prev=newNode;
                head=newNode;
            }

            size_++;
            return newNode;
        }

        void moveToFront(Node<K, V>* node){
            if(node==head) return;

            if(node==tail){
                tail=node->prev;
                tail->next=nullptr;
            }
            else{
                node->prev->next=node->next;
                node->next->prev=node->prev;
            }

            node->prev=nullptr;
            node->next=head;
            head->prev=node;
            head=node;
        }

        void removeNode(Node<K, V>* node){
            if(node==head) head=head->next;
            else if(node==tail){
                tail=node->prev;
                tail->next=nullptr;
            }
            else{
                node->prev->next=node->next;
                node->next->prev=node->prev;
            }

            delete node;
            size_--;
            
            if(size_==0) head=tail=nullptr;
        }

        Node<K, V>* removeLast(){
            if(!tail) return nullptr;

            Node<K, V>* node=tail;

            tail=node->prev;
            if(tail) tail->next=nullptr;
            else head=nullptr;

            size_--;
            return node;
        }

        int getSize() const{ return size_; }

        int isEmpty() const{ return size_==0; }

        Node<K, V>* getHead() const{ return head; }

        Node<K, V>* getTail() const{ return tail; }
};


template<typename T, typename V>
class LRUCache{
    private:
        int capacity;
        DoublyLinkedList<K, V> dll;
        unordered_map<K, Node<K,V>*> cacheMap;

    public:
        LRUCache(int cap) : capacity(cap) {}

        V get(K key){
            auto it=cacheMap.find(key);

            if(it==cacheMap.end()){
                cout<<"Key not found"<<endl;
                throw std::runtime_error("Key not found");
            }

            Node<K, V>* node=it_->second;

            dll.moveToFront(node);

            return node->value;
        }

        void put(K key, V value){
            auto it=cacheMap.find(key);

            if(it!=cacheMap.end()){
                Node<K, V>* node=it->second;
                node->value=value;
                dll.moveToFront(node);
            }
            else{
                if(cacheMap.size()>=capacity){
                    Node<K, V>* node=dll.getTail();
                    cacheMap.erase(node->key);
                    dll.removeNode(node);
                }

                Node<K, V>* newNode=dll.addToFront(key, value);
                cacheMap[key]=newNode;
            }
        }

        bool contains(K key) const{ return cacheMap.find(key)!=cacheMap.end(); }

        int getSize() const{ return cacheMap.size(); }

        bool isEmpty() const{ return cacheMap.empty(); }

        void clear(){
            while (!dll.isEmpty()){
                Node<K,V>* node=dll.removeLast();
                cacheMap.erase(node->key);
                delete node;
            }
        }

        void display() const{
            //  DISPLAY
        }
};