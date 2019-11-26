#include<iostream>
#include<vector>
#include<stdlib.h>>
#include<string>
#include<sstream>
#include<conio.h>

using namespace std;

#define e_C 1
#define e_F 2
#define e_Cl 3
#define e_Br 4
#define e_I 5
#define e_AL 6
#define e_OL 7
#define e_ONE 8
#define e_OIC 9

struct Elem
{
	int type = 1;
	int valency = 4;
	int left = 4;
	int cBonds = 0;
	vector<struct Elem*> singleBonds, r_singleBonds;
	struct Elem* nextInMain = nullptr;
	struct Elem* prevInMain = nullptr;
	Elem()
	{
		type = e_C;
		valency = 4;
		left = 4;
		singleBonds.empty();
		nextInMain = nullptr;
		prevInMain = nullptr;
	}
	Elem(int t, int bondType, Elem* from)
	{
		type = t;
		switch (type)
		{
		case e_C:
			valency = 4;
			break;
		case e_Br:
		case e_Cl:
		case e_F:
		case e_I:
		case e_AL:
		case e_OL:
		case e_OIC:
			valency = 1;
			break;
		case e_ONE:
			valency = 2;
			break;
		}
		left = valency;

		singleBonds.empty();
		r_singleBonds.empty();
		nextInMain = nullptr;
		prevInMain = nullptr;

		if (bondType == 1)
		{
			r_singleBonds.push_back(from);
			left--;
		}
	}

	int AddSingleBond(struct Elem* el)
	{
		if (left >= 1)
		{
			singleBonds.push_back(el);
			left--;
		}		
		else
		{
			cout << "Too many bonds!! Not possible!!!" << endl;
			return 1;
		}
		if (el->type == 1)
		{
			cBonds++;
		}
		return 0;
	}
};

struct ChainDat
{
	int length;
	//int functGrps;
	int dBonds, tBonds;
	int substs;
	ChainDat()
	{
		length = 1;
		dBonds = 0;
		tBonds = 0;
		//functGrps = 0;
		substs = 0;
	}
	ChainDat(int l) :length(l)
	{
		//functGrps = 0;
		tBonds = 0;
		dBonds = 0;
		substs = 0;
	}
};

struct SubstsDat
{
	int carbon = 0;
	int fluorine = 0;
	int chlorine = 0;
	int bromine = 0;
	int iodine = 0;
	SubstsDat()
	{
		carbon = 0;
		fluorine = 0;
		chlorine = 0;
		bromine = 0;
		iodine = 0;
	}
	SubstsDat(int c, int f, int cl, int br, int i) : carbon(c), 
		fluorine(f), chlorine(cl), bromine(br), iodine(i)
	{

	}
};

int inpRevised(Elem* r, string inp , int i)
{
	Elem* focus = r;
	stringstream instream;
	for (i; i < inp.length(); i++)
	{
		if (i < 0)
		{
			return -1;
		}
		if (inp[i] == 'C')
		{
			if (focus->AddSingleBond(new Elem(e_C, 1, r)) != 0)
			{
				return -1;
			}
			
			focus = focus->singleBonds.back();
			continue;
		}
		if (inp[i] == 'F')
		{
			if (focus->AddSingleBond(new Elem(e_F, 1, r)) != 0)
			{
				return -1;
			}
			continue;
		}
		if (inp[i] == 'c')
		{
			if (focus->AddSingleBond(new Elem(e_Cl, 1, r)) != 0)
			{
				return -1;
			}
			continue;
		}
		if (inp[i] == 'b')
		{
			if (focus->AddSingleBond(new Elem(e_Br, 1, r)) != 0)
			{
				return -1;
			}
			continue;
		}
		if (inp[i] == 'I')
		{
			if (focus->AddSingleBond(new Elem(e_I, 1, r)) != 0)
			{
				return -1;
			}
			continue;
		}
		if (inp[i] == '(')
		{
			 i = inpRevised(focus, inp, i+1) - 1;
			 continue;
		}
		if (inp[i] == ')')
		{
			return i + 1;
		}
	}
}

ChainDat findChainDat(Elem* cur)
{
	
	int scount = cur->singleBonds.size();
	int cCount = cur->cBonds;
	ChainDat r(1);
	ChainDat rec(1);
	ChainDat curBest(-100);
	int bestI = 0;

	if (cCount == 0)
	{
		return r;
	}
	if (cCount == 1)
	{
		for (int i = 0; i < scount; i++)
		{
			if(cur->singleBonds[i]->type == e_C)
			{
				cur->nextInMain = cur->singleBonds[i];
				cur->singleBonds[i]->prevInMain = cur;
				rec = findChainDat(cur->nextInMain);
				r.substs += rec.substs;
				r.length += rec.length;
				return r;
			}
		}
	}
	else
	{
		for (int i = 0; i < scount; i++)
		{
			if (cur->singleBonds[i]->type != 1)
			{
				continue;
			}
			if (curBest.length == -100)
			{
				curBest = findChainDat(cur->singleBonds[i]);
				continue;
			}
			rec = findChainDat(cur->singleBonds[i]);
			if (rec.substs > curBest.substs)
				{
					curBest = rec;
					bestI = i;
					continue;
				}
				else if (rec.substs == curBest.substs)
				{
					if (rec.length > curBest.length)
					{
						curBest = rec;
						bestI = i;
						continue;
					}
				}

			}
			
		}

		cur->nextInMain = cur->singleBonds[bestI];
		cur->singleBonds[bestI]->prevInMain = cur;
		r.substs += curBest.substs;
		r.length += curBest.length;
		//r.functGrps += curBest.functGrps;
		return r;
	}

}

