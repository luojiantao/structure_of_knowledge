# multi_index_container
多索引容器，
std::map的一种扩展。有多个key
## 头文件
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>

## 使用数据结构说明


## demo

```C++

#include <string>
#include <iostream>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>

using namespace boost;
using namespace boost::multi_index;
using namespace std;
struct Employee{
  int id;
  string name;
  int age;

  Employee(int id_,std::string name_,int age_):id(id_),name(name_),age(age_){}

  friend std::ostream& operator<<(std::ostream& os,const Employee& e)
  {
    os<<e.id<<" "<<e.name<<" "<<e.age<<std::endl;
    return os;
  }
};
//tag
struct id{};

typedef multi_index_container<
  Employee,
  indexed_by<
    //ordered_unique<member<Employee, int, &Employee::id> >,
    ordered_unique< tag<id>,  BOOST_MULTI_INDEX_MEMBER(Employee,int,id) >,//不重复的key
    ordered_non_unique<member<Employee, string, &Employee::name> >,//可重复的key
    ordered_non_unique<member<Employee, int, &Employee::age> >
  >
> EmployeeContainer;

typedef EmployeeContainer::nth_index<0>::type IdIndex;
//typedef EmployeeContainer::index<id>::type IdIndex;
//typedef boost::multi_index::index<EmployeeContainer, id>::type IdIndex;
typedef EmployeeContainer::nth_index<1>::type NameIndex;
typedef EmployeeContainer::nth_index<2>::type AgeIndex;


int main(){
  EmployeeContainer con;
  con.insert(Employee(0,"Joe",31));
  con.insert(Employee(1,"Robert",27));
  con.insert(Employee(2,"John",40));

  IdIndex& ids = con.get<id>();
  copy(ids.begin(),ids.end(), ostream_iterator<Employee>(cout));
  cout << endl;

  NameIndex& names = con.get<1>();
  copy(names.begin(), names.end(), ostream_iterator<Employee>(cout));
  cout << endl;

  names.erase(names.begin());

  AgeIndex& ages = con.get<2>();
  copy(ages.begin(), ages.end(), ostream_iterator<Employee>(cout));
  cout << endl;

  return 0;
}

```
## 参考链接
[https://www.boost.org/doc/libs/1_70_0/libs/multi_index/doc/examples.html](https://www.boost.org/doc/libs/1_70_0/libs/multi_index/doc/examples.html)