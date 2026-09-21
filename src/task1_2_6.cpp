#include <iostream>
#include <string>
#include <thread>
#include <mutex>

struct someData {
    std::string name;
    std::string surname;
    std::string address;
    int age;
};

class exchangePerson {
public:
    someData data;
    std::mutex m;

    static void JohnDoe(exchangePerson& obj) {
        std::lock_guard<std::mutex> lock(obj.m);
        obj.data.name = "John";
        obj.data.surname = "Doe";
        obj.data.address = "Unknown";
        obj.data.age = 120;
    }

    static void JacobSmith(exchangePerson& obj) {
        std::lock_guard<std::mutex> lock(obj.m);
        obj.data.name = "Jacob";
        obj.data.surname = "Smith";
        obj.data.address = "Known";
        obj.data.age = 1;
    }

    static void Swap(exchangePerson& p1, exchangePerson& p2) {
        if (&p1 == &p2) return; // Захист від однакових посилань

        // Одночасне безпечне блокування двох м'ютексів
        std::lock(p1.m, p2.m);
        std::lock_guard<std::mutex> lock1(p1.m, std::adopt_lock);
        std::lock_guard<std::mutex> lock2(p2.m, std::adopt_lock);

        std::swap(p1.data, p2.data);
    }

    void Print() const {
        std::cout << "Name: " << data.name << ", Surname: " << data.surname
                  << ", Address: " << data.address << ", Age: " << data.age << std::endl;
    }
};

int main() {
    exchangePerson p1, p2;

    std::thread t1(exchangePerson::JohnDoe, std::ref(p1));
    std::thread t2(exchangePerson::JacobSmith, std::ref(p2));

    t1.detach();
    t2.detach();

    // Запускаємо Swap у окремому потоці
    std::thread tSwap([&]() {
        exchangePerson::Swap(p1, p2);
    });

    tSwap.join(); // Гарантуємо отримання результатів до завершення main

    std::cout << "--- Result after Swap ---" << std::endl;
    std::cout << "p1: "; p1.Print();
    std::cout << "p2: "; p2.Print();

    return 0;
}
