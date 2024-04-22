//
// Created by Rafa on 12/04/2024.
//

#include "argumentSolver.h"

/**
 * @brief Read the data from the input in console when executing the program
 * @param atoms vector that contains the different atoms
 * @param relations vector of vectors containing the relations in the way of attacker, deffender
 * @param argv string with the input
 * @param argc how many inputs there are
 */
void argumentSolver::readArgs(char* argv[], int argc){
    for(int i = 0; i < argc; i++){
        std::string argumento = argv[i];
        if(argumento[0] == 'A')
            atoms.push_back(atoi(argv[i]+1));
        else if(argumento.substr(0,3) == "att"){
            int positionComa = argumento.find(',');
            int v1 = stoi(argumento.substr(4,positionComa - 4));
            int v2 = stoi(argumento.substr(positionComa + 1,argumento.length() - 1 - (positionComa + 1)));
            std::vector<int> pair;
            pair.push_back(v1);
            pair.push_back(v2);
            relations.push_back(pair);
        }
    }
}

/**
 * @brief print the atoms and relations
 * @param atoms vector containing the atoms
 * @param relations vector containing the relations
 */
void argumentSolver::showData(const std::vector<int> &atoms, const std::vector<std::vector<int>> &relations){
    std::cout << "ATOMS" << std::endl;
    for(int i = 0; i < atoms.size(); i++){
        std::cout << atoms[i] << std::endl;
    }

    std::cout << "RELATIONS" << std::endl;
    for(int i = 0; i < relations.size(); i++){
        std::cout << relations[i][0] << " -> " << relations[i][1] << std::endl;
    }
}

/**
 * @brief print the conflict free sets
 * @param conflictFreeSets vector of vectors with the sets
 */
