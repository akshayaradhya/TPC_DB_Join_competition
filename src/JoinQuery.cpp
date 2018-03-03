#include "../include/JoinQuery.hpp"
#include <assert.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include<unordered_set>
#include <sstream>
#include <string>
//---------------------------------------------------------------------------
using namespace std;
using std::unordered_map;

std::string lPath,oPath,cPath;
//size_t machinery_avg,building_avg,automobile_avg,furniture_avg,household_avg;
std::unordered_map<std::string, size_t> avg_map;

JoinQuery::JoinQuery(std::string lineitem, std::string order,
                     std::string customer)
{
    lPath = lineitem;
    oPath = order ;
    cPath = customer;
	
    std::unordered_map<std::string,std::unordered_set<size_t>> cust_map = custmap_gen(cPath);
    // std::unordered_map<size_t,size_t> lineitem_map = lineitem_mapgen(lPath);
    // machinery_avg = average_segment(oPath,lineitem_map,cust_map["MACHINERY"]); //Check if its actually going the way you want
    // building_avg = average_segment(oPath,lineitem_map,cust_map["BUILDING"]);
    // automobile_avg = average_segment(oPath,lineitem_map,cust_map["AUTOMOBILE"]);
    // furniture_avg = average_segment(oPath,lineitem_map,cust_map["FURNITURE"]);
    // household_avg = average_segment(oPath,lineitem_map,cust_map["HOUSEHOLD"]);
	std::unordered_set<size_t> order_set_machinery, order_set_building, order_set_automobile, order_set_furniture, order_set_household;
	order_set_machinery = ordermap_gen(oPath, cust_map["MACHINERY"]);
	order_set_building = ordermap_gen(oPath, cust_map["BUILDING"]);
	order_set_automobile = ordermap_gen(oPath, cust_map["AUTOMOBILE"]);
	order_set_furniture = ordermap_gen(oPath, cust_map["FURNITURE"]);
	order_set_household = ordermap_gen(oPath, cust_map["HOUSEHOLD"]);
	std::unordered_map<std::string, std::unordered_set<size_t>> order_map;
	order_map["MACHINERY"] = order_set_machinery;
	order_map["BUILDING"] = order_set_building;
	order_map["AUTOMOBILE"] = order_set_automobile;
	order_map["FURNITURE"] = order_set_furniture;
	order_map["HOUSEHOLD"] = order_set_household;
	
	avg_map = avgmap_gen(lPath,order_map);
}





//---------------------------------------------------------------------------
size_t JoinQuery::avg(std::string segmentParam)
{    
    // if(segmentParam == "MACHINERY")
		// return machinery_avg;
	// else if(segmentParam == "BUILDING")
		// return building_avg;
	// else if(segmentParam == "AUTOMOBILE")
		// return automobile_avg;
	// else if(segmentParam == "FURNITURE")
		// return furniture_avg;
	// else if(segmentParam == "HOUSEHOLD")
		// return household_avg;
	// return 1;
	
	return avg_map[segmentParam];
	
}
//---------------------------------------------------------------------------
size_t JoinQuery::lineCount(std::string rel)
{
   std::ifstream relation(rel);
   assert(relation);  // make sure the provided string references a file
   size_t n = 0;
   for (std::string line; std::getline(relation, line);) n++;
   return n;
}

std::unordered_map<std::string,std::unordered_set<size_t>> JoinQuery::custmap_gen(std::string cPath)
{
    std::unordered_map<std::string,std::unordered_set<size_t>> cust_map;
    std::ifstream relation(cPath);
	assert(relation);
    size_t cust_ID;
    size_t word_count = 0;
    std::unordered_set<size_t> cust_set_automobile;
    std::unordered_set<size_t> cust_set_building;
    std::unordered_set<size_t> cust_set_furniture;
    std::unordered_set<size_t> cust_set_household;
    std::unordered_set<size_t> cust_set_machinery;
	std::string line;
    while(std::getline(relation,line,'\n'))
    while (std::getline(relation, line,'|')) //try to use 2 loops and break
    {
        if(word_count == 0)
        {
			cust_ID=std::stoi(line);
        }

		else if(word_count == 6)
		{
            if(line.compare("BUILDING") == 0)
			    cust_set_building.insert(cust_ID); 
            else if(line.compare("MACHINERY") == 0)
			    cust_set_machinery.insert(cust_ID);
            else if(line.compare("FURNITURE") == 0)
			    cust_set_furniture.insert(cust_ID);
            else if(line.compare("AUTOMOBILE") == 0)
			    cust_set_automobile.insert(cust_ID); 
            else if(line.compare("HOUSEHOLD") == 0)
			    cust_set_household.insert(cust_ID); 
			word_count = 0;
            break;
		}
		word_count++;
    }
    
    cust_map.insert(std::make_pair("AUTOMOBILE",cust_set_automobile)); //prsize_t one of them and check
    cust_map.insert(std::make_pair("BUILDING",cust_set_building));
    cust_map.insert(std::make_pair("FURNITURE",cust_set_furniture));
    cust_map.insert(std::make_pair("HOUSEHOLD",cust_set_household));
    cust_map.insert(std::make_pair("MACHINERY",cust_set_machinery));
    return cust_map; //prsize_t the map
}



