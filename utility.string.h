#pragma once

namespace String
{
	static void Reshape(std::string inputString, std::vector<unsigned> counts, std::vector<std::string>& strVec) {
		strVec.resize(counts.size());

		std::vector<unsigned> accumPos(counts.size() + 1, 0);

		unsigned total = 0;
		for (unsigned i = 0; i < counts.size(); i++)
		{
			total += counts[i];
			accumPos[i + 1] = total;
		}

		for (std::size_t i = 1; i < accumPos.size(); i++)
			strVec[i - 1] = inputString.substr(accumPos[i - 1], counts[i - 1]);
	}


	static unsigned SumTotalLen(std::vector<std::string>& retrived)	{
		return std::accumulate(retrived.begin(), retrived.end(), 0, [&](unsigned sum, auto iter) { return sum + iter.size(); });
	}

	
	static int CountLeadingMatches(std::string& _1st, std::string& _2nd) {
		int count = 0;
		for (std::size_t i = 0; i < std::min(_1st.size(), _2nd.size()); i++)
		{
			if (_1st[i] == _2nd[i])
				count++;
			else
				break;
		}
		return count;
	}

	static void ExtractSubString(std::vector<std::string>& stringVec, unsigned begin, unsigned end, std::vector<std::string>& ancestorPaths)	{
		if (end - begin == stringVec.begin()->size())
		{
			ancestorPaths = stringVec;
			return;
		}

		ancestorPaths.resize(stringVec.size());
		tbb::parallel_for(std::size_t(0), stringVec.size(), [&](std::size_t i)
		{
			ancestorPaths[i] = stringVec[i].substr(begin, end);
		}
		);
	}

	static std::vector<char> ToVec(std::string string)	{
		std::vector<char> vec(string.size());

		std::copy(string.begin(), string.end(), vec.begin());
		return vec;
	}
	
	// strings of same length
	static void ConcatenateUniformStrings(std::vector<std::string>& strVec, std::string& concatenated)	{
		int unitLength = strVec.begin()->size();

		unsigned totalUnits = strVec.size();

		concatenated.resize(totalUnits * unitLength, '0');

		for (std::size_t i = 0; i < strVec.size(); i++)
			concatenated.replace(i * unitLength, unitLength, strVec[i]);
	}

	static int HexToIdx(char c)	{
		int branch = c;

		if (!(branch >= 48 && branch <= 57) && !(branch >= 97 && branch <= 102))
			branch = -1;

		if (branch >= 48 && branch <= 57)  // [0, 9]
			branch = branch - 48;      // to index

		if (branch >= 97 && branch <= 102) // [a, f]
		{
			branch = branch - 97 + 10; // to index
		}

		return branch;
	}
};
