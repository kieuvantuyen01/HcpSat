#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <thread>

using namespace std;

class Vertex {

	private:
		int name;

	public:
		Vertex(int name) {
			this->name = name;
		}

		int getName(){
			return this->name;
		}

		void printVertex(){
			cout << "Name: " << this->name << endl;
		}
};

class Edge {
	private:
		int from, to;

	public:
		Edge(int from, int to) {
			this->from = from;
			this->to = to;
		}

		int getFrom(){
			return this->from;
		}

		int getTo(){
			return this->to;
		}

		void printEdge(){
			cout << "from " << this->from << " to " << this->to << endl;
		}
    
};

vector<string> getLineTokens(string str) {
	string buf; // Have a buffer string
	stringstream ss(str); // Insert the string into a stream

	vector<string> tokens; // Create vector to hold our words

	while (ss >> buf)
		tokens.push_back(buf);
	return tokens;
}

int countDigits(int arg){
	int count = 0;
	while(arg!=0){
		count++;
		arg = arg / 10;
	}
	return count;
}

string createZeros(int count){
	string toReturn;
	string zero = "0";
	for(int i = 1; i <= count; i++){
		toReturn = toReturn + zero;
	}

	return toReturn;
}

string prependZeros(string in, int count){
	return createZeros(count) + in;
}

string normalizeValue(string in, int normalizedLength){
	return prependZeros(in, normalizedLength-in.size());
}

int countVariables(vector<vector<string> > allClauses){
	set<string> vars;

	//input: vector of vector<string> of clauses
	//iterate over all vectors containting vectors
	for(int i = 0; i < allClauses.size(); i++){
		//iterate over all clauses, represented as string in vector<string>
		for(int y = 0; y < allClauses.at(i).size(); y++){
			//separate string containting clause
			vector<string> clauses = getLineTokens(allClauses.at(i).at(y));
			string negation = "-";

			for(int z = 0; z < clauses.size(); z++){
				//just look at variables which are != 0 [=end of line]
				if(clauses.at(z) != "0"){
					//case: negated variable
					if(clauses.at(z).compare(0, negation.length(), negation) == 0){
						string var = clauses.at(z).substr(1,clauses.at(z).size());
						vars.insert(var);
					}
					//case: not negated variable
					if(clauses.at(z).compare(0, negation.length(), negation) != 0){
						vars.insert(clauses.at(z));
					}
				}
			}
		}
	}

	return vars.size();
}

string prependOnVariables(string in){
	vector<string> tokens = getLineTokens(in);

	string toReturn = "";


	for(int i = 0; i < tokens.size(); i++){
		//non empty string
		if(tokens.at(i).size() > 0){
			string negation = "-";
			string zero = "0";

			//do not prepend if 0 (end of line) is read
			if(tokens.at(i) == zero){

			}
			// read != 0
			else{
				//negated var
				if(tokens.at(i).compare(0, negation.length(), negation) == 0){
					toReturn = toReturn + "-1" + tokens.at(i).substr(1) +" ";
				}
				//nonnegated var
				else{
					toReturn = toReturn + "1" + tokens.at(i) +" ";
				}
			}


		}
	}

	return toReturn + "0";
}

string cutLeadingZeros(string in){
	string zero = "0";

	while(in.compare(0, zero.length(), zero) == 0){
		in = in.substr(1, in.size());
	}

	return in;
}

int countVarDigits(int arg){
	int length = 1;
	while ( arg /= 10 )
	   length++;

	return length;
}

bool edgeExists(int node1,int node2,vector<Edge> edges){
    
    for(std::vector<Edge>::iterator it = edges.begin() ; it != edges.end(); ++it){
        if((it->getFrom()==node1)&&(it->getTo()==node2)){
            return true;
        }
    }
    
    return false;
}

