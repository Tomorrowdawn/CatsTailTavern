#include "Listener.h"
#include "Listeners.h"

using namespace cgisim;

struct TokenTest:Listener{
    int tid = 0;
    std::string getName() override {
        printf("hello i am tokentest\n");
        return "TokenTest";
    }
    Elist listen(EventType e){return Elist();}
};


REGISTER_TOKEN(TokenTest, TokenTest);