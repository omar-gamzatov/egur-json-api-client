#include "Requests.cpp"
#include <map>
#include <string>

using namespace std;

int main() {
	
	Requests request = Requests("localhost", "80");
	map<const char*, const char*> operator_info;
	operator_info.insert(pair<const char*, const char*>("depart_num", "12"));
	operator_info.insert(pair<const char*, const char*>("stand_num", "222"));
	operator_info.insert(pair<const char*, const char*>("operator", "B b"));

	map<const char*, const char*> testing_info;
	testing_info.insert(pair<const char*, const char*>("Serial number", "12.222.2023.03.0006"));
	testing_info.insert(pair<const char*, const char*>("param1", "1"));
	testing_info.insert(pair<const char*, const char*>("param2", "2"));
	testing_info.insert(pair<const char*, const char*>("param3", "3"));
	testing_info.insert(pair<const char*, const char*>("param4", "4"));
	testing_info.insert(pair<const char*, const char*>("result", "0"));

	//json response = request.get_serial_number(operator_info);
	json response = request.get_testing_info("12.222.2023.03.0099");
	//json response = request.send_testing_info(testing_info);

	cout << response.dump();
}