vector<string> constraint1VertexInPath(int vertexCount, int positionLength, int nodeLength){
	vector<string> toReturn = vector<string>();

	if(vertexCount == 0) return toReturn;


	//iterate over nodes
	for(int node = 1; node <= vertexCount; node++){
		string nodeConstraint = "";

		//iterate over position
		for(int position = 1; position <= vertexCount+1; position++){

			stringstream ss;
			ss << position;
			string positionString = normalizeValue(ss.str(), positionLength);

			//reset stringstream
			ss.clear();
			ss.str(string());

			ss << node;
			string nodeString = normalizeValue(ss.str(), nodeLength);

			nodeConstraint = nodeConstraint + positionString +  nodeString + " ";
		}

		nodeConstraint = nodeConstraint + "0";
		toReturn.push_back(nodeConstraint);
	}

	return toReturn;
}

vector<string> constraint2NoDoubledNodesInPath(int vertexCount, int positionLength, int nodeLength){
    vector<string> toReturn = vector<string>();
    if(vertexCount == 0) return toReturn;
    
    for(int node = 1; node <= vertexCount; node++){
        for(int pos1 = 1; pos1< vertexCount+1; pos1++){
            for(int pos2 = pos1+1; pos2<= vertexCount+1; pos2++){
            	string nodeConstraint;
                stringstream ss;
                ss << node;
                string nodeString = normalizeValue(ss.str(), nodeLength);
                ss.clear();
                ss.str(string());
                ss << pos1;
                string pos1String = normalizeValue(ss.str(),positionLength);
                ss.clear();
                ss.str(string());
                ss << pos2;
                string pos2String = normalizeValue(ss.str(), positionLength);
                nodeConstraint = "-"+pos1String  + nodeString + " "+"-"+pos2String +nodeString;
                if((pos1!=pos2)&&!((pos1==1)&&(pos2==vertexCount+1))&&!((pos2==1)&&(pos1==vertexCount+1))){
                    nodeConstraint = nodeConstraint + " 0";
                	toReturn.push_back(nodeConstraint);
                }
            }
        }
        
    }
    
	return toReturn;
}

vector<string> constraint3EveryPositionUsed(int vertexCount, int positionLength, int nodeLength){
	vector<string> toReturn = vector<string>();

	if(vertexCount == 0) return toReturn;

	for(int i = 1; i<= vertexCount+1; i++){
		string toPut = "";
		stringstream ss;
		ss << i;
		string positionString = normalizeValue(ss.str(), positionLength);

		string nodeString;
		for(int j = 1; j <= vertexCount; j++){
			ss.clear();
			ss.str(string());

			ss << j;
			nodeString = normalizeValue(ss.str(), nodeLength);
			toPut = toPut + positionString  + nodeString + " ";
		}
        toPut = toPut + "0";
		toReturn.push_back(toPut);
	}
	return toReturn;
}

vector<string> constraint4NoTwoDifferentNodesAtSamePosition(int vertexCount, int positionLength, int nodeLength){
    vector<string> toReturn = vector<string>();
    
    if(vertexCount == 0) return toReturn;
    
    
    //iterate over nodes
    for(int position = 1; position <= vertexCount+1; position++){
       
        
        //iterate over position
        for(int node1 = 1; node1 < vertexCount; node1++){
            for(int node2 = node1+1; node2 <= vertexCount; node2++){
            	string nodeConstraint;
                stringstream ss;
                ss << position;
                string positionString = normalizeValue(ss.str(), positionLength);
            
                //reset stringstream
                ss.clear();
                ss.str(string());
            
                ss << node1;
                string node1String = normalizeValue(ss.str(), nodeLength);
                ss.clear();
                ss.str(string());
            
                ss << node2;
                string node2String = normalizeValue(ss.str(), nodeLength);
     
                nodeConstraint = "-"+positionString  + node1String + " "+"-"+positionString +node2String;
                if(node1!=node2){
                    nodeConstraint = nodeConstraint + " 0";
                    toReturn.push_back(nodeConstraint);
                }
                
            }
        }
        
        
    }
    return toReturn;
}

