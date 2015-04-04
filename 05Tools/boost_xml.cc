#if 0
<?xml version="1.0" encoding="utf-8"?> 
<con> 
  <id>1</id> 
  <name>fansy</name> 
  <urls> 
    <url>http://blog.csdn.net//fansongy</url> 
    <url>http://weibo.com//fansongy</url> 
  </urls> 
</con>   
#endif // 0
#include <iostream>  
#include <boost/property_tree/ptree.hpp> 
#include <boost/property_tree/xml_parser.hpp> 
#include <boost/typeof/typeof.hpp>   

using std::cout; 
using std::endl; 
using std::string; 
using boost::property_tree::read_xml; 
using boost::property_tree::write_xml; 
using boost::property_tree::ptree; 

int  main() 
{ 
    ptree pt;
    read_xml("./conf.xml",pt);   

    cout << "ID: " << pt.get<int>("con.id") << endl;         
    cout << "Try Default" << pt.get<int>("con.no_prop", 100) << endl;   //如果取不到，则使用默认值       

    ptree child = pt.get_child("con");     
    cout << "name is : " << child.get<string>("name") << endl;   
     
    child = pt.get_child("con.urls"); 
    for(BOOST_AUTO(pos,child.begin()); pos != child.end(); ++pos){ 
        cout << pos->second.data() << endl; 
    } 

#if 0 // 输出
     ID: 1
     Try Default100
     name is : fansy
     http://blog.csdn.net//fansongy
     http://weibo.com//fansongy
#endif // 0

    pt.put("con.name",     "Sword");           // 更改某个键值 
    pt.add("con.urls.url", "http://www.baidu.com"); // 增加某个键值 
    write_xml("conf1.xml", pt);              // 写入XML 
    return 0; 
}  
