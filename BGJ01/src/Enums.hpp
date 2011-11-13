#ifndef _ENUMS
#define _ENUMS

enum Party {
    GOOD = 0x01,
    EVIL = 0x10
};

Party OppositeParty(Party p);

#endif