std::unordered_map<std::string,size_t> JoinQuery::avgmap_gen(std::string lPath, std::unordered_map<std::string, std::unordered_set<size_t>> ordermap)
{
    std::unordered_map<std::string,size_t> average_map;
    std::ifstream relation_l(lPath);
    assert(relation_l); 
    size_t ord_ID_l;
    size_t word_count_l = 0;
	std::string line;
	double sum_building = 0, sum_machinery = 0, sum_automobile = 0, sum_household = 0, sum_furniture = 0;
	double count_building = 0, count_machinery = 0, count_automobile = 0, count_household = 0, count_furniture = 0;
	std::unordered_set<size_t> building_cust_set = ordermap["BUILDING"];
	std::unordered_set<size_t> household_cust_set = ordermap["HOUSEHOLD"];
	std::unordered_set<size_t> furniture_cust_set = ordermap["FURNITURE"];
	std::unordered_set<size_t> automobile_cust_set = ordermap["AUTOMOBILE"];
	std::unordered_set<size_t> machinery_cust_set = ordermap["MACHINERY"];
	while(std::getline(relation_l,line,'\n'))
    while (std::getline(relation_l, line,'|')) 
    {
        if(word_count_l == 0)        
            ord_ID_l = std::stoi(line);
    	else if (word_count_l == 4)
		{
			std::unordered_set<size_t>::const_iterator element_found_building = building_cust_set.find (ord_ID_l);
			std::unordered_set<size_t>::const_iterator element_found_machinery = machinery_cust_set.find (ord_ID_l);
			std::unordered_set<size_t>::const_iterator element_found_household = household_cust_set.find (ord_ID_l);
			std::unordered_set<size_t>::const_iterator element_found_furniture = furniture_cust_set.find (ord_ID_l);
			std::unordered_set<size_t>::const_iterator element_found_automobile = automobile_cust_set.find (ord_ID_l);
			// if(element_found == lineitem_map.end())
				// lineitem_map.insert(std::make_pair(ord_ID_l, std::stoi(line)));
			// else
				// lineitem_map[ord_ID_l] += std::stoi(line);
			if(!(element_found_building == building_cust_set.end()))
			{
				sum_building += std::stoi(line);
				count_building++;
			}
			else if(!(element_found_machinery == machinery_cust_set.end()))
			{
				sum_machinery += std::stoi(line);
				count_machinery++;
			}
			else if(!(element_found_household == household_cust_set.end()))
			{
				sum_household += std::stoi(line);
				count_household++;
			}
			else if(!(element_found_furniture == furniture_cust_set.end()))
			{
				sum_furniture += std::stoi(line);
				count_furniture++;
			}
			else if(!(element_found_automobile == automobile_cust_set.end()))
			{
				sum_automobile += std::stoi(line);
				count_automobile++;
			}
			word_count_l = 0;
            break;
		}
		word_count_l++;
    }    
    average_map["MACHINERY"] = ((sum_machinery/count_machinery)) * 100;
	average_map["BUILDING"] = (sum_building/count_building) * 100;
	average_map["AUTOMOBILE"] = (sum_automobile/count_automobile) * 100;
	average_map["FURNITURE"] = (sum_furniture/count_furniture) * 100;
	average_map["HOUSEHOLD"] = (sum_household/count_household) * 100;
    return average_map;
}


std::unordered_set<size_t> JoinQuery::ordermap_gen(std::string oPath,std::unordered_set<size_t> cust_segment_set)
{
    std::ifstream relation_o(oPath);
    assert(relation_o); 
    size_t ord_ID;
    size_t word_count_o = 0;
	std::unordered_set<size_t> orderkey_set;
    //size_t sum = 0;
    //size_t count = 0;
	std::string word;
    
      
	while(std::getline(relation_o,word,'\n'))
    for (std::string line; std::getline(relation_o, line,'|');) 
    {
        if(word_count_o == 0)
        	ord_ID=std::stoi(line);

		else if(word_count_o == 1)
		{
            size_t cust_key = std::stoi(line);
			std::unordered_set<size_t>::const_iterator element_found;
            element_found = cust_segment_set.find(cust_key);
			
			if(!(element_found == cust_segment_set.end()))
            {
                //sum = sum + lineitem_map[ord_ID]; //check sum
                //count++;
				orderkey_set.insert(ord_ID);				
            }
            word_count_o = 0;
            break;
		}
		word_count_o++;
    }
	return orderkey_set;
	
}
//---------------------------------------------------------------------------
