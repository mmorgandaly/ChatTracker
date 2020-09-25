// ChatTracker.cpp

// This is a correct but inefficient implementation of
// the ChatTracker functionality.

#include "ChatTracker.h"
#include <string>
#include <vector>
#include <list>
#include <iostream>     //for testing purposes cout
#include <cassert>
#include <functional>
using namespace std;


// This class does the real work of the implementation.

class ChatTrackerImpl
{
public:
    ChatTrackerImpl(int maxBuckets);
    ~ChatTrackerImpl();
    void join(string user, string chat);
    int terminate(string chat);
    int contribute(string user);
    int leave(string user, string chat);
    int leave(string user);
private:
    struct Info
    {
        Info(string u, string c) : user(u), chat(c), count(0) {}
        string user;
        string chat;
        int count;
    };

    int myHash(const string& num) const;    //hash function
    vector <list<Info*>> hashuser;      //vector of lists for users
   // vector <list<Info*>> hashchat;
    int numofbuckets;

    
    //vector<Info> m_info;
    vector<Info> m_usersWhoLeft;        //keeps track of users who left 
};

ChatTrackerImpl::ChatTrackerImpl(int  maxBuckets)//CHANGED parameter for hashtable
{
    numofbuckets = maxBuckets;      //max number of backets
    hashuser.resize(numofbuckets);  //resize vector to number of buckets
    // Since this implementation does not use a hash table, we don't do
    // anything with the parameter and leave it unnamed so the compiler
    // doesn't issue a warning about an unused parameter.
}

ChatTrackerImpl::~ChatTrackerImpl()     //destructor
{
    for (int index = 0; index < numofbuckets; index++)    //iterate thru vector
    {
        list<Info*>::iterator p = hashuser[index].begin();
        while (p != hashuser[index].end())      //iterate thru list
        {
            delete *(p);                        //delete allocated memory
            p = hashuser[index].erase(p);
        }
    }

}

int ChatTrackerImpl::myHash(const string& num) const    //hash function
{
    hash<string> str_hash; 
    unsigned int value = str_hash(num);                 //unsigned int is always positive
        return (value % numofbuckets);                  //remainder becomes index for vector

}

void ChatTrackerImpl::join(string user, string chat)
{
    int index = myHash(user);                           //hash to user's index
    list<Info*>::iterator p = hashuser[index].begin();
    while (p != hashuser[index].end())              //until end of list
    {
        if ((*p)->user == user && (*p) ->chat == chat)      //if user and chat are already there
        {
            Info* info = (*p);      //create info to the pointer
            hashuser[index].erase(p);     //erase original
            hashuser[index].push_front(info);       //put duplicate in place in back of list
            return;
        }
        p++;
    }
    Info* ptr = new Info(user, chat);       //create new user with chat
    hashuser[index].push_front(ptr);     //add to end of hashuser ll 
    
    ///////////////slow implementation//////////////////////////
    /*
    vector<Info>::iterator p = m_info.end();
    while (p > m_info.begin())
    {
        p--;
        if (p->user == user && p->chat == chat)         //if already there
        {
            Info info = *p;
            m_info.erase(p);
            m_info.push_back(info);
            return;
        }
    }
    m_info.push_back(Info(user, chat));     //add to end of m_info vector
    */
}

int ChatTrackerImpl::terminate(string chat) //all users leave chat
{
    
    int total = 0; 
   // int index = myHash(chat);
    for (int index = 0; index < numofbuckets; index++)      //iterate through vector
    {
        list<Info*>::iterator p = hashuser[index].begin();
        while (p != hashuser[index].end())                  //iterate through list
        {
            if ((*p)->chat == chat)                           //if chat is found
            {
                total += (*p)->count;                          //add contributions to total
                delete* p;
                p = hashuser[index].erase(p);                  //erase 
            }
            else
                p++;
        }
    }

    vector<Info>::iterator k = m_usersWhoLeft.end();        //users who left the chat
    while (k > m_usersWhoLeft.begin())
    {
        if (k->chat == chat)                                //if chat equals chat we want to delete
        {
            total += k->count;                              //add to total
            k = m_usersWhoLeft.erase(k);                     //erase
        }
        else
            k--;
    }

    return total;                                               //return total number of contributions
     
    ///////////////////////old implementation///////////////////////////
    /*
    int total = 0;
    vector<Info>::iterator p = m_info.end();
    while (p > m_info.begin())
    {
        if (p->chat == chat)
        {
            total += p->count;
            p = m_info.erase(p);
        }
        else
            p--;
    }

    p = m_usersWhoLeft.end();
    while (p > m_usersWhoLeft.begin())
    {
        if (p->chat == chat)
        {
            total += p->count;
            p = m_usersWhoLeft.erase(p);
        }
        else
            p--;
    }
    return total;
    */
}

int ChatTrackerImpl::contribute(string user) //every time user contributes one
{
    int index = myHash(user);                               //hash user to find index
    list<Info*>::iterator p = hashuser[index].begin();
        while (p != hashuser[index].end())
        {
            if ((*p)->user == user)     //if user found
            {
                (*p)->count++;          //add count of contributions
                return (*p)->count;     //return user's contributions
            }
            p++;
        }

        return 0;                       //return 0 if user not found

///////////////////////old implementation/////////////////////
    /*
    vector<Info>::iterator p = m_info.end();
    while (p > m_info.begin())
    {
        p--;
        if (p->user == user)
        {
            p->count++;
            return p->count;
        }
    }
    return 0;
    */
}

