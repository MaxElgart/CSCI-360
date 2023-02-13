#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const string TRAIN_FILE = "input/train.dat";
const string TEST_FILE = "input/test.dat";
const string TREE_FILE = "output/tree.txt";
const string PREDICTION_FILE = "output/prediction.txt";

/* Decision tree node structure */
struct node {
    string split_feature;
    string classification;
    vector<int> labeled_examples;
    map<string, node *> children;
    set<string> features_left;
};

/* Read the file into a vector of vector of strings */
vector<vector<string>> read_file(string filename) {
    ifstream file(filename);
    vector<vector<string>> lines;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        vector<string> words;
        string word;
        while (iss >> word) {
            words.push_back(word);
        }
        lines.push_back(words);
    }
    return lines;
}

/* Print the lines which is a vector of vector of strings */
void print_lines(vector<vector<string>> &lines) {
    for (int i = 0; i < lines.size(); i++) {
        for (int j = 0; j < lines[i].size(); j++) {
            cout << lines[i][j] << " ";
        }
        cout << endl;
    }
}


/* Calculate and return the entropy of the labeled_examples */
double calculate_entropy(vector<int> &labeled_examples, map<string, vector<string>> &data_map) {
    /******************** A) Modify this function ********************/
    // TODO start typing your code here
    double entropy = 0;
    double yes_count = 0;
    double no_count = 0;
    double example_amount = labeled_examples.size();
    
    for (int i = 0; i < data_map["Prediction"].size(); i++)
    {
        if (std::find(labeled_examples.begin(), labeled_examples.end(), i) == labeled_examples.end())
        {
            continue;
        }
        
        if(data_map["Prediction"][i] == "yes")
        {
            yes_count += 1;
        }
        else
        {
            no_count += 1;
        }
    }
    
    if (yes_count != 0)
    {
        entropy -= (yes_count / example_amount) * (log2(yes_count / example_amount));
    }
    
    if (no_count != 0)
    {
        entropy -= (no_count / example_amount) * (log2(no_count / example_amount));
    }
    
    return entropy;
    /******************** End of A) Modification *****************/
}



/* Function to calculate information gain given parent entropy and split data
Return the information gain given parent entropy and split of the data wrt to a feature */
double calculate_information_gain(double parent_entropy, map<string, vector<int>> &split_data, double total, map<string, vector<string>> &data_map) {
    /********************* B) Modify this function *********************/
    // TODO start typing your code here
    double total_ent = 0;
    for(map<string, vector<int>>::iterator it = split_data.begin(); it != split_data.end(); it++)
    {
        total_ent += (it->second.size() / total) * calculate_entropy(it->second, data_map);
    }
    
    return parent_entropy - total_ent;
    /********************* End of B) Modification ********************/
}


/* Function to split the data based on the feature 
it returns split_data is a map from feature value to 
a vector of labeled examples that have that feature value */ 
map<string, vector<int>> split_on_feature(string feature, map<string, vector<string>> &data_map, vector<int> &labeled_examples) {
    map<string, vector<int>> split_data;
    for (int labeled_example : labeled_examples) {
        string feature_value = data_map[feature][labeled_example];
        split_data[feature_value].push_back(labeled_example);
    }
    return split_data;
}

/* Function to create a tree node */
node *create_node(string split_feature, string classification, vector<int> labeled_examples, set<string> features_left) {
    node *new_node = new node;
    new_node->split_feature = split_feature;
    new_node->classification = classification;
    new_node->labeled_examples = labeled_examples;
    new_node->features_left = features_left;
    return new_node;
}

/* Function to find if all the data points belong to the same class*/
bool all_same_classification(map<string, vector<string>> &data_map, vector<int> &labeled_examples) {
    string classification = data_map["Prediction"][labeled_examples[0]];
    for (int i = 1; i < labeled_examples.size(); i++) {
        if (classification != data_map["Prediction"][labeled_examples[i]]) {
            return false;
        }
    }
    return true;
}

/* Function to find the most common classification */
string get_most_common_classification(map<string, vector<string>> &data_map, vector<int> &labeled_examples) {
    int yes_count = 0;
    int no_count = 0;
    for (int i = 0; i < labeled_examples.size(); i++) {
        if (data_map["Prediction"][labeled_examples[i]] == "yes") {
            yes_count++;
        } else {
            no_count++;
        }
    }
    if (yes_count > no_count) {
        return "yes";
    } else {
        return "no";
    }
}

