/*
 * Homework 1 for NCTU network programming class
 * ClawMachine simulation using Multi-thread
 *
 * Basic version(Only 1 clawmachine)
 *
 * Author: Eason Chang <eason@easonchang.com>
 */

#include <iostream>
#include <vector>
using namespace std;


// ========== Class definition ==========
//
// ---------- CustomerStatus enumeration ----------
enum CustomerStatus { inactive, playing, resting, queuing, won };
// ---------- ./CustomerStatus enumeration ----------

// ---------- ClawMachineStatus enumeration ----------
enum ClawMachineStatus { idle, busy };
// ---------- ./ClawMachineStatus enumeration ----------

// ---------- Customer Class ---------
class Customer {
  public:
    Customer(int arriveTime, int contPlayRound, int restTime, int totalPlayRound)
      : arriveTime(arriveTime),
        contPlayRound(contPlayRound),
        restTime(restTime),
        totalPlayRound(totalPlayRound),
        currentRound(0),
        currentRestTime(0),
        status(inactive)
    {}

  private:
    const int arriveTime; // Time arrived to join the game
    const int contPlayRound; // Continuously play round
    const int restTime; // Rest time between next try
    const int totalPlayRound; // In which round this customer will get the prize
    int currentRound; // How many times this customer has tried (total, may larger than contPlayRound, but less than totalPlayRound)
    int currentRestTime; // Current rest time, when equal to restTime, user will start to play
    CustomerStatus status;
}
// ---------- ./Customer Class ---------

// ---------- ClawMachine Class ----------
class ClawMachine {
  public:
    ClawMachine()
      : status(idle)
    {}
  private:
    ClawMachineStatus status;
}
// --------- ./ClawMachine class ----------
// ========== ./Class Definitions ==========


// ========== Global variables =========
vector<Customer> customers;
ClawMachine clawMachine1;
int guaranteeNum;
int numCustomers; // how many customers there are
// ========== ./Global variables =========


// ========== Function declarations ==========
void readInput();
// ========== ./Function declarations ==========


// ========== Main function ==========
int main( int argc, char* argv[] ) {
  readInput();
  return 0;
}
// ========== ./Main function =========


// ========== Function implementations ==========
void readInput() {
  cin >> guaranteeNum;
  cin >> numCustomers;
  for ( int i = 0; i < numCustomers; ++i ) {
    int arriveTime, contPlayRound, restTime, totalPlayRound;
    cin >> arriveTime >> contPlayRound >> restTime >> totalPlayRound;
    Customer customer(arriveTime, contPlayRound, restTime, totalPlayRound);
    customers.push_back(customer);
  }
}
// ========== ./Function implementations ==========

