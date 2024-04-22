//
// Created by Rafa on 12/04/2024.
//

#include "Rule.h"

Rule::Rule(std::string &rule, bool defeasible, std::string &id, class argumentBuilder* argumento): defeasible(defeasible), id(id), argumentBuilder(argumento) {
    int linePosition = rule.find("->");
    std::stringstream conditionals = std::stringstream(rule.substr(0,linePosition));
    std::string aux;
    while(std::getline(conditionals,aux,',')){
        bool negated = false;
        if(aux[0] == '~'){
            negated = true;
            aux = aux.substr(1,aux.length());
        }

        Formula* f = const_cast<Formula*> (&*(argumentBuilder->getKnowledgeBase().find(Formula(aux,negated))));
        conditions.insert(f);
        if(negated){
            negated = false;
        }
    }
    conditionals = std::stringstream(rule.substr(linePosition+2,rule.length()));
    while(std::getline(conditionals,aux,',')){
        bool negated = false;
        char isRule = aux[1];
        if(aux[0] == '~'){
            negated = true;
            aux = aux.substr(1,aux.length());
        }
        if(isRule != 'd') {
            Formula *f = const_cast<Formula *> (&*(argumentBuilder->getKnowledgeBase().find(Formula(aux, negated))));
            results.insert(f);
        }else{
            Rule* r;
            r = const_cast<Rule *>(&argumentBuilder->getDefeasibleRules()[std::stoi(aux.substr(1,aux.length()))]);

            for(auto formula : r->results){
                if(negated){
                    Formula f = *formula;
                    if(f.isNegated()){
                        f.setNegated(false);
                    }else{
                        f.setNegated(true);
                    }
                    results.insert(const_cast<Formula*>(&*argumentBuilder->getKnowledgeBase().find(f)));
                }else {
                    results.insert(formula);
                }
            }
        }
        if(negated){
            negated = false;
        }
    }
}

const std::unordered_set<Formula *> &Rule::getConditions() const {
    return conditions;
}

const std::unordered_set<Formula *> &Rule::getResults() const {
    return results;
}

std::ostream &operator<<(std::ostream &os, const Rule &rule) {
    os << "conditions: ";
    for(auto i : rule.conditions){
        os << i->getElement() << " ";
    }
    os << " results: ";
    for(auto i : rule.results){
        os << i->getElement() << " ";
    }
    os << " defeasible: " << rule.defeasible
       << " id: " << rule.id;
    return os;
}

bool Rule::isDefeasible() const {
    return defeasible;
}

bool Rule::operator==(const Rule &rhs) const {
    return id == rhs.id;
}

bool Rule::operator!=(const Rule &rhs) const {
    return !(rhs == *this);
}