vector<string> constraint5OnlyAdjacentEdgesInPath(int vertexCount,vector<Edge> edges, int positionLength, int nodeLength){
	vector<string> toReturn = vector<string>();
	if(vertexCount == 0) return toReturn;
    for(int position = 1; position < vertexCount+1; position++){
        for(int node1 = 1; node1 <= vertexCount; node1++){
            for(int node2 = 1; node2 <= vertexCount; node2++){
            	string nodeConstraint;
                stringstream ss;
                ss << position;
                string position1String = normalizeValue(ss.str(), positionLength);
                
                //reset stringstream
                ss.clear();
                ss.str(string());
                ss << position+1;
                string position2String = normalizeValue(ss.str(), positionLength);
                
                //reset stringstream
                ss.clear();
                ss.str(string());
                
                ss << node1;
                string node1String = normalizeValue(ss.str(), nodeLength);
                ss.clear();
                ss.str(string());
                
                ss << node2;
                string node2String = normalizeValue(ss.str(), nodeLength);
                
                nodeConstraint = "-"+position1String  + node1String + " "+"-"+position2String +node2String;
            
                if(!edgeExists(node1,node2,edges)){
                	nodeConstraint = nodeConstraint + " 0";
                    toReturn.push_back(nodeConstraint);
                }
            }
        }
    }
    
    return toReturn;
}

//vector<string> constraint6StartNodeIsEndNode(){
//	return vector<string>();
//}



void createOutput(vector<Vertex> v, vector<Edge> e, int positionLength, int nodeLength, bool sameLength){
	int edgesInCircle = v.size();
	int vertexCount = edgesInCircle;

	vector<string> constraint1 = constraint1VertexInPath(vertexCount, positionLength, nodeLength);
	vector<string> constraint2 = constraint2NoDoubledNodesInPath(vertexCount, positionLength, nodeLength);
	vector<string> constraint3 = constraint3EveryPositionUsed(vertexCount, positionLength, nodeLength);
	vector<string> constraint4 = constraint4NoTwoDifferentNodesAtSamePosition(vertexCount, positionLength, nodeLength);
	vector<string> constraint5 = constraint5OnlyAdjacentEdgesInPath(vertexCount,e, positionLength, nodeLength);
	//vector<string> constraint6 = constraint6StartNodeIsEndNode();
	int constraintCount = constraint1.size() + constraint2.size() + constraint3.size() + constraint4.size() + constraint5.size(); //+ constraint6.size();

	vector<vector<string> > allConstraints;
	allConstraints.push_back(constraint1);
	allConstraints.push_back(constraint2);
	allConstraints.push_back(constraint3);
	allConstraints.push_back(constraint4);
	allConstraints.push_back(constraint5);


	ofstream outfile("output.txt");
	outfile << "p cnf "<<  countVariables(allConstraints) << " "  << constraintCount << endl;

	outfile << "c constraint 1" << endl;
	//write constraint 1 - every node must be in path
	for(unsigned int i = 0; i < constraint1.size(); i++){
		outfile << prependOnVariables(constraint1.at(i)) << endl;
	}

	outfile << "c constraint 2" << endl;
	for(unsigned int i = 0; i < constraint2.size(); i++){
		outfile << prependOnVariables(constraint2.at(i)) << endl;
	}

	outfile << "c constraint 3" << endl;
	for(unsigned int i = 0; i < constraint3.size(); i++){
		outfile << prependOnVariables(constraint3.at(i)) << endl;
	}

	outfile << "c constraint 4" << endl;
	for(unsigned int i = 0; i < constraint4.size(); i++){
		outfile << prependOnVariables(constraint4.at(i)) << endl;
	}

	outfile << "c constraint 5" << endl;
	for(unsigned int i = 0; i < constraint5.size(); i++){
		outfile << prependOnVariables(constraint5.at(i)) << endl;
	}

//	for(unsigned int i = 0; i < constraint6.size(); i++){
//		outfile << constraint6.at(i) << endl;
//	}


	outfile.close();
}

string getPosition(string code, int positionLength){
	return code.substr(0, positionLength);
}

