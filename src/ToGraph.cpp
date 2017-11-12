//******************************************************************************

#include <iostream>
#include <fstream>
#include "ToGraph.h"

////////////////////////////////////////////////////////////////////////////////

bool ToGraph(sAutoNDE& at, std::string path){
  
  std::ofstream ofs;
  ofs.open(path, std::ofstream::out | std::ofstream::trunc); //On 'ouvre' le fichier à l'écriture

  ofs << "digraph finite_state_machine {\n";  
  ofs <<  "    rankdir=LR;\n"; 
  ofs <<  "    size=\"10,10\"\n\n"; 

  ofs <<  "	node [shape = doublecircle]; ";

  //On liste les états finaux
  for(auto& f : at.finaux){
		ofs << f << " ";
	}
  
  ofs << ";\n";
  ofs << "	node [shape = point ]; q;\n";
  ofs << "	node [shape = circle];\n\n";
  ofs << "	q -> " << at.initial << ";\n"; //On récupère l'état initial

  //On parcourt la matrice de transition état par état et on spécifit dans le fichier chaque transition(s) possible(s).
  for(std::size_t i = 0 ; i != at.trans.size() ; ++i ){
		for(std::size_t j = 0 ; j < at.trans[i].size() ; ++j ){
			for(auto& t : at.trans[i][j]){
			  ofs <<"	"<< i << " -> " << t << " [label = \"" <<  (char)(ASCII_A + j) << "\"];\n";
			}
		}
	}

  ofs << "\n";
  //On parcourt le vecteur contenant les epsilon transitions, si ils en existent, et on les spécifie.
  if( at.epsilon.size() !=0){
    for(size_t i = 0 ; i < at.epsilon.size() ; ++i ){
	   for(auto& e : at.epsilon[i]){
		   ofs <<"	"<< i << " -> " << e << " [label = \"ε\"];" << std::endl;
	   }
    }
  }
  ofs << "\n}\n";
 
  ofs.close();


  return true;
}

//******************************************************************************
