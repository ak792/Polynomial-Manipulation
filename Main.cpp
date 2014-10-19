#include <iostream>
#include "polynomial.h"
#include <vector>
#include "stdlib.h"



//ideally this would be an unordered_map instead of parallel vectors, but didn't want to use C++11
std::vector<std::string> polyNames;
std::vector<Polynomial> polynomials;

std::string usage(std::string command);
void printInterface();
int getPolynomialIndex(const std::string &);
void savePolynomial(const Polynomial & , std::string &);
void getNewPolynomial();
void askToSavePolynomial(Polynomial &);
void printAllPolynomials();
Polynomial getPolynomial(const std::string &);
void operate();
void runUserInput();
void runNoUserInput();


int const MAX_ARGS = 3;

int main()
{
    runUserInput();
    //runNoUserInput();

    return 0;
}

//prints correct usage for the commands
std::string usage(std::string command){
    std::string usageStr;
    if (command == "="){
        usageStr = "'= [polynomial1] +|-|*|%|** [polynomial2|exponent]'";
    }
    else if (command == "ls"){
        usageStr = "'ls'";
    }
    else if (command == "print"){
        usageStr = "'print' followed by your polynomial's variable name";
    }
    else if (command == "new"){
        usageStr = "'new'";
    }
    else if (command == "evaluate"){
        usageStr = "'eval [polynomial] [number]'";
    }

    return usageStr;
}

