#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;


//Note:
//Code for BubbleSort copied from GeeksForGeeks
//The 2 functions make_sets() and print_sets() were written by Muhammad Taimoor, a classmate.



// ****** NOTE: MAKE CHANGES TO THE PIECE OF CODE LABELLED CHANGE PORTION (CONTAINS THE INPUTS)*********





//*******************CHANGE PORTION STARTS****************************************

const vector<string> items = { "Bottle", "Hairbrush", "Jacket", "Socks", "Towel", "Cap", "Phone","Laptop", "Charger", "Handsfree", "Racket"}; //names of items
const vector<int> weights = { 3 ,2 ,4, 1, 3, 2, 4, 5,3, 2, 4 }; //weights of items
const vector<int> values = { 4, 2, 3, 2, 4, 4, 5,6, 4, 5, 6 };  //values of items
const int weight_limit = 20;     //weight limit of backpack
const int S = 4;                 //this represents (reference set size)/2

//*******************CHANGE PORTION ENDS*******************************************


int Total_Items = items.size();          //total number of items

vector<vector<int>> P;           //Initial Diverse set of solutions


//vector<vector<int>> PRset = { {1,0,0,0,1,1}, {1,1,1,0,0,0}, {0,1,0,0,1,0}, {1,1,1,1,1,1} };

bool terminateAlg = false;

//vector<vector<int>> Pset;



void Diversification(int number) {

    srand(time(NULL));

    for (int i = 0; i < number; i++) {
        vector<int> sol;
        for (int i = 0; i < Total_Items; i++) {
            int digit = rand() % 2;
            sol.push_back(digit);

        }
        P.push_back(sol);
    }
}




//Solutions with their fitness value
struct sol_with_fitness {
    vector<int> solution;
    int fitness = 0;
};

//Solutions with their Diversity Value
struct sol_with_diversity {
    vector<int> solution;
    int diversity = 0;
};


//gets fittest solution from a set of solutions
int get_most_fit(vector<sol_with_fitness>& v)
{
    int max = -1, index = 0;
    for (size_t i = 0; i < v.size(); i++)
    {
        if (v[i].fitness > max)
        {
            max = v[i].fitness;
            index = i;
        }
    }
    return index;
}



//a function that qualifies or disqualifies a solution basedon its weight
bool check_weight(const vector<int>& sol) {
    int weight = 0;
    for (int i = 0; i < sol.size(); i++) {
        if (sol[i]) {
            weight += weights[i];
        }
    }
    if (weight > weight_limit) {
        return false;
    }
    else {
        return true;
    }
}


//a function that measures the fitness of a solution
int fitness(const vector<int>& sol) {
    int value = 0;
    if (check_weight(sol)) {
        for (int i = 0; i < sol.size(); i++) {
            if (sol[i]) {
                value += values[i];
            }
        }
        return value;
    }
    else {
        return -1;
    }
}


vector<vector<int>> remove(vector<vector<int>>& p, int index) {
    vector<vector<int>> v;
    for (int i = 0; i < p.size(); i++) {
        if (i != index) {
            v.push_back(p[i]);
        }
    }
    return v;
}



int get_max(const vector<int>& v) {
    int max = -1;
    for (int i = 0; i < v.size(); i++) {
        if (v[i] > max) {
            max = v[i];
        }
    }
    return max;
}

vector<int> min_max(vector<sol_with_diversity>& v, vector<vector<int>>& p) {

    vector<int> sol;
    int index = 0;
    int min = v.size() + 1;
    for (int i = 0; i < v.size(); i++) {
        if (v[i].diversity < min) {
            min = v[i].diversity;
            sol = v[i].solution;
            index = i;
        }
    }
    vector<vector<int>>::iterator it = p.begin() + index;
    p.erase(it);
    v.clear();

    return sol;
}


//a function that measures the distance/diversity between 2 solutions.
int distance(vector<int>& s1, vector<int>& s2) {
    int count = 0;
    for (int i = 0; i < s1.size(); i++) {
        if (s1[i] != s2[i]) {
            count++;
        }
    }
    return count;
}


