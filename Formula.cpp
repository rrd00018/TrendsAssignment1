//
// Created by Rafa on 17/04/2024.
//

#include "Formula.h"

Formula::Formula(const std::string &element, bool negated) : element(element), negated(negated) {}

Formula::~Formula() {

}

const std::string &Formula::getElement() const {
    return element;
}

bool Formula::isNegated() const {
    return negated;
}

void Formula::setNegated(bool negated) {
    Formula::negated = negated;
}

bool Formula::operator==(const Formula &rhs) const {
    return element == rhs.element &&
           negated == rhs.negated;
}

bool Formula::operator!=(const Formula &rhs) const {
    return !(rhs == *this);
}

std::ostream &operator<<(std::ostream &os, const Formula &formula) {
    os << "element: ";
    if(formula.negated){
        os << "~";
    }
    os << formula.element;
    return os;
}

bool Formula::opossed(const Formula &rhs) const {
    return this->element == rhs.element && this->negated != rhs.negated;
}