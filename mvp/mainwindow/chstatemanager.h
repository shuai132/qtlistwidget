#ifndef CHSTATEMANAGER_H
#define CHSTATEMANAGER_H

#include <unordered_map>
#include "conf/config.h"
#include "maintypedef.h"

class ChStateManager
{
public:
    static ChStateManager* getInstance();

    void setState(chnum_t ch, ChState state);
    ChState getState(chnum_t ch);

private:
    ChStateManager();

private:
    std::unordered_map<chnum_t, ChState> _state;
};

#endif // CHSTATEMANAGER_H
