# hermes guard
this header will make it very difficult for a reverser to trace your calls. \
on top of that it also makes static analysis very confusing. \
all it does is hide the destination of a call and also mess with the return-stack.

warning:
this is not a one-and-done standalone anti-crack solution. \
this header is intended to be combined with other products or in-house protection.

# perks
- calls have seemingly random and invalid addresses
- easy to implement with existing source-code

# cons
- performance hit on high-frequency calls

# example usage
this can be used in a security related or core function of your app. \
it will hide the actual destination of your calls which will make \
a reverser have to manually trace the code.

```cpp
int main()
{
    // register the exception handler.
    hermes::initialize();

    // alias a function for a hermes-call.
    HERMES_ADD(Sleep, void(*)(u32));
   
    // test
    Sleep(5000);

    system("pause");
}
```
