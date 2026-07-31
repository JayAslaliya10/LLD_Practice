

// -----------------------------------  DATA STRUCTURE IMPLEMENTATION  -----------------------------------

#include<bits/stdc++.h>

using namespace std;

class LFUCache {
private:
    struct Node{
        int key;
        int value;
        int freq;

        Node(int k, int v): key(k), value(v), freq(1) {}
    };
    
    int cap;
    int minFreq;
    unordered_map<int, list<Node>::iterator> keyMap;
    unordered_map<int,  list<Node>> freqMap;

public:
    LFUCache(int capacity) {
        cap=capacity;
        minFreq=0;
    }

    void updateFrequency(list<Node>::iterator it){
        int key=it->key;
        int value=it->value;
        int oldFreq=it->freq;

        freqMap[oldFreq].erase(it);

        if(freqMap[oldFreq].empty() && oldFreq==minFreq) minFreq++;

        int newFreq=oldFreq+1;
        freqMap[newFreq].push_front(Node(key, value));
        freqMap[newFreq].front().freq=newFreq;

        keyMap[key]=freqMap[newFreq].begin();
    }
    
    int get(int key) {
        if(keyMap.find(key)==keyMap.end()) return -1;

        auto it=keyMap[key];
        int val=it->value;

        updateFrequency(it);

        return val;
    }
    
    void put(int key, int value) {
        if(cap<=0) return;

        if(keyMap.find(key)!=keyMap.end()){
            auto it=keyMap[key];
            it->value=value;
            
            updateFrequency(it);

            return;
        }

        if(keyMap.size()>=cap){
            auto &minFreqList=freqMap[minFreq];
            int keyToRemove=minFreqList.back().key;

            keyMap.erase(keyToRemove);

            minFreqList.pop_back();
        }

        minFreq=1;
        freqMap[1].push_front(Node(key, value));
        keyMap[key]=freqMap[1].begin();
    }
};

/**
 * Your LFUCache object will be instantiated and called as such:
 * LFUCache* obj = new LFUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */