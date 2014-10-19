#include <iostream>
#include <String>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <math.h>
#include <vector>
#include "polynomial.h"

//constants
const std::string ADD = "add";
const std::string SUBTRACT = "subtract";

Node* head;

//sets up Node as a struct
//Node holds a term of the polynomial
struct Node {
    int exponent;
    int coefficient;
    Node* next;

    Node() : exponent(-1), coefficient(0), next(NULL){}

    Node(int exp, int coeff) : exponent(exp), coefficient(coeff), next(NULL){}

    Node(int exp, int coeff, Node* nxt) : exponent(exp), coefficient(coeff), next(nxt){}

    ~Node(){
    }
};

//Constructors
Polynomial::Polynomial(){
    head = NULL;
}

Polynomial::Polynomial(Node* n){
    head = NULL;
    if (n != NULL){
        addToList(n->exponent, n->coefficient);
    }
}

//Create polynomial from vectors
//for debugging
Polynomial::Polynomial(std::vector<int> exps, std::vector<int> coeffs){
    head = NULL;
    for (unsigned int i = 0; i < exps.size(); i++){
        addToList(exps[i], coeffs[i]);
    }
}

//Copy constructor
Polynomial::Polynomial(const Polynomial &poly){
    polyCopy(poly);
}

//copies polynomial into this
void Polynomial::polyCopy(const Polynomial &poly){
    head = NULL;
    Node* curr = poly.getHead();

    while (curr != NULL) {
        addToList(curr->exponent, curr->coefficient);
        curr = curr->next;
    }
}

//overloaded assignment operator
//deletes all nodes in this,
//then copies all nodes from argument into this
Polynomial& Polynomial::operator=(const Polynomial &poly){
    polyClear();
    polyCopy(poly);
    return *this;
}

//deletes all nodes from the polynomial
void Polynomial::polyClear(){
    Node* curr = head;
    Node* prevCurr = NULL;
    while (curr != NULL) {
        prevCurr = curr;
        curr = curr->next;
        delete prevCurr;
    }
}

Polynomial::~Polynomial(){
    polyClear();
}

Node* Polynomial::getHead() const{
    return head;
}

//returns a string of the polynomial
//in the form ax^b + cx^d + ... + ex + f
std::string Polynomial::print() const{
    if (head == NULL){
        return "0";
    }

    Node* curr = head;
    std::stringstream currStr;
    while (curr != NULL) {
        if (curr->exponent == 0){
            currStr << curr->coefficient;
        }
        else {
            if (curr->coefficient == -1){
                currStr << "-";
            }
            else if (curr->coefficient != 1){
                currStr << curr->coefficient;
            }


            currStr << "x";
            if (curr->exponent != 1){
                currStr << "^" << curr->exponent;
            }
        }

        if (curr->next != NULL){
            currStr << " + ";
        }

        curr = curr->next;
    }

    return currStr.str();
}

//Adds a term to the polynomial
//Maintains descending sorted order
void Polynomial::addToList(int newExp, int newCoeff){
    if (newCoeff == 0){
        return;
    }

    //if list empty
    if (head == NULL){
        head = new Node(newExp, newCoeff);
    }
    //if inserting at the front
    else if (head->exponent < newExp){
        head = new Node(newExp, newCoeff, head);
    }
    else {
        Node* curr = head;
        Node* prevCurr = NULL;

        //iterate until the appropriate insertion point
        while (curr != NULL && curr->exponent > newExp){
            prevCurr = curr;
            curr = curr->next;
        }

        //if inserting at the end of the list
        if (curr == NULL){
            prevCurr->next = new Node(newExp, newCoeff);
        }
        //if a term of the same order is already in the list
        else if (curr->exponent == newExp){
            curr->coefficient += newCoeff;
        }
        else {
            prevCurr->next = new Node(newExp, newCoeff, curr);
        }
    }
}


//WARNING:
//Polynomial exponentiation can result in huge numbers.
//May result in overflow
double Polynomial::evaluate(double x){
    Node* curr = head;
    double sum = 0;
    while (curr != NULL) {
        sum += curr->coefficient * pow(x,curr->exponent);
        curr = curr->next;
    }

    return sum;
}

//recursively converts the number to binary
//returns result as a binary vector
void Polynomial::binary(int n, std::vector<int> &currBinary){
    if (n > 0) {
        int remainder = n % 2;
        binary(n >> 1, currBinary);
        currBinary.push_back(remainder);
    }
}

//converts number to binary
//wrapper function to make more user friendly
std::vector<int> Polynomial::binary(int n){
    std::vector<int> currBinary;
    binary(n, currBinary);
    return currBinary;
}

