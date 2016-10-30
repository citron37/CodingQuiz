#include <iostream>
#include <fstream>

using namespace std;

const int MAX_FILE_NAME		= 100;
const int MAX_LINE_LEN		= 10000;
const int UNIT_CHILD_BUFFER	= 10;
const int MOD_VALUE			= 100000000;

//////////////////////////////////////////////////////////////////////////
//	Bracket Tree Class
class CBraketTree
{
public:
	CBraketTree()
	{
		m_chBraket			= '\0';

		m_pkParent			= NULL;

		m_nMaxChildCount	= 0;
		m_nChildCount		= 0;
		m_pkArrChild		= NULL;
	}

	~CBraketTree()
	{
		if (m_pkArrChild != NULL)
		{
			delete [] m_pkArrChild;
			m_pkArrChild	= NULL;
		}
	}

public:
	char			m_chBraket;

	CBraketTree*	m_pkParent;
	
	int				m_nMaxChildCount;
	int				m_nChildCount;
	CBraketTree*	m_pkArrChild;

public:

	//	Add new child
	CBraketTree* AddChild (char chBraket)
	{
		//	Re-allocate child buffer
		if (m_nChildCount == m_nMaxChildCount)
		{
			m_nMaxChildCount				= m_nMaxChildCount + UNIT_CHILD_BUFFER;
			CBraketTree*	pkArrNewChild	= new CBraketTree[m_nMaxChildCount];

			for (int nIndex = 0 ; nIndex < m_nMaxChildCount - UNIT_CHILD_BUFFER ; nIndex++)
			{
				pkArrNewChild[nIndex]	= m_pkArrChild[nIndex];
			}

			if (m_pkArrChild != NULL)
			{
				delete [] m_pkArrChild;
			}

			m_pkArrChild	= pkArrNewChild;
		}

		//	Create child
		m_nChildCount++;

		m_pkArrChild[m_nChildCount - 1].m_pkParent	= this;
		m_pkArrChild[m_nChildCount - 1].m_chBraket	= chBraket;

		return &m_pkArrChild[m_nChildCount - 1];
	}

