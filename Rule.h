//
// Created by Rafa on 12/04/2024.
//

#ifndef IA_RULE_H
#define IA_RULE_H


#include <string>
#include <iostream>
#include <unordered_set>
#include <sstream>
#include "Formula.h"
#include "argumentBuilder.h"

class Rule {
private:
    std::unordered_set<Formula*> conditions;
    std::unordered_set<Formula*> results;
    bool defeasible;
    std::string id;
    argumentBuilder* argumentBuilder;
public:
    Rule(std::string& rule, bool defeasible, std::string& id, class argumentBuilder* argumento);

    const std::unordered_set<Formula *> &getConditions() const;

    const std::unordered_set<Formula *> &getResults() const;

    friend std::ostream &operator<<(std::ostream &os, const Rule &rule);

    bool operator==(const Rule &rhs) const;

    bool operator!=(const Rule &rhs) const;

    bool isDefeasible() const;
};


#endif //IA_RULE_H
