
#include <iostream>
#include <list>
#include <thread>
#include <mutex>

std::list<int> l;
std::mutex m;

void AddToList(int startVal) {
    for (int i = 0; i < 10; ++i) {
        m.lock();
        l.push_back(startVal + i);
        std::cout << "[Add] Added: " << (startVal + i) << std::endl;
        m.unlock();
    }
}

void ListContains(int searchVal) {
    for (int i = 0; i < 10; ++i) {
        m.lock();
        bool found = false;
        for (int val : l) {
            if (val == searchVal) {
                found = true;
                break;
            }
        }
        std::cout << "[Contains] Search " << searchVal << ": " 
                  << (found ? "Found" : "Not found") << std::endl;
        m.unlock();
    }
}

int main() {
    int val = 5;
    std::thread t1(AddToList, val);
    std::thread t2(ListContains, val);

    t1.join();
    t2.join();

    return 0;
}
