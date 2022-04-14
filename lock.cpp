#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

uint32_t lock_prbs()
{   
    static uint32_t shift_register=0xf109c52; // "random" seed value
	int b1 = 0;
	int b2 = 0;
	if (shift_register & (1 << 30))
	{
		b1 = 1;
	}
	if (shift_register & (1 << 27))
	{
		b2 = 1;
	}
	
	shift_register=shift_register << 1;
	shift_register=shift_register | (b1 ^ b2);
	shift_register = shift_register & 0x7fffffff;
    return shift_register ; // return 31 LSB's 
}

int main()
{

	uint32_t lock_randnum;
	int counter_int;
	string counter_str;
	string compare_code;
	string lock_randnum_str;
	
	stringstream ss;
	
	fstream ctr_file;
	ctr_file.open("lock_counter.txt", ios::in);
	
	if (ctr_file.is_open()) {
		getline(ctr_file, counter_str);
		ss.str("");
		ss.clear();
		ss << counter_str;
		ss >> counter_int;
		cout << "||	LOCK.CPP	||	The counter value in lock_counter.txt is:	|| " << counter_int << endl;
	}
	else {														
		cout << "||	LOCK.CPP	||	Cannot Locate file" << endl;				
		exit(0);											
	}														
	ctr_file.close();
	
	for (int i = 0; i < counter_int; i++){
		lock_randnum = lock_prbs();
	}
	
	ss.str("");
	ss.clear();
	ss << hex << lock_randnum;
	ss >> lock_randnum_str;
	
	if (lock_randnum_str.length() == 7){
		lock_randnum_str.insert(0, 1, '0');
	}
	if (lock_randnum_str.length() == 6){
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
	}
	if (lock_randnum_str.length() == 5){
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
	}
	if (lock_randnum_str.length() == 4){
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
	}
	if (lock_randnum_str.length() == 3){
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
	}
	if (lock_randnum_str.length() == 2){
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
	}
	if (lock_randnum_str.length() == 1){
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
		lock_randnum_str.insert(0, 1, '0');
	}//zero padding has to be implimented because all hex numbers 00000000 -> ffffffff are ran. the seed is for the starting value.
		
	cout << "||	LOCK.CPP	||	The Generated Lock Code:			|| " << lock_randnum_str << endl;
	fstream code_file;
	code_file.open("lock_code.txt", ios::in);
	if (code_file.is_open()) {
		getline(code_file, compare_code);
		cout << "||	LOCK.CPP	||	The code in lock_code.txt is:			|| " << compare_code << endl;
	}
	else {														
		cout << "||	LOCK.CPP	||	Cannot Locate file" << endl;				
		exit(0);											
	}														
	code_file.close();
	
	fstream match_file;
	match_file.open("lock_match.txt", ios::out);
	
	
	if (compare_code == lock_randnum_str){
		match_file << "1";
	}
	else {
		match_file << "0";
	}
	return 0;	
	
}
