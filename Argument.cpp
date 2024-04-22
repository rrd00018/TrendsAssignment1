//
// Created by Rafa on 18/04/2024.
//

#include "Argument.h"

Argument::Argument(const std::unordered_set<Formula *> &premises) : conclussions(premises){
    id = idCounter++;
}

bool Argument::operator==(const Argument &rhs) const {
    return conclussions == rhs.conclussions &&
           supportingRules == rhs.supportingRules &&
           supportingArguments == rhs.supportingArguments;
}

bool Argument::operator!=(const Argument &rhs) const {
    return !(rhs == *this);
}

Argument::~Argument() {}

const std::unordered_set<Formula *> &Argument::getConclusion() const {
    return conclussions;
}

Argument::Argument(const std::unordered_set<Formula *> &premises, const std::unordered_set<Rule *> &supportingRules,
                   const std::unordered_set<Argument *> &supportingArguments) : conclussions(premises),
                                                                                supportingRules(supportingRules),
                                                                                supportingArguments(
                                                                                        supportingArguments) {
    id = idCounter++;
}

Argument::Argument(const std::unordered_set<Formula *> &premises,
                   const std::unordered_set<Argument *> &supportingArguments) : conclussions(premises), supportingArguments(
        supportingArguments) {
    id = idCounter++;
}

std::ostream &operator<<(std::ostream &os, const Argument &argument){
    os << "Id: " << argument.id;
    os << " Conclussions: ";
    for(auto i : argument.conclussions){
        os << *i << " ";
    }
    os << " supportingRules: ";
    for(auto i : argument.supportingRules){
        os << i << " ";
    }
    os << " supportingArguments: ";
    for(auto i : argument.supportingArguments){
        os << i << " ";
    }
    os << " attacks: " << argument.attacks.size();
    return os;
}

void Argument::insertAttack(Argument *a) {
    attacks.insert(a);
}

int Argument::getId() const {
    return id;
}



const std::unordered_set<Argument *> &Argument::getSupportingArguments() const {
    return supportingArguments;
}

const std::unordered_set<Rule *> &Argument::getSupportingRules() const {
    return supportingRules;
}
