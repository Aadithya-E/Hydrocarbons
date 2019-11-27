#include<iostream>
#include<vector>
#include<stdlib.h>>
#include<string>
#include<conio.h>

using namespace std;

bool canAddUS = true;
bool G_dBond = false;
bool G_tBond = false;

#define e_C 1
#define e_F 2
#define e_Cl 3
#define e_Br 4
#define e_I 5
#define e_AL 6
#define e_OL 7
#define e_ONE 8
#define e_OIC 9

enum bondType
{
	bond_single,
	bond_double,
	bond_triple,
	bond_undefined
};

struct Elem
{
	int type = 1;
	int valency = 4;
	int left = 4;
	int cBonds = 0;
	vector<struct Elem*> singleBonds, r_singleBonds;
	struct Elem* doubleBond;
	struct Elem* tripleBond;
	struct Elem* r_doubleBond;
	struct Elem* r_tripleBond;
	bondType fromType = bond_undefined;
	struct Elem* nextInMain = nullptr;
	struct Elem* prevInMain = nullptr;
	Elem()
	{
		type = e_C;
		valency = 4;
		left = 4;
		singleBonds.empty();
		r_singleBonds.empty();
		doubleBond = nullptr;
		r_doubleBond = nullptr;
		tripleBond = nullptr;
		r_tripleBond = nullptr;
		nextInMain = nullptr;
		fromType = bond_undefined;
		prevInMain = nullptr;
	}
	Elem(int t, bondType bond, Elem* from)
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
		doubleBond = nullptr;
		r_doubleBond = nullptr;
		tripleBond = nullptr;
		r_tripleBond = nullptr;
		nextInMain = nullptr;
		nextInMain = nullptr;
		fromType = bond;
		//prevInMain = nullptr;

		if (bond == bond_single)
		{
			r_singleBonds.push_back(from);
			left--;
		}

		else if (bond == bond_double)
		{
			r_doubleBond = from;
			left -= 2;
		}

		else if (bond == bond_triple)
		{
			r_tripleBond = from;
			left -= 3;
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
			std::cout << "Too many bonds!! Not possible!!!" << endl;
			return 1;
		}
		if (el->type == 1)
		{
			cBonds++;
		}
		return 0;
	}

	int AddDoubleBond(struct Elem* el)
	{
		if (left >= 2)
		{
			doubleBond = el;
			left -= 2;
			cBonds++;
			return 0;
		}

		else
		{
			std::cout << "Too many bonds!! Not possible!!" << endl;
			return 1;
		}
	}

	int AddTripleBond(struct Elem* el)
	{
		if (left >= 3)
		{
			tripleBond = el;
			left -= 3;
			cBonds++;
			return 0;
		}

		else
		{
			std::cout << "Too many bonds!! Not possible!!" << endl;
			return 1;
		}
	}
};