node *decision_tree(map<string, vector<string>> &data_map, set<string> features_left, vector<int> labeled_examples) {
    /******* Print the data points ********/
    // cout << "labeled examples: ";
    // for(int i = 0; i < labeled_examples.size(); i++){
    //     cout << labeled_examples[i] << " ";
    // }
    // cout << "\n\n" << endl;
    /*************************************/

    string most_common_classification = get_most_common_classification(data_map, labeled_examples);
    

    // when no more features left exit
    if (features_left.size() == 0) {
        return create_node("", most_common_classification, labeled_examples, features_left);
    }

    // if all the data points have the same classification return the classification
    if (all_same_classification(data_map, labeled_examples)) {
        return create_node("", data_map["Prediction"][labeled_examples[0]], labeled_examples, features_left);
    }
    
    double parent_entropy = calculate_entropy(labeled_examples, data_map);
    double best_information_gain = -100000;

    string best_feature = "Not Found";
    map<string, vector<int>> best_split_data;
    for (string feature : features_left) {
        map<string, vector<int>> split_data = split_on_feature(feature, data_map, labeled_examples);
        double information_gain = calculate_information_gain(parent_entropy, split_data, labeled_examples.size(), data_map);
        
        /********** Print the information gain of each feature **********/
        // cout<< "feature" << feature << "information gain" << information_gain << endl;
        /****************************************************************/
        
        if (information_gain > best_information_gain) {
            best_information_gain = information_gain;
            best_feature = feature;
            best_split_data = split_data;
        }
    }
    /*********** Print the best feature and its information gain ***********/
    // cout << "best_feature:" << best_feature << "with information gain" << best_information_gain << endl;
    /***********************************************************************/


    /******************** Print best split data  ****************************/
    //  for(map<string, vector<int> >::iterator it = best_split_data.begin(); it != best_split_data.end(); it++){
    //      cout << it->first << ": ";
    //      for(int i = 0; i < it->second.size(); i++){
    //          cout << it->second[i] << " ";
    //      }
    //      cout << endl;
    //  }
    //  cout << "\n\n\n" << endl;
    /************************************************************************/

    // Remove feature that we just split the data by
    features_left.erase(best_feature);
    node *new_node = create_node(best_feature, most_common_classification, labeled_examples, features_left);

    // for each possible value of the feature to split on, create a child node.
    for (string feature_value : data_map[best_feature + "_values"]) {
        vector<int> &child_data_points = best_split_data[feature_value];

        // if there are no more data points left, then return the most common classification
        if (child_data_points.size() == 0) {
            new_node->children[feature_value] = create_node("", most_common_classification, labeled_examples, features_left);
        } else {
            new_node->children[feature_value] = decision_tree(data_map, features_left, child_data_points);
        }
    }
    return new_node;
}

/* Function to traverse the tree using dfs and print the tree */
void print_dfs(node *root, map<string, vector<string>> &data_map, ofstream &file, int depth = 0) {
    if (root == NULL) {
        return;
    }

    string indent(4 * depth, ' ');

    string node_label = root->split_feature.size() ? root->split_feature : root->classification;

    file << indent << node_label << ": ";
    for (int labeled_example : root->labeled_examples) {
        file << labeled_example << " ";
    }
    file << endl;

    for (auto it : root->children) {
        const string &feature_value = it.first;
        node *child = it.second;

        file << indent << "- " << feature_value << ": " << endl;
        print_dfs(child, data_map, file, depth + 1);
    }
}

/* Function to process the possible values for each feature*/
vector<string> split_on_slash(string line) {
    vector<string> split_line;
    string temp = "";
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == '/') {
            split_line.push_back(temp);
            temp = "";
        } else {
            temp += line[i];
        }
    }
    split_line.push_back(temp);
    return split_line;
}

/* Function that takes in each line of the data file and returns a map of the data
The returned map has the feature name as the key and a vector of the values as the value
The map also has keys of this format "<feature_name>_value" whose value stores a vector of possible values for each feature */
map<string, vector<string>> create_data_map(vector<vector<string>> &lines) {
    map<string, vector<string>> data_map;
    int columns = lines[0].size();
    int rows = lines.size();

    // process data column wise
    for (int j = 0; j < columns; j++) {
        string feature = lines[0][j];
        vector<string> possible_feature_values = split_on_slash(lines[1][j]);
        data_map[feature + "_values"] = possible_feature_values;

        for (int i = 2; i < rows; i++) {
            string feature_value = lines[i][j];

            // If feature has a value not specified in the list of possible values provided earlier, it's invalid.
            if (find(possible_feature_values.begin(), possible_feature_values.end(), feature_value) == possible_feature_values.end()) {
                cout << "Line:" << i + 1 << " Invalid feature value: " << feature_value << " for feature: " << feature << endl;
                exit(1);
            }

            data_map[feature].push_back(feature_value);
        }
    }
    return data_map;
}

int main() {
    vector<vector<string>> lines = read_file(TRAIN_FILE);

    /************* Print the train data *************/
    // print_lines(lines);
    /************************************************/
    
    // Build a map using the first row as the key
    map<string, vector<string>> data_map = create_data_map(lines);

    /********* Print the data map *********/
   /* for(map<string, vector<string> >::iterator it = data_map.begin(); it != data_map.end(); it++){
         cout << it->first << ": ";
         for(int i = 0; i < it->second.size(); i++){
             cout << it->second[i] << " ";
         }
         cout << endl;
     } */
    /**************************************/

    // Build a set of features left from the first row, except for the last column (Prediction)
    set<string> features_left(lines[0].begin(), lines[0].end() - 1);

    // Create a vector of data points, initially equal to all the data points
    vector<int> labeled_examples;
    for (int i = 0; i < lines.size() - 2; i++) {
        labeled_examples.push_back(i);
    }

    // Build the decision tree
    node *root = decision_tree(data_map, features_left, labeled_examples);

    // Write the tree into a file
    ofstream tree_file(TREE_FILE);

    tree_file << "+++++++ DFS of Decision Tree: ++++++++\n"
              << endl;
    print_dfs(root, data_map, tree_file);
    tree_file << "\n++++++++++++++++++++++++++++++++++++\n"
              << endl;

    tree_file.close();

    vector<vector<string>> test_lines = read_file(TEST_FILE);
    
    /******** Print the test data ********/
    //print_file(lines);
    /*************************************/

    // loop through test_lines vectors and print the prediction
    map<string, vector<string>> test_data_map = create_data_map(test_lines);

    ofstream prediction_file(PREDICTION_FILE);

    for (int i = 0; i < test_lines.size() - 2; i++) {
        node *current_node = root;
        while (current_node->children.size() != 0) {
            string feature = current_node->split_feature;
            string feature_value = test_data_map[feature][i];
            current_node = current_node->children[feature_value];
        }
        prediction_file << "Prediction for unlabeled example #" << i << ": " << current_node->classification << endl;
    }

    prediction_file.close();

    return 0;
}