string getNode(string code,int nodeLength){
	return code.substr(code.size()-nodeLength, code.size());
}


int main(int argc, char* argv[]) {
	system("rm output.txt 2>/dev/null 1>/dev/null && rm satResult.txt 2>/dev/null 1>/dev/null");
	vector<Vertex> vertices;
	vector<Edge> edges;

	string cnfFile = "cnfFile.txt";
	string satResult = "satResult.txt";

	//cout << "Parsing file: " << argv[1] << endl;

	int varLength = 0;
	int positionLength = 0;
	int nodeLength = 0;

	bool sameLength = true;

	string fileToRead = string(argv[1]);
	string prependFolder = "";
	fileToRead = prependFolder + fileToRead;

	string line;
	ifstream myfile(fileToRead.c_str());
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			vector<string> lineTokens = getLineTokens(line);
			for(unsigned int i = 0; i < lineTokens.size(); i++){
				if(lineTokens[0].compare("p") == 0){
					int vertexCount = atoi(lineTokens[2].c_str());
					//int edgeCount = atoi(lineTokens[3].c_str());

					for(int y = 1; y <= vertexCount; y++){
						Vertex newVertex = Vertex(y);
						vertices.push_back(newVertex);
						//newVertex.printVertex();
					}
					break;
				}

				if(lineTokens[0].compare("e") == 0){
					Edge newEdge = Edge(atoi(lineTokens[1].c_str()),atoi(lineTokens[2].c_str()));
					edges.push_back(newEdge);
					//newEdge.printEdge();
					break;
				}
			}
		}
		//cout << "vertices:" << vertices.size() << "; Edges: " << edges.size() << endl;
		myfile.close();

		positionLength = countDigits(vertices.size());
		nodeLength = countDigits(vertices.size());

		if(countVarDigits(vertices.size()) != countVarDigits(vertices.size() + 1)){
			positionLength++;
			sameLength = false;
		}

		varLength = positionLength + nodeLength;

		createOutput(vertices, edges, positionLength, nodeLength, sameLength);
		system("riss64 output.txt satResult.txt 2>/dev/null 1>/dev/null");


		//parse result of sat solver
		//cout << "Parsing result of sat solver, filename: " << satResult << endl;
		string resultLine;
		ifstream resultFile(satResult.c_str());

		vector<string> resultTokens;
		if (resultFile.is_open()) {
			while (getline(resultFile, resultLine)) {
				vector<string> resultLineTokens = getLineTokens(resultLine);
				resultTokens.insert(resultTokens.end(), resultLineTokens.begin(), resultLineTokens.end());
			}
		}
		resultFile.close();

		//TODO: catch unexpected values
		if(resultTokens.size() > 0){
			cout << resultTokens.at(0) << " " << resultTokens.at(1) << endl;

			if(resultTokens.at(1) == "UNSATISFIABLE"){
				return 20;
			}
			if(resultTokens.at(1) == "SATISFIABLE"){
				vector<string> path;
				//TODO catch errors
				//iterate over variables of sat result
				for(int i = 3; i<resultTokens.size(); i++){
					string negation = "-";
					//if variable doesn't start with "-" or == "0" write it in path vector which is to be decoded
					if(resultTokens.at(i).compare(0, negation.length(), negation) != 0 && resultTokens.at(i) != "0"){
						path.push_back(resultTokens.at(i).substr(1,resultTokens.at(i).size()));
					}
				}
				//iterate over path coding
				cout << "v";
				for(int i = 0; i<path.size()-1; i++){
					//cout << path.at(i) << ",";
					cout  << " " << cutLeadingZeros(getNode(path.at(i), nodeLength));

					//cout << endl << path.at(i).substr(digitsToRemove,path.at(i).size()) << "->";
				}


				//print path vector
//				cout << endl;
//				for(int i = 0; i<path.size(); i++){
//					cout << path.at(i) << ",";
//				}
				cout << endl;
				return 10;
			}
		}
		return 20;
	}


	else
		cout << "s UNSATISFIABLE";

	return 20;
}

