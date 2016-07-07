#include <iostream>
#include <string>
#include <stdlib.h>
#include <unordered_map> 
using namespace std;

struct site
{
    int siteName;
    int hash;
    site* newer;
    site* older;
    
    site();
    site(int &inName, int &inCacheSize, site* inNewer, site* inOlder);
    site& operator=(site& rhsObj);
};

site::site()
{
    hash = -1;
    siteName = -1;
    newer = NULL;
    older = NULL;
}

site::site(int &inName, int &inCacheSize, site* inNewer, site* inOlder)
{
    siteName = inName;
    newer = inNewer;
    older = inOlder;
    
    hash = inName % inCacheSize;
}

site& site::operator=(site &rhsObj)
{
    this->siteName = rhsObj.siteName;
    this->hash = rhsObj.hash;
    this->newer = rhsObj.newer;
    this->older = rhsObj.older;
    
    return *this;
}

class cache
{
public:
    site* hashtable;
    site* oldest;
    site* newest;
    int cacheSize;
    
    cache(int &inCacheSize);
    ~cache();
    
    void update(int &inSite);
    void fillHash();
    void browse();
};

cache::cache(int &inCacheSize)
{
    cacheSize = inCacheSize;
    hashtable = new site[cacheSize];
    oldest = newest = NULL;
}

cache::~cache()
{
    delete hashtable;
}

void cache::update(int &inSite)
{
    site temp(inSite, cacheSize, NULL, NULL);
    hashtable[temp.hash] = temp;
    
    //no hashtable members yet
    if(newest == NULL)
    {
        oldest = &hashtable[temp.hash];
        newest = &hashtable[temp.hash];
    }
    //only one hashtable member
    else if (newest == oldest)
    {
        newest->newer = &hashtable[temp.hash];
        hashtable[temp.hash].older = newest;
        oldest = newest;
        newest = &hashtable[temp.hash];
        
    }
    //all else
    else
    {
        newest->newer = &hashtable[temp.hash];
        newest->newer->older = newest;
        newest = &hashtable[temp.hash];
        site* deleteMe = oldest;
        oldest = oldest->newer;
        oldest->older = NULL;
        deleteMe->hash = -1;
        
    }
}

void cache::fillHash()
{
    for(int ii = 0; ii < cacheSize; ii++)
    {
        int random_site = rand();
        update(random_site);
        cout << "Adding in: " << random_site << endl;
    }
}

int main()
{
    int size = 5;
    cache myCache(size);
    myCache.fillHash();
    
    int browse = rand();
    myCache.update(browse);
    
    for (int ii = 0; ii < size; ii++)
    {
        cout << myCache.hashtable[ii].siteName << endl;
    }
}