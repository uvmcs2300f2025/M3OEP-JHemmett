
#include "PaymentPortal.h"

int PaymentPortal::getBalance(){return balance;}
int PaymentPortal::getOverDraft(){return overDraft;};

void PaymentPortal::deposit(int amount){
 if (amount > 0) {
   if (!overDraft){
     balance += amount;
   } else if (overDraft - amount < 0){
     balance = amount - overDraft;
     overDraft = 0;
   } else {
     overDraft -= amount;
   }
 }

}
void PaymentPortal::withdraw(int amount){
  if (amount > 0) {
    if (overDraft){
      overDraft += amount;
    } else if (balance - amount >= 0){
      balance -= amount;
    } else {
      amount -= balance;
      balance = 0;

      overDraft = amount;
    }
  }
}