int ChatTrackerImpl::leave(string user, string chat)
{
    int index = myHash(user);
    list<Info*>::iterator p = hashuser[index].begin();
    while (p != hashuser[index].end())
    {
        if ((*p)->user == user && (*p)->chat == chat)       //if user and chat found
        {
            int count = (*p)->count;
            m_usersWhoLeft.push_back(**p);                  //add to end of users who left vector
            delete* p;
            hashuser[index].erase(p);                       //erase hashuser
            return count;                                   //return count of contributions
        }
        p++;
    }

    return -1;              //if user and chat not found

    ///////////////////old implementation///////////////////
    /*
    vector<Info>::iterator p = m_info.end();
    while (p > m_info.begin())
    {
        p--;
        if (p->user == user && p->chat == chat)
        {
            int count = p->count;
            m_usersWhoLeft.push_back(*p);       //add to person who left vector
            m_info.erase(p);                    //erase vector in info
            return count;
        }
    }
    return -1;
    */
}

int ChatTrackerImpl::leave(string user) //very similar to leave function above
{
    int index = myHash(user);              
    list<Info*>::iterator p = hashuser[index].begin();
    while (p != hashuser[index].end())
    {
        if ((*p)->user == user)                     //if user is found 
        {
            int count = (*p)->count;                
            m_usersWhoLeft.push_back(*(*p));          //add to user who left vector
            delete* p;
            hashuser[index].erase(p);                  //erase hash user
            
            return count;                           // return count of contributions
        }
        p++;    
    }

    return -1;      //return -1 if user not found
    ///////////////////////old implementation/////////////////////
    /*
    vector<Info>::iterator p = m_info.end();
    while (p > m_info.begin())
    {
        p--;
        if (p->user == user)
        {
            int count = p->count;           //up here bc p->count = 0 after erased 
            m_usersWhoLeft.push_back(*p);
            m_info.erase(p);
            return count;
        }
    }
    return -1;
    */
}

//*********** ChatTracker functions **************

// These functions simply delegate to ChatTrackerImpl's functions.
// You probably don't want to change any of this code.

ChatTracker::ChatTracker(int maxBuckets)
{
    m_impl = new ChatTrackerImpl(maxBuckets);
}

ChatTracker::~ChatTracker()
{
    delete m_impl;
}

void ChatTracker::join(string user, string chat)
{
    m_impl->join(user, chat);
}

int ChatTracker::terminate(string chat)
{
    return m_impl->terminate(chat);
}

int ChatTracker::contribute(string user)
{
    return m_impl->contribute(user);
}

int ChatTracker::leave(string user, string chat)
{
    return m_impl->leave(user, chat);
}

int ChatTracker::leave(string user)
{
    return m_impl->leave(user);
}

/*
int main()
{
ChatTracker ct;
 ct.join("Fred", "Breadmaking");
  ct.join("Ethel", "Breadmaking");
  assert(ct.contribute("Fred") == 1);
 ct.join("Lucy", "Lint Collecting");
 assert(ct.contribute("Ethel") == 1);
  assert(ct.contribute("Fred") == 2);
  assert(ct.contribute("Fred") == 3);
  ct.join("Ricky", "Lint Collecting");
  ct.join("Ricky", "Strawberry");
  ct.join("Fred", "Lint Collecting");
  assert(ct.contribute("Fred") == 1);
  ct.join("Lucy", "Elbonian Politics");
  assert(ct.contribute("Fred") == 2);
  assert(ct.contribute("Ricky") == 1);
 assert(ct.contribute("Lucy") == 1);
  ct.join("Lucy", "Breadmaking");
  ct.join("Ethel", "Breadmaking");
  assert(ct.contribute("Lucy") == 1);
  
  
   assert(ct.terminate("Lint Collecting") == 3);
   ct.join("Lucy", "Burmese Cats");
   assert(ct.contribute("Lucy") == 1);
    ct.join("Lucy", "Worm Farming");
    ct.join("Lucy", "Elbonian Politics");

    assert(ct.leave("Lucy", "Breadmaking") == 1);
   assert(ct.contribute("Lucy") == 2);
    assert(ct.leave("Ethel") == 1);
   assert(ct.leave("Lucy") == 2);
  ct.join("Ricky", "Lint Collecting");
 assert(ct.contribute("Ricky") == 1);
 assert(ct.contribute("Ricky") == 2);
 assert(ct.leave("Ricky") == 2);
 ct.join("Lucy", "Breadmaking");
 assert(ct.leave("Fred") == 3);

 ct.join("Morgan", "UCLA");
 ct.join("Nick", "UCLA");
 ct.join("Nick", "Tennis");
 assert(ct.leave("Nick") == 1);
 assert(ct.terminate("UCLA") == 2);
 assert(ct.terminate("Tennis") == 1);
  cout << "hello";
}
*/