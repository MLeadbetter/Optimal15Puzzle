#include <iostream>

#include "solver.h"
#include "fsm.h"

using namespace std;

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cerr << "Usage: generate_tables <depth>" << endl;
        return 1;
    }
    unsigned int depth = atoi(argv[1]);
    BackwardsSolver solver;
    std::unordered_set<std::string> redundant = solver.findRedundant(depth);
    FSM fsm;
    std::vector<State> fsmvec = fsm.generateFSM(redundant);
    fsm.saveFSM(string("fsm")+argv[1], fsmvec);
    cout << fsmvec.size() << " states in genereated FSM" << endl;
}
