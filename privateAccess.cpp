//
// Created by michael.hamilton on 5/6/21.
// I want to pass an unmutable reference (no copy) of a private vector to the outside in order to use the vector
// but not have it be changable.
// Auto declaration type deduction makes a copy (pass by value) by default.
//

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>

class MyClass
{
public:
    explicit MyClass(std::vector<int> myIntVec):
            myVec_(std::move(myIntVec))
    {}

    // Just do something to test what is happening
    int addUp()
    {
        return std::accumulate(myVec_.begin(), myVec_.end(), 0);
    }

    void * getMyVecAddr() {
        return &myVec_;
    }

    const std::vector<int>& getMyVec() const {return myVec_;}

private:
    std::vector<int> myVec_;
};



int main()
{
    std::vector<int> theVector {1,2,3,4,5};

    std::unique_ptr<MyClass> myUpObj;
    myUpObj = std::make_unique<MyClass>(theVector);

    const std::vector<int>& constVec = myUpObj->getMyVec();
    std::vector<int> newVec = myUpObj->getMyVec();
    auto autoVec = myUpObj->getMyVec();

    theVector[3] = 100;
    newVec[3] = 200;
    newVec.push_back(10);

    // These are both compiler errors
//    constVec[2] = 10;
//    constVec.push_back(30);

    // Addresses
    std::cout << "orig vector: " << &theVector << std::endl;
    std::cout << "inside object: " << myUpObj->getMyVecAddr() << std::endl;
    std::cout << "constVec: " << &constVec << " <---what I want: a non-mutable reference pass by value!" << std::endl;
    std::cout << "newVec: " << &newVec << std::endl;
    std::cout << "autoVec: " << &autoVec << " <---disappointing that auto defaults to pass by value copy." << std::endl;


    // this makes a copy. Ideally, it would say can't mod a const.
    std::cout << "theVector(mod):" << std::accumulate(theVector.begin(), theVector.end(), 0) << std::endl;
    std::cout << "inside Obj: " << myUpObj->addUp() << std::endl;
    std::cout << "newVec(mod):" << std::accumulate(newVec.begin(), newVec.end(), 0) << std::endl;
}

