#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "ArgumentManager.h"

struct Sentence
{
    public:
    bool isFirst = false;
    string sentence;

    string LastWordsearchable;
    string FirstWordSearchable;

    Sentence * prev;
    Sentence * next;

    Sentence() {
    sentence = "";
	LastWordsearchable = "";
	FirstWordSearchable = "";
	prev = NULL;
	next = NULL;
    }
};
class Paragraph
{
    public:
    Sentence* head;
    Paragraph();
    void fillNode(string fFile);
    Sentence* search(string x);
    bool IcanFindIt(string x);
    void swapThem(Sentence *A, Sentence *B);
    void sortSentences();
    void addNode(string line);
    Sentence* getSentence(int index);
    void print(string oFile);
    int size=0;
    int startAt=0;
};
Paragraph:: Paragraph()
{head=0;}

Sentence* Paragraph::getSentence(int index)
{
    Sentence *curr = head;
    if (index == 0)
    {
        return head;
    }

    for ( int i=0; i < index; i++)
    {
        curr = curr->next;
    } 

    return curr;
}
void Paragraph::fillNode(string fFile)
{
    string theLine;

    ifstream myFile(fFile);
    //if (!myFile.is_open()) {cout << "NOT OPEN";} // DEBUGGING
    if (myFile.is_open() )
    {
        while(getline(myFile, theLine))
        {
            addNode(theLine);
            size++;
        }
    }
    myFile.close();
}

string clean(string str) {
	string res ="";

	for (int i = 0; i < str.length(); i++) {
		char ch = str[i];

		if (ch >= 'A' && ch <= 'Z')
			ch = tolower(ch);
		else if (ch != ' ' && (ch < 'a' || ch > 'z'))
			continue;

		res += ch;
	}

	return res;
}

void Paragraph::addNode(string line)
{
	Sentence *tmp = new Sentence();
	tmp->sentence = line;

	string firstWord = clean(line.substr(0, line.find(" ")));
	string lastWord = clean(line.substr(line.find_last_of(" ") + 1));

	tmp->FirstWordSearchable = firstWord;
	tmp->LastWordsearchable = lastWord;

	if (head == NULL) {
		head = tmp;
		return;
	}
	
	Sentence *curr = head;
	while (curr->next != NULL)
		curr = curr->next;
	curr->next = tmp;

}

void Paragraph:: swapThem(Sentence *A, Sentence *B)
{
    //cout << "\n now the swap is comparing " << A->sentence << " and " << B->sentence << endl; // DEBUGGING
    Sentence *temp = new Sentence;

    temp->sentence = B->sentence;
    temp->FirstWordSearchable = B->FirstWordSearchable;
    temp->LastWordsearchable = B->LastWordsearchable;

    B->sentence = A->sentence;
    B->FirstWordSearchable = A->FirstWordSearchable;
    B->LastWordsearchable = A->LastWordsearchable;

    A->sentence = temp->sentence;
    A->FirstWordSearchable = temp->FirstWordSearchable;
    A->LastWordsearchable = temp->LastWordsearchable;

    //cout <<"SWAP SUCCESS" << endl; //DEBUGGING
}

void Paragraph::sortSentences()
{
    Sentence *Collide = new Sentence;
    Sentence *curr = head;
    Sentence *temp = curr->next;
    if (head != 0)
    {
        for (int i =0; i < size-1; i++)
        { 
            bool found = false;
            for (int j = i+1; j<size; j++)
            {
                if ((getSentence(j))->FirstWordSearchable == (getSentence(i))->LastWordsearchable)
                {   
                if ( j != i+1 )
                    {
                    swapThem(getSentence(i+1), getSentence(j));
                    found = true;
                    break;
                    }
                    found = true;
                    break;
                }
            }
            if (found == false)
            {   
                Sentence *Collide = new Sentence;
                startAt=0;
                for (int k=0; k <i+1; k++)
                {
                    Collide->sentence = Collide->sentence + getSentence(k)->sentence;
                    Collide->sentence = Collide->sentence + "\n";
                    startAt++;
                }

	            string firstWord = clean(Collide->sentence.substr(0, Collide->sentence.find(" ")));
	            string lastWord = clean(Collide->sentence.substr(Collide->sentence.find_last_of(" ") + 1));

	            Collide->FirstWordSearchable = firstWord;
	            Collide->LastWordsearchable = lastWord;
                Collide->sentence = Collide->sentence + "\n";


                Sentence *last = head;
                while (last->next != nullptr)
                {last = last->next;}

                last->next = Collide;
                size++;

                //i = -1;
            }
        }
    }
}

//void Paragraph::print(string oFile)
void Paragraph::print(string oFile)
{
    sortSentences();
    ofstream outFile;
    outFile.open(oFile);

    for (int i =startAt; i<size; i++)
    {
        if (getSentence(i)->sentence != "\n")
        outFile << getSentence(i)->sentence << "\n"; 
    }

}
bool oneLine(string fFile)
{
    string theLine;
    int countLines;
    ifstream myFile(fFile);
    //if (!myFile.is_open()) {cout << "NOT OPEN";} // DEBUGGING
    if (myFile.is_open() )
    {
        while(getline(myFile, theLine))
        {
            countLines++;
        }
    }
    myFile.close();
    if (countLines == 1)
        {return true;}
    return false;
}


string returnOneLine(string fFile)
{

    string theLine;
    ifstream myFile(fFile);
    //if (!myFile.is_open()) {cout << "NOT OPEN";} // DEBUGGING
    if (myFile.is_open() )
    {
        while(getline(myFile, theLine))
        {
            theLine = theLine;
        }
    }
    myFile.close();

    return theLine;
}


using namespace std;
int main(int argc, char * argv[])
{
   ArgumentManager am(argc, argv);
    string infilename = am.get("input");
    string outfilename = am.get("output");
    Paragraph Thoughts;

    //CHECK IF ONLY ONE LINE


    if (oneLine(infilename))
    {
    ofstream outFile;
    outFile.open(outfilename);
        outFile << returnOneLine(infilename);
    outFile.close();
    }

    Thoughts.fillNode(infilename);
    Thoughts.print(outfilename);


return 0;
}