void argumentSolver::showConflictFreeSets(const std::vector<std::unordered_set<int>>& conflictFreeSets){
    std::cout << "CONFLICT FREE SETS" << std::endl;
    for(int i = 0; i < conflictFreeSets.size(); i++){
        for(const auto& element : conflictFreeSets[i]){
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief check if a set is conflict free
 * @param currentSet unordered set to check. use unordered cause the insertion, search and delete are constant time
 * @param relations vector of vectors with the relations among the atoms in the way of attacker,defender
 * @return true or false if the set is conflict free or not
 */
bool argumentSolver::isConflictFree(const std::unordered_set<int> &currentSet,const std::vector<std::vector<int>> &relations){
    for(int i = 0; i < relations.size(); i++){
        if((currentSet.find(relations[i][0]) != currentSet.end()) && currentSet.find(relations[i][1]) != currentSet.end())
            return false;
    }
    return true;
}
//TODO meter el vacio
/**
 * @brief recursive function that calculates the conflict free sets of a logical program
 * @param atoms vector with all the atoms
 * @param relations vector of vectors with the relations among the atoms in the way of attacker,defender
 * @param conflictFreeSets vector of vectors each of one containing a different conflict free set
 * @param paso integer that allow the recursive and backtracking to work correctly
 * @param currentSet vector containing the set that is currently being constructed or analysed
 */
void argumentSolver::conflictFree(const std::vector<int>& atoms, const std::vector<std::vector<int>>& relations, std::vector<std::unordered_set<int>>& conflictFreeSets, int paso, std::unordered_set<int>& currentSet){
    if(isConflictFree(currentSet,relations) && paso >= 1){
        conflictFreeSets.push_back(currentSet);
    }
    for(int i = paso; i < atoms.size(); i++){
        currentSet.insert(atoms[i]);
        conflictFree(atoms,relations,conflictFreeSets,i+1,currentSet);
        currentSet.erase(atoms[i]);
    }
}

/**
 * @brief check if an element is acceptable in the current set -> if someone attacks him, it has to be defended by someone in the set
 * @param element element to check
 * @param currentSet set of atoms to check
 * @param relations vector of vectors with the relations among the atoms in the way of attacker,defender
 * @return true or false depending if the element is acceptable or not
 */
bool argumentSolver::acceptable(int element, const std::unordered_set<int> &currentSet, const std::vector<std::vector<int>> &relations){
    bool esAtacado = false;
    for(int i = 0; i < relations.size(); i++){
        if(element == relations[i][1]){
            esAtacado = true;
            int atacante = relations[i][0];
            for(int j = 0; j < relations.size(); j++){
                if(atacante == relations[j][1] &&  currentSet.find(relations[j][0]) != currentSet.end()){
                    return true;
                }
            }
        }
    }
    if(!esAtacado)
        return true;
    return false;
}

/**
 * @brief check if a set is admissible -> all their elements are acceptable among each other
 * @param relations vector of vectors with the relations among the atoms in the way of attacker,defender
 * @param sets vector of unordered_sets with all the conflict free sets
 * @param admissibleSets vector of unordered sets to save the solution
 */
void argumentSolver::admissible(const std::vector<std::vector<int>>& relations, const std::vector<std::unordered_set<int>>& sets, std::vector<std::unordered_set<int>>& admissibleSets){
    bool aceptable = true;
    for(int i = 0; i < sets.size(); i++){
        for(const auto& element: sets[i]){
            if(!acceptable(element,sets[i],relations)){
                aceptable = false;
                break;
            }
        }
        if(aceptable){
            admissibleSets.push_back(sets[i]);
        }else{
            aceptable = true;
        }
    }
}

/**
 * @brief print the admissibleSets
 * @param admissibleSets vector of unordered sets containing the admissible sets
 */
void argumentSolver::showAdmissibleSets(const std::vector<std::unordered_set<int>>& admissibleSets){
    std::cout << "ADMISSIBLE SETS " << std::endl;
    for(int i = 0; i < admissibleSets.size(); i++){
        for(auto &element : admissibleSets[i]){
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief check if a element is attacked by any element of the set
 * @param element element to check
 * @param set set that attacks the element
 * @param relations vector of vectors with the relations among the atoms in the way of attacker,defender
 * @return true or false if the element is attacked or not by the set
 */
bool argumentSolver::attackedBySet(int element, const std::unordered_set<int>& set, const std::vector<std::vector<int>> &relations){
    for(int i = 0; i < relations.size(); i++){
        if(relations[i][1] == element && set.find(relations[i][0]) != set.end()){
            return true;
        }
    }
    return false;
}

/**
 * @brief calculate the stable semantics of a framework -> a stable extension is a admissible set where each element out of it is attacked by a element in the set
 * @param atoms vector of atoms
 * @param relations vector of vectors with the relations among the atoms in the way of attacker,defender
 * @param admissibleSets vector of sets containing the admissible sets of the framework
 * @param stableExtensions vector of unordered sets used for storing the stable extensions
 */
void argumentSolver::stable(const std::vector<int>& atoms, const std::vector<std::vector<int>>& relations, const std::vector<std::unordered_set<int>>& admissibleSets, std::vector<std::unordered_set<int>>& stableExtensions){
    bool stable = true;
    for(int i = 0; i < admissibleSets.size(); i++){
        for(int j = 0; j < atoms.size(); j++){
            if(!(admissibleSets[i].find(atoms[j]) != admissibleSets[i].end())){ //Si el atomo no pertenece al conjunto hay q ver si es atacado por el
                if(!attackedBySet(atoms[j],admissibleSets[i],relations)){
                    stable = false;
                    break;
                }
            }
        }
        if(stable){
            stableExtensions.push_back(admissibleSets[i]);
        }else{
            stable = true;
        }
    }
}

/**
 * @brief print the stable extensions
 * @param stableExtensions vector of unordered sets used containing the stable extensions
 */
void argumentSolver::showStableExtensions(const std::vector<std::unordered_set<int>>& stableExtensions){
    std::cout << "STABLE EXTENSIONS " << std::endl;
    for(int i = 0; i < stableExtensions.size(); i++){
        for(auto &element : stableExtensions[i]){
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief calculates if a set is maximal of a set of sets -> it can't  be subset of any other
 * @param set unordered set to check
 * @param admissibleSets vector of unordered admissible sets (set of sets from the description)
 * @return true or false if the set is maximal
 */
bool argumentSolver::preferred(const std::unordered_set<int>& set, const std::vector<std::unordered_set<int>>& admissibleSets){
    int elementsInOtherSet = 0;
    for(int i = 0; i < admissibleSets.size(); i++){
        for(int element : set){
            if((admissibleSets[i].find(element) != admissibleSets[i].end())){ //Si encuentra el elemento en otro conjunto suma al contador
                elementsInOtherSet++;
            }
        }

        if(elementsInOtherSet == set.size() && set.size() != admissibleSets[i].size()){
            return false;
        }else{
            elementsInOtherSet = 0;
        }
    }
    return true;
}

/**
 * @brief calculates the preferred semantics
 * @param admissibleSets vector of sets containing the admissible sets
 * @param preferredExtensions vector of set that will contain the result
 */
void argumentSolver::preferredExtensions(const std::vector<std::unordered_set<int>>& admissibleSets, std::vector<std::unordered_set<int>> &preferredExtensions){
    for(int i = 0; i < admissibleSets.size(); i++){
        if(preferred(admissibleSets[i],admissibleSets)){
            preferredExtensions.push_back(admissibleSets[i]);
        }
    }
}

/**
 * @brief print the preferred extensions of the framework
 * @param preferredExtensions vector of unordered sets containing the preferred extensions
 */
void argumentSolver::showPreferredExtensions(const std::vector<std::unordered_set<int>>& preferredExtensions){
    std::cout << "PREFERRED SEMANTICS " << std::endl;
    for(int i = 0; i < preferredExtensions.size(); i++){
        for(auto &element : preferredExtensions[i]){
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
}

/**
 * @brief Calculate the conflict free sets, admissible sets, stable semantics, preferred semantics
 */
void argumentSolver::run() {

    //Calculo de conflict free sets.
    auto start = high_resolution_clock::now();
    conflictFree(atoms,relations,conflictFreeSets,0,currentSet);
    std::unordered_set<int> vacio;
    vacio.insert(0);
    conflictFreeSets.push_back(vacio);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    showConflictFreeSets(conflictFreeSets);

    std::cout << "Tiempo en calcular conflict free sets " << duration.count() << " ms" << std::endl;

    start = high_resolution_clock::now();
    admissible(relations,conflictFreeSets,admissibleSets);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);

    showAdmissibleSets(admissibleSets);

    std::cout << "Tiempo en calcular admissible sets " << duration.count() << " ms" << std::endl;

    start = high_resolution_clock::now();
    stable(atoms,relations,admissibleSets,stableExtensions);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);

    showStableExtensions(stableExtensions);

    std::cout << "Tiempo en calcular stable extensions " << duration.count() << " ms" << std::endl;

    start = high_resolution_clock::now();
    preferredExtensions(admissibleSets,preferredSemantics);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);

    showPreferredExtensions(preferredSemantics);

    std::cout << "Tiempo en calcular preferred semantics " << duration.count() << " ms" << std::endl;

}

/**
 * @brief Constructor of the solver
 * @param argc number of elements in the array
 * @param argv vector of atoms and relations
 */
argumentSolver::argumentSolver(int argc, char* argv[]) {
    readArgs(argv,argc);
    run();
}

argumentSolver::~argumentSolver() {

}