//Adds or subtracts two lists
Polynomial addSubtractLists(std::string operation, const Polynomial &poly1, const Polynomial &poly2){
    if (poly2.getHead() == NULL){
        return Polynomial(poly1);
    }

    if (poly1.getHead() == NULL){
        return Polynomial(poly2);
    }

    Node* curr = poly1.getHead();
    Node* curr2 = poly2.getHead();
    Polynomial resPoly;

    //while still has nodes
    while (curr != NULL || curr2 != NULL) {
        //take the highest exponent element not already dealt with
        if (curr2 == NULL || (curr != NULL && curr->exponent > curr2->exponent)){
            //if second operand has the highest order term, add the term to the new list
            resPoly.addToList(curr->exponent, curr->coefficient);
            curr = curr->next;
        }
        else if (curr == NULL || (curr2 != NULL && curr->exponent < curr2->exponent)){
            //if second operand has the highest order term, add or subtract the term to the new list
            if (operation == ADD){
                resPoly.addToList(curr2->exponent, curr2->coefficient);
            }
            else if (operation == SUBTRACT){
                resPoly.addToList(curr2->exponent, -1 * curr2->coefficient);
            }

            curr2 = curr2->next;
        }
        else {
            //if a term of the same order is already in the list, combine with that term
            if (operation == ADD) {
                resPoly.addToList(curr->exponent, curr->coefficient + curr2->coefficient);
            }
            else if (operation == SUBTRACT) {
                resPoly.addToList(curr->exponent, curr->coefficient - curr2->coefficient);
            }
            curr = curr->next;
            curr2 = curr2->next;
        }

    }

    return resPoly;
}

Polynomial operator+(const Polynomial &poly1, const Polynomial &poly2){
    return addSubtractLists(ADD, poly1, poly2);
}

Polynomial operator-(const Polynomial &poly1, const Polynomial &poly2){
    return addSubtractLists(SUBTRACT, poly1, poly2);
}

Polynomial operator*(const Polynomial &poly1, const Polynomial &poly2){
    Polynomial resPoly;
    if (poly1.getHead() == NULL){
        resPoly = Polynomial(poly2);
        return resPoly;
    }

    if (poly2.getHead() == NULL){
        resPoly = Polynomial(poly1);
        return resPoly;
    }

    Node* curr = poly1.getHead();
    Node* head2 = poly2.getHead();
    Node* curr2;

    //multiplies each term of the first list by each term of the second list
    while (curr != NULL){
        curr2 = head2;
        while (curr2 != NULL){
            resPoly.addToList(curr->exponent + curr2->exponent, curr->coefficient * curr2->coefficient);
            curr2 = curr2->next;
        }
        curr = curr->next;
    }
    return resPoly;
}

//finds exponent of a polynomial
Polynomial Polynomial::exponent(int exp){
    Polynomial resPoly;

    if (exp < 0){
        std::cout << "Exponent must be nonnegative" << std::endl;
        return NULL;
    }

    //if exponent is zero, the result is just 1
    if (exp == 0){
        resPoly.addToList(0, 1);
        return resPoly;
    }

    std::vector<int> binaryVec = binary(exp);

    Polynomial currPoly = Polynomial(*this);

    //repeatedly squares currP
    //adds to the result when currP is of an order needed
    //to write the exponent as a sum of powers of 2
    for (int i = binaryVec.size() - 1; i >= 0; i--){
        if (binaryVec[i] == 1){
            resPoly = currPoly * resPoly;
        }

        //if has another iteration
        if (i >= 0){
            currPoly = currPoly * currPoly;
        }
    }

    return resPoly;
}

bool Polynomial::modulo(const Polynomial &poly1, const Polynomial &poly2){
    polyClear();
    if (poly2.getHead() == NULL){
        return false;
    }
    if (poly2.getHead()->coefficient != 1){
        std::cout << "Ensure leading coefficient of dividend is 1" << std::endl;
        return false;
    }
    if (poly1.getHead() == NULL){
        polyCopy(poly2);
    }

    polyCopy(poly1);
    int expDiff, resLeadingCoeff;

    //multiplies the second operand by the term needed to give it the same
    //order as the highest power of the first operand
    while (head != NULL && head->exponent >= poly2.getHead()->exponent){
        expDiff = head->exponent - poly2.getHead()->exponent;
        resLeadingCoeff = head->coefficient;

        Polynomial factor = Polynomial(new Node(expDiff, resLeadingCoeff));
        Polynomial eqexp = poly2 * factor;
        *this = *this - eqexp;
    }

    return true;
}






