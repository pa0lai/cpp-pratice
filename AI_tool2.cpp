// AI_tool2.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <vector>
#include <cmath>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;
//std::ifstream ifs("articlelower.txt", std::ios::in);


double languageModel(string inputSentence, map<string, int>
	inputWord, int wordNumber);

string userInput;
int main()
{
	int count = 0;
	std::map<std::string, int> wordCount;

	// 開啟txt檔案
	std::ifstream inputFile("articlelower.txt");

	// 確保檔案成功打開
	if (inputFile.is_open()) {
		std::string line;

		// 逐行讀取檔案內容
		while (std::getline(inputFile, line)) {
			// 使用stringstream來分割每一行的單詞
			std::istringstream iss(line);
			std::string word;

			while (iss >> word) {
				// 插入或更新map中的計數
				wordCount[word]++;
				count++;
			}
		}

		// 關閉檔案
		inputFile.close();

		// 顯示map中的內容
		for (const auto& pair : wordCount) {
			std::cout << "Count<" << pair.first << "> = " << pair.second << std::endl;
		}
	}
	else {
		std::cerr << "無法開啟檔案." << std::endl;
	}






	cout << count << endl;
	cout << "input a sentence:";
	std::getline(std::cin, userInput);

	cout << "P('" << userInput << "') : " << languageModel(userInput, wordCount, count) << endl;

}

double languageModel(string inputSentence, map<string, int> inputWord, int wordNumber)
{
	const int T = 10, N0 = 80000;//T:threshold
	double p = 0.0, N = 0.0;
	double caculate1 = 0.0, caculateNplus1 = 0.0, caculateN = 0.0, x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0, x1Nplus1 = 0.0, y1Nplus1 = 0.0, x2Nplus1 = 0.0, y2Nplus1 = 0.0, x1N = 0.0, y1N = 0.0, x2N = 0.0, y2N = 0.0;
	int n = 0;
	map<int, int> Nword;

	stringstream testSentence(inputSentence);
	vector<string> inputVector;
	string inputToken;

	for (map<string, int>::iterator it = inputWord.begin(); it != inputWord.end(); it++)//map<string, number> 
		Nword[it->second]++;//put word number count to index, value of same index:word number count

	while (testSentence >> inputToken)
	{
		for (int i = 0; i < inputToken.length(); i++)
			inputToken[i] = tolower(inputToken[i]);//to small character
		inputVector.push_back(inputToken);

		testSentence.clear();//clear string
		testSentence.str("");//clear string
	}

	//Bayes' theorem: P(s)=P(w1)*P(w2|w1)*...*P(w_n|w_n-1)  
	if (inputVector.size() > 0)
	{
		//P(wi)=Count(wi)/sum of Count(wj)
		map<string, int>::iterator it = inputWord.find(inputVector[0]);

		if (it == inputWord.end() || it->second == 0)//Count(wi)=0 
		{
			N = N0 - inputWord.size();

			map<int, int>::iterator it1 = Nword.lower_bound(1), preit1;//find iterator>=1(map word value(word number count))

			if (it1 == Nword.begin())//equal to first iterator(map word value(word number count))
			{
				if (it1 == Nword.end())//compare last iterator(map word value(word number count) and value of same index:word number count)
					caculate1 = 1;
				else
					caculate1 = it1->second;
			}
			preit1 = it1;

			if (preit1 != Nword.begin())
			{
				preit1--;//previous iterator than it1
				x1 = preit1->first;//x1: it1 previous iterator(map word value(word number count))
			}
			y1 = preit1->second;//y1:it1 previous iterator value(value of same index:word number count) 
			x2 = it1->first;//x2:it1 iterator(map word value(word number count)) 
			y2 = it1->second;//y2:it1 iterator value(value of same index:word number count)  
			caculate1 = y1 + (y2 - y1) / (x2 - x1) * (1 - x1);

			p = (double)caculate1 / N / wordNumber;//p(wi)=N1/N 
		}
		else if (it->second < T)//0<Count(wi)<T
		{
			//Count*(wi)
			n = inputWord[inputVector[0]];

			map<int, int>::iterator itPlus1 = Nword.lower_bound(n + 1), preitPlus1;//find iterator>=n+1(map word value(word number count))

			if (itPlus1 == Nword.begin())//equal to first iterator(map word value(word number count))
			{
				if (itPlus1 == Nword.end())//compare last iterator(map word value(word number count) and value of same index:word number count)
					caculateNplus1 = 1;
				else
					caculateNplus1 = itPlus1->second;
			}
			preitPlus1 = itPlus1;

			if (preitPlus1 != Nword.begin())
			{
				preitPlus1--;//previous iterator than itPlus1
				x1Nplus1 = preitPlus1->first;//x1Nplus1: itPlus1 previous iterator(map word value(word number count))
			}
			y1Nplus1 = preitPlus1->second;//y1Nplus1:itPlus1 previous iterator value(value of same index:word number count)  
			x2Nplus1 = itPlus1->first;//x2Nplus1:itPlus1 iterator(map word value(word number count)) 
			y2Nplus1 = itPlus1->second;//y2Nplus1:itPlus1 iterator value(value of same index:word number count)  
			caculateNplus1 = y1Nplus1 + (y2Nplus1 - y1Nplus1) / (x2Nplus1 - x1Nplus1) * ((n + 1) - x1Nplus1);

			if (n == 0)
				caculateN = 80000;

			map<int, int>::iterator itN = Nword.lower_bound(n), preitN;//find iterator>=n(map word value(word number count))

			if (itN == Nword.begin())//equal to first iterator(map word value(word number count))
			{
				if (itN == Nword.end())//compare last iterator(map word value(word number count) and value of same index:word number count)
					caculateN = 1;
				else
					caculateN = itN->second;
			}
			preitN = itN;

			if (preitN != Nword.begin())
			{
				preitN--;//previous iterator than itN
				x1N = preitN->first;//x1N: itN previous iterator(map word value(word number count))
			}
			y1N = preitN->second;//y1N:itN previous iterator value(value of same index:word number count) 
			x2N = itN->first;//x2N:itN iterator(map word value(word number count)) 
			y2N = itN->second;//y2N:itN iterator value(value of same index:word number count) 
			caculateN = y1N + (y2N - y1N) / (x2N - x1N) * (n - x1N);

			if (n < T)
				p = (double)(n + 1) * (caculateNplus1 / caculateN) / wordNumber;//p(wi)=((Count(wi)+1)*NCount(wi)+1/NCount(wi))/N
			else//n>=T 
				p = (double)n / wordNumber;//p(wi)=Count(wi)/N
		}
		else//Count(wi)>T 
			p = (double)it->second / wordNumber;//p(wi)=Count(wi)/N
	}
	return p;//user input sentence probability
}
