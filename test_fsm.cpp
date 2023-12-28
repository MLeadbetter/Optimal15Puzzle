#include "gtest/gtest.h"

#include <unordered_set>
#include <string>

#include "fsm.h"

using namespace std;

TEST(fsm, test_num_states) {
    FSM fsm;
    unordered_set<string> invalidStrings;
    invalidStrings.insert("ud");
    invalidStrings.insert("du");
    invalidStrings.insert("rl");
    invalidStrings.insert("lr");
    vector<State> states = fsm.generateFSM(invalidStrings);
    EXPECT_EQ(5, states.size());
}

TEST(fsm, test_num_states_longer) {
    FSM fsm;
    unordered_set<string> invalidStrings;
    invalidStrings.insert("ud");
    invalidStrings.insert("du");
    invalidStrings.insert("rl");
    invalidStrings.insert("lr");
    invalidStrings.insert("uldruld");
    vector<State> states = fsm.generateFSM(invalidStrings);
    EXPECT_EQ(10, states.size());
}

TEST(fsm, test_num_states_longer2) {
    FSM fsm;
    unordered_set<string> invalidStrings;
    invalidStrings.insert("ud");
    invalidStrings.insert("du");
    invalidStrings.insert("rl");
    invalidStrings.insert("lr");
    invalidStrings.insert("uldruld");
    invalidStrings.insert("druldru");
    vector<State> states = fsm.generateFSM(invalidStrings);
    EXPECT_EQ(15, states.size());
}