//Creates the reference set
vector<vector<int>> CreateRefSet(int N) {
    vector<sol_with_fitness> sols;
    //for each solution in P

    for (int i = 0; i < P.size(); i++) {
        sol_with_fitness temp;
        temp.solution = P[i];
        temp.fitness = fitness(P[i]);
        sols.push_back(temp);
    }
    //now all solutions in P are in sols set;
    //sorting based on fitness using bubble sort.

    bool swap = true;
    while (swap) {
        swap = false;
        for (int j = 0; j < sols.size() - 1; j++) {
            if (sols[j].fitness > sols[j + 1].fitness) {
                swap = true;
                sol_with_fitness temp;
                temp = sols[j];
                sols[j] = sols[j + 1];
                sols[j + 1] = temp;
            }
        }
    }

    vector<vector<int>> Refset;

    //top 3 fittest solutions in refset
    size_t s = sols.size();
    for (size_t i = s - 1; i > s - (N + 1); i--) {
        // cout << "Pushed Back";
        Refset.push_back(sols[i].solution);
        sols.pop_back();
    }
    /*for (size_t i = 0; i < 3; i++) {
        sols.pop_back();
    }*/


    //the remaining solutions in sol are the remaining solutions to be in P.
    P.clear();
    for (int i = 0; i < sols.size(); i++) {
        P.push_back(sols[i].solution);
    }




    //Now we need 3 diverse solutions for Refset


    for (int i = 0; i < N; i++) {
        vector<sol_with_diversity> sols_d;
        for (int i = 0; i < P.size(); i++) {
            vector<int> diff_array;
            for (int j = 0; j < Refset.size(); j++) {
                diff_array.push_back(distance(P[i], Refset[j]));
            }
            sol_with_diversity temp;
            temp.solution = P[i];
            temp.diversity = get_max(diff_array);
            sols_d.push_back(temp);
        }
        //using the P solutions we have here, we will take the one with min-max diversity
        //Refset.push_back(min_max(sols_d, P));
        vector<int> sol;
        int index = 0;
        int min = sols_d.size() + 1;
        for (int i = 0; i < sols_d.size(); i++) {
            if (sols_d[i].diversity < min) {
                min = sols_d[i].diversity;
                sol = sols_d[i].solution;
                index = i;
            }
        }
        //vector<vector<int>>::iterator it = P.begin() + index;
        P = remove(P, index);
        if (check_weight(sol))
            Refset.push_back(sol);
        else
            i--;
    }

    //now 3 fit and 3 diverse solutions are part of refset.
    //return Refset;

    /*for (int i = 0; i < Refset.size(); i++) {
        cout << "Solution " << i << ": ";
        for (int j = 0; j < Refset[i].size(); j++) {
            cout << Refset[i][j] << " ";
        }
        cout << endl;
    }*/

    return Refset;
}



bool equal(vector<int>& sol1, vector<int>& sol2) {
    bool ret = true;
    for (size_t i = 0; i < sol1.size(); i++) {
        if (sol1[i] != sol2[i])
            ret = false;
    }
    return ret;
}

bool present_inside(vector<vector<int>>& set, vector<int>& sol) {
    bool ret = false;
    for (size_t i = 0; i < set.size(); i++) {
        if (equal(set[i], sol)) {
            ret = true;
        }
    }
    return ret;
}

//Updates the reference set in each Iteration
vector<vector<int>> RefsetUpdate(int N, vector<vector<int>> refset, vector<vector<int>> PRset) {
    vector<vector<int>> updated_refset;
    vector<vector<int>> union_set;


    for (size_t i = 0; i < refset.size(); i++) {
        union_set.push_back(refset[i]);
    }
    for (size_t i = 0; i < PRset.size(); i++) {
        if (!present_inside(union_set, PRset[i])) {
            union_set.push_back(PRset[i]);
        }

    }

    //Now we select the best N from the refset
    vector<sol_with_fitness> sols;
    for (size_t i = 0; i < union_set.size(); i++) {
        sol_with_fitness temp;
        temp.solution = union_set[i];
        temp.fitness = fitness(union_set[i]);
        sols.push_back(temp);
    }


    //now all solutions in P are in sols set;
    //sorting based on fitness using bubble sort.


    bool swap = true;
    while (swap) {
        swap = false;
        for (int j = 0; j < sols.size() - 1; j++) {
            if (sols[j].fitness > sols[j + 1].fitness) {
                swap = true;
                sol_with_fitness temp;
                temp = sols[j];
                sols[j] = sols[j + 1];
                sols[j + 1] = temp;
            }
        }
    }


    //top 3 fittest solutions in refset
    size_t s = sols.size();
    for (size_t i = s - 1; i > s - (N + 1); i--) {
        // cout << "Pushed Back";
        updated_refset.push_back(sols[i].solution);
        sols.pop_back();
    }

    //we compare refset with updated_refset; if no difference, we terminate the algorithm
    int count = 0;
    bool terminate = true;
    for (size_t i = 0; i < updated_refset.size(); i++) {
        if (!present_inside(refset, updated_refset[i])) {
            cout << "\n\n(Refset has changed)\n\n";
            terminate = false;
            break;
        }
    }
    if (terminate) {
        cout << "\n\n(Refset has not changed. Terminate Algorithm and return final refset.)\n\n";
        terminateAlg = true;
    }

    /*

    int count = 0;
    for (size_t i = 0; i < updated_refset.size(); i++) {
        if (equal(refset[i], updated_refset[i])) {
            count++;
        }
    }

    if (count == updated_refset.size()) {
        cout << "Terminate Algorithm and return final refset";
    }

    */
    return updated_refset;
}


