#include "chstatemanager.h"
#include <cassert>

#define CHECK_CHNUM(ch) do{assert(ChNumMin <= ch && ch <= AllChMax);} while(0)

ChStateManager* ChStateManager::getInstance()
{
    static ChStateManager* instance = nullptr;
    if (instance == nullptr) {
        instance = new ChStateManager();
    }
    return instance;
}

void ChStateManager::setState(chnum_t ch, ChState state)
{
    CHECK_CHNUM(ch);
    _state[ch] = state;
}

ChState ChStateManager::getState(chnum_t ch)
{
    CHECK_CHNUM(ch);
    return _state.at(ch);
}

ChStateManager::ChStateManager()
{

}
