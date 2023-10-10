#include<iostream>
#include"FactoryTest.hpp"
int main(){
    auto T = Factory<Listener>::produce("TokenTest");
    T->getName();
    return 0;
}