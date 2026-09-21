
#include <iostream>
#include <thread>

void Thread1() {
    std::cout << "1 ";
}

void Thread2() {
    std::cout << "2 ";
}

int main() {
    std::thread t1(Thread1);
    std::thread t2(Thread2);

    t1.detach();
    t2.detach();

    // Головний потік може завершитися раніше, ніж дочірні потоки виведуть дані
    return 0;
}
