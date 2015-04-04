#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
 
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::stringstream;
using std::ostringstream;
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
using std::make_pair;

bool json_to_ptree(const string& data, ptree& pt){
     try{
          stringstream ss(data);
          read_json(ss, pt);
          return true;
     }catch(...){
          cerr << "parse json failed" << endl;
          return false;
     }
}

string ptree_to_json(const ptree& pt){
     try{
          ostringstream ss;
          write_json (ss, pt, false);
          return ss.str();
     }catch(...){
          return "";
     }
}

void deal_recv_data(const string& json_data);

#if 0 // json string
{
   "menu":
   {
      "foo": true,
      "bar": "true",
      "value": 128,
      "popup":
      [
         {"value": "New", "onclick": "CreateNewDoc()"},
         {"value": "Open", "onclick": "OpenDoc()"},
      ]
   }
} 
#endif // 0
int main()
{
     ptree popup; // popup
     ptree popup_child1;
     ptree popup_child2;

     popup_child1.put<string>("value",  "New"); // 向popup子树中压入数据
     popup_child1.put<string>("onclick", "CreateNewDoc()");
     popup.push_back(make_pair("", popup_child1)); // 添加popup子树    

     popup_child2.put<string>("value", "Open");
     popup_child2.put<string>("onclick", "OpenDoc()");
     popup.push_back(make_pair("", popup_child2)); 

     ptree menu;
     menu.put<bool>("foo",   true);
     menu.put<string>("bar", "true");
     menu.put<int>("value",  128);
     menu.add_child("popup", popup);

     ptree ptree_data;
     ptree_data.add_child("menu", menu);

     string json_data = ptree_to_json(ptree_data);
     cout << json_data << endl; 

     deal_recv_data(json_data); // 读取刚才得到的json数据

    return 0;
}

void deal_recv_data(const string& json_data){
     ptree recv_data;
     if(!json_to_ptree(json_data, recv_data)){
          return;// parse json failed
     }

     ptree menu = recv_data.get_child("menu");
     ptree popup = menu.get_child("popup");

     cout << "foo:  " << menu.get<bool>("foo")  << endl;
     cout << "bar:  " << menu.get<string>("bar") << endl;
     cout << "value: " << menu.get<int>("value") << endl;

     ptree::iterator iter = popup.begin();
     for(; iter != popup.end(); ++iter){
          ptree popup_child = iter->second; 
// notice：push_back(make_pair("", popup_child1));

    cout << "value:  " << popup_child.get<string>("value") << endl;
    cout << "onclick: " << popup_child.get<string>("onclick")<< endl;
 }
}

#if 0 // 程序中json_data的输出结果
{
    "menu":
     {
       "foo":"true",
       "bar":"true",
       "value":"128",  
       "popup":[
           {"value":"New","onclick":"CreateNewDoc()"},
           {"value":"Open","onclick":"OpenDoc()"}       
       ]
     }
}
注意： "value":"128", 这里将int打包成了字符串 不符合json规范


deal_recv_data()函数输出结果：
foo:   1
bar:   true
value: 128
value:   New
onclick: CreateNewDoc()
value:   Open
onclick: OpenDoc()

#endif // 0
