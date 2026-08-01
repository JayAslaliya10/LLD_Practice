

// -----------------------------------  FACADE  -----------------------------------


#include <bits/stdc++.h>

using namespace std;


enum class BookStatus{
    AVAILABLE,
    BORROWED,
    RESERVED,
    LOST
};


class Book{
    private:
        string bookId;
        string title;
        string author;
        string publisher;
        int publicationYear;
        string isbn;
        BookStatus status;

    public:
        Book(string bookId, string title, string author, string publisher, string isbn, int publicationYear) : bookId(bookId), title(title), author(author), publisher(publisher), isbn(isbn), publicationYear(publicationYear), status(BookStatus::AVAILABLE) {}

        string getBookId() const{ return bookId; }

        string getTitle() const{ return title; }

        string getAuthor() const{ return author; }

        string getPublisher() const{ return publisher; }

        int getPublicationYear(){ return publicationYear; }

        string getIsbn() const{ return isbn; }

        BookStatus getStatus() const{ return status; }

        void setStatus(BookStatus status){ this->status=status; }

        void displayInfo() const{
            //   DISPLAY INFO
        }
};


class Member{
    private:
        string memberId;
        string name;
        string email;
        string phone;
        vector<Book*> borrowedBooks;
        bool active;

    public:
        Member(string memberId, string name, string email, string phone) : memberId(memberId), name(name), email(email), phone(phone), active(true) {}

        string getMemberId() const{ return memberId; }

        string getName() const{ return  name; }

        string getEmail() const{ return email; }

        string getPhone() const{ return phone; }

        bool isActive() const{ return active; }

        const vector<Book*>& getBorrowedBooks() const{ return borrowedBooks; }

        void addBorrowedBook(Book* book){ borrowedBooks.push_back(book); }

        void removeBorrowedBook(Book* book){
            auto it=find(borrowedBooks.begin(), borrowedBooks.end(), book);

            if(it!=borrowedBooks.end()) borrowedBooks.erase(it);
        }

        void setActive(bool status){ active=status; }

        void displayInfo() const{
            //  DISPLAY INFO
        }
};


enum class TransactionType{
    BORROW,
    RETURN,
    RESERVE,
    RENEW
};


class Transaction{
    private:
        string transactionId;
        Book* book;
        Member* member;
        TransactionType type;
        std::chrono::system_clock::time_point issueDate;
        std::chrono::system_clock::time_point dueDate;
        std::chrono::system_clock::time_point returnDate;
        double fine;

    public:
        Transaction(string transactionId, Book* book, Member* member, TransactionType type, std::chrono::system_clock::time_point issueDate, std::chrono::system_clock::time_point dueDate) : transactionId(transactionId), book(book), member(member), type(type), issueDate(issueDate), dueDate(dueDate), fine(0.0) {}

        string getTransactionId() const{ return transactionId; }

        Book* getBook() const{ return book; }

        Member* getMember() const{ return member; }

        TransactionType getType() const{ return type; }

        std::chrono::system_clock::time_point getIssueDate() const{ return  issueDate; }

        std::chrono::system_clock::time_point getDueDate() const{ return dueDate; }

        std::chrono::system_clock::time_point getReturnDate() const{ return returnDate;}
        
        double getFine() const{ return fine; }

        void setReturnDate(std::chrono::system_clock::time_point date){ returnDate=date; }

        void setFine(double amount){ fine=amount; }

        void displayInfo() const{
            //   DISPLAY INFO
        }
};


class LibraryManager{
    private:
        vector<Book*> books;
        vector<Member*> members;
        vector<Transaction*> transactions;
        int bookIdCounter;
        int memberIdCounter;
        int transactionIdCounter;
        double finePerDay;

        Book* findBook(string& bookId) const{
            for(auto book: books){
                if(book->getBookId()==bookId) return book;
            }

            return nullptr;
        }

        Member* findMember(string& memberId) const{
            for(auto member: members){
                if(member->getMemberId()==memberId) return member;
            }

            return nullptr;
        }

        Transaction* findTransaction(string& bookId, string& memberId) const{
            for(auto transaction: transactions){
                if(transaction->getBook()->getBookId()==bookId && transaction->getMember()->getMemberId()==memberId && transaction->getType()==TransactionType::BORROW) return transaction;
            }

            return nullptr;
        }

        string generateBookId(){ return "B"+to_string(bookIdCounter++); }

        string generateMemberId(){ return "M"+to_string(memberIdCounter++); }

        string generateTransactionId(){ return "T"+to_string(transactionIdCounter++); }

