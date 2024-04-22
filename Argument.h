//
// Created by Rafa on 18/04/2024.
//

#ifndef IA_ARGUMENT_H
#define IA_ARGUMENT_H
#include <unordered_set>
#include <ostream>
#include "Formula.h"

class Rule;

static int idCounter = 0;

class Argument {
private:
    int id;
    std::unordered_set<Formula *> conclussions;
    std::unordered_set<Rule *> supportingRules;
    std::unordered_set<Argument *> supportingArguments;
    std::unordered_set<Argument *> attacks;

public:
    Argument(const std::unordered_set<Formula *> &premises);

    Argument(const std::unordered_set<Formula *> &premises, const std::unordered_set<Rule *> &supportingRules,
             const std::unordered_set<Argument *> &supportingArguments);

    Argument(const std::unordered_set<Formula *> &premises, const std::unordered_set<Argument *> &supportingArguments);

    bool operator==(const Argument &rhs) const;

    bool operator!=(const Argument &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Argument &argument);

    virtual ~Argument();

    const std::unordered_set<Formula *> &getConclusion() const;

    void insertAttack(Argument* a);

    int getId() const;

    const std::unordered_set<Argument *> &getSupportingArguments() const;

    const std::unordered_set<Rule *> &getSupportingRules() const;

    struct ArgumentHash {
        std::size_t operator()(const Argument &arg) const {
            // Combina los hash de los punteros de premises, supportingRules y supportingArguments
            std::size_t premisesHash = 0;
            for (const auto &premise: arg.conclussions) {
                premisesHash ^= std::hash<Formula *>{}(premise);
            }

            std::size_t rulesHash = 0;
            for (const auto &rule: arg.supportingRules) {
                rulesHash ^= std::hash<Rule *>{}(rule);
            }

            std::size_t argumentsHash = 0;
            for (const auto &argument: arg.supportingArguments) {
                argumentsHash ^= std::hash<Argument *>{}(argument);
            }

            // Combinar los hash
            std::size_t combinedHash = premisesHash ^ (rulesHash << 1) ^ (argumentsHash << 2);
            return combinedHash;
        }
    };
};


#endif //IA_ARGUMENT_H
