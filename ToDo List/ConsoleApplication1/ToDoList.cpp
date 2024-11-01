#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct ToDo {
  string input;
  string item;
  string description;

  vector <string> ListItem;
  vector <string> ListDescription;
  vector <string> ItemDone;
  vector <string> DescriptionDone;


  void AddToDo() {

    cout << "enter item " << endl;
    getline(cin, item);

    cout << "enter description " << endl;
    getline(cin, description);
    ListItem.push_back(item);   
    ListDescription.push_back(description);

    cout << item << ": " << description;
    cout << endl << endl;
  }
  
  void DeleteToDo() {

    int num;

    cout << "which todo to delete?" << endl;
    cin >> num;
    
    if (ListItem.size() > 0) {
      ListItem.erase(ListItem.begin() + num - 1); 
      ListDescription.erase(ListDescription.begin() + num - 1); 
    }
    else {
      cout << "no todos, what were you going to delete?" << endl << endl;
    }
    
  }

  void PullUpList() {

     if (ListItem.size() > 0) {
       cout << "To do: " << endl;
      for (int i = 0; i < ListItem.size(); i++) {
        cout << i + 1 << ". " << ListItem[i] << ": " << ListDescription[i] << endl << endl;
     } 
    }
    else {
      cout << "no todos are made, you should start with some" << endl << endl;
    }

     if (ItemDone.size() > 0) {
       cout << "Done: " << endl;
       for (int i = 0; i < ItemDone.size(); i++) {
         cout << i + 1 << ". " << ItemDone[i] << ": " << DescriptionDone[i] << endl << endl;
       }
     }
     else  if (ListItem.size() != 0) {
       cout << "No todos are done, how so?" << endl << endl;
     }

  }

  void MarkAsDone() {
    int num;

    cout << "which todo to mark as done?" << endl << endl;
    cin >> num;

    if (ListItem.size() > 0) {
      ItemDone.push_back(item);
      DescriptionDone.push_back(description);
      ListItem.erase(ListItem.begin() + num - 1); 
      ListDescription.erase(ListDescription.begin() + num - 1);
    }
    else {
      cout << "there are no todos found" << endl << endl;
    }
  }

};

 int main()
{ 
   ToDo list1;
   while (true) {
     cout << "what to do: " << endl << "1. Add" << endl << "2. delete" << 
       endl << "3. mark as done" << endl  << "4. Pull up list" << endl << "0. exit" << endl << endl;
     getline(cin, list1.input); // when cin >> , doesn't work properly
     cout << endl;

     if (list1.input == "1") { 
       list1.AddToDo();
     }
     else if (list1.input == "2") {
       list1.DeleteToDo();
     }
     else if (list1.input == "3") {
       list1.MarkAsDone();
     }
     else if (list1.input == "4") {
       list1.PullUpList();
     }
     else if (list1.input == "0") {
       break;
     }
   }

} 

