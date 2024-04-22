//
// Created by Rafa on 17/04/2024.
//

#ifndef IA_FORMULA_H
#define IA_FORMULA_H


#include <string>
#include <iostream>

class Formula {
private:
    std::string element;
    bool negated;

public:
    Formula(const std::string &element, bool negated);

    virtual ~Formula();

    const std::string &getElement() const;

    bool isNegated() const;

    void setNegated(bool negated);

    bool operator==(const Formula &rhs) const;

    bool operator!=(const Formula &rhs) const;

    friend std::ostream &operator<<(std::ostream &os, const Formula &formula);

    bool opossed(const Formula&rhs) const;

    // Función hash para usar con unordered_set
    struct Hash {
        size_t operator()(const Formula& f) const {
            return std::hash<std::string>()(f.element) ^ std::hash<bool>()(f.negated);
        }
    };
};


#endif //IA_FORMULA_H
