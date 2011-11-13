#include "Enums.hpp"

Party OppositeParty(Party p) {
    return p == GOOD ? EVIL : GOOD;
}
