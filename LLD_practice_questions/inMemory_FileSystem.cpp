

// -----------------------------------  COMPOSITE  ,  FACADE  -----------------------------------


#include<bits/stdc++.h>

using namespace std;


class Directory;


class FileSystemEntry{
    protected:
        string name;
        Directory *parent;

    public:
        virtual ~FileSystemEntry()=default;

        FileSystemEntry(string name, Directory* parent) : name(name), parent(parent) {}

        string getName() const{ return name; }

        void setName(string &newName){ name=newName; }

        Directory* getParent() const{ return parent; }

        void setParent(Directory* newParent){ parent=newParent; }

        virtual bool isDirectory() const=0;

        string getFullPath() const;
};


class File: public FileSystemEntry{
    private:
        string content;

    public:
        File(string name, Directory* parent) : FileSystemEntry(name, parent) {}

        bool isDirectory() const override{ return false; }

        string getContent() const{ return content; }

        void setContent(string newContent){ content=newContent; }

        size_t getSize() const{ return content.size(); }
};


class Directory: public FileSystemEntry{
    private:
        unordered_map<string, unique_ptr<FileSystemEntry*>> children;

    public:
        Directory(string name, Directory* parent) : FileSystemEntry(name, parent) {}

        bool isDirectory() const override{ return true; }

        bool hasChild(string& childName) const{ return children.count(childName)>0; }

        FileSystemEntry* getChild(string& childName) const{
            auot it=children.find(childName);
            return it!=children.end() ? it->second.get() : nullptr;
        }

        File* createFile(string& childName){
            if(hasChild(childName)) return nullptr;

            auto file=make_unique<File>(childName, this);
            File* raw=file.get();

            children[childName]=move(file);
            return raw;
        }

        Directory* createDirectory(string& childName){
            if(hasChild(childName)) return nullptr;

            auto dir=make_unique<Directory>(childName, this);
            Directory* raw=dir.get();

            children[childName]=move(dir);
            return raw;
        }

        bool removeChild(string& childName){ return children.erase(childName)>0; }

        unique_ptr<FileSystemEntry> detachChild(string& childName){
            auto it=children.find(childName);

            if(it==children.end()) return nullptr;

            auto entry=move(it->second);
            children.erase(it);

            return entry;
        }

        void attachChild(unique_ptr< =FileSystemEntry> entry){
            entry->setParent(this);
            children[entry->getName()]=move(entry);
        }

        vector<FileSystemEntry*> getChildEntries() const{
            vector<FileSystemEntry*> result;
            result.reserve(children.size());

            for(auto&[child, entry]: children) result.push_back(entry.get());
            return result;
        }

        vector<string> listContents() const{
            vector<string> names;
            names.reserve(children.size());

            for(auto&[child, entry]: children) names.push_back(child);
            sort(names.begin(), names.end());

            return names;
        }

        size_t getChildrenCount() const{ return children.size(); }
};


string FileSystemEntry::getFullPath() const{
    vector<string> segments;
    
    FileSystemEntry* curr=this;

    while(curr->parent){
        segments.push_back(curr->name);
        curr=curr->parent;
    }

    if(segments.empty()) return "/";

    reverse(segments.begin(), segments.end());
    string path;

    for(string& s: segments) path+=("/"+s);
    return path;
}


class FileSystem{
    private:
        unique_ptr<Directory> root;

        vector<string> splitPath(string& path) const{
            vector<string> result;
            stringstream ss(path);
            string segment;

            while(getline(ss, segment, '/')){
                if(!segment.empty()) result.push_back(segment);
            }

            return result;
        }

        pair<Directory*, string> resolveParent(string& path) const{
            vector<string> parts=splitPath(path);

            if(parts.empty()) return {nullptr, ""};

            string fileName=parts.back();
            parts.pop_back();

            FileSystemEntry* curr=root.get();

            for(string& part: parts){
                if(!curr->isDirectory()) return {nullptr, ""};
                curr=static_cast<Directory*>(curr)->getChild(part);
                if(!curr) return {nullptr, ""};
            }

            if(!curr->isDirectory()) return {nullptr, ""};
            return {static_cast<Directory*>(curr), fileName};
        }

        void serachByNameHelper(Directory* dir, string& query, vector<string>& results) const{
            for(FileSystemEntry* child : dir->getChildEntries()){
                if(child->getName().find(query) != string::npos) results.push_back(child->getFullPath());
                if(child->isDirectory()) searchByNameHelper(static_cast<Directory*>(child), query, results);
            }
        }

        void serachByContentHelper(Directory* dir, string& query, vector<string>& results) const{
            for(FileSystemEntry* child : dir->getChildEntries()){
                if(child->isDirectory()) searchByContentHelper(static_cast<Directory*>(child), query, results);
                else{
                    File* f = static_cast<File*>(child);
                    if(f->getContent().find(query) != string::npos){
                        results.push_back(f->getFullPath());
                    }
                }
            }
        }

