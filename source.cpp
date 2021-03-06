#include <iostream>
#include "list2.h"
#include <fstream>
#include <string>
using namespace std;
bool fill_list(list2& list, const char* filename);
//void show_list(const list2& list, bool reversed = false);
void show_list(const list2& list, bool reversed = false)
{
    elem* curr = reversed ? list.last : list.first;
    elem* cmp = reversed ? list.first : list.last;
    if (!curr) cout << "empty list";
    else while (curr)
    {
        cout << curr->x;
        curr = reversed ? curr->prev : curr->next;
    }
}
bool check(char* value);
void fill(list2& list, char* str);
void resh(list2& list1, list2& list2);
void show_content();
void show_menu();
void show_header();

void main() {
    cout << "Content-type: text/html; charset=Windows-1251\n\n";
    ifstream f("_index");
    if (f.is_open())
    {
        auto sz = 65536;
        auto buf = new char[sz];
        while (!f.eof()) {
            f.getline(buf, sz);
            if (strcmp(buf, "<!--#content#-->") == 0) {
                show_content();
            }
            else if (!strcmp(buf, "<!--#menu#-->"))
            {
                show_menu();
            }
            else if (!strcmp(buf, "<!--#header#-->"))
            {
                show_header();
            }
            cout << buf;
        }
        delete[] buf;
        f.close();
    }
    else {
        cout << "Не удалось вывести страницу";
    }
}

bool fill_list(list2& list, const char* filename)
{
    ifstream f(filename);
    if (f.is_open())
    {
        while (!f.eof())
        {
            double x;
            f >> x;
            if (f.fail())
            {
                clear(list);
                return false;
            }
            add(list, x);
        }
        f.close();
        return true;
    }
    else
    {
        cout << "file fall" << endl;
        return false;
    }
}
/*void show_list(const list2& list, bool reversed = false)
{
    elem* curr = reversed ? list.last : list.first;
    elem* cmp = reversed ? list.first : list.last;
    if (!curr) cout << "empty list";
    else while (curr)
    {
        cout << curr->x;
        curr = reversed ? curr->prev : curr->next;
    }
}
*/
bool check(char* value)
{
    for (int i = 1; i < strlen(value) - 1; i++)
    {
        if (value[i] == '+' && value[i - 1] != ' ' && value[i + 1] != ' ') return true;
    }
    return false;
}
void fill(list2& list, char* str)
{

    /*
    int cnt = strlen(str);
    int temp = strlen(str);
    int p = strlen(str)/9;
    int op = strlen(str) % 9;
    while (temp/9!=0)
    {
            int sum = 0;
            int q = 0;
            int u=strlen(str);
            for (int j = u; j > 9*p - op; j--) {

                int s = (int)str[j] - 48;
                sum = sum + s * pow(10, q);
                q++;
                cnt--;
            }
            u-=9;
            p--;
            add(list, sum);
        temp -= 9;
    }
    
    */
    int cnt = 0;
    int temp = strlen(str);
    int p = 1;
    while (temp / 9 != 0)
    {
        int sum = 0;
        int q = 0;
        for (int j = cnt; j < 9 * p; j++) {

            int s = (int)str[j] - 48;
            sum = sum + s * pow(10, q);
            q++;
            cnt++;
        }
        p++;
        add_to_begin(list, sum);
        temp -= 9;
    }
    if (temp != 0)
    {
        int q = 0;
        int sum = 0;
        for (int j = cnt; j < strlen(str); j++) {
            int s = (int)str[j] - 48;
            sum = sum + s * pow(10, q);
            q++;
        }
        add_to_begin(list,sum);
    }
    /*
    if (temp != 0)
    {
        int q = 0;
        int sum = 0;
        for (int j = cnt+1; j > 0 ; j--) {
            int s = (int)str[j] - 48;
            sum = sum + s * pow(10, q);
            q++;
            cnt--;
        }
        add(list, sum);
    }*/
}
void resh(list2& list1, list2& list2)
{
    int a = list1.count;
    int b = list2.count;
    elem* cur1 = list1.last;
    elem* cur2 = list2.last;
    if (a > b)
    {
        for (int i = b; i > 0; i--)
        {
            cur1->x += cur2->x;
            if (cur1->x > 999999999)
            {
                cur1->x = cur1->x % 1000000000;
                cur1->prev->x += 1;
            }
            cur1 = cur1->prev;
            cur2 = cur2->prev;
        }
        show_list(list1);
    }
    else if (a < b)
    {
        for (int i = a; i > 0; i--)
        {
            cur2->x += cur1->x;
            if (cur2->x > 999999999)
            {
                cur2->x = cur2->x % 1000000000;
                cur2->prev->x += 1;
            }
            cur1 = cur1->prev;
            cur2 = cur2->prev;
        }
        show_list(list2);
    }
    else
    {
        for (int i = b; i > 0; i--)
        {
            cur1->x += cur2->x;
            if (i > 1 && cur1->x > 999999999)
            {
                cur1->x = cur1->x % 1000000000;
                cur1->prev->x += 1;
            }
            if (i == 1 && cur1->x > 999999999)
            {
                cur1->x = cur1->x % 1000000000;
                cout << 1;
            }
            cur1 = cur1->prev;
            cur2 = cur2->prev;
        }
        show_list(list1);
    }
}
void show_content()
{
    list2 list1{ 0 };
    list2 list2{ 0 };
    cout << "<form method='get' action='d_spisok.cgi'>";
    cout << "<input type='text' class='enter' name='dlist' placeholder='Введите выражение'>";
    cout << "<input type='submit' value='Отправить' class='button'></input>";
    cout << "</form>";
    cout << "<h2 class = 'result'>Итоговое значение: </h2>";
    cout << "<div class = 'output'>";
    char* data = nullptr;
    get_form_data(data);
    if ((data && strlen(data)) > 0) {
        char* value = nullptr; 
        get_param_value(value, "dlist", data);
        if (check(value))
        {
            char* context = nullptr;
            char* number = strtok_s(context, "+", &value);
            cout << number;
            cout << " + ";
            cout << value;
            cout << " = ";
            int rest1 = strlen(number) % 2;
            for (int i = 0; i < (strlen(number) - rest1) / 2; i++) {
                swap(number[i], number[strlen(number) - 1 - i]);
            }
            int rest2 = strlen(value) % 2;
            for (int i = 0; i < (strlen(value) - rest2) / 2; i++) {
                swap(value[i], value[strlen(value) - 1 - i]);
            }
            fill(list1, number);
            fill(list2, value);
           
            
            resh(list1, list2);
        }
        else
        {
            cout << "Ошибка";
        }
    }
}
void show_header()
{
    cout << "Двусвязный список";
}
void show_menu()
{
    ifstream f("menu");
    if (f.is_open())
    {
        auto sz = 1024;
        auto buf = new char[sz];
        while (!f.eof())
        {
            f.getline(buf, sz);
            char* name;
            char* addrr = strtok_s(buf, " ", &name);
            cout << "<div class='menu-item'>"
                "<a href='" << addrr << "'>" <<
                name << "</a>"
                "</div>";
        }
    }
    f.close();
}
