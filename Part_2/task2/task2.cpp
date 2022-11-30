#include <iostream>
#include <fstream>
#include "MAPFInstance.h"
#include "AStarPlanner.h"
#include <tuple>

int main(int argc, char *argv[]) {
    MAPFInstance ins;
    string input_file = argv[1];
    string output_file = argv[2];
    if (ins.load_instance(input_file)) {
        ins.print_instance();
    } else {
        cout << "Fail to load the instance " << input_file << endl;
        exit(-1);
    }

    AStarPlanner a_star(ins);
    vector<Path> paths(ins.num_of_agents);

    // assign priority ordering to agents
    // By default, we use the index ordering of the agents where
    // the first always has the highest priority.
    list<int> priorities;
    for (int i = 0; i < ins.num_of_agents; i++) {
        priorities.push_back(i);
    }
    
    list<Constraint> constraints;
    // plan paths
  //  constraints.push_back(make_tuple(1, ins.goal_locations[1], -1, 2));
    for (int i : priorities) {
        // TODO: Transform already planned paths into constraints
        //  Replace the following line with something like paths[i] = a_star.find_path(i, constraints);
        paths[i] = a_star.find_path(i, constraints);
       // ins.my_map[ins.goal_locations[i]] = true;
        for (int j = i + 1; j < priorities.size(); j++)
        {
            for (int x = 0; x < paths[i].size(); x++)
            {
                if (paths[i][x] == ins.goal_locations[i])
                {
                    for(int g = x; g <= 10; g++)
                    {
                        list<int> adj = ins.get_adjacent_locations(paths[i][g]);
                        for (auto a : adj)
                        {
                            constraints.push_back(make_tuple(j, a, paths[i][x], g));
                        }
                        constraints.push_back(make_tuple(j, paths[i][x], -1, g));
                    }
                }
                else
                {
                    constraints.push_back(make_tuple(j, paths[i][x], -1, x));
                    if (paths[i][x] == ins.goal_locations[j])
                    {
                        for (int l = x - 1; l >= 0; l--)
                        {
                            constraints.push_back(make_tuple(j, paths[i][x], -1, l));
                        }
                    }
                    list<int> adj = ins.get_adjacent_locations(paths[i][x]);
                    for (auto a : adj)
                    {
                        constraints.push_back(make_tuple(j, a, paths[i][x], x));
                    }
                }
            }
        }
        if (paths[i].empty()) {
            cout << "Fail to find any solutions for agent " << i << endl;
            return 0;
        }
    }
   /* for (auto a : constraints)
    {
        cout << "agent: " << get<0>(a) << " from: " << get<1>(a) << " to: " << get<2>(a) << " time: " << get<3>(a) << endl;
    } */
    // print paths
    cout << "Paths:" << endl;
    int sum = 0;
    for (int i = 0; i < ins.num_of_agents; i++) {
        cout << "a" << i << ": " << paths[i] << endl;
        sum += (int)paths[i].size() - 1;
    }
    cout << "Sum of cost: " << sum << endl;

    // save paths
    ofstream myfile (output_file.c_str(), ios_base::out);
    if (myfile.is_open()) {
        for (int i = 0; i < ins.num_of_agents; i++) {
            myfile << paths[i] << endl;
        }
        myfile.close();
    } else {
        cout << "Fail to save the paths to " << output_file << endl;
        exit(-1);
    }
    return 0;
}