SubstsDat findCarbonCount(Elem* c)
{
	if (c->cBonds == 0)
	{
		return SubstsDat(1, 0, 0, 0, 0);
	}
	SubstsDat r(1, 0, 0, 0, 0);
	if (c->cBonds == 1)
	{
		for (int i = 0; i < c->singleBonds.size(); i++)
		{
			if (c->singleBonds[i]->type == e_C)
			{
				//return findCarbonCount(c->singleBonds[i]).carbon++;
				r = findCarbonCount(c->singleBonds[i]);
				r.carbon++;
				return r;
			}
		}
	}
	else
	{
		SubstsDat temp;
		for (int i = 0; i < c->singleBonds.size(); i++)
		{
			if (c->singleBonds[i]->type == e_C)
			{
				temp = findCarbonCount(c->singleBonds[i]);
				r.carbon += temp.carbon;
				r.fluorine += temp.fluorine;
				r.chlorine += temp.chlorine;
				r.bromine += temp.bromine;
				r.iodine += temp.iodine;
			}  
		}
		return r;	
	}
}

int main()
{
	
	vector<Elem*> mainChain;
	bool reverse = false;
	int smallest = 2000;
	int largest = 0;
	Elem* inpChain = new Elem(1, 0, nullptr);
	int inp;
	cout << "Enter the structural formula of the alkane in the reuqested format."
		"\nEnsure that the first carbon, which is added by default, is part of the main chain.";
	string chain;
	cin >> chain;
	if (inpRevised(inpChain, chain, 0) <0)
	{
		return 1;
	}
END:
	inpChain = inpChain;
	Elem* index = inpChain;
	int l = 0;
	ChainDat x = findChainDat(inpChain);
	int methylCount = 0, ethylCount = 0, propylCount = 0, butylCount = 0;
	vector<int> chloros, bromos, fluoros, iodos, methyl, ethyl, propyl, butyl;
	while (index != nullptr)
	{
		//l++;
		mainChain.push_back(index);
		for (int i = 0; i < index->singleBonds.size(); i++)
		{
			if (index->singleBonds[i] != index->nextInMain)
			{
				SubstsDat data = findCarbonCount(index->singleBonds[i]);
				switch (data.carbon)
				{
				case 1:
					methyl.push_back(l);
					break;
				case 2:
					ethyl.push_back(l);
					break;
				case 3:
					propyl.push_back(l);
					break;
				case 4:
					butyl.push_back(l);
					break;
				}
			}
		}
		index = index->nextInMain;
		l++;
	}

	string str;
	stringstream stream(str);

	int j = 0;
	if (methyl.size() != 0)
	{
		for (j = 0; j < methyl.size(); j++)
		{
			stream << methyl[j] + 1;
			if (methyl[j] + 1 < smallest)
			{
				smallest = methyl[j] + 1;
			}
			if(methyl[j] + 1 > largest)
			{
				largest = methyl[j] + 1;
			}
			if (methyl.size() - j != 1)
			{
				stream << ",";
			}
		}
		stream << "-";
		switch (methyl.size())
		{
		case 2:
			stream << "di";
			break;
		case 3:
			stream << "tri";
			break;
		case 4:
			stream << "tetra";
			break;
		case 5:
			stream << "penta";
			break;
		}
		stream << "methyl ";
	}
	if (ethyl.size() != 0)
	{
		for (j = 0; j < ethyl.size(); j++)
		{
			stream << ethyl[j] + 1;
			if (ethyl.size() - j != 1)
			{
				stream << ",";
			}
			if (ethyl[j] + 1 < smallest)
			{
				smallest = ethyl[j] + 1;
			}
			if (ethyl[j] + 1 > largest)
			{
				largest = ethyl[j] + 1;
			}
		}
		stream << "-";
		switch (ethyl.size())
		{
		case 2:
			stream << "di";
			break;
		case 3:
			stream << "tri";
			break;
		case 4:
			stream << "tetra";
			break;
		case 5:
			stream << "penta";
			break;
		}
		stream << "ethyl ";
	}
	
	if (propyl.size() != 0)
	{
		for (j = 0; j < propyl.size(); j++)
		{
			stream << propyl[j] + 1;
			if (propyl.size() - j != 1)
			{
				stream << ",";
			}
			if (propyl[j] + 1 < smallest)
			{
				smallest = propyl[j] + 1;
			}
			if (propyl[j] + 1 > largest)
			{
				largest = propyl[j] + 1;
			}
		}
		stream << "-";
		switch (propyl.size())
		{
		case 2:
			stream << "di";
			break;
		case 3:
			stream << "tri";
			break;
		case 4:
			stream << "tetra";
			break;
		case 5:
			stream << "penta";
			break;
		}
		stream << "propyl ";
	}
	if (butyl.size() != 0)
	{
		for (j = 0; j < butyl.size(); j++)
		{
			stream << butyl[j] + 1;
			if (butyl.size() - j != 1)
			{
				stream << ",";
			}
			if (butyl[j] + 1 < smallest)
			{
				smallest = butyl[j] + 1;
			}
			if (butyl[j] + 1 > largest)
			{
				largest = butyl[j] + 1;
			}
		}
		stream << "-";
		switch (butyl.size())
		{
		case 2:
			stream << "di";
			break;
		case 3:
			stream << "tri";
			break;
		case 4:
			stream << "tetra";
			break;
		case 5:
			stream << "penta";
			break;
		}
		stream << "butyl ";
	}

	switch (l)
	{
	case 1:
		stream << "methane";
		break;
	case 2:
		stream << "ethane";
		break;
	case 3:
		stream << "propane";
		break;
	case 4:
		stream << "butane";
		break;
	case 5:
		stream << "pentane";
		break;
	case 6:
		stream << "hexane";
		break;
	case 7:
		stream << "septane";
		break;
	case 8:
		stream << "octane";
		break;
	case 9:
		stream << "nonane";
		break;
	case 10:
		stream << "decane";
		break;
	default:
		stream << l << "ane";
		break;
	}
	str = stream.str();
	if ((l - smallest)+1 < smallest)
	{
		string str2;
		stringstream stream2(str2);
		if (methyl.size() != 0)
		{
			for (j = methyl.size(); j > 0; j--)
			{
				stream2 << (l) - methyl[j - 1];
				if (j != methyl.size())
				{
					stream2 << ",";
				}
			}
			stream2 << "-";
			switch (methyl.size())
			{
			case 2:
				stream2 << "di";
				break;
			case 3:
				stream2 << "tri";
				break;
			case 4:
				stream2 << "tetra";
				break;
			case 5:
				stream2 << "penta";
				break;
			}
			stream2 << "methyl ";
		}
		if (ethyl.size() != 0)
		{
			for (j = ethyl.size(); j > 0; j--)
			{
				stream2 << (l) - ethyl[j - 1];
				if (j != ethyl.size())
				{
					stream2 << ",";
				}
			}
			stream2 << "-";
			switch (ethyl.size())
			{
			case 2:
				stream2 << "di";
				break;
			case 3:
				stream2 << "tri";
				break;
			case 4:
				stream2 << "tetra";
				break;
			case 5:
				stream2 << "penta";
				break;
			}
			stream2 << "ethyl ";
		}

		if (propyl.size() != 0)
		{
			for (j = methyl.size(); j > 0; j--)
			{
				stream2 << (l) - propyl[j - 1];
				if (j != propyl.size())
				{
					stream2 << ",";
				}
			}
			stream2 << "-";
			switch (propyl.size())
			{
			case 2:
				stream2 << "di";
				break;
			case 3:
				stream2 << "tri";
				break;
			case 4:
				stream2 << "tetra";
				break;
			case 5:
				stream2 << "penta";
				break;
			}
			stream2 << "propyl ";
		}
		if (butyl.size() != 0)
		{
			for (j = methyl.size(); j > 0; j--)
			{
				stream2 << (l) - butyl[j - 1];
				if (j != butyl.size())
				{
					stream2 << ",";
				}
			}
			stream2 << "-";
			switch (butyl.size())
			{
			case 2:
				stream2 << "di";
				break;
			case 3:
				stream2 << "tri";
				break;
			case 4:
				stream2 << "tetra";
				break;
			case 5:
				stream2 << "penta";
				break;
			}
			stream2 << "butyl ";
		}

		switch (l)
		{
		case 1:
			stream2 << "methane";
			break;
		case 2:
			stream2 << "ethane";
			break;
		case 3:
			stream2 << "propane";
			break;
		case 4:
			stream2 << "butane";
			break;
		case 5:
			stream2 << "pentane";
			break;
		case 6:
			stream2 << "hexane";
			break;
		case 7:
			stream2 << "septane";
			break;
		case 8:
			stream2 << "octane";
			break;
		case 9:
			stream2 << "nonane";
			break;
		case 10:
			stream2 << "decane";
			break;
		default:
			stream2 << l << "ane";
			break;
		}
		str2 = stream2.str();
		cout << str2;
	}
	else
	{
		cout << str;
	}
	cout << "\nPress any key to exit...";
	_getch();

	return 0;
}