	//////////////////////////////////////////////////////////////////////////
	//	Value	= 내 숫자 * Child 숫자 들의 합
	int	GetValue ()
	{
		int	nMyValue	= GetNumeric();

		//	Child 가 없는 경우
		if (m_nChildCount == 0)
		{
			return nMyValue;
		}

		//	Child 가 있는 경우
		int	nChildValue	= 0;
		for (int nChildIndex = 0 ; nChildIndex < m_nChildCount ; nChildIndex++)
		{
			nChildValue	+= m_pkArrChild[nChildIndex].GetValue();
		}

		return (nMyValue * nChildValue) % MOD_VALUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//	Utility function
	bool IsPair (char chBraket)
	{
		if		(m_chBraket == '(' && chBraket == ')')	{	return true;	}
		else if (m_chBraket == '{' && chBraket == '}')	{	return true;	}
		else if (m_chBraket == '[' && chBraket == ']')	{	return true;	}

		return false;
	}

	int GetNumeric ()
	{
		if		(m_chBraket == '(')		{	return 1;	}
		else if (m_chBraket == '{')		{	return 2;	}
		else if (m_chBraket == '[')		{	return 3;	}
		else							{	return 1;	}	//	Root
	}

	//////////////////////////////////////////////////////////////////////////
	//	Operator
	CBraketTree&	operator=(const CBraketTree& kBracket)
	{
		m_chBraket			= kBracket.m_chBraket;
		
		m_pkParent			= kBracket.m_pkParent;
		m_nMaxChildCount	= kBracket.m_nMaxChildCount;
		m_nChildCount		= kBracket.m_nChildCount;

		if (m_nMaxChildCount == 0)
		{
			m_pkArrChild	= NULL;
		}
		else
		{
			m_pkArrChild		= new CBraketTree[m_nMaxChildCount];
			for (int nChildIndex = 0 ; nChildIndex < m_nChildCount ; nChildIndex++)
			{
				m_pkArrChild[nChildIndex]	= kBracket.m_pkArrChild[nChildIndex];
			}
		}

		return *this;
	}

	//////////////////////////////////////////////////////////////////////////
	//	Static utility function
	static bool IsOpenBraket (char chBraket)
	{
		if (chBraket == '('	||
			chBraket == '{'	||
			chBraket == '[')
		{
			return true;
		}

		return false;
	}

	static bool IsCloseBraket (char chBraket)
	{
		if (chBraket == ')'	||
			chBraket == '}'	||
			chBraket == ']')
		{
			return true;
		}

		return false;
	}
};

//////////////////////////////////////////////////////////////////////////
//	Main
int main (void)
{
	//////////////////////////////////////////////////////////////////////////
	//	Input File Name
	cout << "Enter input file name : ";

	char strInputFile[MAX_FILE_NAME + 1];
	memset (strInputFile, 0x00, sizeof(char));
	cin.getline(strInputFile, MAX_FILE_NAME);

	ifstream kInFile(strInputFile);
	if (kInFile.is_open() == false)
	{
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	//	Output File Name
	cout << "Enter Output file name : ";

	char strOutputFile[MAX_FILE_NAME + 1];
	memset (strOutputFile, 0x00, sizeof(char));
	cin.getline(strOutputFile, MAX_FILE_NAME);

	ofstream kOutFile(strOutputFile);
	if (kOutFile.is_open() == false)
	{
		kInFile.close();
		return 0;
	}
	
	//////////////////////////////////////////////////////////////////////////
	//	Read testcase count
	char	strTestCase [MAX_LINE_LEN + 1];
	int		nTestCase	= 0;

	memset (strTestCase, 0x00, sizeof (char) * MAX_LINE_LEN);
	kInFile.getline(strTestCase, MAX_LINE_LEN);
	
	nTestCase	= atoi (strTestCase);

	//////////////////////////////////////////////////////////////////////////
	//	Read / Calculate / Write
	int	nLineLength	= 0;
	for (int nIndex = 0 ; nIndex < nTestCase ; nIndex++)
	{
		CBraketTree*	pkBraketRoot	= new CBraketTree;
		CBraketTree*	pkCurrentNode	= pkBraketRoot;

		//	Read testcase string
		memset (strTestCase, 0x00, sizeof (char) * MAX_LINE_LEN);
		kInFile.getline(strTestCase, MAX_LINE_LEN);

		nLineLength	= strlen (strTestCase);

		//	Parsing - Create Tree
		for (int nStrIndex = 0 ; nStrIndex < nLineLength ; nStrIndex++)
		{
			//	Open Braket - Create child node
			if (CBraketTree::IsOpenBraket(strTestCase[nStrIndex]) == true)
			{
				pkCurrentNode	= pkCurrentNode->AddChild (strTestCase[nStrIndex]);
			}

			//	Close Brakset - go to parent node
			else if (CBraketTree::IsCloseBraket(strTestCase[nStrIndex]) == true)
			{
				if (pkCurrentNode->IsPair(strTestCase[nStrIndex]) == true)
				{
					pkCurrentNode	= pkCurrentNode->m_pkParent;
				}
				else
				{
					//	Pair 가 맞지 않는 경우
					pkCurrentNode	= NULL;
					break;
				}
			}

			//	비정상적인 경우
			else
			{
				pkCurrentNode	= NULL;
				break;;
			}
		}

		//	Root 로 돌아오지 못한 경우
		if (pkCurrentNode != pkBraketRoot)
		{
			kOutFile << 0 << endl;
		}
		
		//	정상적인 경우
		else
		{
			kOutFile << pkBraketRoot->GetValue() << endl; 
		}

		//
		delete pkBraketRoot;
	}

	kInFile.close();
	kOutFile.close();

	return 0;
};