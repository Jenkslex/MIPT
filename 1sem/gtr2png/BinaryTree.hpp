
#ifndef __BINARYTREE__
#define __BINARYTREE__
 
template<typename tType> class cNode
{
protected:
    cNode*  mRight;
    cNode*  mLeft;
    tType   mData;  
           
public:
    cNode(tType aData);
    cNode(cNode& aOther);
    cNode();
    ~cNode();

	cNode& operator=(cNode& aOther);
    
    cNode*  Right();
    cNode*  Left();
    void    SetRight(cNode* aChild);
    void    SetLeft(cNode* aChild);
    void    RemoveRight();
    void    RemoveLeft();
    tType&  Data();
    void    Data(tType aData);
};
                   

template<typename tType> class CBinaryTree
{
private:
    cNode<tType>  mRoot;
    
public:
    CBinaryTree();
    
    cNode<tType>* Root();

};


template<typename tType> CBinaryTree<tType>::CBinaryTree() :
    mRoot(0)
{
}

template<typename tType> cNode<tType>::cNode() :
        mRight(0),
        mLeft(0)
{
}

template<typename tType> cNode<tType>::cNode(tType aData) :
        mData(aData),
        mRight(0),
        mLeft(0)
{
}

template<typename tType> cNode<tType>::cNode(cNode<tType>& aOther) :
        mRight(0),
        mLeft(0)
{
	*this = aOther;
}

template<typename tType> cNode<tType>::~cNode()
{
    //delete mRight;
    //delete mLeft;
}

template<typename tType> cNode<tType>& cNode<tType>::operator=(cNode<tType>& aOther)
{
	mData = aOther.Data();
	if (aOther.Left())
	{
		mLeft = new cNode<tType>();
		*mLeft = *aOther.Left();
	}
	else
	{
		mLeft = 0;
	}
	if (aOther.Right())
	{
		mRight = new cNode<tType>();
		*mRight = *aOther.Right();
	}
	else
	{
		mRight = 0;
	}
	return *this;
}

template<typename tType> cNode<tType>* CBinaryTree<tType>::Root()
{
    return &mRoot;
} 

template<typename tType> cNode<tType>* cNode<tType>::Left()
{
    return mLeft;
}   

template<typename tType> cNode<tType>* cNode<tType>::Right()
{
    return mRight;
}  

template<typename tType> void cNode<tType>::SetLeft(cNode<tType>* aChild)
{
    //if (mLeft) throw "CBinaryTree::cNode::SetLeft: Left is not null";
    mLeft = aChild;
}   

template<typename tType> void cNode<tType>::SetRight(cNode<tType>* aChild)
{
    //if (mRight) throw "CBinaryTree::cNode::SetRight: Right is not null";
    mRight = aChild;
}   

template<typename tType> void cNode<tType>::RemoveLeft()
{
    delete mLeft;
	mLeft = 0;
}   

template<typename tType> void cNode<tType>::RemoveRight()
{               
    delete mRight;
	mRight = 0;
}    

template<typename tType> void cNode<tType>::Data(tType aData)
{               
    mData = aData;
}   

template<typename tType> tType& cNode<tType>::Data()
{               
    return mData;
}

#endif
