//******************************************************************************

#include <iostream>
#include <cassert>

#include "Equivalence.h"
#include "Accept.h"

////////////////////////////////////////////////////////////////////////////////

using namespace std;

std::string Automate2ExpressionRationnelle(sAutoNDE at){
  //TODO définir cette fonction

  std::string sr;

  return sr;
}

////////////////////////////////////////////////////////////////////////////////

bool PseudoEquivalent(const sAutoNDE& a1, const sAutoNDE& a2, unsigned int word_size_max, string word) {
  //TODO définir cette fonction
	cout << word << "  " << word.size() << endl;
	if((Accept(a1, word) && Accept(a2, word)) || (!Accept(a1, word) && !Accept(a2, word))){
		if(word.size() == word_size_max) 
			return true;
		else{
			for(unsigned int symb=ASCII_A; symb<ASCII_Z; symb++){
				string test_word = word;
				string letter;
				letter = (char)symb;
				test_word = test_word+letter;
				 if (!PseudoEquivalent(a1, a2, word_size_max, test_word))
					 return false;
			}
			return true;
		}
	} else
		return false;
}

////////////////////////////////////////////////////////////////////////////////

bool Equivalent(const sAutoNDE& a1, const sAutoNDE& a2) {
  //TODO définir cette fonction

  return true;
}

//******************************************************************************
