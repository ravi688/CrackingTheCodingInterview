// Stackoverflow: https://stackoverflow.com/questions/5501959/why-does-c11s-lambda-require-mutable-keyword-for-capture-by-value-by-defau
// Answer:
// It requires mutable because by default, a function object should produce the same result every time it's called. 
// This is the difference between an object orientated function and a function using a global variable, 
// effectively.

#include <iostream>

int main()
{
        int captureVar = 100;
        auto functionObj = [captureVar]() mutable
        {
                captureVar += 1;
                std::cout << "CaptureVar: " << captureVar << std::endl;
        };
        // SHOULD PRINT 101
        functionObj();
        // SHOULD PRINT 102
        functionObj();
        // SHOULD PRINT 103
        functionObj();
        // SHOULD PRINT 104
        functionObj();
        return 0;
}
