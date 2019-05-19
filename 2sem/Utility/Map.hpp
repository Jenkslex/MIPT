#ifndef __MAP__
#define __MAP__

template<typename tType> class CMap : public CObject
{

private:
    tType*          mData;
    unsigned        mSize;
    unsigned        mLen;
    
public:
    CMap();
    
};

#endif