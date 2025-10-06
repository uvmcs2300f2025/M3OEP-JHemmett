cmake_minimum_required(VERSION 3.8)
project(M10EP)

set(CMAKE_CXX_STANDARD 17)

add_executable(M10EP
        paymentPortal.cpp
        Main.cpp
        Customer.cpp
        Employee.cpp
        Manager.cpp
        Item.cpp
        Transaction.cpp
        Settings.cpp
)

add_executable(test
        paymentPortal.cpp
        Tester.cpp
        Customer.cpp
        Employee.cpp
        Manager.cpp
        Item.cpp
        Transaction.cpp
        Settings.cpp
)