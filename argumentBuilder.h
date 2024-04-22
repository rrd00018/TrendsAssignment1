//
// Created by Rafa on 12/04/2024.
//

#ifndef IA_ARGUMENTBUILDER_H
#define IA_ARGUMENTBUILDER_H


#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include "Formula.h"
#include "Argument.h"

class Rule;

class argumentBuilder {
private:
    std::unordered_set<Formula,Formula::Hash> knowledgeBase;
    std::unordered_set<Formula*> premises;
    std::unordered_set<Formula*> axioms;
    std::vector<Rule> defeasibleRules;
    std::vector<Rule> topRules;
    std::unordered_set<Argument,Argument::ArgumentHash> arguments;
    std::vector<std::string> attacksAndReasons;
    void checkRules(bool strict);
    void checkAttacks();
    void addSupportingArgumentsRecursive(Argument* arg, std::unordered_set<Argument*>& allSupportingArgs);
public:
    argumentBuilder();

    void run();

    const std::unordered_set<Formula, Formula::Hash> &getKnowledgeBase() const;

    virtual ~argumentBuilder();

    void loadData(std::string path);

    bool undercut(Argument* a1, Argument* a2);

    bool rebut(Argument* a1, Argument* a2);

    bool undermine(Argument* a1, Argument* a2);

    const std::vector<Rule> &getDefeasibleRules() const;

    const std::vector<Rule> &getTopRules() const;

};


#endif //IA_ARGUMENTBUILDER_H
