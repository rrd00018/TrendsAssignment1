//
// Created by Rafa on 12/04/2024.
//

#ifndef IA_ARGUMENTSOLVER_H
#define IA_ARGUMENTSOLVER_H


#include <vector>
#include <unordered_set>
#include <iostream>
#include <chrono>
using namespace std::chrono;

class argumentSolver {
private:


    std::vector<int> atoms;
    std::vector<std::vector<int>> relations;
    std::vector<std::unordered_set<int>> conflictFreeSets;
    std::vector<std::unordered_set<int>> admissibleSets;
    std::vector<std::unordered_set<int>> stableExtensions;
    std::vector<std::unordered_set<int>> preferredSemantics;
    std::unordered_set<int> currentSet;

public:
    argumentSolver(int argc, char **argv);
    void readArgs(char* argv[], int argc);
    void showData(const std::vector<int> &atoms, const std::vector<std::vector<int>> &relations);
    void showConflictFreeSets(const std::vector<std::unordered_set<int>>& conflictFreeSets);
    bool isConflictFree(const std::unordered_set<int> &currentSet,const std::vector<std::vector<int>> &relations);
    void conflictFree(const std::vector<int>& atoms, const std::vector<std::vector<int>>& relations, std::vector<std::unordered_set<int>>& conflictFreeSets, int paso, std::unordered_set<int>& currentSet);
    bool acceptable(int element, const std::unordered_set<int> &currentSet, const std::vector<std::vector<int>> &relations);
    void admissible(const std::vector<std::vector<int>>& relations, const std::vector<std::unordered_set<int>>& sets, std::vector<std::unordered_set<int>>& admissibleSets);
    void showAdmissibleSets(const std::vector<std::unordered_set<int>>& admissibleSets);
    bool attackedBySet(int element, const std::unordered_set<int>& set, const std::vector<std::vector<int>> &relations);
    void stable(const std::vector<int>& atoms, const std::vector<std::vector<int>>& relations, const std::vector<std::unordered_set<int>>& admissibleSets, std::vector<std::unordered_set<int>>& stableExtensions);
    void showStableExtensions(const std::vector<std::unordered_set<int>>& stableExtensions);
    bool preferred(const std::unordered_set<int>& set, const std::vector<std::unordered_set<int>>& admissibleSets);
    void preferredExtensions(const std::vector<std::unordered_set<int>>& admissibleSets, std::vector<std::unordered_set<int>> &preferredExtensions);
    void showPreferredExtensions(const std::vector<std::unordered_set<int>>& preferredExtensions);
    void run();

    virtual ~argumentSolver();
};


#endif //IA_ARGUMENTSOLVER_H