//prints the text-based user interface
void printInterface(){
    std::cout << std::endl;
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Polynomial Manipulation" << std::endl;
    std::cout << "To add a polynomial, enter " << usage("new") << std::endl;
    std::cout << "To operate on polynomials, enter " << usage("=") << std::endl;
    std::cout << "To evaluate a polynomial, enter " << usage("evaluate")  << std::endl;
    std::cout << "To print a polynomial, enter " << usage("print")  << std::endl;
    std::cout << "To view all saved polynomials, enter " << usage("ls")  << std::endl;
    std::cout << "To exit, enter 'q'" << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

//gets the index of a given polynomial in the parallel vectors
//returns -1 if not found
int getPolynomialIndex(const std::string &polyName){
    for (int i = 0; i < polyNames.size(); i++){
        if (polyNames[i] == polyName){
            return i;
        }
    }

    return -1;
}

//saves the polynomial in the parallel vectors
void savePolynomial(const Polynomial &p , std::string &polyName){

    while (getPolynomialIndex(polyName) != -1){
        std::cout << "There already exists a polynomial named " << polyName << std::endl;
        std::cout << "Enter another name: " << std::endl;
        
        std::getline(std::cin, polyName);
    }

    polynomials.push_back(p);
    polyNames.push_back(polyName);
}

//allows user to input more polynomials
void getNewPolynomial(){
    double dCoeff, dExp;
    std::string coeffStr, expStr;
    bool validInput = false;

    Polynomial poly;

    std::string moreTerms = "Y";
    while (moreTerms == "Y" || moreTerms == "y"){
        validInput = false;
        while (!validInput){
            try {
                std::cout << "Enter coefficient of new term" << std::endl;
                std::getline(std::cin, coeffStr);
                dCoeff = stod(coeffStr);
                if (static_cast<int> (dCoeff + .5) != dCoeff){
                    std::cout << "Rounding " << dCoeff << " to " << static_cast<int> (dCoeff + .5) << std::endl;
                }
                validInput = true;
                
            }
            catch (const std::invalid_argument& err){
                std::cout << "Invalid input for your coefficient." << std::endl;
            }
            catch (const std::out_of_range& err){
                std::cout << "Coefficient out of range" << std::endl;
            }
        }
        validInput = false;
        while (!validInput){
            try {
                std::cout << "Enter exponent of new term" << std::endl;
                std::getline(std::cin, expStr);
                dExp = stod(expStr);
                if (static_cast<int> (dExp + .5) != dExp){
                    std::cout << "Rounding " << dExp << " to " << static_cast<int> (dExp + .5) << std::endl;
                }
                validInput = true;
                
            }
            catch (const std::invalid_argument& err){
                std::cout << "Invalid input for your exponent." << std::endl;
            }
            catch (const std::out_of_range& err){
                std::cout << "Exponent out of range" << std::endl;
            }
        }

        
        
        
        poly.addToList(static_cast<int> (dExp + .5), static_cast<int> (dCoeff + .5));

        moreTerms = "";
        while (moreTerms.compare("Y") && moreTerms.compare("y") && moreTerms.compare("N") && moreTerms.compare("n")){
            if (moreTerms == ""){
                std::cout << "Would you like to add another term? Y/N" << std::endl;
            }
            else {
                std::cout << "Invalid input. Please enter 'Y' or 'N' only" << std::endl;
            }
            std::getline(std::cin, moreTerms);
        }
    }
    
    std::cout << "Enter a name for your polynomial" << std::endl;
    std::string polyName;
    std::getline(std::cin, polyName);
    polyName = polyName.substr(0, polyName.find(" "));
    savePolynomial(poly, polyName);
    std::cout << "Saving polynomial " << poly.print() << " as \"" << polyName << "\"" << std::endl;
    
}

//asks user if he/she wants to save their polynomial that they created
void askToSavePolynomial(Polynomial &poly){
    std::string userCommand;

    std::cout << "Save polynomial? Y/N" << std::endl;
    std::getline(std::cin, userCommand);
    
    userCommand = userCommand.substr(0, userCommand.find(" "));
    
    while (userCommand != "Y" && userCommand != "y" && userCommand != "N" && userCommand != "n"){
        std::getline(std::cin, userCommand);
        userCommand = userCommand.substr(0, userCommand.find(" "));
        std::cout << "Invalid input. Please enter 'Y' or 'N' only" << std::endl;
    }
    if (userCommand == "Y" || userCommand == "y"){
        std::cout << "Enter a name for your polynomial:" << std::endl;

        std::string polyName;
        std::getline(std::cin, polyName);
        polyName = polyName.substr(0, polyName.find(" "));
        savePolynomial(poly, polyName);
    }
}

//displays all polynomials
void printAllPolynomials(){
    if (polyNames.size() == 0){
        std::cout << "No polynomials have been saved" << std::endl;
        return;
    }

    for (int i = 0; i < polyNames.size(); i++){
        std::cout << polyNames[i] << ": " << polynomials[i].print() << std::endl;
    }
}

//returns the polynomial with the given name
Polynomial getPolynomial(const std::string &polyName){
    for (int i = 0; i < polyNames.size(); i++){
        if (polyNames[i] == polyName){
            return polynomials[i];
        }
    }
    return NULL;
}

//users wants to do an operation
void operate(std::string operateLine){
    
    Polynomial resPoly;
    std::string args[MAX_ARGS];
    args[0] = operateLine.substr(0, operateLine.find(" "));
    
    int nextSpacePos = operateLine.find(" ",operateLine.find(" ") + 1);
    
    args[1] = operateLine.substr(operateLine.find(" ") + 1, nextSpacePos - operateLine.find(" ") - 1);
    
    double dPow;
    int pow;
    

    int prevSpacePos = nextSpacePos;
    nextSpacePos = operateLine.find(" ",operateLine.find(" ", prevSpacePos) + 1);
    args[2] = operateLine.substr(operateLine.find(" ", prevSpacePos) + 1, nextSpacePos - operateLine.find(" "));
    
    bool validInput = true;
    if (getPolynomialIndex(args[0]) == -1){
        std::cout << "Polynomial " << args[0] << " not found" << std::endl;
        validInput = false;
    }
    else if (args[1].compare("**") && getPolynomialIndex(args[2]) == -1){
        std::cout << "Polynomial " << args[2] << " not found" << std::endl;
        validInput = false;
    }
    if (!validInput){
        return;
    }
    

    if (!args[1].compare("+")){
        resPoly = getPolynomial(args[0]) + getPolynomial(args[2]);
    }
    else if (!args[1].compare("-")){
        resPoly = getPolynomial(args[0]) - getPolynomial(args[2]);
    }
    else if (!args[1].compare("*")){
        resPoly = getPolynomial(args[0]) * getPolynomial(args[2]);
    }
    else if (!args[1].compare("%")){
        validInput = resPoly.modulo(getPolynomial(args[0]), getPolynomial(args[2]));
        if (!validInput){
            return;
        }
    }
    else if (!args[1].compare("**")){
        try {
            dPow = stod(args[2]);
            
            if (dPow < 0){
                std::cout << "Exponent must be positive" << std::endl;
                return;
            }
            
            pow = static_cast<int> (dPow + 0.5); //debug
            
            resPoly = getPolynomial(args[0]).exponent(pow);
        }
        catch (const std::invalid_argument& err){
            std::cout << "Invalid input for your exponent" << std::endl;
            return;
        }
        catch (const std::out_of_range& err){
            std::cout << "Exponent out of range" << std::endl;
            return;
        }
        
        
    }
    else {
        std::cout << "Invalid input." << std::endl;
        std::cout << "Correct usage:" << usage("operate") << std::endl;
        return;
    }

    std::cout << resPoly.print() << std::endl;
    askToSavePolynomial(resPoly);
}

//controls the entire program
//displays the UI and takes commands
void runUserInput(){
    std::string args[MAX_ARGS];

    std::string userInput;
    std::string userCommand;
    std::string argsStr;
    while (true){
        printInterface();
        
        std::getline(std::cin, userInput);
        userCommand = userInput.substr(0, userInput.find(" "));
        argsStr = userInput.substr(userInput.find(" ") + 1);

        if (!userCommand.compare("new")){
            getNewPolynomial();
        }
        else if (!userCommand.compare("=")){
            operate(argsStr);
        }
        else if (!userCommand.compare("eval")){
            args[0] = argsStr.substr(0, argsStr.find(" "));
            
            int nextSpacePos = argsStr.find(" ",argsStr.find(" ") + 1);
            
            args[1] = argsStr.substr(argsStr.find(" ") + 1, nextSpacePos - argsStr.find(" ") - 1);

            int ind = getPolynomialIndex(args[0]);
            if (ind == -1){
                std::cout << "Polynomial " << args[0] << " does not exist" << std::endl;
            }
            else {
                double x;
                try {
                    x = stod(args[1]);
                    std::cout << polynomials[ind].evaluate(x) << std::endl;
                }
                catch (const std::invalid_argument& err){
                    std::cout << "Invalid input for your number" << std::endl;
                }
                catch (const std::out_of_range& err){
                    std::cout << "Number out of range" << std::endl;
                }
                
            }
        }
        else if (!userCommand.compare("print")){
            
            args[0] = argsStr.substr(0, argsStr.find(" "));

            int ind = getPolynomialIndex(args[0]);

            if (ind == -1){
                std::cout << "Polynomial " << args[0] << " does not exist" << std::endl;
            }
            else {
                std::cout << polynomials[ind].print() << std::endl;
            }

        }
        else if (!userCommand.compare("ls")){
            printAllPolynomials();
        }
        else if (!userCommand.compare("q")) {
            return;
        }
        else {
            std::cout << "Invalid input." << std::endl;
        }
    }
}

//runs program without asking user for input
//for testing only
//can input your exponents and coefficients in parallel arrays
void runNoUserInput(){
    int exp[] = {4,3,1,0};
    int coeff[] = {3,2,1,1};
    int exp2[] = {0};
    int coeff2[] = {1};


    std::vector<int> exps (exp, exp + sizeof(exp) / sizeof(exp[0]));
    std::vector<int> coeffs (coeff, coeff + sizeof(coeff) / sizeof(coeff[0]));


    std::vector<int> exps2 (exp2, exp2 + sizeof(exp2) / sizeof(exp2[0]));
    std::vector<int> coeffs2 (coeff2, coeff2 + sizeof(coeff2) / sizeof(coeff2[0]));

    Polynomial poly1(exps, coeffs);
    Polynomial poly2(exps2, coeffs2);

    std::cout << "poly1: ";
    std::cout << poly1.print()<< std::endl;

    std::cout << "poly2: ";
    std::cout << poly2.print()<< std::endl;

    Polynomial poly3 = poly1.exponent(0);

    std::cout << "poly3: ";
    std::cout <<  poly3.print()<< std::endl;

}