    public:
        FileSystem() : root(make_unique<Directory>("", nullptr)) {}

        Directory* getRoot() const{ return root.get(); }

        FileSystemEntry* resolvePath(string& path) const{
            if(path=="/" || pathh.empty()) return root.get();

            vector<string> parts=splitPath(path);
            FileSystemEntry* curr=root.get();

            for(string& part: parts){
                if(!curr->isDirectory()) return nullptr;
                curr=static_cast<Directory*>(curr)->getChild(part);
                if(!curr) return nullptr;
            }

            return curr;
        }

        bool createFile(string path, string content=""){
            auto[parentDir, name]=resolveParent(path);

            if(!parentDir || name.empty()) return false;

            File* file=parentDir->createFile(name);
            if(!file) return false;

            file->setContent(content);
            return true;
        }

        bool createDirectory(string path){
            auto[parentDir, name]=resolveParent(path);

            if(!parentDir || name.empty()) return false;

            return parentDir->createDirectory(name)!=nullptr;
        }

        bool deleteEntry(string path){
            auto[parentDir, name]=resolveParent(path);

            if(!parentDir || name.empty()) return false;

            return parentDir->removeChild(name);
        }

        vector<string> listDirectory(string& path) const{
            FileSystemEntry* entry=resolvePath(path);
            if(!entry || !entry->isDirectory()) return {};
            return static_cast<Directory*>(entry)->listContents();
        }

        bool rename(string& path, string& newName){
            auto [parentDir, oldName] = resolveParent(path);

            if(!parentDir) return false;
            if(newName.empty() || newName.find('/') != string::npos) return false;
            if(parentDir->hasChild(newName)) return false;

            auto entry = parentDir->detachChild(oldName);

            if(!entry) return false;

            entry->setName(newName);
            parentDir->attachChild(std::move(entry));

            return true;
        }

        bool move(string& sourcePath, string& destDirPath){
            auto [sourceParent, name] = resolveParent(sourcePath);
            if(!sourceParent) return false;

            FileSystemEntry* destEntry = resolvePath(destDirPath);
            if(!destEntry || !destEntry->isDirectory()) return false;

            Directory* destDir = static_cast<Directory*>(destEntry);

            if(destDir->hasChild(name)) return false;

            FileSystemEntry* sourceEntry = sourceParent->getChild(name);
            if(!sourceEntry) return false;

            if(sourceEntry->isDirectory()){
                Directory* walker = destDir;
                while(walker){
                    if(walker == sourceEntry) return false;
                    walker = walker->getParent();
                }
            }

            auto entry = sourceParent->detachChild(name);
            destDir->attachChild(std::move(entry));
            return true;
        }

        vector<string> searchByName(const string& startPath, const string& query) const{
            FileSystemEntry* start = resolvePath(startPath);
            if(!start || !start->isDirectory()) return {};

            vector<string> results;
            searchByNameHelper(static_cast<Directory*>(start), query, results);
            return results;
        }

        vector<string> searchByContent(const string& startPath, const string& query) const{
            FileSystemEntry* start = resolvePath(startPath);
            if(!start || !start->isDirectory()) return {};

            vector<string> results;
            searchByContentHelper(static_cast<Directory*>(start), query, results);
            return results;
        }

        string getFullPath(FileSystemEntry* entry) const{ return entry ? entry->getFullPath() : ""; }
};




int main(){
    FileSystem fs;

    cout << boolalpha;
    fs.createDirectory("/home");
    fs.createDirectory("/home/user");
    fs.createDirectory("/home/user/docs");
    fs.createDirectory("/home/user/pics");

    fs.createFile("/home/user/docs/resume.txt", "My resume content");
    fs.createFile("/home/user/docs/notes.txt", "Meeting notes");

    for(auto& name : fs.listDirectory("/home/user/docs")) cout << "  " << name << endl;

    FileSystemEntry* entry = fs.resolvePath("/home/user/docs/resume.txt");
    cout << "Full path: " << fs.getFullPath(entry) << endl;

    fs.rename("/home/user/docs/resume.txt", "cv.txt");
    fs.move("/home/user/docs/cv.txt", "/home/user/pics");
    fs.deleteEntry("/home/user/pics");

    fs.createFile("/home/user/docs/todo.txt", "buy milk, call dentist");
    fs.createFile("/home/user/docs/journal.txt", "today I learned about hash maps");

    cout << "searchByName results: ";
    for(auto& p : fs.searchByName("/home", ".txt")) cout << p << " ";
    cout << endl;

    cout << "searchByContent results: ";
    for(auto& p : fs.searchByContent("/home", "call")) cout << p << " ";
    cout << endl;

    return 0;
}