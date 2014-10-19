#ifndef Polynomial_Manipulation_Header_h
#define Polynomial_Manipulation_Header_h

#include <vector>

struct Node;

class Polynomial{

private:
    Node* head;
    void removeFromList(Node* &curr, Node* &prevCurr);
    void binary(int n, std::vector<int> &currBinary);
    std::vector<int> binary(int n);
    Node* getHead() const;
    void polyClear();
    void polyCopy(const Polynomial &);

public:
    Polynomial();
    Polynomial(Node*);
    Polynomial(std::vector<int> , std::vector<int> );
    Polynomial(const Polynomial &);
    ~Polynomial();

    double evaluate(double x);
    std::string print() const;

    void addToList(int newExp, int newCoeff);

    Polynomial exponent(int exp);
    bool modulo(const Polynomial &, const Polynomial &);


    Polynomial& operator=(const Polynomial &);
    friend Polynomial operator*(const Polynomial &, const Polynomial &);
    friend Polynomial operator+(const Polynomial &, const Polynomial &);
    friend Polynomial operator-(const Polynomial &, const Polynomial &);
    friend Polynomial addSubtractLists(std::string operation, const Polynomial &, const Polynomial &);

};


#endif