struct ChainDat
{
	int length;
	int functGrps;
	int substs;
	int dBonds;
	int tBonds;
	ChainDat()
	{
		length = 1;
		functGrps = 0;
		substs = 0;
		dBonds = 0;
		tBonds = 0;
	}
	ChainDat(int l, int d, int t) :length(l), dBonds(d), tBonds(t)
	{
		functGrps = 0;
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

int inpRevised(Elem* r, string inp, int i, bondType b)
{
	Elem* focus = r;
	for (i; i < inp.length(); i++)
	{
		if (i < 0)
		{
			return -1;
		}
		if (inp[i] == 'C')
		{
			if (b == bond_double)
			{
				if (focus->AddDoubleBond(new Elem(e_C, bond_double, r)) != 0)
				{
					return -1;
				}
				focus = focus->doubleBond;
				b = bond_single;
				continue;
			}
			if (b == bond_triple)
			{
				if (focus->AddTripleBond(new Elem(e_C, bond_triple, r)) != 0)
				{
					return -1;
				}
				focus = focus->tripleBond;
				b = bond_single;
				continue;
			}
			else
			{
				if (focus->AddSingleBond(new Elem(e_C, bond_single, r)) != 0)
				{
					return -1;
				}
				focus = focus->singleBonds.back();
				continue;
			}
		}
		if (inp[i] == '=')
		{
			if (!canAddUS)
			{
				std::cout << "Too many double or triple bonds" << endl;
				return -1;
			}
			b = bond_double;
			G_dBond = true;
			canAddUS = false;
			continue;
		}

		if (inp[i] == '#')
		{
			if (!canAddUS)
			{
				std::cout << "Too many double or triple bonds" << endl;
				return -1;
			}
			b = bond_triple;
			G_tBond = true;
			canAddUS = false;
			continue;
		}

		if (inp[i] == '(')
		{
			i = inpRevised(focus, inp, i + 1, b) - 1;
			continue;
		}
		if (inp[i] == ')')
		{
			return i + 1;
		}
	}

	return 1;
}

ChainDat findChainDat(Elem* cur)
{
	int scount = cur->singleBonds.size();
	int dcount = 0;
	(cur->doubleBond != nullptr) ? dcount = 1 : 0;
	int tcount = 0;
	(cur->tripleBond != nullptr) ? tcount = 1 : 0;
	int cCount = cur->cBonds;
	ChainDat r(1, 0, 0);
	ChainDat rec(1, 0, 0);
	ChainDat curBest(-100, -100, -100);
	int bestI = 0;
	bondType bestBond = bond_single;

	if (cCount == 0)
	{
		return r;
	}
	if (dcount == 1)
	{
		cur->nextInMain = cur->doubleBond;
		cur->doubleBond->prevInMain = cur;
		rec = findChainDat(cur->nextInMain);
		r.dBonds++;
		r.length += rec.length;
		return r;
	}
	if (tcount == 1)
	{
		cur->nextInMain = cur->tripleBond;
		cur->tripleBond->prevInMain = cur;
		rec = findChainDat(cur->nextInMain);
		r.tBonds++;
		r.length += rec.length;
		return r;
	}
	if (cCount == 1)
	{
		for (int i = 0; i < scount; i++)
		{
			if (cur->singleBonds[i]->type == e_C)
			{
				cur->nextInMain = cur->singleBonds[i];
				cur->singleBonds[i]->prevInMain = cur;
				rec = findChainDat(cur->nextInMain);
				//r.functGrps += rec.functGrps;
				//r.substs += rec.substs;
				r.length += rec.length;
				r.dBonds += rec.dBonds;
				r.tBonds += rec.tBonds;
				return r;
			}
		}
	}
	else
	{
		for (int i = 0; i < scount; i++)
		{
			if (cur->singleBonds[i]->cBonds == 0)
			{
				continue;
			}
			if (curBest.length < 0)
			{
				curBest = findChainDat(cur->singleBonds[i]);
				bestI = i;
				continue;
			}
			rec = findChainDat(cur->singleBonds[i]);
			if (rec.tBonds > curBest.tBonds)
			{
				curBest = rec;
				bestI = i;
				continue;
			}
			if (rec.tBonds == curBest.tBonds)
			{
				if (rec.dBonds > curBest.dBonds)
				{
					curBest = rec;
					bestI = i;
					continue;
				}
				if (rec.dBonds == curBest.dBonds)
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
		//r.substs += curBest.substs;
		r.length += curBest.length;
		//r.functGrps += curBest.functGrps;
		r.dBonds += curBest.dBonds;
		r.tBonds += curBest.tBonds;
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
	return SubstsDat(0, 0, 0, 0, 0);
}

int main()
{
	vector<Elem*> mainChain;
	bool reverse = false;
	Elem* inpChain = new Elem(e_C, bond_undefined, nullptr);
	int inp;
	std::cout << "Enter the structural formula of the alkane in the reuqested format."
		"\nEnsure that the first carbon, which is added by default, is part of the main chain.";
	string chain;
	cin >> chain;
	if (inpRevised(inpChain, chain, 0, bond_undefined) < 0)
	{
		return 1;
	}
	inpChain = inpChain;
	Elem* index = inpChain;
	int l = 0;
	ChainDat x = findChainDat(inpChain);
	vector<int> methyl, ethyl, propyl, butyl, ene, yne;
	while (index != nullptr)
	{
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

	int smallest = INT_MAX;
	int largest = -1;

	for (int i = 0; i < methyl.size(); i++)
	{
		if (methyl[i] < smallest)
		{
			smallest = methyl[i];
		}
		if (methyl[i] > largest)
		{
			largest = methyl[i];
		}
	}
	for (int i = 0; i < ethyl.size(); i++)
	{
		if (ethyl[i] < smallest)
		{
			smallest = ethyl[i];
		}
		if (ethyl[i] > largest)
		{
			largest = ethyl[i];
		}
	}
	for (int i = 0; i < propyl.size(); i++)
	{
		if (propyl[i] < smallest)
		{
			smallest = propyl[i];
		}
		if (propyl[i] > largest)
		{
			largest = propyl[i];
		}
	}
	for (int i = 0; i < butyl.size(); i++)
	{
		if (butyl[i] < smallest)
		{
			smallest = butyl[i];
		}
		if (butyl[i] > largest)
		{
			largest = butyl[i];
		}
	}

	string str;

	int firstD = 1;
	int firstT = 1;
	bool stop = false;
	Elem* ptr = inpChain;
	if (G_dBond || G_tBond)
	{
		while (!stop)
		{
			if (ptr->nextInMain == nullptr)
			{
				break;
			}
			if (ptr->nextInMain->fromType == bond_double)
			{
				break;
			}
			if (ptr->nextInMain->fromType == bond_triple)
			{
				break;
			}
			ptr = ptr->nextInMain;
			firstD++;
			firstT++;
		}
	}

	if (G_tBond)
	{
		if (firstT > l / 2)
		{
			if (methyl.size() != 0)
			{
				for (int i = 0; i < methyl.size(); i++)
				{
					(i != methyl.size() - 1) ? std::cout << l + 1 - methyl[i] << "," : std::cout << methyl[i];
				}
				std::cout << "-";
				switch (methyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << methyl.size() << ")";
					break;
				}
				std::cout << "methyl ";
			}
			if (ethyl.size() != 0)
			{
				for (int i = 0; i < ethyl.size(); i++)
				{
					(i != ethyl.size() - 1) ? std::cout << l + 1 - ethyl[i] << "," : std::cout << ethyl[i];
				}
				std::cout << "-";
				switch (ethyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << ethyl.size() << ")";
					break;
				}
				std::cout << "-ethyl ";
			}
			if (propyl.size() != 0)
			{
				for (int i = 0; i < propyl.size(); i++)
				{
					(i != propyl.size() - 1) ? std::cout << l + 1 - propyl[i] << "," : std::cout << propyl[i];
				}
				std::cout << "-";
				switch (propyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "di";
					break;
				case 3:
					std::cout << "tri";
					break;
				case 4:
					std::cout << "tetra";
					break;
				case 5:
					std::cout << "penta";
					break;
				default:
					std::cout << "(" << propyl.size() << ")";
					break;
				}
				std::cout << "propyl ";
			}
			if (butyl.size() != 0)
			{
				for (int i = 0; i < butyl.size(); i++)
				{
					(i != butyl.size() - 1) ? std::cout << l + 1 - butyl[i] << "," : std::cout << butyl[i];
				}
				std::cout << "-";
				switch (butyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << butyl.size() << ")";
					break;
				}
				std::cout << "butyl ";
			}
			switch (l)
			{
			case 2:
				std::cout << "eth-";
				break;
			case 3:
				std::cout << "prop-";
				break;
			case 4:
				std::cout << "but-";
				break;
			case 5:
				std::cout << "pent-";
				break;
			case 6:
				std::cout << "hex-";
				break;
			case 7:
				std::cout << "sept-";
				break;
			case 8:
				std::cout << "oct-";
				break;
			case 9:
				std::cout << "non-";
				break;
			case 10:
				std::cout << "dec-";
				break;
			default:
				std::cout << "(" << l << ")-";
				break;
			}
			std::cout << l - firstT << "-yne";
		}
		else
		{
			if (methyl.size() != 0)
			{
				for (int i = 0; i < methyl.size(); i++)
				{
					(i != methyl.size() - 1) ? std::cout << methyl[i] << "," : std::cout << methyl[i];
				}
				std::cout << "-";
				switch (methyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << methyl.size() << ")";
					break;
				}
				std::cout << "methyl ";
			}
			if (ethyl.size() != 0)
			{
				for (int i = 0; i < ethyl.size(); i++)
				{
					(i != ethyl.size() - 1) ? std::cout << ethyl[i] << "," : std::cout << ethyl[i];
				}
				std::cout << "-";
				switch (ethyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << ethyl.size() << ")";
					break;
				}
				std::cout << "ethyl ";
			}
			if (propyl.size() != 0)
			{
				for (int i = 0; i < propyl.size(); i++)
				{
					(i != propyl.size() - 1) ? std::cout << propyl[i] << "," : std::cout << propyl[i];
				}
				std::cout << "-";
				switch (propyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << propyl.size() << ")";
					break;
				}
				std::cout << "propyl ";
			}
			if (butyl.size() != 0)
			{
				for (int i = 0; i < butyl.size(); i++)
				{
					(i != butyl.size() - 1) ? std::cout << butyl[i] << "," : std::cout << butyl[i];
				}
				std::cout << "-";
				switch (butyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << butyl.size() << ")";
					break;
				}
				std::cout << "butyl ";
			}
			switch (l)
			{
			case 2:
				std::cout << "eth-";
				break;
			case 3:
				std::cout << "prop-";
				break;
			case 4:
				std::cout << "but-";
				break;
			case 5:
				std::cout << "pent-";
				break;
			case 6:
				std::cout << "hex-";
				break;
			case 7:
				std::cout << "sept-";
				break;
			case 8:
				std::cout << "oct-";
				break;
			case 9:
				std::cout << "non-";
				break;
			case 10:
				std::cout << "dec-";
				break;
			default:
				std::cout << "(" << l << ")-";
				break;
			}
			std::cout << firstT << "-yne";
		}
	}

	else if (G_dBond)
	{
		if (firstD > l / 2)
		{
			if (methyl.size() != 0)
			{
				for (int i = 0; i < methyl.size(); i++)
				{
					(i != methyl.size() - 1) ? std::cout << l + 1 - methyl[i] << "," : std::cout << methyl[i];
				}
				std::cout << "-";
				switch (methyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << methyl.size() << ")";
					break;
				}
				std::cout << "methyl ";
			}
			if (ethyl.size() != 0)
			{
				for (int i = 0; i < ethyl.size(); i++)
				{
					(i != ethyl.size() - 1) ? std::cout << l + 1 - ethyl[i] << "," : std::cout << ethyl[i];
				}
				std::cout << "-";
				switch (ethyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << ethyl.size() << ")";
					break;
				}
				std::cout << "ethyl ";
			}
			if (propyl.size() != 0)
			{
				for (int i = 0; i < propyl.size(); i++)
				{
					(i != propyl.size() - 1) ? std::cout << l + 1 - propyl[i] << "," : std::cout << propyl[i];
				}
				std::cout << "-";
				switch (propyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << propyl.size() << ")";
					break;
				}
				std::cout << "propyl ";
			}
			if (butyl.size() != 0)
			{
				for (int i = 0; i < butyl.size(); i++)
				{
					(i != butyl.size() - 1) ? std::cout << l + 1 - butyl[i] << "," : std::cout << butyl[i];
				}
				std::cout << "-";
				switch (butyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << butyl.size() << ")";
					break;
				}
				std::cout << "butyl ";
			}
			switch (l)
			{
			case 2:
				std::cout << "eth-";
				break;
			case 3:
				std::cout << "prop-";
				break;
			case 4:
				std::cout << "but-";
				break;
			case 5:
				std::cout << "pent-";
				break;
			case 6:
				std::cout << "hex-";
				break;
			case 7:
				std::cout << "sept-";
				break;
			case 8:
				std::cout << "oct-";
				break;
			case 9:
				std::cout << "non-";
				break;
			case 10:
				std::cout << "dec-";
				break;
			default:
				std::cout << "(" << l << ")-";
				break;
			}
			std::cout << l - firstD << "-ene";
		}
		else
		{
			if (methyl.size() != 0)
			{
				for (int i = 0; i < methyl.size(); i++)
				{
					(i != methyl.size() - 1) ? std::cout << methyl[i] << "," : std::cout << methyl[i];
				}
				std::cout << "-";
				switch (methyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << methyl.size() << ")";
					break;
				}
				std::cout << "methyl ";
			}
			if (ethyl.size() != 0)
			{
				for (int i = 0; i < ethyl.size(); i++)
				{
					(i != ethyl.size() - 1) ? std::cout << ethyl[i] << "," : std::cout << ethyl[i];
				}
				std::cout << "-";
				switch (ethyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << ethyl.size() << ")";
					break;
				}
				std::cout << "ethyl ";
			}
			if (propyl.size() != 0)
			{
				for (int i = 0; i < propyl.size(); i++)
				{
					(i != propyl.size() - 1) ? std::cout << propyl[i] << "," : std::cout << propyl[i];
				}
				std::cout << "-";
				switch (propyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << propyl.size() << ")";
					break;
				}
				std::cout << "propyl ";
			}
			if (butyl.size() != 0)
			{
				for (int i = 0; i < butyl.size(); i++)
				{
					(i != butyl.size() - 1) ? std::cout << butyl[i] << "," : std::cout << butyl[i];
				}
				std::cout << "-";
				switch (butyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << butyl.size() << ")";
					break;
				}
				std::cout << "butyl ";
			}
			switch (l)
			{
			case 2:
				std::cout << "eth-";
				break;
			case 3:
				std::cout << "prop-";
				break;
			case 4:
				std::cout << "but-";
				break;
			case 5:
				std::cout << "pent-";
				break;
			case 6:
				std::cout << "hex-";
				break;
			case 7:
				std::cout << "sept-";
				break;
			case 8:
				std::cout << "oct-";
				break;
			case 9:
				std::cout << "non-";
				break;
			case 10:
				std::cout << "dec-";
				break;
			default:
				std::cout << "(" << l << ")-";
				break;
			}
			std::cout << firstD << "-ene";
		}
	}

	else
	{
		if ((l + 1) - largest < smallest)
		{
			if (methyl.size() != 0)
			{
				for (int i = 0; i < methyl.size(); i++)
				{
					(i != methyl.size() - 1) ? std::cout << l + 1 - methyl[i] << "," : std::cout << methyl[i];
				}
				std::cout << "-";
				switch (methyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << methyl.size() << ")";
					break;
				}
				std::cout << "methyl ";
			}
			if (ethyl.size() != 0)
			{
				for (int i = 0; i < ethyl.size(); i++)
				{
					(i != ethyl.size() - 1) ? std::cout << l + 1 - ethyl[i] << "," : std::cout << ethyl[i];
				}
				std::cout << "-";
				switch (ethyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << ethyl.size() << ")";
					break;
				}
				std::cout << "ethyl ";
			}
			if (propyl.size() != 0)
			{
				for (int i = 0; i < propyl.size(); i++)
				{
					(i != propyl.size() - 1) ? std::cout << l + 1 - propyl[i] << "," : std::cout << propyl[i];
				}
				std::cout << "-";
				switch (propyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << propyl.size() << ")";
					break;
				}
				std::cout << "propyl ";
			}
			if (butyl.size() != 0)
			{
				for (int i = 0; i < butyl.size(); i++)
				{
					(i != butyl.size() - 1) ? std::cout << l + 1 - butyl[i] << "," : std::cout << butyl[i];
				}
				std::cout << "-";
				switch (butyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << butyl.size() << ")";
					break;
				}
				std::cout << "butyl ";
			}
			switch (l)
			{
			case 1:
				std::cout << "methane";
			case 2:
				std::cout << "ethane";
				break;
			case 3:
				std::cout << "propane";
				break;
			case 4:
				std::cout << "butane";
				break;
			case 5:
				std::cout << "pentane";
				break;
			case 6:
				std::cout << "hexane";
				break;
			case 7:
				std::cout << "septane";
				break;
			case 8:
				std::cout << "octane";
				break;
			case 9:
				std::cout << "nonane";
				break;
			case 10:
				std::cout << "decane";
				break;
			default:
				std::cout << "(" << l << ")ane";
				break;
			}
		}
		else
		{
			if (methyl.size() != 0)
			{
				for (int i = 0; i < methyl.size(); i++)
				{
					(i != methyl.size() - 1) ? std::cout << methyl[i] << "," : std::cout << methyl[i];
				}
				std::cout << "-";
				switch (methyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << methyl.size() << ")";
					break;
				}
				std::cout << "methyl ";
			}
			if (ethyl.size() != 0)
			{
				for (int i = 0; i < ethyl.size(); i++)
				{
					(i != ethyl.size() - 1) ? std::cout << ethyl[i] << "," : std::cout << ethyl[i];
				}
				std::cout << "-";
				switch (ethyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << ethyl.size() << ")";
					break;
				}
				std::cout << "ethyl ";
			}
			if (propyl.size() != 0)
			{
				for (int i = 0; i < propyl.size(); i++)
				{
					(i != propyl.size() - 1) ? std::cout << propyl[i] << "," : std::cout << propyl[i];
				}
				std::cout << "-";
				switch (propyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << propyl.size() << ")";
					break;
				}
				std::cout << "propyl ";
			}
			if (butyl.size() != 0)
			{
				for (int i = 0; i < butyl.size(); i++)
				{
					(i != butyl.size() - 1) ? std::cout << butyl[i] << "," : std::cout << butyl[i];
				}
				std::cout << "-";
				switch (butyl.size())
				{
				case 1:
					break;
				case 2:
					std::cout << "-di";
					break;
				case 3:
					std::cout << "-tri";
					break;
				case 4:
					std::cout << "-tetra";
					break;
				case 5:
					std::cout << "-penta";
					break;
				default:
					std::cout << "-(" << butyl.size() << ")";
					break;
				}
				std::cout << "butyl ";
			}
			switch (l)
			{
			case 1:
				std::cout << "methane";
			case 2:
				std::cout << "ethane";
				break;
			case 3:
				std::cout << "propane";
				break;
			case 4:
				std::cout << "butane";
				break;
			case 5:
				std::cout << "pentane";
				break;
			case 6:
				std::cout << "hexane";
				break;
			case 7:
				std::cout << "septane";
				break;
			case 8:
				std::cout << "octane";
				break;
			case 9:
				std::cout << "nonane";
				break;
			case 10:
				std::cout << "decane";
				break;
			default:
				std::cout << "(" << l << ")ane";
				break;
			}
		}
	}

	/*int j = 0;
	if (methyl.size() != 0)
	{
		for (j = 0; j < methyl.size(); j++)
		{
			std::cout << methyl[j] + 1;
			if (methyl[j] + 1 < smallest)
			{
				smallest = methyl[j] + 1;
			}
			if (methyl[j] + 1 > largest)
			{
				largest = methyl[j] + 1;
			}
			if (methyl.size() - j != 1)
			{
				std::cout << ",";
			}
		}
		std::cout << "-";
		switch (methyl.size())
		{
		case 2:
			std::cout << "di";
			break;
		case 3:
			std::cout << "tri";
			break;
		case 4:
			std::cout << "tetra";
			break;
		case 5:
			std::cout << "penta";
			break;
		}
		std::cout << "methyl ";
	}
	if (ethyl.size() != 0)
	{
		for (j = 0; j < ethyl.size(); j++)
		{
			std::cout << ethyl[j] + 1;
			if (ethyl.size() - j != 1)
			{
				std::cout << ",";
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
		std::cout << "-";
		switch (ethyl.size())
		{
		case 2:
			std::cout << "di";
			break;
		case 3:
			std::cout << "tri";
			break;
		case 4:
			std::cout << "tetra";
			break;
		case 5:
			std::cout << "penta";
			break;
		}
		std::cout << "ethyl ";
	}

	if (propyl.size() != 0)
	{
		for (j = 0; j < propyl.size(); j++)
		{
			std::cout << propyl[j] + 1;
			if (propyl.size() - j != 1)
			{
				std::cout << ",";
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
		std::cout << "-";
		switch (propyl.size())
		{
		case 2:
			std::cout << "di";
			break;
		case 3:
			std::cout << "tri";
			break;
		case 4:
			std::cout << "tetra";
			break;
		case 5:
			std::cout << "penta";
			break;
		}
		std::cout << "propyl ";
	}
	if (butyl.size() != 0)
	{
		for (j = 0; j < butyl.size(); j++)
		{
			std::cout << butyl[j] + 1;
			if (butyl.size() - j != 1)
			{
				std::cout << ",";
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
		std::cout << "-";
		switch (butyl.size())
		{
		case 2:
			std::cout << "di";
			break;
		case 3:
			std::cout << "tri";
			break;
		case 4:
			std::cout << "tetra";
			break;
		case 5:
			std::cout << "penta";
			break;
		}
		std::cout << "butyl ";
	}
	switch (l)
	{
	case 1:
		std::cout << "meth";
		break;
	case 2:
		std::cout << "eth";
		break;
	case 3:
		std::cout << "prop";
		break;
	case 4:
		std::cout << "but";
		break;
	case 5:
		std::cout << "pent";
		break;
	case 6:
		std::cout << "hex";
		break;
	case 7:
		std::cout << "sept";
		break;
	case 8:
		std::cout << "oct";
		break;
	case 9:
		std::cout << "non";
		break;
	case 10:
		std::cout << "dec";
		break;
	default:
		std::cout << l;
		break;
	}
	str = std::cout.str();
	if (G_tBond)
	{
		if ((float)firstT > (float)l / 2.00f)
		{
			stringstd::cout std::cout2(str);
			if (methyl.size() != 0)
			{
				for (j = methyl.size(); j > 0; j--)
				{
					std::cout2 << (l)-methyl[j - 1];
					if (j != methyl.size())
					{
						std::cout2 << ",";
					}
				}
				std::cout2 << "-";
				switch (methyl.size())
				{
				case 2:
					std::cout2 << "di";
					break;
				case 3:
					std::cout2 << "tri";
					break;
				case 4:
					std::cout2 << "tetra";
					break;
				case 5:
					std::cout2 << "penta";
					break;
				}
				std::cout2 << "methyl ";
			}
			if (ethyl.size() != 0)
			{
				for (j = ethyl.size(); j > 0; j--)
				{
					std::cout2 << (l)-ethyl[j - 1];
					if (j != ethyl.size())
					{
						std::cout2 << ",";
					}
				}
				std::cout2 << "-";
				switch (ethyl.size())
				{
				case 2:
					std::cout2 << "di";
					break;
				case 3:
					std::cout2 << "tri";
					break;
				case 4:
					std::cout2 << "tetra";
					break;
				case 5:
					std::cout2 << "penta";
					break;
				}
				std::cout2 << "ethyl ";
			}

			if (propyl.size() != 0)
			{
				for (j = methyl.size(); j > 0; j--)
				{
					std::cout2 << (l)-propyl[j - 1];
					if (j != propyl.size())
					{
						std::cout2 << ",";
					}
				}
				std::cout2 << "-";
				switch (propyl.size())
				{
				case 2:
					std::cout2 << "di";
					break;
				case 3:
					std::cout2 << "tri";
					break;
				case 4:
					std::cout2 << "tetra";
					break;
				case 5:
					std::cout2 << "penta";
					break;
				}
				std::cout2 << "propyl ";
			}
			if (butyl.size() != 0)
			{
				for (j = methyl.size(); j > 0; j--)
				{
					std::cout2 << (l)-butyl[j - 1];
					if (j != butyl.size())
					{
						std::cout2 << ",";
					}
				}
				std::cout2 << "-";
				switch (butyl.size())
				{
				case 2:
					std::cout2 << "di";
					break;
				case 3:
					std::cout2 << "tri";
					break;
				case 4:
					std::cout2 << "tetra";
					break;
				case 5:
					std::cout2 << "penta";
					break;
				}
				std::cout2 << "butyl ";
			}

			switch (l)
			{
			case 1:
				std::cout2 << "meth";
				break;
			case 2:
				std::cout2 << "eth";
				break;
			case 3:
				std::cout2 << "prop";
				break;
			case 4:
				std::cout2 << "but";
				break;
			case 5:
				std::cout2 << "pent";
				break;
			case 6:
				std::cout2 << "hex";
				break;
			case 7:
				std::cout2 << "sept";
				break;
			case 8:
				std::cout2 << "oct";
				break;
			case 9:
				std::cout2 << "non";
				break;
			case 10:
				std::cout2 << "dec";
				break;
			default:
				std::cout2 << l;
				break;
			}
			std::cout << '-' << l - 1 - firstT << "-yne" << endl;
			str = std::cout2.str();
		}
	}
	else if (G_dBond)
	{
		if ((float)firstD > (float)l / 2.00f)
		{
			stringstd::cout std::cout2(str);
			if (methyl.size() != 0)
			{
				for (j = methyl.size(); j > 0; j--)
				{
					std::cout2 << (l)-methyl[j - 1];
					if (j != methyl.size())
					{
						std::cout2 << ",";
					}
				}
				std::cout2 << "-";
				switch (methyl.size())
				{
				case 2:
					std::cout2 << "di";
					break;
				case 3:
					std::cout2 << "tri";
					break;
				case 4:
					std::cout2 << "tetra";
					break;
				case 5:
					std::cout2 << "penta";
					break;
				}
				std::cout2 << "methyl ";
			}
			if (ethyl.size() != 0)
			{
				for (j = ethyl.size(); j > 0; j--)
				{
					std::cout2 << (l)-ethyl[j - 1];
					if (j != ethyl.size())
					{
						std::cout2 << ",";
					}
				}
				std::cout2 << "-";
				switch (ethyl.size())
				{
				case 2:
					std::cout2 << "di";
					break;
				case 3:
					std::cout2 << "tri";
					break;
				case 4:
					std::cout2 << "tetra";
					break;
				case 5:
					std::cout2 << "penta";
					break;
				}
				std::cout2 << "ethyl ";
			}

			if (propyl.size() != 0)
			{
				for (j = methyl.size(); j > 0; j--)
				{
					std::cout2 << (l)-propyl[j - 1];
					if (j != propyl.size())
					{
						std::cout2 << ",";
					}
				}
				std::cout2 << "-";
				switch (propyl.size())
				{
				case 2:
					std::cout2 << "di";
					break;
				case 3:
					std::cout2 << "tri";
					break;
				case 4:
					std::cout2 << "tetra";
					break;
				case 5:
					std::cout2 << "penta";
					break;
				}
				std::cout2 << "propyl ";
			}
			if (butyl.size() != 0)
			{
				for (j = methyl.size(); j > 0; j--)
				{
					std::cout2 << (l)-butyl[j - 1];
					if (j != butyl.size())
					{
						std::cout2 << ",";
					}
				}
				std::cout2 << "-";
				switch (butyl.size())
				{
				case 2:
					std::cout2 << "di";
					break;
				case 3:
					std::cout2 << "tri";
					break;
				case 4:
					std::cout2 << "tetra";
					break;
				case 5:
					std::cout2 << "penta";
					break;
				}
				std::cout2 << "butyl ";
			}

			switch (l)
			{
			case 1:
				std::cout2 << "meth";
				break;
			case 2:
				std::cout2 << "eth";
				break;
			case 3:
				std::cout2 << "prop";
				break;
			case 4:
				std::cout2 << "but";
				break;
			case 5:
				std::cout2 << "pent";
				break;
			case 6:
				std::cout2 << "hex";
				break;
			case 7:
				std::cout2 << "sept";
				break;
			case 8:
				std::cout2 << "oct";
				break;
			case 9:
				std::cout2 << "non";
				break;
			case 10:
				std::cout2 << "dec";
				break;
			default:
				std::cout2 << l;
				break;
			}
			std::cout << '-' << l - 1 - firstD << "-ene" << endl;
			str = std::cout2.str();
		}
		else
		{
			std::cout << "-" << firstD << "-ene" << endl;
		}
	}
	else if ((l - smallest) + 1 < smallest)
	{
		//string str2;
		stringstd::cout std::cout2(str);
		if (methyl.size() != 0)
		{
			for (j = methyl.size(); j > 0; j--)
			{
				std::cout2 << (l)-methyl[j - 1];
				if (j != methyl.size())
				{
					std::cout2 << ",";
				}
			}
			std::cout2 << "-";
			switch (methyl.size())
			{
			case 2:
				std::cout2 << "di";
				break;
			case 3:
				std::cout2 << "tri";
				break;
			case 4:
				std::cout2 << "tetra";
				break;
			case 5:
				std::cout2 << "penta";
				break;
			}
			std::cout2 << "methyl ";
		}
		if (ethyl.size() != 0)
		{
			for (j = ethyl.size(); j > 0; j--)
			{
				std::cout2 << (l)-ethyl[j - 1];
				if (j != ethyl.size())
				{
					std::cout2 << ",";
				}
			}
			std::cout2 << "-";
			switch (ethyl.size())
			{
			case 2:
				std::cout2 << "di";
				break;
			case 3:
				std::cout2 << "tri";
				break;
			case 4:
				std::cout2 << "tetra";
				break;
			case 5:
				std::cout2 << "penta";
				break;
			}
			std::cout2 << "ethyl ";
		}

		if (propyl.size() != 0)
		{
			for (j = methyl.size(); j > 0; j--)
			{
				std::cout2 << (l)-propyl[j - 1];
				if (j != propyl.size())
				{
					std::cout2 << ",";
				}
			}
			std::cout2 << "-";
			switch (propyl.size())
			{
			case 2:
				std::cout2 << "di";
				break;
			case 3:
				std::cout2 << "tri";
				break;
			case 4:
				std::cout2 << "tetra";
				break;
			case 5:
				std::cout2 << "penta";
				break;
			}
			std::cout2 << "propyl ";
		}
		if (butyl.size() != 0)
		{
			for (j = methyl.size(); j > 0; j--)
			{
				std::cout2 << (l)-butyl[j - 1];
				if (j != butyl.size())
				{
					std::cout2 << ",";
				}
			}
			std::cout2 << "-";
			switch (butyl.size())
			{
			case 2:
				std::cout2 << "di";
				break;
			case 3:
				std::cout2 << "tri";
				break;
			case 4:
				std::cout2 << "tetra";
				break;
			case 5:
				std::cout2 << "penta";
				break;
			}
			std::cout2 << "butyl ";
		}

		switch (l)
		{
		case 1:
			std::cout2 << "methane";
			break;
		case 2:
			std::cout2 << "ethane";
			break;
		case 3:
			std::cout2 << "propane";
			break;
		case 4:
			std::cout2 << "butane";
			break;
		case 5:
			std::cout2 << "pentane";
			break;
		case 6:
			std::cout2 << "hexane";
			break;
		case 7:
			std::cout2 << "septane";
			break;
		case 8:
			std::cout2 << "octane";
			break;
		case 9:
			std::cout2 << "nonane";
			break;
		case 10:
			std::cout2 << "decane";
			break;
		default:
			std::cout2 << l << "ane";
			break;
		}
		str = std::cout2.str();
		//std::std::cout << str2;
	}*/
PRINT:
	//std::std::cout << str;
	std::cout << "\nPress any key to exit...";
	_getch();
	return 0;
}