//
// Created by Rafa on 12/04/2024.
//

#include "argumentBuilder.h"
#include "Rule.h"

argumentBuilder::argumentBuilder() {

}

argumentBuilder::~argumentBuilder() {
}

void argumentBuilder::loadData(std::string path) {
    std::ifstream file(path);
    std::string line;
    int defeasible = 0;
    int strict = 0;
    try {
        if (file.is_open()) {
            while (getline(file, line)) {
                int primerCorchete = line.find('{');//Looks for the first { in order to know the position where the elements of a set start
                switch (line[0]) {
                    case 'L': {
                        std::string clear = line.substr(primerCorchete+1,line.length());
                        clear = clear.substr(0,clear.length()-1);
                        std::stringstream elements(clear);
                        std::string aux;
                        while(std::getline(elements,aux,',')){
                            bool negated = false;
                            Formula f(aux,true);
                            Formula f1(aux,false);
                            knowledgeBase.insert(f1);
                            knowledgeBase.insert(f);
                        }
                        break;
                    }
                    case 'D': {
                        std::string lineWithoutD = line.substr(2,line.length());
                        std::string id = "d " + std::to_string(defeasible);
                        defeasible++;
                        Rule regla(lineWithoutD, true, id,this);
                        defeasibleRules.push_back(regla);
                        break;
                    }
                    case 'S': {
                        std::string lineWithoutS = line.substr(2,line.length());
                        std::string id = "s" + strict;
                        Rule regla(lineWithoutS, false,id,this);
                        topRules.push_back(regla);
                        break;
                    }
                    case 'K': {
                        char id = line[1];
                        std::string clear = line.substr(primerCorchete+1,line.length());
                        clear = clear.substr(0,clear.length()-1);
                        std::stringstream elements(clear);
                        std::string aux;
                        while(std::getline(elements,aux,',')){
                            bool negated = false;
                            if(aux[0] == '~'){
                                negated = true;
                                aux = aux.substr(1,aux.length());
                            }
                            Formula* f = const_cast<Formula*> (&*(knowledgeBase.find(Formula(aux,negated))));
                            if (id == 'n') {
                                premises.insert(f);
                            } else {
                                axioms.insert(f);
                            }
                        }
                    }
                }
            }
        } else {
            std::cerr << "Impossible to read input file" << std::endl;
        }


    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
}

const std::unordered_set<Formula, Formula::Hash> &argumentBuilder::getKnowledgeBase() const {
    return knowledgeBase;
}

void argumentBuilder::run() {
    for(auto i : premises){ //All of the premises are arguments
        std::unordered_set<Formula*> set;
        set.insert(i);
        arguments.insert(Argument(set));
    }

    checkRules(true);
    checkRules(false);

    checkAttacks();

    for(auto i : arguments){
        std::cout << i << std::endl;
    }
    for(auto j: attacksAndReasons){
        std::cout << j << std::endl;
    }
}

/**
 * @brief Check the rules for constructing arguments
 * @param strict controls which rules are going to be checked, true if topRules, false if defeasibleRules
 */
void argumentBuilder::checkRules(bool strict) {
    std::vector<Rule>* set;
    if(strict){
        set = &topRules;
    }else{
        set = &defeasibleRules;
    }
    for(auto i : *set){ //Recorro las rules
        bool found = true;
        std::unordered_set<Argument*> subArguments;
        std::unordered_set<Formula*> argumentPremises;
        std::unordered_set<Rule*> supportingRule;
        for(auto j : i.getConditions()){ //Si para cada condicion de las rules esta en lo conocido, la conclusion se mete en lo conocido
            if(axioms.find(j) != axioms.end()){
                argumentPremises.insert(j);
            }
            bool foundInArgument = false;
            for(auto k : arguments){
                if(k.getConclusion().find(j) != k.getConclusion().end()){
                    foundInArgument = true; //Encuentra
                    //addSupportingArgumentsRecursive(&k,subArguments);
                    subArguments.insert(&k);
                }
            }
            if(!foundInArgument){
                found = false;
            }else{
                found = true;
            }
        }
        if(found){ //Si se encuentra entonces se crea un argumento con eso
            supportingRule.insert(&i);
            for(auto element : i.getResults()){
                argumentPremises.insert(element);
            }
            Argument arg(argumentPremises,supportingRule,subArguments);
            arguments.insert(arg);
        }
    }
}

void argumentBuilder::checkAttacks() {
    for(auto argument : arguments){
        for(auto argument2 : arguments){
            if(argument != argument2){
                if(undercut(&argument,&argument2)){
                    argument.insertAttack(&argument2);
                    std::string atack = std::to_string(argument.getId()) + "," + std::to_string(argument2.getId()) + "," + std::to_string(1);
                    attacksAndReasons.push_back(atack);
                }else if(undermine(&argument,&argument2)){
                    argument.insertAttack(&argument2);
                    std::string atack = std::to_string(argument.getId()) + "," + std::to_string(argument2.getId()) + "," + std::to_string(2);
                    std::cout << atack << std::endl;
                    attacksAndReasons.push_back(atack);
                }else if(rebut(&argument,&argument2)){
                    argument.insertAttack(&argument2);
                    std::string atack = std::to_string(argument.getId()) + "," + std::to_string(argument2.getId()) + "," + std::to_string(3);
                    std::cout << atack << std::endl;
                    attacksAndReasons.push_back(atack);
                }
            }
        }
    }
}

void argumentBuilder::addSupportingArgumentsRecursive(Argument *arg, std::unordered_set<Argument *> &allSupportingArgs) {
    allSupportingArgs.insert(arg);
    if(!arg->getSupportingArguments().empty()) {
        for (Argument *supportingArg: arg->getSupportingArguments()) {
            // Check if the supporting argument has additional supporting arguments
            if (!supportingArg->getSupportingArguments().empty()) {
                // Recursively add supporting arguments of the supporting argument
                addSupportingArgumentsRecursive(supportingArg, allSupportingArgs);
            }
        }
    }
}

bool argumentBuilder::undermine(Argument *a1, Argument *a2) { //Ataco una de las premisas de a2
    for(auto conclusion : a1->getConclusion()){
        for(auto argRule : a2->getSupportingRules()){
            for(auto element : argRule->getConditions()){
                if(conclusion->opossed(*element)){
                    return true;
                }
            }
        }
    }
    return false;
}

bool argumentBuilder::undercut(Argument *a1, Argument *a2) { //Si una de las reglas
    for(auto a : a2->getSupportingRules()){
        if(a->isDefeasible()) {
            for (auto conclusion: a->getConditions()) {
                for (auto argument1Conclussion: a1->getConclusion()) {
                    if (conclusion->opossed(*argument1Conclussion)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool argumentBuilder::rebut(Argument *a1, Argument *a2) {
    for(Formula* conclusion : a1->getConclusion()){
        for(Rule* argRule : a2->getSupportingRules()){
            for(Formula* element : argRule->getResults()){
                if(conclusion->opossed(*element)){
                    return true;
                }
            }
        }
    }
    return false;
}

const std::vector<Rule> &argumentBuilder::getDefeasibleRules() const {
    return defeasibleRules;
}

const std::vector<Rule> &argumentBuilder::getTopRules() const {
    return topRules;
}
