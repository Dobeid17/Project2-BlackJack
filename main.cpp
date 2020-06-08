/*
 * File:   main.cpp
 * Author: Danny Obeid
 * Created on May 25th, 2020, 6:50 PM
 * Purpose:  Project 2
*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <map>
#include <set>
#include <list>
#include <stack>
#include <queue>
#include <unordered_map>
#include <stdlib.h>
#include <algorithm>
#include <string>

using namespace std;

struct Card
{
    string cardType;
    int cardValue;

    Card() {}
    Card(string name, int value)
    {
        cardType = name;
        cardValue = value;
    }
};

void printHand(list<Card> hand);    // Displays either the player or dealers hand
void playGame();                    // Where the game is played and everything is utilized
stack<Card> shuDeck(stack<Card> &deck);              // the deck is built and shuffled 
Card hitCard(stack<Card>& deck);    // If the player or dealer wants another card
bool isOver(list<Card> hand);       // checks if either the player or dealer is over 21
bool isDealOver(list<Card> dealHand);// checks to see if the dealer is over 21 after a hit
bool is21(list<Card> hand);         // checks to see if dealar or player has 21
bool ifAce(list<Card> hand);        // checks to see if card is an Ace
int compareHands(list<Card> myHand, list<Card> dealHand); //comares the hands to see who wins if neither are 21
void replaceAce(list<Card>& hand); // If there is an Ace, you can replace the 1 with an 11
void loadRecursive(stack<Card> &reDeck, int valCount);

int main()
{
    srand((unsigned)time(0));

    int choice = 0;

    //Main Menu
    cout << "Hello and welcome to Black Jack!" << endl
        << "What would you like to do?" << endl
        << "1.) Hear rules." << endl
        << "2.) Play." << endl
        << "3.) Quit. " << endl;
    cin >> choice;

    switch (choice)
    {
        //Case 1 allows you to view the rules then make another decision 
    case 1: cout << "The rules of Black Jack are simple, it is you versus the dealer. The goal is beat the dealers hand without going over 21. Face cards are worth 10 and Aces are worth 11 or 1. Each player starts wth two cards and can hit to add another. If you go over 21, you lose. If the dealer goes over 21 you win! If it is a standoff your bet get returned. Good luck!" << endl;
        cout << "2.) Play." << endl
            << "3.) Quit. " << endl;
        cin >> choice;

        if (choice == 2)
        {
            playGame();
        }

        else if (choice == 3)
        {
            cout << "Thank you! Goodbye!" << endl;
        }
        else
        {
            cout << "INVALID CHOICE! PROGRAM CLOSING!" << endl;
            return 0;
        }
        break;
        // Case 2 plays the gamees
    case 2: playGame();

        break;
        //Quit just ends the program 
    case 3: cout << "Thank You! Goodbye!" << endl;

        break;
        //Anything else thats not an option just closes the program
    default: cout << "INVALID CHOICE! PROGRAM CLOSING!" << endl;
    }

    return 0;
}

void playGame()
{
    char choice = 'y';
    int toHit = 0;
    int toPlay = 0;
    int balance = 100; // Starting balance 
    int bet = 0;
    bool play = true;

    stack<Card> deck;   // Deck we will be using
    
    //Dislpay user balance
    cout << "Here is your current balance: $";
    cout << balance << endl;
    //Asked for how much they want to bet
    //Check to see if they have what amount to bet
    while (balance >= 5 && play == true)
    {
        list<Card> myHand;     // Your hand
        list<Card> dealHand;   // Dealers hand 
        toHit = 0;             // reset choices for new game 
        toPlay = 0;            // reset choices for new game 
        bool dealBust = false; // reset parameters for new game 
        bool dealWon = false;  // reset parameters for new game 

        cout << "How much do you want to bet this hand?\n"
            << "Must be more than $5 and less than $ " << balance << endl;
        cin >> bet;

        while (bet < 5 || bet > balance)
        {
            cout << "INVALID BET. TRY AGAIN" << endl;
            cin >> bet;
        }

        balance = balance - bet;
        // Loads recursive deck
        loadRecursive(deck, 1);
        //shuffles the recursive deck
        deck = shuDeck(deck);
        
        // Passes out the cards one at a time to each hand
        dealHand.push_back(hitCard(deck));
        myHand.push_back(hitCard(deck));
        dealHand.push_back(hitCard(deck));
        myHand.push_back(hitCard(deck));

        // Displays your hand
        cout << "Here is your hand: ";
        printHand(myHand);
        // Checks to see if your hand contains an Ace to see if you want to change it to an 11 
        if (ifAce(myHand) == true)
        {
            cout << "You pulled an Ace. Would you like to turn your Ace into an 11? (y/n)" << endl;
            cin >> choice;
            if (choice == 'y' || choice == 'Y')
            {
                // turn 1 into 11
                replaceAce(myHand);

                cout << "Here is your new hand: " << endl;
                printHand(myHand);

            }
        }
        cout << "Here is what the dealer is showing: ";

        list<Card>::iterator it = dealHand.begin();
        advance(it, 1);
        // We dont want to show the dealers full hand show this is to show only one card
        cout << it->cardValue << " " << it->cardType;

        //If the dealer has an Ace we turn it into an 11 instead of a 1
        if (ifAce(dealHand) == true)
        {
            replaceAce(dealHand);
        }

        cout << "\nWhat are you going to do? \n"
            << "1.) Hit\n"
            << "2.) Stay" << endl;

        cin >> toHit;

        while (toHit == 1 && toPlay != 1)
        {
            // Adds another card to your hand
            myHand.push_back(hitCard(deck));
            cout << "This is your new hand" << endl;
            printHand(myHand);

            //Checks to see if that hit put you over 21
            if (isOver(myHand) == true)
            {
                cout << "You're over 21! You Lose.\n"
                    << "Here is your balance: $" << balance << endl << endl;

                cout << "Play again?\n"
                    << "1.) Yes.\n"
                    << "2.) Quit.\n";

                cin >> toPlay;

                if (toPlay == 2)
                {
                    return;
                }
            }

            else
            {
                cout << "\nWhat are you going to do? \n"
                    << "1.) Hit\n"
                    << "2.) Stay" << endl;

                cin >> toHit;
            }

        }

        // Once we are done hitting or not hitting it is the dealers turn
        if (toPlay != 1)
        {
            cout << "Here is the dealers hand." << endl;
            printHand(dealHand);
            //Checks to see if the other card was an ace to replace it
            if (ifAce(dealHand) == true)
            {
                replaceAce(dealHand);
            }
            // Compare hands to see if the dealer beat you before having to hit
            if (compareHands(myHand, dealHand) == -1)
            {
                dealWon = true;
            }

            //Checks to see if the dealer is over 17, if not he automatically hits
            while (isDealOver(dealHand) == false && dealWon == false)
            {
                cout << "The dealer hits." << endl;
                //Adds another card to dealers hand
                dealHand.push_back(hitCard(deck));

                printHand(dealHand);

                //Checks to see if the dealer got 21 after the hit
                if (is21(dealHand) == true)
                {
                    cout << "The dealer got 21!\n";
                }

                // Checks to see if the dealer went over 21 on the hit
                else if (isOver(dealHand) == true)
                {
                    cout << "The dealer busted! You Win!" << endl;
                    balance = balance + (bet * 2);
                    cout << "Here is your balance: $" << balance << " continue playing?" << endl << endl;
                    dealBust = true;
                }

            }
            // If neither you or the dealer got 21, we compare hands
            if (dealBust == false)
            {
                // See if your hand beats the dealers hand
                if (compareHands(myHand, dealHand) == 1)
                {
                    cout << "YOU WIN!\n";
                    balance = balance + (bet * 2);

                    cout << "Here is your new balance: $" << balance << endl << endl;


                }
                //See if the dealers hand beats your hand
                else if (compareHands(myHand, dealHand) == -1)
                {
                    cout << "You Lose!\n";
                    cout << "Here is your new balance: $ " << balance << endl << endl;
                }
                // See if its a stand off
                else
                {
                    cout << "Its a draw! Your bet will be returned\n";
                    balance = (balance + bet);
                    cout << "Here is your balance: $" << balance << endl << endl;
                }
            }
            // After the hand is over, Ask what you want to do now
            cout << "What do you want to do?\n"
                << "1.) Play again. \n"
                << "2.) Quit" << endl;
            cin >> toPlay;



            // If you dont want to play quit program
            if (toPlay == 2)
            {
                return;
            }
        }
    }
    // If your balance is less than the minimum bet you cant play
    if (balance < 5)
    {
        cout << "INSUFFICANT FUNDS. You are unable to continue." << endl;
    }

}


void printHand(list<Card> hand)
{
    // For each loop used to go through and display either the dealers hand or the players hand
    for (Card card : hand)
    {
        cout << card.cardValue << " " << card.cardType << " ";
    }
    cout << endl;
}

void replaceAce(list<Card>& hand)
{
    list<Card>::iterator it = hand.begin();
    string type;
    Card newCard;

    // iterate through to find if the dealer or player is carrying an Ace
    for (int i = 0; i < hand.size(); i++)
    {
        if (it->cardValue == 1)
        {
            // Create a new Ace card and make it an 11
            type = it->cardType;
            newCard = Card(type, 11);
            (*it) = newCard;
            break;

        }
        advance(it, 1);
    }
}

stack<Card> shuDeck(stack<Card> &deck)
{
    unordered_map<int, Card> newDeck;
    stack<Card> doneDeck;
    Card transfer;
    int count = 0;
    srand(time(NULL));
    int deckSize = deck.size();

    for (int i = 0; i < deckSize; i++)
    {
        transfer = deck.top();
        deck.pop();
        newDeck.emplace(i, transfer);   
    }

    while (newDeck.size() > 0)
    {
        unordered_map<int, Card>::iterator random_it = newDeck.begin();
        advance(random_it, (rand() % newDeck.size()));
        doneDeck.push(random_it->second);
        newDeck.erase(random_it);
    }

    return doneDeck;
}

Card hitCard(stack<Card>& deck)
{
    // Adds a new card to either the player or dealers hand then pop it off the deck
    Card hitCard = (deck.top());
    deck.pop();

    return hitCard;
}

bool isOver(list<Card> hand)
{
    // Checks if the current hand goes over 21
    int sum = 0;
    for (auto& x : hand)
    {
        sum = sum + x.cardValue;
    }
    if (sum > 21)
    {

        return true;
    }
    else
    {
        return false;
    }
}

bool isDealOver(list<Card> dealHand)
{
    int dealSum = 0;

    // Checks to see if the dealer goes over 17 so they can stop hitting
    for (auto& x : dealHand)
    {
        dealSum = dealSum + x.cardValue;
    }
    if (dealSum < 17)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool is21(list<Card> hand)
{

    int sum = 0;

    // Checks to see if the dealer got 21
    for (auto& x : hand)
    {
        sum = sum + x.cardValue;
    }
    if (sum == 21)
    {

        return true;
    }

    else
    {
        return false;
    }

}

bool ifAce(list<Card> hand)
{
    // Identifies if their is an Ace in either the players or dealers hand
    
    for (auto& x : hand)
    {
        if (x.cardValue == 1)
        {
            return true;
        }
    }

    return false;
}

int compareHands(list<Card> myHand, list<Card> dealHand)
{
    int mySum = 0;
    int dealSum = 0;

    // If no one gets 21, We have to go in and see who has the higher hand
    for (auto& x : dealHand)
    {
        dealSum = dealSum + x.cardValue;
    }

    for (auto& x : myHand)
    {
        mySum = mySum + x.cardValue;
    }

    if (dealSum > mySum)
    {
        return -1;
    }

    else if (mySum > dealSum)
    {
        return 1;
    }

    else
    {
        return 0;
    }

}

void loadRecursive(stack<Card> &reDeck, int valCount)
{
    
    //Base Case
    if (reDeck.size() == 52)
    {
        return;
    }
    //Recursive Case
    else
    {
        Card newOne;

        if (reDeck.size() < 13)
        {    
           
            newOne.cardType = "Hearts";
        }
        
        if (reDeck.size() >= 13 && reDeck.size() < 26)
        {         
            newOne.cardType = "Spades";
        }

        if (reDeck.size() >= 26 && reDeck.size() < 39)
        {         
            newOne.cardType = "Clovers";       
        }

        if (reDeck.size() >= 39 && reDeck.size() < 52)
        {
            newOne.cardType = "Diamonds";       
        }

        if (valCount >= 10)
        {
            newOne.cardValue = 10;
        }

        else
        {
            newOne.cardValue = valCount;
        }

        reDeck.push(newOne);
        
        if (valCount >= 13)
        {
            valCount = 1;      
        }
        
        else
        {
            valCount += 1;
        }

        loadRecursive(reDeck, valCount);
    }
}

// Alternate use for Recursive Sort
/*
    Recursive Sort, quickSort
 //-----------------------------------------------------------------
    void swap(int* x, int* y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

int partition(int myArray[], int min, int max)
{
    int pivot = myArray[max];  
    int low = min - 1  

    for (int i = min; i <= max - 1; i++)
    {
        if (myArray[i] < pivot)
        {
            low++;   
            swap(&myArray[low], &myArray[i]);
        }
    }
    swap(&myArray[low + 1], &myArray[max]);
    return (low + 1);
}

void quickSort(int myArray[], int min, int max)
{
    if (min < max)
    {
        int pi = partition(myArray, min, max);

        quickSort(myArray, min, pi - 1);
        quickSort(myArray, pi + 1, max);
    }
}

int main()
{
    int myArray[] = { 5,7,6,1,2,3,7,2,0,8,12,48,64,8 };
    int size = sizeof(myArray) / sizeof(myArray[0]);
    quickSort(myArray, 0, size - 1);

    return 0;
}

*/

