// Move semantics and what is left after "moving"
// M. Hamilton
//
// Thinking bout https://stackoverflow.com/questions/25341407/in-c11-why-not-right-to-use-moved-variable-after-stdm
// And will try to access the moved vector after I give it to the object.
// At least clang-tidy/clion should not be happy, even if it compiles.

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>

class MyClass
{
public:

    /**
     * This tests the move in the constructor. I expected that this would move the variable delivered in the ctor into
     * the object, making it non-usable outside after the move. It does not. It creates a copy of the variable which
     * is "moved". Can someone please reference the page in Meyers which explains this  ;-) ?
     */
    explicit MyClass(std::vector<int> myIntVec):
        myVec_(std::move(myIntVec))
    {
    }

    // Just do something to test what is happening
    int addUp()
    {
        return std::accumulate(myVec_.begin(), myVec_.end(), 0);
    }

private:
    std::vector<int> myVec_;
};

int main() {
    std::vector<int> moveMe {1,2,3,4,5};
    std::vector<int> moveMe2 {6,7,8,9,10};

    //! First try.
    // unique pointer to my object here isn't really relevant to this question
    std::unique_ptr<MyClass> myUpObj;
    // The move happens inside the ctor, which copies the variable? But why?
    myUpObj = std::make_unique<MyClass>(moveMe);
    std::cout << "inside object (pre-change):" << myUpObj->addUp() << std::endl;
    // Why doesn't this freak out?  Did I not "move" this to myUpObj? CLion doesn't even complain about using something
    // after moving it.
    moveMe[3] = 100;
    std::cout << "local (item 3 is 100) :" << std::accumulate(moveMe.begin(), moveMe.end(), 0) << std::endl;
    std::cout << "inside object (note item 3 not changed):" << myUpObj->addUp() << std::endl;


    //! Now do this a different way to see what happens
    std::unique_ptr<MyClass> myUpObj2;
    std::cout << "local baseline :" << std::accumulate(moveMe2.begin(), moveMe2.end(), 0) << std::endl;
    // The move happens here and send lvalue to ctor. So, can't use anymore locally as expected.
    myUpObj2 = std::make_unique<MyClass>(std::move(moveMe2));
    std::cout << "inside object2 (pre-change):" << myUpObj2->addUp() << std::endl;
    // Clang-tidy in CLion tells me this is used after move.
    std::cout << "local (after moved, no change) :" << std::accumulate(moveMe2.begin(), moveMe2.end(), 0) << std::endl;

    // This is a segfault even! But, Clang-tidy doesn't complain anymore - maybe only complains on the first one?
    moveMe2[4] = 200;
    std::cout << "local (changed) :" << std::accumulate(moveMe2.begin(), moveMe2.end(), 0) << std::endl;

    return 0;
}
