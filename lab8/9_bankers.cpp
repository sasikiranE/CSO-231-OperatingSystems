#include <cassert>
#include <fstream>
#include <iostream>
using namespace std;

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

// available amount of each resource
int available[NUMBER_OF_RESOURCES];

// the maximum demand of each customer
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// the amount currently allocated to each customer
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// the remaining need of each customer
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

bool request_resources(const int customer_num, const int request[]) {
    assert(customer_num < NUMBER_OF_CUSTOMERS);
    int try_allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
    int try_need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
    int try_available[NUMBER_OF_RESOURCES];

    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            // initialize
            try_allocation[i][j] = allocation[i][j];
            try_need[i][j] = need[i][j];
        }
        if (i == customer_num) {
            for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                try_allocation[i][j] += request[j];
                try_need[i][j] -= request[j];

                try_available[j] = available[j] - request[j];
                assert(try_available[j] >= 0);
            }
        }
    }
    // check if allocation is safe
    bool is_safe = false;
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        bool is_job_safe = true;
        // check if their need can be fullfilled if maximum resources are
        // requested
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            if (try_need[i][j] > try_available[j]) {
                cout << "Job " << i << " won't be able to finish.\n";
                is_job_safe = false;
                break;
            }
        }
        if (is_job_safe) {
            is_safe = true;
            break;
        }
    }

    if (is_safe) {
        // update allocation, need, available
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                // update allocation
                allocation[i][j] = try_allocation[i][j];

                // update need
                need[i][j] = try_need[i][j];
            }
        }

        // update available
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            available[j] = try_available[j];
        }
    }

    return is_safe;
}

void release_resources(const int customer_num, const int released[]) {
    assert(customer_num < NUMBER_OF_CUSTOMERS);
    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
        if(allocation[customer_num][j] < released[j]) {
            available[j] += allocation[customer_num][j]; 
            allocation[customer_num][j] = 0; 
        } else  {
            allocation[customer_num][j] -= released[j];
            available[j] += released[j];
        }
    }
}

void print_allocation() {
    cout << "Allocation Table\n";
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        cout << "Process " << i << " : ";
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            cout << allocation[i][j] << "\t";
        }
        cout << '\n';
    }
}

void print_need() {
    cout << "\nNeed Table\n";
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        cout << "Process " << i << " : ";
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            cout << need[i][j] << "\t";
        }
        cout << '\n';
    }
}

void print_available() {
    cout << "\nAvailable\n";
    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
        cout << available[j] << "\t";
    }
    cout << '\n';
}

int main(int argc, char *argv[]) {
    // get maximum available resources
    assert(argc - 1 == NUMBER_OF_RESOURCES);
    for (int i = 1; i < argc; i++) {
        available[i - 1] = atoi(argv[i]);
    }

    // input the maximum requirement of each customer
    ifstream infile;
    infile.open("9_input.txt");
    if (!infile) { // file couldn't be opened
        cerr << "Error: file could not be opened" << endl;
        exit(1);
    }
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            infile >> maximum[i][j];
            need[i][j] = maximum[i][j];

            // initialize
            allocation[i][j] = 0;
        }
    }

    string command;
    cout << "> ";
    cin >> command;
    while (command != "q") {
        if (command == "RQ") {
            // request
            int customer_num;
            int request[NUMBER_OF_RESOURCES];
            cout << ">> ";
            cin >> customer_num;
            cout << ">>> ";

            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                cin >> request[i];
            }
            bool request_accepted = request_resources(customer_num, request);
            if (request_accepted)
                cout << "Request Granted\n";
            else
                cout << "Request Denied\n";
        }
        if (command == "RL") {
            // release
            int customer_num;
            int request[NUMBER_OF_RESOURCES];
            cout << ">> ";
            cin >> customer_num;
            cout << ">>> ";

            for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                cin >> request[i];
            }
            release_resources(customer_num, request);
        }
        if (command == "*") {
            // print
            print_allocation();
            print_need();
            print_available();
        }
        cout << "> ";
        cin >> command;
    }

    return 0;
}