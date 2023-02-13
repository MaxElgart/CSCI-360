#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sstream>
#include <vector>
#include <cmath>
#include <float.h>
#include <limits.h>
#include <random>

using namespace std;
#define g(x) (1.0/(1.0+exp(-x)))
#define gprime(x) (g(x)*(1-g(x))) 

// Print out progress bar
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void printProgress(double percentage, string info) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("%s \r%3d%% [%.*s%*s] ", info.c_str(), val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

// Process words to match words in the vocabulary file
string ProcessWord(string s)
{
    string t;
    // Remove punctuation.
    for (int i = 0; i < s.size(); i++)
        if (!ispunct(s[i]))
            t += s[i];

    // Convert to lower case.
    std::transform(t.begin(), t.end(), t.begin(), ::tolower);
    
    return t;

}

// The feature we use is the number of occurances of a word in the file
vector<double> GetFeature(string filename, unordered_map<string, int> &word_to_idx)
{
    /*
    Args:
    - filename: path name to the data file
    - word_to_idx: a mapping where a word maps to a preceptron weight's index.
    */

    // Open file.
    ifstream in;
    in.open(filename.c_str());
    if (!in.is_open())
    {
        cerr << "File not found: " << filename << endl;
        exit(1);
    }
    
    // TODO: obtain feature
    // ---- START ENTERING CODE ---- //
    string word = "";
    vector<double> feature_count(word_to_idx.size(), 0);
    while(in >> word)
    {
        word = ProcessWord(word);
        if (word_to_idx.find(word) != word_to_idx.end())
        {
            feature_count[word_to_idx[word]] += 1;
        }
    }
    // ---- STOP ENTERING CODE ---- //
    
    return feature_count;
}

void save_trained_weights(vector<double> &weight)
{
    ofstream output_file("trained_weights");
    ostream_iterator<double> output_iterator(output_file, "\n");
    copy(weight.begin(), weight.end(), output_iterator);
    output_file.close();
}

int main()
{       
    // Open vocabulary file
    ifstream dict_file;
    dict_file.open("imdb.vocab");
    if (!dict_file.is_open())
    {
        cerr << "Dictionary not found." << endl;
        exit(1);
    }

    // TODO: Create word to weight index map dictionary based on the imdb.vocab
    // ---- START ENTERING CODE ---- //
    string dict_word = "";
    int i = 0;
    unordered_map<string, int> word_weight;
    while (dict_file >> dict_word)
    {
        word_weight.insert(std::pair<string, int>(dict_word, i));
        i += 1;
    }
    // ---- STOP ENTERING CODE ---- //

    dict_file.close();

    // initialize accuracy output file
    ofstream output_acc_file;
    output_acc_file.open("accuracy.txt");

    // TODO: Initialize perceptron weights and training parameters. 
    // Hint: check parameters in the AND perceptron code example
    // ---- START ENTERING CODE ---- //
    srand(time(0));
    vector<double> weights;
    for (unordered_map<string, int>::iterator it = word_weight.begin(); it != word_weight.end(); it++)
    {
        int randomBit = rand() % 2;
        double r = ((double) rand() / (RAND_MAX)) * pow(-1, randomBit);
        weights.push_back(r);
    }
  
    /*ifstream train_file;
    train_file.open("training_list");
    int training_examples = 0;
    string filename = "";
    while(getline(train_file, filename))
    {
        training_examples += 1;
    }
    int features = word_weight.size();
    train_file.close();*/
    // ---- STOP ENTERING CODE ---- //
    float alpha = 1; 
    int total_epoch = 20;

    // Train the weights
    for (int epoch = 0; epoch < total_epoch; ++epoch) 
    {   
        // Read in train_list
        string file_name;
        string delimiter = "\t";
        ifstream train_file;
        train_file.open("training_list");
        float line_count = 0.0;
        cout << "Epoch " << epoch;

        // TODO: Update perceptron weights based on each data instance
        while (getline(train_file, file_name))
        {   
            // Hints (not required to follow): 
            // (1) retrieve the data and class of the data 
            // (2) obtain feature 
            // (3) update weights
            // ---- START ENTERING CODE ---- //
            int ex_class = 0;
            
            int index_class = file_name.find("\t") + 1;
            
            if (index_class != std::string::npos)
            {
                if (file_name[index_class] == '1')
                {
                    ex_class = 1;
                }
                else
                {
                    ex_class = 0;
                }
                file_name = file_name.substr(0, index_class - 1);
            }
            
            vector<double> features = GetFeature(file_name, word_weight);
            
            double weighted_sum = 0;
            for (int i = 0; i < features.size(); i++)
            {
                weighted_sum += features[i] * weights[i];
            }
            
            for (int i = 0; i < features.size(); i++)
            {
                weights[i] -= alpha * (g(weighted_sum) - ex_class) * gprime(weighted_sum) * features[i];
            }
            // ---- STOP ENTERING CODE ---- //

            // Output current progress on training with progress bar
            line_count += 1;
            printProgress(line_count/10000, "Epoch "+ to_string(epoch));
        } 
        train_file.close();
        cout << endl;

        // Save trained weights
        save_trained_weights(weights);
        
        // Evaluate trained weights with test data
        ifstream test_file;
        test_file.open("test_list");
        float correct_pred = 0.0; // should update during evaluation
        line_count = 0.0;

        // TODO: Predict negative or positive review with current trained preceptron weights
        while (getline(test_file, file_name))
        {

            // Hint: similar to training, but without updating the weights. Should also compute prediction results.
            // ---- START ENTERING CODE ---- //
            int ex_class = 0;
            
            int index_class = file_name.find("\t") + 1;
            
            if (index_class != std::string::npos)
            {
                if (file_name[index_class] == '1')
                {
                    ex_class = 1;
                }
                else
                {
                    ex_class = 0;
                }
                file_name = file_name.substr(0, index_class - 1);
            }
            
            vector<double> features = GetFeature(file_name, word_weight);
            
            double weighted_sum = 0;
            for (int i = 0; i < features.size(); i++)
            {
                weighted_sum += features[i] * weights[i];
            }
            double prediction = g(weighted_sum);
            // ---- STOP ENTERING CODE ---- //


            // TODO: Check if the prediction is correct. If so, increase correct_pred by one.
            // ---- START ENTERING CODE ---- //
            if ((prediction > .5 && ex_class == 1) || (prediction <= .5 && ex_class == 0))
            {
                correct_pred += 1;
            }
            // ---- STOP ENTERING CODE ---- //

            // Output current progress on evaluation with progress bar
            line_count += 1.0;
            printProgress(line_count/1000, "Evaluating...");
        } 
        test_file.close();

        // Compute prediction accuracy
        float acc = (correct_pred/line_count)*100.0;
        printf("Accuracy: %.2f \n", acc);
        output_acc_file << "Epoch " << epoch << ": Accuracy = " << acc << "%" << endl;
    }
    output_acc_file.close();
}
