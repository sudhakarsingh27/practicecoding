// A fast IO program
#include <bits/stdc++.h>
using namespace std;

// create string id from integer indices
string createId(int r, int c)
{
    return to_string(r) + "," + to_string(c);
}

// Find Parent with path-compression
string findParent(unordered_map<string,string>& parent, string id)
{
    if(parent[id] != id)
    {
        parent[id] = findParent(parent, parent[id]);
    }
    return parent[id];
}

// Union with rank to balance size of the disjoint sets
void unionMerge(unordered_map<string,string>& parent, unordered_map<string, int>& rank, string& id1, string& id2)
{
    string parent1 = findParent(parent, id1);
    string parent2 = findParent(parent, id2);
    
    if(rank[parent1] <= rank[parent2])
    {
        parent[parent2] = parent1;
        rank[parent1] = rank[parent1] + rank[parent2];
    }
    else
    {
        parent[parent1] = parent2;
        rank[parent2] = rank[parent2] + rank[parent1];
    }
}

int main()
{
	// For faster I/O
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	
	int tests;
    cin >> tests;
    int rows, cols; 
    for(int t = 0; t < tests; t++)
    {
        // get rows and cols
        cin >> rows >> cols;
        
        // create matrix
        vector<vector<char>> board (rows, vector<char> (cols));
        
        // parent and rank lookup tables
        unordered_map<string, string> parent;
        unordered_map<string, int> rank;
        unordered_map<string, char> value;
        
        // Create the crossword board from stdin.
        // Also create each cell its own parent which would later be used in
        // the Union-Find algorithm
        string row;
        for(int r = 0; r < rows; r++)
        {
            cin >> row;
            for(int c = 0; c < cols; c++)
            {                
                board[r][c] = row[c];
                
                // add a cell as it's own parent
                if(board[r][c] != '#')
                {
                    string id = createId(r, c);

                    parent[id] = id;
                    rank[id] = 1;
                }
            }
        }
        
        // find out all palindromic words and for each of those words, create equivalent 
        // relationships. More like connections (undirected edges) in a graph
        for(int r = 0; r < rows; r++)
        {
            for(int c = 0; c < cols; c++)
            {
                if(board[r][c] != '#')
                {
                    // find palindrome towards right
                    if(c == 0 || ((c > 0) && (board[r][c-1] == '#')) )
                    {
                        // find the end of the palindrome
                        int endcol = c;
                        while(endcol < cols && board[r][endcol] != '#')
                            endcol++;
                        endcol--;

                        // for this palindrome, create the required sets
                        for(int i = c; i <=(c + endcol)/2 ; i++)
                        {
                            string id1 = createId(r,i);
                            string id2 = createId(r, endcol-(i-c));
                            
                            // pair up (r, i) and (r, endcol-(i-c))
                            // merge the two cells
                            unionMerge(parent, rank, id1, id2);
                        }
                    }

                    // find palindromes towards bottom
                    if(r == 0 || ((r > 0) && (board[r-1][c] == '#')))
                    {
                        // find the end of the palindrome
                        int endrow = r;
                        while(endrow < rows && board[endrow][c] != '#')
                            endrow++;
                        endrow--;

                        // for this palindrome, create the required sets
                        for(int i = r; i <=(r + endrow)/2 ; i++)
                        {
                            string id1 = createId(i, c);
                            string id2 = createId(endrow-(i-r), c);

                            // pair up (i, c) and (endrow-(i-r), c)
                            // merge the two cells
                            unionMerge(parent, rank, id1, id2);
                        }
                    }
                }
            }
        }
        
        // For any known cells, mark their parents as that cell
        for(int r = 0; r < rows; r++)
        {
            for(int c = 0; c < cols; c++)
            {
                if(board[r][c] != '#' && board[r][c] != '.')
                {
                    // For the current cell, find its parent
                    string id = createId(r,c);
                    string p = findParent(parent, id);
                    
                    // For this parent, if there's nothing in the "value" hash table, 
                    // add the content of this cell as its value.
                    // Later on, all the cells which point to this parent can retrieve this value
                    if(value.find(p) == value.end())
                        value[p] = board[r][c];
                    else
                    {
                        // sanity check that this other value is the same as this cell's (current) value,
                        // otherwise, it's an invalid crossword
                        assert (board[r][c] == value[p]);
                    }
                }
            }
        }
        
        // Go thru the board to count the number of inferences (changes) we can draw
        int changes = 0;

        // Meanwhile, also build a string output instead of making another pass through 
        // the board
        ostringstream s1;

        // Finally, output the crossboard
        for(int r = 0; r < rows; r++)
        {
            for(int c = 0; c < cols; c++)
            {
                if(board[r][c] == '.')
                {
                    string id = createId(r,c);
                    string p = findParent(parent, id);
                    
                    if(value.find(p) != value.end())
                    {
                        s1 << value[p];
                        changes++;
                    }
                    else
                        s1 << board[r][c];
                }
                else
                    s1 << board[r][c];
            }
            s1 << "\n";
        }
        
        cout << "Case #" << t+1 << ": " << changes << "\n" << s1.str();
        
    }
	return 0;
}