        double calculateFine(std::chrono::system_clock::time_point dueDate, std::chrono::system_clock::time_point returnDate) const{
            if(returnDate<=dueDate) return 0.0;
            
            auto lateDuration=returnDate-dueDate;
            int daysLate=ceil(std::chrono::duration_cast<std::chrono::hours>(lateDuration).count()/24);
            
            return daysLate*finePerDay;
        }

    public:
        ~LibraryManager(){
            for(auto book: books) delete book;
            for(auto member: members) delete member;
            for(auto transaction: transactions) delete transaction;
        }

        LibraryManager(double finePerDay) : finePerDay(finePerDay), bookIdCounter(1), memberIdCounter(1), transactionIdCounter(1) {}

        void addBook(string title, string author, string publisher, string isbn, int publicationYear){
            Book* book=new Book(generateBookId(), title, author, publisher, isbn, publicationYear);
            books.push_back(book);
        }

        void addMember(string name, string email, string phone){
            Member* member=new Member(generateMemberId(), name, email, phone);
            members.push_back(member);
        }

        bool borrowBook(string memberId, string bookId, std::chrono::system_clock::time_point issueDate = std::chrono::system_clock::now()){
            Member* member=findMember(memberId);
            Book* book=findBook(bookId);

            if(!member || !member->isActive() || !book || book->getStatus()!=BookStatus::AVAILABLE) return false;

            // auto issueDate=std::chrono::system_clock::now();
            auto dueDate=issueDate+std::chrono::hours(24*7);

            Transaction* transaction= new Transaction(generateTransactionId(), book, member, TransactionType::BORROW, issueDate, dueDate);

            transactions.push_back(transaction);

            book->setStatus(BookStatus::BORROWED);
            member->addBorrowedBook(book);

            return true;
        }

        bool returnBook(string memberId, string bookId, std::chrono::system_clock::time_point returnDate = std::chrono::system_clock::now()){
            Member* member=findMember(memberId);
            Book* book=findBook(bookId);
            Transaction* transaction=findTransaction(bookId, memberId);

            if(!member || !transaction || !book || book->getStatus()!=BookStatus::BORROWED) return false;

            double fine=calculateFine(transaction->getDueDate(), returnDate);
            transaction->setFine(fine);

            if(fine>0.0) cout << "  Book returned late. Fine charged: $" << fixed << setprecision(2) << fine << endl;

            book->setStatus(BookStatus::AVAILABLE);
            member->removeBorrowedBook(book);

            Transaction* newTransaction= new Transaction(generateTransactionId(), book, member, TransactionType::RETURN, returnDate, returnDate);

            transactions.push_back(newTransaction);

            return true;
        }

        double getMemberTotalFine(string& memberId) const{
            double total = 0.0;

            for(auto transaction : transactions){
                if(transaction->getMember()->getMemberId()==memberId) total+=transaction->getFine();
            }

            return total;
        }

        bool reserveBook(string memberId, string bookId){
            Member* member=findMember(memberId);
            Book* book=findBook(bookId);

            if(!member || !member->isActive() || !book || book->getStatus()!=BookStatus::AVAILABLE) return false;

            book->setStatus(BookStatus::RESERVED);

            auto issueDate=std::chrono::system_clock::now();
            auto dueDate=issueDate+std::chrono::hours(24*7);

            Transaction* transaction= new Transaction(generateTransactionId(), book, member, TransactionType::RESERVE, issueDate, dueDate);

            transactions.push_back(transaction);

            return true;
        }

        bool renewBook(string memberId, string bookId){
            Member* member=findMember(memberId);
            Book* book=findBook(bookId);
            Transaction* transaction=findTransaction(bookId, memberId);

            if(!member || !transaction || !book || book->getStatus()!=BookStatus::BORROWED) return false;

            auto newIssueDate=std::chrono::system_clock::now();
            auto newDueDate=newIssueDate+std::chrono::hours(24*7);

            Transaction* newTransaction= new Transaction(generateTransactionId(), book, member, TransactionType::BORROW, newIssueDate, newDueDate);

            transactions.push_back(newTransaction);

            return true;
        }

        void displayAvailableBooks() const{
            //   DISPLAY AVAILABLE BOOKS
        }

        void displayMemberBooks(string memberId) const{
            // DISPLAY MEMBER BOOKS
        }

        void displayAllMember() const{
            //  DISPLAY ALL MEMBERS
        }

        void displayTransactionHistory() const{
            //  DISPLAY TRANSACTION HISTORY
        }
};