//using the updated_refset, makes all possible sets of 2
vector<vector<vector<int>>>  make_sets(vector<vector<int>>& a) {
    vector < vector <vector <int > > > sets;
    //  a is name of the vector of vector that it is making sets from
    int len_of_bits = a[0].size();    // number of elements in the solution
    int len_of_solution = a.size();   // number of solutions

    for (int i = 0; i < len_of_solution - 1; i++) {   //loop that runs for solutions-1 times. One by one, each solution is picked
        vector <int> temp;

        for (int j = 0; j < len_of_bits; j++) { // this loop picks one solution from the 2D vector and stores it in temp vector
            temp.push_back(a[i][j]);
        }

        for (int k = i + 1; k < len_of_solution; k++) {   // this loop pick all the solutions that lies ahead of the solution stored in temp
            vector <int> temp2;
            for (int j = 0; j < len_of_bits; j++) { // the next solution that lies ahead of temp is picked and stored in temp2
                temp2.push_back(a[k][j]);
            }

            vector < vector <int> > temp3;  // a 2D vector to store temp and temp2
            temp3.push_back(temp);  // temp is assigned
            temp3.push_back(temp2); // temp2 is assigned
            sets.push_back(temp3);  // now the the 2D vector is assiged to the 3D vector
        }   // this loop now runs again (if possible) and stores the next solution (of temp2) in temp2 
    }   // loop ends one solution before it reaches the end of the solutionS.
    return sets;
}

//a function to print sets made
void print_sets(vector<vector<vector<int>>>& sets) {
    int i = sets[0][0].size();
    cout << "Values in a set:" << i << endl;
    int j = sets[0].size();
    cout << "Each set has solutions:" << j << endl;
    int k = sets.size();
    cout << "Number of solutions:" << k << endl;

    for (int x = 0; x < k; x++) {
        for (int y = 0; y < j; y++) {
            for (int z = 0; z < i; z++) {
                cout << sets[x][y][z];
            }
            cout << " ";
        }
        cout << endl;
    }

}

//does forward and backward path relinking on 2 solutions and returns the best product solution
vector<int> path_relinking(vector<int>& src, vector<int>& dest) {
    vector<vector<int>> pr_set;
    pr_set.push_back(src);

    vector<int> temp = src;
    for (size_t i = 0; i < src.size(); i++) {
        temp[i] = dest[i];
        /*if (!present_inside(pr_set, temp)) {
            pr_set.push_back(temp)
        }*/
        pr_set.push_back(temp);
    }

    vector<sol_with_fitness> sols;
    for (size_t i = 0; i < pr_set.size(); i++) {
        sol_with_fitness temp;
        temp.solution = pr_set[i];
        temp.fitness = fitness(pr_set[i]);
        sols.push_back(temp);
    }

    int index = get_most_fit(sols);
    return sols[index].solution;
}

//does path relinking on each set of 2 (the solutions in each set are the input for Path Relinking)
vector<vector<int>> pr_on_sets(vector<vector<vector<int>>>& set_of_sets) {
    //we have to do forward plus backward pathrelinking on each set
    vector<vector<int>> pr_set;
    for (size_t i = 0; i < set_of_sets.size(); i++) {
        vector<int> best_sol_f = path_relinking(set_of_sets[i][0], set_of_sets[i][1]);
        vector<int> best_sol_b = path_relinking(set_of_sets[i][1], set_of_sets[i][0]);
        if (fitness(best_sol_f) > fitness(best_sol_b)) {
            pr_set.push_back(best_sol_f);
        }
        else {
            pr_set.push_back(best_sol_b);
        }
    }
    return pr_set;
}

void printSet(const vector<vector<int>>& v) {
    for (int i = 0; i < v.size(); i++) {
        cout << "Solution " << i << ": ";
        for (int j = 0; j < v[i].size(); j++) {
            cout << v[i][j] << " ";
        }
        cout << " Fitness: " << fitness(v[i]) << endl;
    }
}

void printsol(const vector<int>& v) {
    
    for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << " Fitness: " << fitness(v) << endl;
}


int main()
{
    cout << "\n************* Scatter Search with Path Relinking for the 0/1 Knapsack Problem*****************\n\n";

    //Initial Random Set of solutions
    Diversification(40);


    //Creation of Reference set from Initial Set
    vector<vector<int>> refset = CreateRefSet(S);
    cout << "Initial Refset\n";
    printSet(refset);
    
    //Looping part of the Algorithm
    while(!terminateAlg){
        
        //making sets
        vector<vector<vector<int>>> sets = make_sets(refset);
        //print_sets(sets);      

        //cout << endl << "PR on sets" << endl;

        //doing path relinking on sets
        vector<vector<int>> prset = pr_on_sets(sets);
        //printSet(prset);

        //updating refset
        vector<vector<int>> newrefset = RefsetUpdate(S * 2, refset, prset);
        if(!terminateAlg){
            cout << "New refset" << endl;
            printSet(newrefset);
            refset = newrefset;
        }

    }

    //returning the best solution in the reference set. In the case of multiple best solutions, returning 1 of them.
    cout << "\n\n***********************************************************\n\n";
    cout << "Final Best Solution: ";
    vector<int> best = refset[0];
    printsol(best);


    cout << "Based on the results of the Algorithm, the most valuable combination of items to include in your backpack is: ";
    for (int i = 0; i < best.size(); i++) {
        if (best[i]) {
            cout << items[i] << ", ";
        }
    }
    cout << endl;

}

